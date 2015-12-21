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
#include "MainMenuScreen.h"
#include "WatchScreen.h"
#include "FileScreen.h"
#include "JogScreen.h"
#include "JogScreenBasic.h"
#include "CalibrateScreen.h"
#include "SelectToolheadScreen.h"
#include "ControlScreen.h"
#include "PrepareScreen.h"
#include "ChocoMenu.h"
#include "ProbeScreen.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "modules/utils/player/PlayerPublicAccess.h"
#include "PublicData.h"
#include "checksumm.h"
#include "ModifyValuesScreen.h"
#include "Robot.h"
#include "Planner.h"
#include "StepperMotor.h"
#include "EndstopsPublicAccess.h"
#include "ChocoMenu.h"
#include "CNCMenu.h"
#include "LaserMenu.h"


#include <string>
using namespace std;

#define extruder_checksum CHECKSUM("extruder")

MainMenuScreen::MainMenuScreen()
{
    // Children screens
    this->jog_screen     = (new JogScreen()     )->set_parent(this);
    this->jog_screen_basic = (new JogScreenBasic())->set_parent(this);
    this->select_toolhead_screen = (new SelectToolheadScreen() )->set_parent(this);
    this->calibrate_screen = (new CalibrateScreen() )->set_parent(this);
    this->watch_screen   = (new WatchScreen()   )->set_parent(this);
    this->file_screen    = (new FileScreen()    )->set_parent(this);
    this->prepare_screen = (new PrepareScreen() )->set_parent(this);
    this->choco_screen = (new ChocoMenu() )->set_parent(this);
    this->laser_screen = (new LaserMenu() )->set_parent(this);
    this->cnc_screen = (new CNCMenu() )->set_parent(this);
    
    
    this->set_parent(this->watch_screen);
}

// setup and enter the configure screen
void MainMenuScreen::setupConfigureScreen()
{
    auto mvs= new ModifyValuesScreen(true); // delete itself on exit
    mvs->set_parent(this);

    // acceleration
    mvs->addMenuItem("Acceleration", // menu name
        []() -> float { return THEKERNEL->planner->get_acceleration(); }, // getter
        [this](float acc) { send_gcode("M204", 'S', acc); }, // setter
        10.0F, // increment
        1.0F, // Min
        10000.0F // Max
        );

    // steps/mm
    mvs->addMenuItem("X steps/mm",
        []() -> float { return THEKERNEL->robot->actuators[0]->get_steps_per_mm(); },
        [](float v) { THEKERNEL->robot->actuators[0]->change_steps_per_mm(v); },
        0.1F,
        1.0F
        );

    mvs->addMenuItem("Y steps/mm",
        []() -> float { return THEKERNEL->robot->actuators[1]->get_steps_per_mm(); },
        [](float v) { THEKERNEL->robot->actuators[1]->change_steps_per_mm(v); },
        0.1F,
        1.0F
        );

    mvs->addMenuItem("Z steps/mm",
        []() -> float { return THEKERNEL->robot->actuators[2]->get_steps_per_mm(); },
        [](float v) { THEKERNEL->robot->actuators[2]->change_steps_per_mm(v); },
        0.1F,
        1.0F
        );

    mvs->addMenuItem("Z Home Ofs",
        []() -> float { void *rd; PublicData::get_value( endstops_checksum, home_offset_checksum, &rd ); return rd==nullptr ? 0.0F : ((float*)rd)[2]; },
        [this](float v) { send_gcode("M206", 'Z', v); },
        0.1F
        );

    mvs->addMenuItem("Contrast",
        []() -> float { return THEPANEL->lcd->getContrast(); },
        [this](float v) { THEPANEL->lcd->setContrast(v); },
        1,
        0,
        255,
        true // instant update
        );

    THEPANEL->enter_screen(mvs);
}

