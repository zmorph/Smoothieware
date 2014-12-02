#include "Panel.h"
#include "ui/CompositeItem.h"
#include "SlowTicker.h"
#include "screen/Screen.h"
#include "ui/FileShiftRegister.h"
#include "ui/Layout.h"
#include "ui/Link.h"
#include "ui/Group.h"
#include "ui/Cell.h"
#include "ui/Item.h"
#include "ui/Widget.h"
#include "I18n.h"
#include "ui/Event.h"
#include "modules/robot/RobotPublicAccess.h"
#include "PlayerPublicAccess.h"
#include "PublicDataRequest.h"
#include "PublicData.h"
#include "TemperatureControlPublicAccess.h"
#include "PublicData.h"
#include "checksumm.h"
#include <functional>
#include <tuple>
#include "Kernel.h"

#include "mri.h"
//FOR SENDING GCODES
#include "Gcode.h"


#define hotend_temp_checksum 	 CHECKSUM("hotend_temperature")
#define bed_temp_checksum    	 CHECKSUM("bed_temperature")
#define hotend_temp_ABS_checksum CHECKSUM("hotend_temperature_ABS")
#define bed_temp_ABS_checksum    CHECKSUM("bed_temperature_ABS")
#define hotend_temp_PLA_checksum CHECKSUM("hotend_temperature_PLA")
#define bed_temp_PLA_checksum    CHECKSUM("bed_temperature_PLA")


void send_gcode(std::string g)
{
    Gcode gcode(g, &(StreamOutput::NullStream));
    THEKERNEL->call_event(ON_GCODE_RECEIVED, &gcode );
}

template <size_t i>
float get_current_pos_()
{
	void *returned_data;

    bool ok = PublicData::get_value( robot_checksum, current_position_checksum, &returned_data );
    if(ok)
    {
    	float *p = static_cast<float *>(returned_data);
		return p[i];
	}	
	else
	{
		return 0;
	}
}

void set_x_position(float position)
{
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "G0 X%f", position);
	send_gcode(buffer);
}

void set_y_position(float position)
{
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "G0 Y%f", position);
	send_gcode(buffer);
}

void set_z_position(float position)
{
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "G0 Z%f", position);
	send_gcode(buffer);
}

void set_hotend_temperature(float temperature)
{
	// THEKERNEL->config->value( panel_checksum, hotend_temp_ABS_checksum )->by_default(235.0f)->as_number();
	// THEKERNEL->config->value( panel_checksum, bed_temp_ABS_checksum    )->by_default(100.0f)->as_number();
	PublicData::set_value( temperature_control_checksum, hotend_checksum, &temperature );
}

void set_hotbed_temperature(float temperature)
{
	// default_hotend_temperature_PLA = THEKERNEL->config->value( panel_checksum, hotend_temp_PLA_checksum )->by_default(185.0f )->as_number();
	// default_bed_temperature_PLA    = THEKERNEL->config->value( panel_checksum, bed_temp_PLA_checksum    )->by_default(60.0f  )->as_number();
	PublicData::set_value( temperature_control_checksum, bed_checksum, &temperature );
}

std::tuple<float, float> get_hotend_temperature()
{
	void *returned_data;
	if(PublicData::get_value( temperature_control_checksum, hotend_checksum, current_temperature_checksum, &returned_data ))
	{
		pad_temperature t =  *static_cast<struct pad_temperature *>(returned_data);
		return std::make_tuple(t.target_temperature, t.current_temperature);
	}
	else
	{
		return std::make_tuple(0, 0);
	}
}

std::tuple<float, float> get_hotbed_temperature()
{
	void *returned_data;
	if(PublicData::get_value( temperature_control_checksum, bed_checksum, current_temperature_checksum, &returned_data ))
	{
		pad_temperature t =  *static_cast<struct pad_temperature *>(returned_data);
		return std::make_tuple(t.target_temperature, t.current_temperature);
	}
	else
	{
		return std::make_tuple(0, 0);
	}
}

std::tuple<uint32_t, uint32_t, std::string> get_progress()
{
    void *returned_data;
    if (PublicData::get_value( player_checksum, get_progress_checksum, &returned_data )) 
    {
    	pad_progress p =  *static_cast<struct pad_progress *>(returned_data);
        return(std::make_tuple(p.percent_complete, p.elapsed_secs, p.filename));
    } else {
		return(std::make_tuple(0, 0, ""));
    }
}

