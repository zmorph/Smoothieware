#ifndef _DESCRIPTOR_HID_H
#define _DESCRIPTOR_HID_H

#include <stdint.h>

/* */
#define HID_VERSION_1_11    (0x0111)

/* HID Class */
#define HID_CLASS           (3)
#define HID_SUBCLASS_NONE   (0)
#define HID_PROTOCOL_NONE   (0)

/* Descriptors */
#define HID_DESCRIPTOR          (33)
#define HID_DESCRIPTOR_LENGTH   (0x09)
#define HID_REPORT_DESCRIPTOR       (34)

/* Class requests */
#define HID_GET_REPORT (0x1)
#define HID_GET_IDLE   (0x2)
#define HID_SET_REPORT (0x9)
#define HID_SET_IDLE   (0xa)

/* HID Class Report Descriptor */
/* Short items: size is 0, 1, 2 or 3 specifying 0, 1, 2 or 4 (four) bytes */
/* of data as per HID Class standard */

/* Main items */
#define HID_INPUT(size)             (0x80 | size)
#define HID_OUTPUT(size)            (0x90 | size)
#define HID_FEATURE(size)           (0xb0 | size)
#define HID_COLLECTION(size)        (0xa0 | size)
#define HID_END_COLLECTION(size)    (0xc0 | size)

/* Global items */
#define HID_USAGE_PAGE(size)        (0x04 | size)
#define HID_LOGICAL_MINIMUM(size)   (0x14 | size)
#define HID_LOGICAL_MAXIMUM(size)   (0x24 | size)
#define HID_PHYSICAL_MINIMUM(size)  (0x34 | size)
#define HID_PHYSICAL_MAXIMUM(size)  (0x44 | size)
#define HID_UNIT_EXPONENT(size)     (0x54 | size)
#define HID_UNIT(size)              (0x64 | size)
#define HID_REPORT_SIZE(size)       (0x74 | size)
#define HID_EPORT_COUNT(size)      (0x94 | size)
#define HID_PUSH(size)              (0xa4 | size)
#define HID_POP(size)               (0xb4 | size)

/* Local items */
#define HID_USAGE(size)                 (0x08 | size)
#define HID_USAGE_MINIMUM(size)         (0x18 | size)
#define HID_USAGE_MAXIMUM(size)         (0x28 | size)
#define HID_DESIGNATOR_INDEX(size)      (0x38 | size)
#define HID_DESIGNATOR_MINIMUM(size)    (0x48 | size)
#define HID_DESIGNATOR_MAXIMUM(size)    (0x58 | size)
#define HID_STRING_INDEX(size)          (0x78 | size)
#define HID_STRING_MINIMUM(size)        (0x88 | size)
#define HID_STRING_MAXIMUM(size)        (0x98 | size)
#define HID_DELIMITER(size)             (0xa8 | size)

/* HID Report */
/* Where report IDs are used the first byte of 'data' will be the */
/* report ID and 'length' will include this report ID byte. */

#define MAX_HID_REPORT_SIZE (64)

typedef struct {
    uint32_t length;
    uint8_t data[MAX_HID_REPORT_SIZE];
} HID_REPORT;


typedef struct __attribute__ ((packed)) {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wcdHID;

	uint8_t bCountryCode;
	uint8_t bNumDescriptors;
	uint8_t bDescriptorType2;
	uint16_t wDescriptorLength;
} usbhid_report;

#endif
