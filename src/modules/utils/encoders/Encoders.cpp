#include "Encoders.h"
#include "libs/Kernel.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "ConfigValue.h"
#include "libs/StreamOutput.h"

#include "Gcode.h"
#include "Config.h"
#include "checksumm.h"
#include "DigipotBase.h"
#include "cstdio"


#include "libs/Kernel.h"

#include "libs/nuts_bolts.h"
#include "libs/utils.h"

#include "PublicDataRequest.h"
#include "PublicData.h"

// add new encoders here
#include "AS5510.h"

#include <string>
using namespace std;

#define encoders_module_enable_checksum         CHECKSUM("encoders_module_enable")

Encoders::Encoders()
{
    encoders = NULL;
}

void Encoders::on_module_loaded()
{
    if( !THEKERNEL->config->value( encoders_module_enable_checksum )->by_default(false)->as_bool() ) {
        // as this module is not needed free up the resource
        delete this;
        return;
    }

    // allocate encoder, if already allocated delete it first
    delete encoders;

    // see which chip to use

    encoders = new AS5510();

    this->register_for_event(ON_GCODE_RECEIVED);
    this->register_for_event(ON_MAIN_LOOP);
}

void Encoders::send_gcode(std::string g)
{
    Gcode gcode(g, &(StreamOutput::NullStream));
    THEKERNEL->call_event(ON_GCODE_RECEIVED, &gcode );
}

void Encoders::on_gcode_received(void *argument)
{
    Gcode *gcode = static_cast<Gcode*>(argument);
    if (gcode->has_m) {
        if (gcode->m == 700) 
        {
            send_gcode("G0 Z10");
            int val = 0;
            char *str;
 //           val = this->encoders->get_position_value();
            sprintf(str, "M117 %d", val);
            this->command = str;
            gcode->stream->printf(this->command);

            send_gcode(command);
        }
    }
}

void Encoders::on_main_loop()
{
    // change actual axis value
//if (this->command == nullptr) return;
    // send_command(this->command);
    // this->command = nullptr;

    // char buf[32];
    // int n = snprintf(buf, sizeof(buf), "M117 %d", val);
    // string g(buf, n);
 //   this->command = nullptr;
}