ui::Cell const main_menu_cells[] = 
{
	ui::Cell{/*x*/  2,  /*y*/   1, /*w*/ 40, /*h*/ 30},
	ui::Cell{/*x*/  2,  /*y*/  33, /*w*/ 40, /*h*/ 30},
	ui::Cell{/*x*/ 44,  /*y*/   1, /*w*/ 40, /*h*/ 30},
	ui::Cell{/*x*/ 44,  /*y*/  33, /*w*/ 40, /*h*/ 30},
	ui::Cell{/*x*/ 86,  /*y*/   1, /*w*/ 40, /*h*/ 30},
	ui::Cell{/*x*/ 86,  /*y*/  33, /*w*/ 40, /*h*/ 30}
};

ui::Cell const stacked_menu_cells[] = 
{
	ui::Cell{/*x*/ 2,  /*y*/   0, /*w*/ 124, /*h*/ 21},
	ui::Cell{/*x*/ 2,  /*y*/  21, /*w*/	124, /*h*/ 21},
	ui::Cell{/*x*/ 2,  /*y*/  42, /*w*/	124, /*h*/ 22}
};

ui::Cell const splash_cell[] = 
{
	ui::Cell{/*x*/ 0,  /*y*/   0, /*w*/ 128, /*h*/ 64},
};

ui::Layout default_layout(main_menu_cells);
ui::Layout stacked_layout(stacked_menu_cells);

enum class MainMenu : size_t
{
	Move,
	Heat,
	Print,
	Maintenance,
	Status,
	Settings
};

enum class MoveMenu : size_t
{
	Back,
	Home,
	Z,
	X,
	Y
};

enum class ManualHeatMenu : size_t
{
	Back,
	Hotend,
	Hotbed
};

enum class SetupMenu : size_t
{
	HotendTemperature,
	HotbedTemperature,
	ZPosition,
	PlayerStatus
};


enum class HeatMenu : size_t
{
	Back,
	PreheatAbs,
	PreheatPla,
	ManualPreheat,
	CoolDown
};

enum class HomeMenu : size_t
{
	Back,
	Z,
	XY,
	XYZ
};

enum class StatusMenu : size_t
{
	HotendTemperature,
	HotbedTemperature,
	Z,
	Progress	
};

enum class MaintenanceMenu : size_t
{
	Back,
	ManualExtrusion,
	PrimePrinthead,
	FilamentChange,
	LevelBed
};

enum class ExtrudeMenu : size_t
{
	Back,
	Extrude,
	Retract
};

CompositeItem status_menu_items[] = 
{
	ui::FloatFloatInfo(i18n::hotend_temperature_caption, get_hotend_temperature),
	ui::FloatFloatInfo(i18n::hotbed_temperature_caption, get_hotbed_temperature),
	ui::FloatInfo(i18n::z_caption, get_current_pos_<2>),
	ui::PlayInfo(i18n::progress_caption, get_progress)
};

CompositeItem heat_menu_items[] = 
{
	ui::Item(i18n::back_caption),
	ui::Command(i18n::preheat_abs_caption, []{
		set_hotend_temperature(245); 
		set_hotbed_temperature(100);
	}),
	ui::Command(i18n::preheat_pla_caption, []{
		set_hotend_temperature(220); 
		set_hotbed_temperature(60);
	}),
	ui::Item(i18n::manual_preheat_caption),
	ui::Command(i18n::cool_down_caption, []{set_hotend_temperature(0); set_hotbed_temperature(0);})
};

CompositeItem manual_heat_menu_items[] = 
{
	ui::Item(i18n::back_caption),
	ui::HeatControl(i18n::hotend_temperature_caption, get_hotend_temperature, set_hotend_temperature, 150),
	ui::HeatControl(i18n::hotbed_temperature_caption, get_hotbed_temperature, set_hotbed_temperature, 0)
};

CompositeItem home_menu_items[] = 
{
	ui::Item(i18n::back_caption),
	ui::Command(i18n::home_z_caption, std::bind(send_gcode, "G28 Z")),
	ui::Command(i18n::home_xy_caption, std::bind(send_gcode, "G28 XY")),
	ui::Command(i18n::home_xyz_caption, std::bind(send_gcode, "G28 XYZ"))
};

