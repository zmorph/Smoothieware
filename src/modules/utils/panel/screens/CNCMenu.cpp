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
#include "CNCMenu.h"
#include "ExtruderScreen.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "checksumm.h"
#include "PublicDataRequest.h"
#include "PublicData.h"
#include "TemperatureControlPublicAccess.h"
#include "ModifyValuesScreen.h"
#include "PrepareScreen.h"


#include <string>
using namespace std;

CNCMenu::CNCMenu()
{
    this->command = nullptr;
   if(THEPANEL->cnc_power_screen != nullptr)
       THEPANEL->cnc_power_screen->set_parent(this);
}

void CNCMenu::on_enter()
{
    THEPANEL->enter_menu_mode();
    THEPANEL->setup_menu(10);
    this->refresh_menu();
}

void CNCMenu::on_refresh()
{
    if ( THEPANEL->menu_change() ) {
        this->refresh_menu();
    }
    if ( THEPANEL->click() ) {
        this->clicked_menu_entry(THEPANEL->get_menu_current_line());
    }
}

void CNCMenu::display_menu_line(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->lcd->printf("Back"             ); break;
        case 1: THEPANEL->lcd->printf("Spindle ON"       ); break;
        case 2: THEPANEL->lcd->printf("Set spindle power"); break;
        case 3: THEPANEL->lcd->printf("Spindle OFF"      ); break;
        case 4: THEPANEL->lcd->printf("Home All Axis"    ); break;
        case 5: THEPANEL->lcd->printf("Home To Center"   ); break;
        case 6: THEPANEL->lcd->printf("Home X"           ); break;
        case 7: THEPANEL->lcd->printf("Home Y"           ); break;
        case 8: THEPANEL->lcd->printf("Home Z"           ); break;
        case 9: THEPANEL->lcd->printf("Motors OFF"       ); break;
    }
}

void CNCMenu::clicked_menu_entry(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->enter_screen(this->parent); break;
        case 1: command = "M3 S255"; break;
        case 2: THEPANEL->enter_screen(THEPANEL->cnc_power_screen); break;
        case 3: command = "M5"; break;
        case 4: command = "G91\nG0 Z2 F1000\nG90\nG28 X0 Y0\nG28 Z0"; break;
        case 5: command = "G91\nG0 Z2 F1000\nG90\nG28 X0\nG28 Y0\nG0 X115 Y125 F2000\nG28 Z0"; break;
        case 6: command = "G28 X0"; break;
        case 7: command = "G28 Y0"; break;
        case 8: command = "G28 Z0"; break;
        case 9: command = "M84"; break;
    }
}

// queuing commands needs to be done from main loop
void CNCMenu::on_main_loop()
{
    // change actual axis value
    if (this->command == nullptr) return;
    send_command(this->command);
    this->command = nullptr;
}
