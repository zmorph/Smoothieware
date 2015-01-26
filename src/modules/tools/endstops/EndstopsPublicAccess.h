#ifndef __ENDSTOPSPUBLICACCESS_H_
#define __ENDSTOPSPUBLICACCESS_H_

// addresses used for public data access
#define endstops_checksum    CHECKSUM("endstop")
#define trim_checksum        CHECKSUM("trim")
#define home_offset_checksum CHECKSUM("home_offset")
#define homed_axes_checksum  CHECKSUM("homed_axes")
#endif
