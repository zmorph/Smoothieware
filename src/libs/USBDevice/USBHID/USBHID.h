/*
*		Code by ZMorph, ported from mbed USBHID, according to existing USBCDC and USBMSD patterns
*/

#ifndef USBHID_H
#define USBHID_H

#include "USB.h"

/* These headers are included for child class. */
#include "USBEndpoints.h"
#include "USBDescriptor.h"
#include "USBDevice_Types.h"

#include "descriptor_hid.h"

class USBHID : public USB_Endpoint_Receiver {
public:
	USBHID(USB *, uint8_t output_report_length, uint8_t input_report_length);

protected:
    uint16_t reportLength;

    bool send(HID_REPORT *report);
    bool sendNB(HID_REPORT *report);

    bool read(HID_REPORT * report);
	bool readNB(HID_REPORT * report);

	// not yet sure what should the following do, but they are enforced:
    virtual bool USBEvent_Request(CONTROL_TRANSFER&);
    virtual bool USBEvent_RequestComplete(CONTROL_TRANSFER&, uint8_t*, uint32_t);
    
    // ain't got no default actions for ins and outs
    //virtual bool USBEvent_EPIn(uint8_t, uint8_t);
    //virtual bool USBEvent_EPOut(uint8_t, uint8_t);


    virtual uint8_t * reportDesc();
    virtual uint16_t reportDescLength();

    USB *usb;

    // descriptors
    usbdesc_interface   HID_interface;
    usbhid_report       HID_hid_descriptor;
    usbdesc_endpoint    HID_endpoint_in;
    usbdesc_endpoint    HID_endpoint_out;

    usbdesc_string_l(12) HID_description;

    HID_REPORT outputReport;
    uint8_t output_length;
    uint8_t input_length;

};

#endif
