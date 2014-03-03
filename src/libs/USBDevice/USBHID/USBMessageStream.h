/*
*		Code by ZMorph
*/

#ifndef USBMESSAGESTREAM_H
#define USBMESSAGESTREAM_H



class USBMessageStream : public USBHID, public Module, public StreamOutput
{
public:
	USBMessageStream(USB *);

protected:
    bool USBEvent_EPIn(uint8_t, uint8_t);
    bool USBEvent_EPOut(uint8_t, uint8_t);
	
};







#endif