#ifndef CURRENTCONTROL_H
#define CURRENTCONTROL_H

#include "Module.h"
#define mcp4728_adress                          CHECKSUM("mcp4728_adress")
#define mcp4726_adress                          CHECKSUM("mcp4726_adress")

class DigipotBase;
#define mcp4728_checksum                        CHECKSUM("mcp4728")

class CurrentControl : public Module {
    public:
        CurrentControl();
        virtual ~CurrentControl() {};

        void on_module_loaded();
        void on_gcode_received(void *);

    private:
        DigipotBase* digipot;

};





#endif
