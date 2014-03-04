/*
	file added by ZMorph
*/

#include "USBMessageStream.h"

#include "libs/Kernel.h"
#include "libs/SerialMessage.h"

USBMessageStream::USBMessageStream(USB *u) : USBHID(u, 64,64){}


void USBMessageStream::on_module_loaded()
{
    this->register_for_event(ON_MAIN_LOOP);
}

void USBMessageStream::on_main_loop(void *argument)
{
	// execute all pending messages
	/*HID_REPORT recv_report;
	//THEKERNEL->streams->printf("befor puts");
	//puts("sabaka");
	//THEKERNEL->streams->printf("after puts");
	while (readNB(&recv_report)) 
	{
		THEKERNEL->streams->printf("I have recieved an HID report!\n");

        struct SerialMessage message;
        // ZTODO the following conversion is dangerous - unsigned to signed
        message.message = string((const char *)recv_report.data);
        if (message.message.find("sendme") != string::npos)
        	puts("something");
        message.stream = this;
        THEKERNEL->streams->printf("message: %s", message.message.c_str());
        THEKERNEL->call_event(ON_CONSOLE_LINE_RECEIVED, &message);
	}*/
}

int USBMessageStream::puts(const char *str)
{
	// ZTODO check for strings longer then 64 bytes

	int length = strlen(str);

	HID_REPORT send_report;
	send_report.length = length;
	memcpy(send_report.data, str, length);

	// ZTODO consider if it should be blocking or non-blocking
	sendNB(&send_report);
	return length;
}


bool USBMessageStream::USBEvent_EPIn(uint8_t, uint8_t) {
	THEKERNEL->streams->printf("EPIn called!\n");
	return false;}
bool USBMessageStream::USBEvent_EPOut(uint8_t, uint8_t) {
	THEKERNEL->streams->printf("EPOut called!\n");
	// execute all pending messages
	HID_REPORT recv_report;
	while (readNB(&recv_report)) 
	{
		THEKERNEL->streams->printf("I have recieved an HID report!\n");

        struct SerialMessage message;
        // ZTODO the following conversion is dangerous - unsigned to signed
        message.message = string((const char *)recv_report.data);
        if (message.message.find("sendme") != string::npos)
        	puts("something");
        message.stream = this;
        THEKERNEL->streams->printf("message: %s", message.message.c_str());
        THEKERNEL->call_event(ON_CONSOLE_LINE_RECEIVED, &message);
	}
	return false;}