CompositeItem move_menu_items[] = 
{
	ui::Item(i18n::back_caption),
	ui::Item(i18n::home_caption),
	ui::PositionControl(i18n::z_caption, get_current_pos_<2>, set_z_position),
	ui::PositionControl(i18n::x_caption, get_current_pos_<0>, set_x_position),
	ui::PositionControl(i18n::y_caption, get_current_pos_<1>, set_y_position)
};

CompositeItem main_menu_items[] = 
{
	ui::Item(i18n::move_caption),
	ui::Item(i18n::heat_caption),
	ui::Item(i18n::print_caption),
	ui::Item(i18n::maintenance_caption),
	ui::Item(i18n::status_caption),
	ui::Item(i18n::settings_caption)
};

ui::FileShiftRegister file_browser;

CompositeItem file_menu_items[] = 
{
	ui::File(file_browser, 0),
	ui::File(file_browser, 1),
	ui::File(file_browser, 2)
};

CompositeItem maintenance_menu_items[] = 
{
	ui::Item(i18n::back_caption),
	ui::Item(i18n::manual_extrusion_caption),
	ui::Item(i18n::prime_printhead_caption),
	ui::Item(i18n::filament_change_caption),
	ui::Item(i18n::level_bed_caption)
};

CompositeItem extrude_menu_items[] = 
{
	ui::Item(i18n::back_caption),
	ui::Command(i18n::extrude_caption, []{send_gcode("G91");send_gcode("G1 E5 F100");send_gcode("G90");}),
	ui::Command(i18n::retract_caption, []{send_gcode("G91");send_gcode("G1 E-5 F100");send_gcode("G90");})
};

ui::Widget main_menu_widget(&default_layout);
ui::Widget move_menu_widget(&stacked_layout); // correctness is not checked during compilation
ui::Widget file_menu_widget(&stacked_layout);
ui::Widget heat_menu_widget(&stacked_layout);
ui::Widget home_menu_widget(&stacked_layout);
ui::Widget manual_heat_menu_widget(&stacked_layout);
ui::Widget status_menu_widget(&stacked_layout);
ui::Widget maintenance_menu_widget(&stacked_layout);
ui::Widget extrude_menu_widget(&stacked_layout);

ui::Link main_menu_links[6];
ui::Link move_menu_links[5];
ui::Link file_menu_links[3];
ui::Link heat_menu_links[5];
ui::Link home_menu_links[4];
ui::Link manual_heat_menu_links[3];
ui::Link status_menu_links[4];
ui::Link maintenance_menu_links[5];
ui::Link extrude_menu_links[3];

ui::Group main_menu(main_menu_items, main_menu_links, main_menu_widget);
ui::Group move_menu(move_menu_items, move_menu_links, move_menu_widget);
ui::Group file_menu(file_menu_items, file_menu_links, file_menu_widget);
ui::Group heat_menu(heat_menu_items, heat_menu_links, heat_menu_widget);
ui::Group home_menu(home_menu_items, home_menu_links, home_menu_widget);
ui::Group manual_heat_menu(manual_heat_menu_items, manual_heat_menu_links, manual_heat_menu_widget);
ui::Group status_menu(status_menu_items, status_menu_links, status_menu_widget);
ui::Group maintenance_menu(maintenance_menu_items, maintenance_menu_links, maintenance_menu_widget);
ui::Group extrude_menu(extrude_menu_items, extrude_menu_links, extrude_menu_widget);

