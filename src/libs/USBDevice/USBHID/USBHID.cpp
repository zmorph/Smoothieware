/*
*		Code by ZMorph, ported from mbed USBHID, according to existing USBCDC and USBMSD patterns
*/


#include <cstdint>
#include <cstdio>
#include <cstring>

#include "USBHID.h"
#include "libs/Kernel.h"

#define iprintf(...) do { } while (0)

USBHID::USBHID(USB *u, uint8_t output_report_length, uint8_t input_report_length) {
	usb = u;

    output_length = output_report_length;
    input_length = input_report_length;

	HID_interface = {
        DL_INTERFACE,               // bLength
        DT_INTERFACE,               // bDescType
        0,                          // bInterfaceNumber: filled out during addInterface()
        0,                          // bAlternateSetting
        2,                          // bNumEndpoints
        UC_HID,                     // bInterfaceClass
        HID_SUBCLASS_NONE,	        // bInterfaceSubClass
        HID_PROTOCOL_NONE, 			// bInterfaceProtocol
        0,                          // iInterface
        0, 0, 0,                    // dummy padding
        this,                       // callback
    };

    HID_hid_descriptor = {
    	HID_DESCRIPTOR_LENGTH,          // bLength
        HID_DESCRIPTOR,                 // bDescriptorType
        HID_VERSION_1_11,          		// wcdHID
        0x00,                           // bCountryCode
        0x01,                           // bNumDescriptors
        HID_REPORT_DESCRIPTOR,          // bDescriptorType
        this->reportDescLength(),  		// wDescriptorLength
    };

    HID_endpoint_in = {
		DL_ENDPOINT,     				// bLength
        DT_ENDPOINT,            		// bDescriptorType
        EP_DIR_IN,          			// bEndpointAddress: we provide direction, address is filled in by addEndpoint()
        EA_INTERRUPT,                   // bmAttributes
        MAX_PACKET_SIZE_EPINT,     		// wMaxPacketSize
        1,                              // bInterval (milliseconds)
        0,                      		// dummy padding        
        this,                  			// endpoint callback        
    };

    HID_endpoint_out = {
		DL_ENDPOINT,     				// bLength
        DT_ENDPOINT,            		// bDescriptorType
        EP_DIR_OUT,          			// bEndpointAddress: we provide direction, address is filled in by addEndpoint()
        EA_INTERRUPT,                   // bmAttributes
        MAX_PACKET_SIZE_EPINT,     		// wMaxPacketSize
        1,                              // bInterval (milliseconds)
        0,                      		// dummy padding        
        this,                  			// endpoint callback        
    };

    usbdesc_string_l(13) s = usbstring("Smoothie HID");
    memcpy(&HID_description, &s, sizeof(HID_description));

    usb->addInterface(&HID_interface);

    usb->addDescriptor(&HID_hid_descriptor);
	//usb->addDescriptor(reportDesc());
	//usb->addDescriptor(entireHIDdescriptor());

    usb->addEndpoint(&HID_endpoint_in);
    usb->addEndpoint(&HID_endpoint_out);

    HID_interface.iInterface = 
    	usb->addString(&HID_description);
}


bool USBHID::send(HID_REPORT *report)
{
    return usb->write(HID_endpoint_in.bEndpointAddress, report->data, report->length, MAX_HID_REPORT_SIZE);
}

bool USBHID::sendNB(HID_REPORT *report)
{
    return usb->writeNB(HID_endpoint_in.bEndpointAddress, report->data, report->length, MAX_HID_REPORT_SIZE);
}


bool USBHID::read(HID_REPORT *report)
{
    uint32_t bytesRead = 0;
    bool result;
    result = usb->readEP(HID_endpoint_out.bEndpointAddress, report->data, &bytesRead, MAX_HID_REPORT_SIZE);
    if(!usb->readStart(HID_endpoint_out.bEndpointAddress, MAX_HID_REPORT_SIZE))
        return false;
    report->length = bytesRead;
    return result;
}

bool USBHID::readNB(HID_REPORT *report)
{
    uint32_t bytesRead = 0;
    bool result;
    result = usb->readEP_NB(HID_endpoint_out.bEndpointAddress, report->data, &bytesRead, MAX_HID_REPORT_SIZE);
    report->length = bytesRead;
    if(!usb->readStart(HID_endpoint_out.bEndpointAddress, MAX_HID_REPORT_SIZE))
        return false;
    return result;
}

