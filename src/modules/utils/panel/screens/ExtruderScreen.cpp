/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "libs/Kernel.h"
#include "libs/SerialMessage.h"
#include "Panel.h"
#include "PanelScreen.h"
#include "ExtruderScreen.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "modules/tools/temperaturecontrol/TemperatureControlPublicAccess.h"
#include "PublicData.h"
#include "SwitchPublicAccess.h"
#include "checksumm.h"
#include <math.h>
#include <string>

using namespace std;


ExtruderScreen::ExtruderScreen()
{
}


void ExtruderScreen::on_enter()
{
    int menu_line = 5;
    THEPANEL->enter_menu_mode();
    get_temp_data();
    if ( this->hotendtemp == -2 ) {
        menu_line -= 2;
    }
    if ( this->hotend2temp == -2 ) {
        menu_line -= 2;
    }
    THEPANEL->setup_menu(menu_line);
    this->refresh_menu();
}

void ExtruderScreen::on_refresh()
{
    if ( THEPANEL->menu_change() ) {
        this->refresh_menu();
    }
    if ( THEPANEL->click() ) {
        this->clicked_menu_entry(THEPANEL->get_menu_current_line());
    }

    // Update Only every 20 refreshes, 1 a second
    static int update_counts = 0;
    update_counts++;
    if ( update_counts % 20 == 0 ) {
        get_temp_data();
    }
}

void ExtruderScreen::get_temp_data() 
{
    void *returned_data;
    bool ok;

    ok = PublicData::get_value( temperature_control_checksum, bed_checksum, current_temperature_checksum, &returned_data );
    if (ok) {
        struct pad_temperature temp =  *static_cast<struct pad_temperature *>(returned_data);
        this->bedtemp = round(temp.current_temperature);
        if (this->bedtemp > 100000) this->bedtemp = -2;
    } else {
        this->bedtemp = -1;
    }

    ok = PublicData::get_value( temperature_control_checksum, hotend_checksum, current_temperature_checksum, &returned_data );
    if (ok) {
        struct pad_temperature temp =  *static_cast<struct pad_temperature *>(returned_data);
        this->hotendtemp = round(temp.current_temperature);
        if (this->hotendtemp > 100000) this->hotendtemp = -2;
    } else {
        this->hotendtemp = -1;
    }

    ok = PublicData::get_value( temperature_control_checksum, hotend2_checksum, current_temperature_checksum, &returned_data );
    if (ok) {
        struct pad_temperature temp =  *static_cast<struct pad_temperature *>(returned_data);
        this->hotend2temp = round(temp.current_temperature);
        if (this->hotend2temp > 100000) this->hotend2temp = -2;
    } else {
        this->hotend2temp = -1;
    }
}


void ExtruderScreen::display_menu_line(uint16_t line)
{
    char extrude_H_text[19]{}, retract_H_text[19]{};
    char extrude_Q_text[19]{}, retract_Q_text[19]{};
    bool hotend_enabled{true};
    bool hotend2_enabled{true};
    if ( this->hotendtemp != -2 ) {
        sprintf(extrude_H_text, "Extrude H 5mm      ");
        sprintf(retract_H_text, "Retract H 5mm      ");
    } else {
        hotend_enabled = false;
    }
    if ( this->hotend2temp != -2 ) {
        sprintf(extrude_Q_text, "Extrude Q 5mm      ");
        sprintf(retract_Q_text, "Retract Q 5mm      ");
    } else {
        hotend2_enabled = false;
    }

    if (!hotend_enabled && !hotend2_enabled && line > 0)
        return;

    if ((!hotend_enabled || !hotend2_enabled) && line > 2)
        return;

    switch ( line ) {
        case 0: THEPANEL->lcd->printf("Back", this->hotendtemp);  break;
        case 1: THEPANEL->lcd->printf("%19s", hotend_enabled?extrude_H_text:extrude_Q_text); break;
        case 2: THEPANEL->lcd->printf("%19s", hotend_enabled?retract_H_text:retract_Q_text); break;
        case 3: THEPANEL->lcd->printf("%19s", extrude_Q_text); break;
        case 4: THEPANEL->lcd->printf("%19s", retract_Q_text); break;
        default: break;
    }
}

void ExtruderScreen::clicked_menu_entry(uint16_t line)
{
    bool hotend_enabled{this->hotendtemp!=-2};
    bool hotend2_enabled{this->hotend2temp!=-2};

    if (!hotend_enabled && hotend2_enabled && line > 0)
        line+=2;

    switch ( line ) {
        case 0: THEPANEL->enter_screen(this->parent); return;
        case 1: command = "T0\nG91\nG1 E5 F100\nG90"; break;
        case 2: command = "T0\nG91\nG1 E-5 F100\nG90"; break;
        case 3: command = "T1\nG91\nG1 E5 F100\nG90"; break;
        case 4: command = "T1\nG91\nG1 E-5 F100\nG90"; break;
        default : break;
    }
}

// queuing commands needs to be done from main loop
void ExtruderScreen::on_main_loop()
{
    if (this->command.empty()) return;
    send_command(this->command.c_str());
    this->command.clear();
}
