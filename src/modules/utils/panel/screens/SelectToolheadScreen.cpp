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
#include "SelectToolheadScreen.h"
#include "ExtruderScreen.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "checksumm.h"
#include "PublicDataRequest.h"
#include "PublicData.h"
#include "TemperatureControlPublicAccess.h"
#include "modules/tools/laser/Laser.h"

#include <string>
using namespace std;


#define extruder_checksum CHECKSUM("extruder")

//OprionsScreen because ConfigureScreen is already used in Smoothie.
SelectToolheadScreen::SelectToolheadScreen()
{
    this->command = nullptr;
}

void SelectToolheadScreen::on_enter()
{
    THEPANEL->enter_menu_mode();
    // if no heaters or extruder then don't show related menu items
    //THEPANEL->setup_menu((THEPANEL->temperature_screen != nullptr) ? 9 : 5);
    
    THEPANEL->setup_menu(12);

     this->refresh_menu();
}

void SelectToolheadScreen::on_refresh()
{
    if ( THEPANEL->menu_change() ) {
        this->refresh_menu();
    }
    if ( THEPANEL->click() ) {
        this->clicked_menu_entry(THEPANEL->get_menu_current_line());
    }
}

void SelectToolheadScreen::display_menu_line(uint16_t line)
{
    switch ( line ) {
        case 0: {
            THEPANEL->lcd->printf("Back");
            break; }
        case 1: {
            if(THEPANEL->get_selected_toolhead() != TOOLHEAD_EXTRU175 && THEPANEL->get_selected_toolhead() != TOOLHEAD_TIMELAPSE_EXTRU175) {
                THEPANEL->lcd->printf("Single Head 1.75"  );
            } else {
                THEPANEL->lcd->printf("> Single Head 1.75");
            }
            break; }
        case 2: {
            if(THEPANEL->get_selected_toolhead() != TOOLHEAD_EXTRU300 && THEPANEL->get_selected_toolhead() != TOOLHEAD_TIMELAPSE_EXTRU300) {
                THEPANEL->lcd->printf("Single Head 3.00"  );
            } else {
                THEPANEL->lcd->printf("> Single Head 3.00");
            }
            break; }
        case 3: {
            if(THEPANEL->get_selected_toolhead() != TOOLHEAD_DUALPRO) {
                THEPANEL->lcd->printf("Dual PRO"  );
            } else {
                THEPANEL->lcd->printf("> Dual PRO");
            }
            break; }
        case 4: {
            if(THEPANEL->get_selected_toolhead() != TOOLHEAD_DUAL) {
                THEPANEL->lcd->printf("DualHead 1.75"    );
            } else {
                THEPANEL->lcd->printf("> DualHead 1.75"  );
            }
            break; }
        case 5: {
            if(THEPANEL->get_selected_toolhead() != TOOLHEAD_CHOCO && THEPANEL->get_selected_toolhead() != TOOLHEAD_TIMELAPSE_CHOCO) {
                THEPANEL->lcd->printf("Chocolate"         );
            } else {
                THEPANEL->lcd->printf("> Chocolate"       );
            }
            break; }
        case 6: {
            if(THEPANEL->get_selected_toolhead() != TOOLHEAD_CERAMICS && THEPANEL->get_selected_toolhead() != TOOLHEAD_TIMELAPSE_CERAMICS) {
                THEPANEL->lcd->printf("Ceramics (syringe)" );
            } else {
                THEPANEL->lcd->printf("> Ceramics (syringe");
            }
            break; }
        case 7: {
            if(THEPANEL->get_selected_toolhead() != TOOLHEAD_LASER) {
                THEPANEL->lcd->printf("Laser"   );
            } else {
                THEPANEL->lcd->printf("> Laser" );
            }
            break; }
        case 8: {
            if(THEPANEL->get_selected_toolhead() != TOOLHEAD_CNCPRO  && THEPANEL->get_selected_toolhead() != TOOLHEAD_TIMELAPSE_CNCPRO) {
                THEPANEL->lcd->printf("CNC PRO"          );
            } else {
                THEPANEL->lcd->printf("> CNC PRO"        );
            }
            break; }
        case 9: {
            if(THEPANEL->get_selected_toolhead() != TOOLHEAD_5AXIS) {
                THEPANEL->lcd->printf("5-Axis"            );
            } else {
                THEPANEL->lcd->printf("> 5-Axis"          );
            }
            break; }
        case 10: {
            if(THEPANEL->get_selected_toolhead() != TOOLHEAD_SCANNER) {
                THEPANEL->lcd->printf("3D Scanner"            );
            } else {
                THEPANEL->lcd->printf("> 3D Scanner"          );
            }
            break; }
        case 11: {
            if(THEPANEL->get_selected_toolhead() != TOOLHEAD_TIMELAPSE) {
                THEPANEL->lcd->printf("Time-lapse Rail"            );
            } else {
                THEPANEL->lcd->printf("> Time-lapse Rail"          );
            }
        }
    }
}