bool USBHID::USBEvent_Request(CONTROL_TRANSFER &transfer) {

	THEKERNEL->streams->printf("USBEvent_Request fired\n");

	bool success = false;

    // Process additional standard requests

    if ((transfer.setup.bmRequestType.Type == STANDARD_TYPE))
    {
    	THEKERNEL->streams->printf("STANDARD_TYPE request\n");
        switch (transfer.setup.bRequest)
        {
            case GET_DESCRIPTOR:
    			THEKERNEL->streams->printf("GET_DESCRIPTOR request\n");
                switch (DESCRIPTOR_TYPE(transfer.setup.wValue))
                {
                    case HID_REPORT_DESCRIPTOR:
                        if ((reportDesc() != NULL) \
                            && (reportDescLength() != 0))
                        {
                        	THEKERNEL->streams->printf("HID_REPORT_DESCRIPTOR requested\n");

                            transfer.remaining = reportDescLength();
                            transfer.ptr = reportDesc();
                            transfer.direction = DEVICE_TO_HOST;
                            success = true;
                        }
                        break;
                    case HID_DESCRIPTOR:
                            // Find the HID descriptor, after the configuration descriptor
                            
                    		// WARNING - not sure if commenting out following lines is ok
                            //hidDescriptor = usb->findDescriptor(HID_DESCRIPTOR);
                            //if (hidDescriptor != NULL)
                            //{
                    		THEKERNEL->streams->printf("HID_DESCRIPTOR requested\n");
                                transfer.remaining = HID_DESCRIPTOR_LENGTH;
                                transfer.ptr = (uint8_t *) &HID_hid_descriptor;
                                transfer.direction = DEVICE_TO_HOST;
                                success = true;
                            //}
                            break;
                     
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    // Process class-specific requests

    if (transfer.setup.bmRequestType.Type == CLASS_TYPE)
    {
    	THEKERNEL->streams->printf("CLASS_TYPE request\n");
    	THEKERNEL->streams->printf("request number: %u\n", transfer.setup.bRequest);
        switch (transfer.setup.bRequest)
        {
            case HID_REPORT_DESCRIPTOR:
                if ((reportDesc() != NULL) \
                    && (reportDescLength() != 0))
                {
                	THEKERNEL->streams->printf("HID_REPORT_DESCRIPTOR requested\n");

                    transfer.remaining = reportDescLength();
                    transfer.ptr = reportDesc();
                    transfer.direction = DEVICE_TO_HOST;
                    success = true;
                }
                break;

             case HID_SET_REPORT:
             	THEKERNEL->streams->printf("HID_SET_REPORT fired\n");
                // First byte will be used for report ID
                outputReport.data[0] = transfer.setup.wValue & 0xff;
                outputReport.length = transfer.setup.wLength + 1;

                transfer.remaining = sizeof(outputReport.data) - 1;
                transfer.ptr = &outputReport.data[1];
                transfer.direction = HOST_TO_DEVICE;
                transfer.notify = true;
                success = true;
            default:
                break;
        }
    }

    return success;
}

uint8_t * USBHID::reportDesc() {
    static uint8_t reportDescriptor[] = {
        0x06, LSB(0xFFAB), MSB(0xFFAB),
        0x0A, LSB(0x0200), MSB(0x0200),
        0xA1, 0x01,         // Collection 0x01
        0x75, 0x08,         // report size = 8 bits
        0x15, 0x00,         // logical minimum = 0
        0x26, 0xFF, 0x00,   // logical maximum = 255
        0x95, input_length, // report count
        0x09, 0x01,         // usage
        0x81, 0x02,         // Input (array)
        0x95, output_length,// report count
        0x09, 0x02,         // usage
        0x91, 0x02,         // Output (array)
        0xC0                // end collection

    };
    reportLength = sizeof(reportDescriptor);
    return reportDescriptor;
}


uint16_t USBHID::reportDescLength() {
    reportDesc();
    return reportLength;
}


bool USBHID::USBEvent_RequestComplete(CONTROL_TRANSFER &transfer, uint8_t *buf, uint32_t length)
{
    return true;
}


uint8_t * USBHID::entireHIDdescriptor()
{
	static uint8_t entiredescriptor[] = {		
        0x1e,  // bDescriptorLength
        HID_REPORT_DESCRIPTOR,          // bDescriptorType
        0x06, LSB(0xFFAB), MSB(0xFFAB),
        0x0A, LSB(0x0200), MSB(0x0200),
        0xA1, 0x01,         // Collection 0x01
        0x75, 0x08,         // report size = 8 bits
        0x15, 0x00,         // logical minimum = 0
        0x26, 0xFF, 0x00,   // logical maximum = 255
        0x95, input_length, // report count
        0x09, 0x01,         // usage
        0x81, 0x02,         // Input (array)
        0x95, output_length,// report count
        0x09, 0x02,         // usage
        0x91, 0x02,         // Output (array)
        0xC0                // end collection
	};
	return entiredescriptor;
}