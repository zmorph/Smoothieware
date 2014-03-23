/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stdint.h"

#include "USBMouse.h"

#include "libs/Kernel.h"

bool USBMouse::update(int16_t x, int16_t y, uint8_t button, int8_t z) {
    switch (mouse_type) {
        case REL_MOUSE:
            while (x > 127) {
                if (!mouseSend(127, 0, button, z)) return false;
                x = x - 127;
            }
            while (x < -128) {
                if (!mouseSend(-128, 0, button, z)) return false;
                x = x + 128;
            }
            while (y > 127) {
                if (!mouseSend(0, 127, button, z)) return false;
                y = y - 127;
            }
            while (y < -128) {
                if (!mouseSend(0, -128, button, z)) return false;
                y = y + 128;
            }
            return mouseSend(x, y, button, z);
        case ABS_MOUSE:
            HID_REPORT report;

            report.data[0] = x & 0xff;
            report.data[1] = (x >> 8) & 0xff;
            report.data[2] = y & 0xff;
            report.data[3] = (y >> 8) & 0xff;
            report.data[4] = -z;
            report.data[5] = button & 0x07;

            report.length = 6;

            return send(&report);
        default:
            return false;
    }
}

bool USBMouse::mouseSend(int8_t x, int8_t y, uint8_t buttons, int8_t z) {
    HID_REPORT report;
    report.data[0] = buttons & 0x07;
    report.data[1] = x;
    report.data[2] = y;
    report.data[3] = -z; // >0 to scroll down, <0 to scroll up

    report.length = 4;

    return send(&report);
}

bool USBMouse::move(int16_t x, int16_t y) {
    return update(x, y, button, 0);
}

bool USBMouse::scroll(int8_t z) {
    return update(0, 0, button, z);
}


bool USBMouse::doubleClick() {
    if (!click(MOUSE_LEFT))
        return false;
    //wait(0.1);
    return click(MOUSE_LEFT);
}

bool USBMouse::click(uint8_t button) {
    if (!update(0, 0, button, 0))
        return false;
    //wait(0.01);
    return update(0, 0, 0, 0);
}

bool USBMouse::press(uint8_t button_) {
    button = button_ & 0x07;
    return update(0, 0, button, 0);
}

bool USBMouse::release(uint8_t button_) {
    button = (button & (~button_)) & 0x07;
    return update(0, 0, button, 0);
}


uint8_t * USBMouse::reportDesc() {

    if (mouse_type == REL_MOUSE) {
        static uint8_t reportDescriptor[] = {
            HID_USAGE_PAGE(1),      0x01,       // Genric Desktop
            HID_USAGE(1),           0x02,       // Mouse
            HID_COLLECTION(1),      0x01,       // Application
            HID_USAGE(1),           0x01,       // Pointer
            HID_COLLECTION(1),      0x00,       // Physical

            HID_REPORT_COUNT(1),    0x03,
            HID_REPORT_SIZE(1),     0x01,
            HID_USAGE_PAGE(1),      0x09,       // Buttons
            HID_USAGE_MINIMUM(1),       0x1,
            HID_USAGE_MAXIMUM(1),       0x3,
            HID_LOGICAL_MINIMUM(1),     0x00,
            HID_LOGICAL_MAXIMUM(1),     0x01,
            HID_INPUT(1),           0x02,
            HID_REPORT_COUNT(1),    0x01,
            HID_REPORT_SIZE(1),     0x05,
            HID_INPUT(1),           0x01,

            HID_REPORT_COUNT(1),    0x03,
            HID_REPORT_SIZE(1),     0x08,
            HID_USAGE_PAGE(1),      0x01,
            HID_USAGE(1),           0x30,       // X
            HID_USAGE(1),           0x31,       // Y
            HID_USAGE(1),           0x38,       // scroll
            HID_LOGICAL_MINIMUM(1),     0x81,
            HID_LOGICAL_MAXIMUM(1),     0x7f,
            HID_INPUT(1),           0x06,       // Relative data

            HID_END_COLLECTION(0),
            HID_END_COLLECTION(0),
        };
        reportLength = sizeof(reportDescriptor);
        return reportDescriptor;
    } else if (mouse_type == ABS_MOUSE) {
        static uint8_t reportDescriptor[] = {

            HID_USAGE_PAGE(1), 0x01,           // Generic Desktop
            HID_USAGE(1), 0x02,                // Mouse
            HID_COLLECTION(1), 0x01,           // Application
            HID_USAGE(1), 0x01,                // Pointer
            HID_COLLECTION(1), 0x00,           // Physical

            HID_USAGE_PAGE(1), 0x01,            // Generic Desktop
            HID_USAGE(1), 0x30,                 // X
            HID_USAGE(1), 0x31,                 // Y
            HID_LOGICAL_MINIMUM(1), 0x00,       // 0
            HID_LOGICAL_MAXIMUM(2), 0xff, 0x7f, // 32767
            HID_REPORT_SIZE(1), 0x10,
            HID_REPORT_COUNT(1), 0x02,
            HID_INPUT(1), 0x02,                 // Data, Variable, Absolute

            HID_USAGE_PAGE(1), 0x01,            // Generic Desktop
            HID_USAGE(1), 0x38,                 // scroll
            HID_LOGICAL_MINIMUM(1), 0x81,       // -127
            HID_LOGICAL_MAXIMUM(1), 0x7f,       // 127
            HID_REPORT_SIZE(1), 0x08,
            HID_REPORT_COUNT(1), 0x01,
            HID_INPUT(1), 0x06,                 // Data, Variable, Relative

            HID_USAGE_PAGE(1), 0x09,            // Buttons
            HID_USAGE_MINIMUM(1), 0x01,
            HID_USAGE_MAXIMUM(1), 0x03,
            HID_LOGICAL_MINIMUM(1), 0x00,       // 0
            HID_LOGICAL_MAXIMUM(1), 0x01,       // 1
            HID_REPORT_COUNT(1), 0x03,
            HID_REPORT_SIZE(1), 0x01,
            HID_INPUT(1), 0x02,                 // Data, Variable, Absolute
            HID_REPORT_COUNT(1), 0x01,
            HID_REPORT_SIZE(1), 0x05,
            HID_INPUT(1), 0x01,                 // Constant

            HID_END_COLLECTION(0),
            HID_END_COLLECTION(0)
        };
        reportLength = sizeof(reportDescriptor);
        return reportDescriptor;
    }
    return NULL;
}


void USBMouse::on_module_loaded()
{
    this->register_for_event(ON_MAIN_LOOP);
}

void USBMouse::on_main_loop(void *argument)
{
    move(1,0);
}

bool USBMouse::USBEvent_EPIn(uint8_t, uint8_t) {
    THEKERNEL->streams->printf("EPIn called!\n");
    return false;}
bool USBMouse::USBEvent_EPOut(uint8_t, uint8_t) {
    THEKERNEL->streams->printf("EPOut called!\n");
    return false;}