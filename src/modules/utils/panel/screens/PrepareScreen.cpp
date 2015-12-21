/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#include "libs/Kernel.h"
#include "Panel.h"
#include "PanelScreen.h"
#include "LcdBase.h"
#include "PrepareScreen.h"
#include "ExtruderScreen.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "checksumm.h"
#include "PublicDataRequest.h"
#include "PublicData.h"
#include "TemperatureControlPublicAccess.h"

#include <string>
using namespace std;

PrepareScreen::PrepareScreen()
{
    this->command = nullptr;

    // Children screens
    if(THEPANEL->temperature_screen != nullptr) {
        this->extruder_screen = (new ExtruderScreen())->set_parent(this);
        THEPANEL->temperature_screen->set_parent(this);
    }else{
        this->extruder_screen= nullptr;
    }
}

void PrepareScreen::on_enter()
{
    THEPANEL->enter_menu_mode();
    // if no heaters or extruder then don't show related menu items
    //THEPANEL->setup_menu((THEPANEL->temperature_screen != nullptr) ? 9 : 5);
    THEPANEL->setup_menu(14);
    this->refresh_menu();
}

void PrepareScreen::on_refresh()
{
    if ( THEPANEL->menu_change() ) {
        this->refresh_menu();
    }
    if ( THEPANEL->click() ) {
        this->clicked_menu_entry(THEPANEL->get_menu_current_line());
    }
}

void PrepareScreen::display_menu_line(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->lcd->printf("Back"           ); break;
        case 1: THEPANEL->lcd->printf("Preheat PLA 200"); break;
        case 2: THEPANEL->lcd->printf("Preheat ABS 245"); break;
        case 3: THEPANEL->lcd->printf("Home All Axis"  ); break;
        case 4: THEPANEL->lcd->printf("Extrude/Retract"); break;
        case 5: THEPANEL->lcd->printf("Adjust Temperature"); break;
        case 6: THEPANEL->lcd->printf("Flush Nozzles"  ); break;
        case 7: THEPANEL->lcd->printf("Cool Down"      ); break;
        case 8: THEPANEL->lcd->printf("Park Extruder"  ); break;
        case 9: THEPANEL->lcd->printf("Home To Center" ); break;
        case 10: THEPANEL->lcd->printf("Home X"         ); break;
        case 11: THEPANEL->lcd->printf("Home Y"         ); break;
        case 12: THEPANEL->lcd->printf("Home Z"         ); break;
        case 13: THEPANEL->lcd->printf("Motors OFF"     ); break;
        //case 2: THEPANEL->lcd->printf("Set Home"       ); break;
        //case 3: THEPANEL->lcd->printf("Set Z0"         ); break;
        //case 4: THEPANEL->lcd->printf("Pre Heat"       ); break;
    }
}

void PrepareScreen::clicked_menu_entry(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->enter_screen(this->parent); break;
        case 1: command = "T1\nM104 S200\nG4 P1\nT0\nM104 S200\nM140 S60"; break;
        case 2: command = "T1\nM104 S245\nG4 P1\nT0\nM104 S245\nM140 S100"; break;
        case 3:  command = "G91\nG0 Z2 F1000\nG90\nG28 X0 Y0\nG28 Z0"; break;
        case 4: THEPANEL->enter_screen(this->extruder_screen); break;
        case 5: THEPANEL->enter_screen(THEPANEL->temperature_screen); break;
        case 6: command = "T0\nG92 E0\nG1 E35 F300\nG92 E0\nT1\nG92 E0\nG1 E35 F300\nG92 E0\nT0"; break;
        case 7: this->cooldown(); break;
        case 8: command = "G28 X0 Y0 Z0\nG0 X25 Y 25 Z20"; break;
        case 9:  command = "G91\nG0 Z2 F1000\nG90\nG28 X0\nG28 Y0\nG0 X115 Y125 F2000\nG28 Z0"; break;
        case 10: command = "G28 X0"; break;
        case 11: command = "G28 Y0"; break;
        case 12: command = "G28 Z0"; break;
        case 13: command = "M84"; break;
        //case 2: command = "G92 X0 Y0 Z0"; break;
        //case 3: command = "G92 Z0"; break;
        //case 4: this->preheat(); break;
    }
}

void PrepareScreen::preheat()
{
    float t = THEPANEL->get_default_hotend_temp();
    PublicData::set_value( temperature_control_checksum, hotend_checksum, &t );
    PublicData::set_value( temperature_control_checksum, hotend2_checksum, &t );
    t = THEPANEL->get_default_bed_temp();
    PublicData::set_value( temperature_control_checksum, bed_checksum, &t );
}

void PrepareScreen::cooldown()
{
    float t = 0;
    PublicData::set_value( temperature_control_checksum, hotend_checksum, &t );
    PublicData::set_value( temperature_control_checksum, hotend2_checksum, &t );
    PublicData::set_value( temperature_control_checksum, bed_checksum, &t );
}

// queuing commands needs to be done from main loop
void PrepareScreen::on_main_loop()
{
    // change actual axis value
    if (this->command == nullptr) return;
    send_command(this->command);
    this->command = nullptr;
}