void SelectToolheadScreen::clicked_menu_entry(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->enter_screen(this->parent); break;
        case 1: {
            command = "T0\nM92 E100\nM907 E1.25\nG4 P100\nconfig-set sd delta_current 1.25 \nG4 P100\nconfig-set sd extruder.hotend.steps_per_mm 100 \nG4 P1000";
            THEPANEL->set_toolhead(TOOLHEAD_EXTRU175);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_FILAMENT);
            this->refresh_menu();
            break; }
        case 2: {
            command = "T0\nM92 E400\nM907 E1.25\nG4 P100\nconfig-set sd delta_current 1.25 \nG4 P100\nconfig-set sd extruder.hotend.steps_per_mm 400 \nG4 P1000";
            THEPANEL->set_toolhead(TOOLHEAD_EXTRU300);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_FILAMENT);
            this->refresh_menu();
            break; }
        case 3: {
            command = "T1\nM92 E901\nM907 A0.5\nM222 X0 Y0\nT0\nM92 E901\nM907 E0.5\nG4 P100\nconfig-set sd delta_current 0.5 \nG4 P1000\nG4 P100\nconfig-set sd epsilon_current 0.5 \nG4 P100\nconfig-set sd extruder.hotend.steps_per_mm 901 \nG4 P100\nconfig-set sd extruder.hotend2.steps_per_mm 901\nG4 P100\nconfig-set sd extruder.hotend2.x_offset 0\nG4 P100\nconfig-set sd extruder.hotend2.y_offset 0\nG4 P1000";
            THEPANEL->set_toolhead(TOOLHEAD_DUALPRO);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_FILAMENT);
            this->refresh_menu();
            break; }
        case 4: {
            command = "T1\nM92 E900\nM907 A0.5\nM222 X-11.25 Y0\nT0\nM92 E900\nM907 E0.5\nG4 P100\nconfig-set sd delta_current 0.5 \nG4 P1000\nG4 P100\nconfig-set sd epsilon_current 0.5 \nG4 P100\nconfig-set sd extruder.hotend.steps_per_mm 900 \nG4 P100\nconfig-set sd extruder.hotend2.steps_per_mm 900\nG4 P100\nconfig-set sd extruder.hotend2.x_offset -11.25\nG4 P1000";
            THEPANEL->set_toolhead(TOOLHEAD_DUAL);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_FILAMENT);
            this->refresh_menu();
            break; }
        case 5: {
            command = "T0\nM92 E401\nM907 E1.0\nG4 P100\nconfig-set sd delta_current 1.0 \nG4 P100\nconfig-set sd extruder.hotend.steps_per_mm 401 \nG4 P1000";
            THEPANEL->set_toolhead(TOOLHEAD_CHOCO);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_CHOCO);
            this->refresh_menu();
            break; }
        case 6: {
            command = "T0\nM92 E402\nM907 E0.8\nG4 P100\nconfig-set sd delta_current 0.8 \nG4 P100\nconfig-set sd extruder.hotend.steps_per_mm 402 \nG4 P1000";
            THEPANEL->set_toolhead(TOOLHEAD_CERAMICS);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_CHOCO);
            this->refresh_menu();
            break; }
        case 7: {
            if (!Laser::isActivated()) {
                Laser::enableDynamicActivation();
                THEKERNEL->add_module( new Laser() );
            }
            command = "T1\nM92 E1\nM907 A0.5\nG4 P100\nT0\nM92 E1\nM1 E0.5";
            THEPANEL->set_toolhead(TOOLHEAD_LASER);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_LASER);
            this->refresh_menu();
            break; }
        case 8: {
            THEPANEL->set_toolhead(TOOLHEAD_CNCPRO);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_CNC);
            this->refresh_menu();
            break;
        }
        case 9: {
            command = "T1\nM92 E122.04\nM907 A0.5\nT0\nM92 E888.88\nM907 E0.5\nG4 P100\nconfig-set sd delta_current 0.5 \nG4 P1000\nG4 P100\nconfig-set sd epsilon_current 0.5 \nG4 P100\nconfig-set sd extruder.hotend.steps_per_mm 888.88 \nG4 P100\nconfig-set sd extruder.hotend2.steps_per_mm 122.04 \nG4 P1000";
            THEPANEL->set_toolhead(TOOLHEAD_5AXIS);
            this->refresh_menu();
            break; }
        case 10: {
            command = "T1\nM92 E24228\nM907 A1.25";
            THEPANEL->set_toolhead(TOOLHEAD_SCANNER);
            this->refresh_menu();
            break; }
        case 11: {
            command = "T1\nM92 E3200\nM907 A1.0\nT0\nG4 P100\nconfig-set sd epsilon_current 1.0 \nG4 P100\nconfig-set sd extruder.hotend2.steps_per_mm 3200 \nG4 P1000";
            THEPANEL->set_toolhead(TOOLHEAD_TIMELAPSE);
            this->refresh_menu();
            break; }

    }
}

// queuing commands needs to be done from main loop
void SelectToolheadScreen::on_main_loop()
{
    // change actual axis value
    if (this->command == nullptr) return;
    send_command(this->command);
    this->command = nullptr;
}
