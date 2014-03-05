/*
*		Code by ZMorph
*/

#ifndef USBMESSAGESTREAM_H
#define USBMESSAGESTREAM_H

#include "USBHID.h"
#include "CircBuffer.h"

#include "Module.h"
#include "StreamOutput.h"

class USBMessageStream : public USBHID, public Module, public StreamOutput
{
public:
	USBMessageStream(USB *);

    int _putc(int c);
    int _getc();
    int puts(const char *);
	
    uint8_t available();

    CircBuffer<uint8_t> rxbuf;
    CircBuffer<uint8_t> txbuf;

    void on_module_loaded(void);
    void on_main_loop(void *);


    uint16_t writeBlock(const uint8_t * buf, uint16_t size);

protected:
    bool USBEvent_EPIn(uint8_t, uint8_t);
    bool USBEvent_EPOut(uint8_t, uint8_t);

    void ensure_tx_space(int);
	
    volatile int nl_in_rx;
    bool flush_to_nl;
};

#endif