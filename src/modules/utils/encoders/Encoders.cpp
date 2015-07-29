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

#include "libs/SlowTicker.h"

#include "libs/Kernel.h"

#include "libs/nuts_bolts.h"
#include "libs/utils.h"

#include "PublicDataRequest.h"
#include "PublicData.h"

// add new encoders here
#include "AS5048B.h"

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

    encoders = new AS5048B();

    this->register_for_event(ON_GCODE_RECEIVED);
    this->register_for_event(ON_MAIN_LOOP);

  //  THEKERNEL->slow_ticker->attach(1, this, &Encoders::periodic_encoder_check);
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
            unsigned int val = 0;
            char buffer [30];
            val = this->encoders->get_position_value();
            sprintf(buffer, "encoder value is %d", val);
            gcode->stream->printf(buffer);
        }
    }
}

uint32_t Encoders::periodic_encoder_check(uint32_t dummy)
{
    unsigned int val = 0;
    val = this->encoders->get_position_value();
    sprintf(this->command, "M117 %d", val);
    //send_gcode("M117 this->command");
    return 0;
}

void Encoders::on_main_loop()
{
  //      send_gcode(this->command);
  //      sprintf(this->command, "");
}