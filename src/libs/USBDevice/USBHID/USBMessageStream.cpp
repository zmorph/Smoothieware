/*
	file added by ZMorph
*/

#include "USBMessageStream.h"

#include "libs/Kernel.h"
#include "libs/SerialMessage.h"

#define iprintf(...) do {} while(0) //THEKERNEL->streams->printf

USBMessageStream::USBMessageStream(USB *u) : USBHID(u, 64,64),
	rxbuf(256 + 8), 
	txbuf(128 + 8),
	nl_in_rx(0),
	flush_to_nl(false)
{}


void USBMessageStream::on_module_loaded()
{
    this->register_for_event(ON_MAIN_LOOP);
    //THEKERNEL->streams->append_stream(this);
}

uint8_t USBMessageStream::available()
{
    return rxbuf.available();
}

void USBMessageStream::on_main_loop(void *argument)
{
    if (nl_in_rx)
    {
    	iprintf("on_main_loop: there is nl in rx\n");

        std::string received;
        //int idebug = 0;
        while (available())
        {
            char c = _getc();
            iprintf("got char: %u number %d \n", c, idebug++);
            if( c == '\n' || c == '\r')
            {
                struct SerialMessage message;
                message.message = received;
                message.stream = this;
                iprintf("USBMessageStream Received: %s\n", message.message.c_str());
                THEKERNEL->call_event(ON_CONSOLE_LINE_RECEIVED, &message );
                return;
            }
            else
            {
                received += c;
            }
        }
    }
}

int USBMessageStream::puts(const char *str)
{
	iprintf("UMS puts: %s\n", str);

    int i = 0;
    while (*str)
    {
        ensure_tx_space(1);
        txbuf.queue(*str);
        if ((txbuf.available() % 64) == 0)
            usb->endpointSetInterrupt(HID_endpoint_in.bEndpointAddress, true);
        i++;
        str++;
    }
    usb->endpointSetInterrupt(HID_endpoint_in.bEndpointAddress, true);
    return i;
}


void USBMessageStream::ensure_tx_space(int space)
{
    while (txbuf.free() < space)
    {
        usb->endpointSetInterrupt(HID_endpoint_in.bEndpointAddress, true);
        usb->usbisr();
    }
}

uint16_t USBMessageStream::writeBlock(const uint8_t * buf, uint16_t size)
{
    if (size > txbuf.free())
    {
        size = txbuf.free();
    }
    if (size > 0)
    {
        for (uint8_t i = 0; i < size; i++)
        {
            txbuf.queue(buf[i]);
        }
        usb->endpointSetInterrupt(HID_endpoint_in.bEndpointAddress, true);
    }
    return size;
}


bool USBMessageStream::USBEvent_EPIn(uint8_t bEP, uint8_t bEPStatus) 
{
    /*
     * Called in ISR context
     */

    bool r = true;

    if (bEP != HID_endpoint_in.bEndpointAddress)
        return false;

    iprintf("USBMessageStream:EpIn: 0x%02X\n", bEPStatus);

   	HID_REPORT report;    
    memset(report.data, 0, output_length);

    int l = txbuf.available();
    iprintf("%d bytes queued\n", l);
    if (l > 0)
    {
        if (l > MAX_PACKET_SIZE_EPBULK)
            l = MAX_PACKET_SIZE_EPBULK;
        iprintf("Sending %d bytes:\n\t", l);
        int i;
        for (i = 0; i < l; i++) {
            txbuf.dequeue(&report.data[i]);
            if (report.data[i] >= 32 && report.data[i] < 128)
                iprintf("%c", report.data[i]);
            else {
                iprintf("\\x%02X", report.data[i]);
            }
        }
        iprintf("\nSending...\n");
        report.length = output_length;
        send(&report);
        iprintf("Sent\n");
        if (txbuf.available() == 0)
            r = false;
    }
    else
    {
        r = false;
    }
    iprintf("USBMessageStream:EpIn Complete\n");
    return r;
}

bool USBMessageStream::USBEvent_EPOut(uint8_t bEP, uint8_t bEPStatus) 
{
    /*
     * Called in ISR context
     */

    bool r = true;

    iprintf("USBMessageStream:EpOut\n");
    if (bEP != HID_endpoint_out.bEndpointAddress)
        return false;

	if (rxbuf.free() < MAX_PACKET_SIZE_EPBULK)
		return false;


   	HID_REPORT report;

    // we read the report, and put the data in the circular buffer
    read(&report);
    iprintf("Read report of length %lu\n", report.length);
	for (uint8_t i = 0; i < report.length; i++) {

		// we ignore blank data
		if (report.data[i] == 0x00)
			continue;

        if (flush_to_nl == false)
            rxbuf.queue(report.data[i]);

        if (report.data[i] >= 32 && report.data[i] < 128)
        {
            iprintf("%c", report.data[i]);
        }
        else
        {
            iprintf("\\x%02X", report.data[i]);
        }

        if (report.data[i] == '\n' || report.data[i] == '\r')
        {
            if (flush_to_nl)
                flush_to_nl = false;
            else
                nl_in_rx++;
        }
        else if (rxbuf.isFull() && (nl_in_rx == 0))
        {
            // to avoid a deadlock with very long lines, we must dump the buffer
            // and continue flushing to the next newline
            rxbuf.flush();
            flush_to_nl = true;
        }        
	}   

    iprintf("\nQueued, %d empty\n", rxbuf.free());

    if (rxbuf.free() < MAX_PACKET_SIZE_EPBULK)
    {
        // if buffer is full, stall endpoint, do not accept more data
        r = false;

        if (nl_in_rx == 0)
        {
            // we have to check for long line deadlock here too
            flush_to_nl = true;
            rxbuf.flush();

            // and since our buffer is empty, we can accept more data
            r = true;
        }
    }

	usb->readStart(HID_endpoint_out.bEndpointAddress, MAX_PACKET_SIZE_EPBULK);
    iprintf("USBMessageStream:EpOut Complete\n");
    return r;
}

int USBMessageStream::_putc(int c)
{
    ensure_tx_space(1);
    txbuf.queue(c);

    usb->endpointSetInterrupt(HID_endpoint_in.bEndpointAddress, true);
    return 1;
}

int USBMessageStream::_getc()
{
    uint8_t c = 0;
    //setled(4, 1); while (rxbuf.isEmpty()); setled(4, 0);
    rxbuf.dequeue(&c);
    if (rxbuf.free() == MAX_PACKET_SIZE_EPBULK)
    {
        usb->endpointSetInterrupt(HID_endpoint_out.bEndpointAddress, true);
        iprintf("rxbuf has room for another packet, interrupt enabled\n");
    }
    else if ((rxbuf.free() < MAX_PACKET_SIZE_EPBULK) && (nl_in_rx == 0))
    {
        // handle potential deadlock where a short line, and the beginning of a very long line are bundled in one usb packet
        rxbuf.flush();
        flush_to_nl = true;

        usb->endpointSetInterrupt(HID_endpoint_out.bEndpointAddress, true);
        iprintf("rxbuf has room for another packet, interrupt enabled\n");
    }
    if (nl_in_rx > 0)
        if (c == '\n' || c == '\r')
            nl_in_rx--;

    return c;
}
