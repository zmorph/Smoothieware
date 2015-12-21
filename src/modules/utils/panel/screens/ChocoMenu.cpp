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
#include "ChocoMenu.h"
#include "ExtruderScreen.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "checksumm.h"
#include "PublicDataRequest.h"
#include "PublicData.h"
#include "TemperatureControlPublicAccess.h"

#include <string>
using namespace std;

ChocoMenu::ChocoMenu()
{
    this->command = nullptr;
}

void ChocoMenu::on_enter()
{
    THEPANEL->enter_menu_mode();
    THEPANEL->setup_menu(9);
    this->refresh_menu();
}

void ChocoMenu::on_refresh()
{
    if ( THEPANEL->menu_change() ) {
        this->refresh_menu();
    }
    if ( THEPANEL->click() ) {
        this->clicked_menu_entry(THEPANEL->get_menu_current_line());
    }
}

void ChocoMenu::display_menu_line(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->lcd->printf("Back"           ); break;
        case 1: THEPANEL->lcd->printf("Extrude"        ); break;
        case 2: THEPANEL->lcd->printf("Retract"        ); break;
        case 3: THEPANEL->lcd->printf("Home All Axis"  ); break;
        case 4: THEPANEL->lcd->printf("Home To Center" ); break;
        case 5: THEPANEL->lcd->printf("Home X"         ); break;
        case 6: THEPANEL->lcd->printf("Home Y"         ); break;
        case 7: THEPANEL->lcd->printf("Home Z"         ); break;
        case 8: THEPANEL->lcd->printf("Motors OFF"     ); break;
    }
}

void ChocoMenu::clicked_menu_entry(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->enter_screen(this->parent); break;
        case 1: command = "T0\nG91\nG1 E15 F200\nG90"; break;
        case 2: command = "T0\nG91\nG1 E-15 F200\nG90"; break;
        case 3: command = "G91\nG0 Z2 F1000\nG90\nG28 X0 Y0\nG28 Z0"; break;
        case 4: command = "G91\nG0 Z2 F1000\nG90\nG28 X0\nG28 Y0\nG0 X115 Y125 F2000\nG28 Z0"; break;
        case 5: command = "G28 X0"; break;
        case 6: command = "G28 Y0"; break;
        case 7: command = "G28 Z0"; break;
        case 8: command = "M84"; break;
    }
}

// queuing commands needs to be done from main loop
void ChocoMenu::on_main_loop()
{
    // change actual axis value
    if (this->command == nullptr) return;
    send_command(this->command);
    this->command = nullptr;
}
