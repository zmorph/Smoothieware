/*
*		Code by ZMorph
*/

#ifndef USBMESSAGESTREAM_H
#define USBMESSAGESTREAM_H

#include "USBHID.h"

#include "Module.h"
#include "StreamOutput.h"

class USBMessageStream : public USBHID, public Module, public StreamOutput
{
public:
	USBMessageStream(USB *);

    int puts(const char *);
	
    void on_module_loaded(void);
    void on_main_loop(void *);

protected:
    bool USBEvent_EPIn(uint8_t, uint8_t);
    bool USBEvent_EPOut(uint8_t, uint8_t);
	
};

#endif