Panel::Panel()
:up_button(7), down_button(7), select_button(7), user_interface(status_menu.get_link_to(0), screen)
{
	file_browser.open_directory("/");
	main_menu.set_link_for(index(MainMenu::Move), move_menu.get_link_to(index(MoveMenu::Back)));
	main_menu.set_link_for(index(MainMenu::Heat), heat_menu.get_link_to(index(HeatMenu::Back)));
	main_menu.set_link_for(index(MainMenu::Print), file_menu.get_link_to(0));
	main_menu.set_link_for(index(MainMenu::Status), status_menu.get_link_to(0));
	main_menu.set_link_for(index(MainMenu::Maintenance), maintenance_menu.get_link_to(index(MaintenanceMenu::Back)) );

	move_menu.set_link_for(index(MoveMenu::Back), main_menu.get_link_to(index(MainMenu::Move)));
	move_menu.set_link_for(index(MoveMenu::Home), home_menu.get_link_to(index(HomeMenu::Back)));
	home_menu.set_link_for(index(HomeMenu::Back), move_menu.get_link_to(index(MoveMenu::Back)));
	
	file_menu.set_link_for(0, main_menu.get_link_to(index(MainMenu::Move)));
	
	heat_menu.set_link_for(index(HeatMenu::Back), main_menu.get_link_to(index(MainMenu::Move)));
	heat_menu.set_link_for(index(HeatMenu::ManualPreheat), manual_heat_menu.get_link_to(index(ManualHeatMenu::Hotend)));
	
	manual_heat_menu.set_link_for(index(ManualHeatMenu::Back), heat_menu.get_link_to(index(HeatMenu::Back) ));
	maintenance_menu.set_link_for(index(MaintenanceMenu::Back), main_menu.get_link_to(index(MainMenu::Maintenance)) );
	maintenance_menu.set_link_for(index(MaintenanceMenu::ManualExtrusion), extrude_menu.get_link_to(index(ExtrudeMenu::Back)) );
	extrude_menu.set_link_for(index(ExtrudeMenu::Back), maintenance_menu.get_link_to(index(MaintenanceMenu::Back)));
	
	status_menu.set_link_for(0, main_menu.get_link_to(index(MainMenu::Move)));
	status_menu.set_link_for(1, main_menu.get_link_to(index(MainMenu::Move)));
	status_menu.set_link_for(2, main_menu.get_link_to(index(MainMenu::Move)));
	status_menu.set_link_for(3, main_menu.get_link_to(index(MainMenu::Move)));

 //    default_hotend_temperature 	   = THEKERNEL->config->value( panel_checksum, hotend_temp_checksum )->by_default(185.0f )->as_number();
 //    default_bed_temperature    	   = THEKERNEL->config->value( panel_checksum, bed_temp_checksum    )->by_default(60.0f  )->as_number();
	// default_hotend_temperature_ABS = THEKERNEL->config->value( panel_checksum, hotend_temp_ABS_checksum )->by_default(235.0f )->as_number();
 //    default_bed_temperature_ABS    = THEKERNEL->config->value( panel_checksum, bed_temp_ABS_checksum    )->by_default(100.0f  )->as_number();
	// default_hotend_temperature_PLA = THEKERNEL->config->value( panel_checksum, hotend_temp_PLA_checksum )->by_default(185.0f )->as_number();
 //    default_bed_temperature_PLA    = THEKERNEL->config->value( panel_checksum, bed_temp_PLA_checksum    )->by_default(60.0f  )->as_number();

}

void Panel::on_module_loaded()
{
	this->register_for_event(ON_IDLE);
    this->register_for_event(ON_MAIN_LOOP);

    THEKERNEL->slow_ticker->attach( 23U,  this, &Panel::button_tick );
    THEKERNEL->slow_ticker->attach( 20, this,  &Panel::refresh_tick );
}

uint32_t Panel::button_tick(uint32_t dummy)
{
	this->button_state = this->screen.read_buttons();
	if(this->button_state & BUTTON_UP)
	{
		up_button.press();
	}
	else
	{
		up_button.tick();
	}
	if(this->button_state & BUTTON_DOWN)
	{
		down_button.press();
	}
	else
	{
		down_button.tick();
	}
	if(this->button_state & BUTTON_SELECT)
	{
		select_button.press();
	}
	else
	{
		select_button.tick();
	}
	return 0;
}

uint32_t Panel::refresh_tick(uint32_t dummy)
{
	this->refresh_flag = true;
	return 0;
}

void Panel::on_main_loop(void* argument)
{
	if(up_button.read())
	{
		user_interface.dispatch(ui::UpEvent());
	}
	if(down_button.read())
	{
		user_interface.dispatch(ui::DownEvent());
	}
	if(select_button.read())
	{
		screen.clear();
		user_interface.dispatch(ui::OkEvent());
	}
}

void Panel::on_idle(void* argument)
{
	if(refresh_flag)
	{
		refresh_flag = false;
		user_interface.render();
		user_interface.refresh();
	}
}