void MainMenuScreen::on_enter()
{
    THEPANEL->enter_menu_mode();
    THEPANEL->setup_menu(7);
    float *rd; 
    PublicData::get_value( extruder_checksum, (void **)&rd );
    int steps = *rd;
    switch (steps){
        case 100:{
            THEPANEL->set_toolhead(TOOLHEAD_EXTRU175);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_FILAMENT);
            break;
        }
        case 400: {
            THEPANEL->set_toolhead(TOOLHEAD_EXTRU300);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_FILAMENT);
            break;
        }
        case 900: {
            THEPANEL->set_toolhead(TOOLHEAD_DUAL);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_FILAMENT);
            break;
        }        
        case 901: {
            THEPANEL->set_toolhead(TOOLHEAD_DUALPRO);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_FILAMENT);
            break;
        }
        case 401: {
            THEPANEL->set_toolhead(TOOLHEAD_CHOCO);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_CHOCO);
            break;
        }
        case 402: {
            THEPANEL->set_toolhead(TOOLHEAD_CERAMICS);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_CHOCO);
            break;
        }
        case 888: {
            THEPANEL->set_toolhead(TOOLHEAD_5AXIS);
            THEPANEL->set_toolhead_group(TOOLHEAD_GROUP_FILAMENT);
            break;
        }
     }
    this->refresh_menu();
}

void MainMenuScreen::on_refresh()
{
    if ( THEPANEL->menu_change() ) {
        this->refresh_menu();
    }
    if ( THEPANEL->click() ) {
        this->clicked_menu_entry(THEPANEL->get_menu_current_line());
    }
}

void MainMenuScreen::display_menu_line(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->lcd->printf("Info Screen"); break;
        case 1: 
        {
            if(THEPANEL->get_toolhead_group() == TOOLHEAD_GROUP_FILAMENT)
                THEPANEL->lcd->printf("Filament menu"); 
            else if(THEPANEL->get_toolhead_group() == TOOLHEAD_GROUP_CHOCO)
                THEPANEL->lcd->printf("Choco/ceramics menu");
            else if(THEPANEL->get_toolhead_group() == TOOLHEAD_GROUP_LASER)
                THEPANEL->lcd->printf("Laser menu");
            else if(THEPANEL->get_toolhead_group() == TOOLHEAD_GROUP_CNC)
                THEPANEL->lcd->printf("CNC menu");
            break;
        
        }
        case 2: THEPANEL->lcd->printf(THEPANEL->is_playing() ? "Abort" : "Play"); break;
        case 3: THEPANEL->lcd->printf("Move Axis"); break;
        case 4: THEPANEL->lcd->printf("Select Toolhead"); break;
        case 5: THEPANEL->lcd->printf("Calibrate"); break;
        case 6: THEPANEL->lcd->printf("Custom"); break;
        //case 2: THEPANEL->lcd->printf("Jog"); break;
        //case 5: THEPANEL->lcd->printf("Configure"); break;
        //case 6: THEPANEL->lcd->printf("Probe"); break;
    }
}

void MainMenuScreen::clicked_menu_entry(uint16_t line)
{
    switch ( line ) {
        case 0: THEPANEL->enter_screen(this->watch_screen   ); break;
        case 1: 
            if(THEPANEL->get_toolhead_group() == TOOLHEAD_GROUP_FILAMENT)
                {
                    THEPANEL->enter_screen(this->prepare_screen ); 
                    break;
                }
            else if(THEPANEL->get_toolhead_group() == TOOLHEAD_GROUP_CHOCO)
                {
                    THEPANEL->enter_screen(this->choco_screen ); 
                    break;
                }
            else if(THEPANEL->get_toolhead_group() == TOOLHEAD_GROUP_LASER)
                {
                    THEPANEL->enter_screen(this->laser_screen ); 
                    break;
                }
            else if(THEPANEL->get_toolhead_group() == TOOLHEAD_GROUP_CNC)
                {
                    THEPANEL->enter_screen(this->cnc_screen ); 
                    break;
                }
            break;
        





        case 2: THEPANEL->is_playing() ? abort_playing() : THEPANEL->enter_screen(this->file_screen); break;
        case 3: THEPANEL->enter_screen(this->jog_screen_basic ); break;
        case 4: THEPANEL->enter_screen(this->select_toolhead_screen ); break;
        case 5: THEPANEL->enter_screen(this->calibrate_screen ); break;
        case 6: THEPANEL->enter_screen(THEPANEL->custom_screen ); break;
        //case 2: THEPANEL->enter_screen(this->jog_screen     ); break;
        //case 5: setupConfigureScreen(); break;
        //case 6: THEPANEL->enter_screen((new ProbeScreen())->set_parent(this)); break;
    }
}

void MainMenuScreen::abort_playing()
{
    PublicData::set_value(player_checksum, abort_play_checksum, NULL);
    THEPANEL->enter_screen(this->watch_screen);
}

