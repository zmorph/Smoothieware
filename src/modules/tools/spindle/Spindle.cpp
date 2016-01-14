/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "libs/Module.h"
#include "libs/Kernel.h"
#include "Spindle.h"
#include "Config.h"
#include "libs/nuts_bolts.h"
#include "checksumm.h"
#include "ConfigValue.h"
#include "Gcode.h"
#include "StreamOutputPool.h"
#include "SlowTicker.h"
#include "Conveyor.h"
#include "system_LPC17xx.h"

#include "libs/Pin.h"
#include "InterruptIn.h"
#include "PwmOut.h"
#include "port_api.h"
#include "us_ticker_api.h"

#define spindle_enable_checksum          CHECKSUM("spindle_enable")
#define spindle_pwm_pin_checksum         CHECKSUM("spindle_pwm_pin")
#define spindle_pwm_period_checksum      CHECKSUM("spindle_pwm_period")
#define spindle_feedback_pin_checksum    CHECKSUM("spindle_feedback_pin")
#define spindle_pulses_per_rev_checksum  CHECKSUM("spindle_pulses_per_rev")
#define spindle_default_rpm_checksum     CHECKSUM("spindle_default_rpm")
#define spindle_control_P_checksum       CHECKSUM("spindle_control_P")
#define spindle_control_I_checksum       CHECKSUM("spindle_control_I")
#define spindle_control_D_checksum       CHECKSUM("spindle_control_D")
#define spindle_control_smoothing_checksum CHECKSUM("spindle_control_smoothing")

#define UPDATE_FREQ 1000

Spindle::Spindle()
{
}

void Spindle::on_module_loaded()
{
   spindle_on = true;

    if (!THEKERNEL->config->value(spindle_enable_checksum)->by_default(true)->as_bool())
    {
      delete this; // Spindle control module is disabled
      return;
    }

    // Get the pin for hardware pwm
    {
        Pin *smoothie_pin = new Pin();
        smoothie_pin->from_string(THEKERNEL->config->value(spindle_pwm_pin_checksum)->by_default("nc")->as_string());
        spindle_pin = smoothie_pin->as_output()->hardware_pwm();
        output_inverted = smoothie_pin->inverting;
        delete smoothie_pin;
    }

    if (spindle_pin == NULL)
    {
        THEKERNEL->streams->printf("Error: Spindle PWM pin must be P2.0-2.5 or other PWM pin\n");
        delete this;
        return;
    }

    int period = THEKERNEL->config->value(spindle_pwm_period_checksum)->by_default(1000)->as_int();
    spindle_pin->period_us(period);
    spindle_pin->write(output_inverted ? 1 : 0);


    THEKERNEL->slow_ticker->attach(UPDATE_FREQ, this, &Spindle::on_update_speed);
    register_for_event(ON_GCODE_RECEIVED);
    register_for_event(ON_GCODE_EXECUTE);
}

void Spindle::on_pin_rise()
{
    
}

uint32_t Spindle::on_update_speed(uint32_t dummy)
{
    return 0;
}


void Spindle::on_gcode_received(void* argument)
{
    Gcode *gcode = static_cast<Gcode *>(argument);

    if (gcode->has_m)
    {
        if (gcode->m == 3 || gcode->m == 5)
        {
            // M3: Spindle on, M5: Spindle off
            THEKERNEL->conveyor->append_gcode(gcode);
        }
    }
}

void Spindle::on_gcode_execute(void* argument)
{
    Gcode *gcode = static_cast<Gcode *>(argument);

    if (gcode->has_m)
    {
        if (gcode->m == 3)
        {
            // M3: Spindle on
            spindle_on = true;

            if (gcode->has_letter('S'))
            {
                target_rpm = gcode->get_value('S');
            }
        }
        else if (gcode->m == 5)
        {
            spindle_on = false;
        }
    }
}

float Spindle::get_current_rpm()
{
    return target_rpm;
}