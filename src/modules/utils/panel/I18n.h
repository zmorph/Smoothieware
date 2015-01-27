#ifndef __I18N_H__
#define __I18N_H__

#ifdef __LANG_PL__

namespace i18n
{
char const * const move_caption          		= "Ruch";
char const * const heat_caption	         		= "Grzanie";
char const * const print_caption         		= "Druk";
char const * const status_caption        		= "Status";
char const * const settings_caption      		= "Opcje";
char const * const maintenance_caption   		= "Serwis";
char const * const back_caption          		= "Wstecz";
char const * const home_caption          		= "Zeruj osie";
char const * const z_caption             		= "Ruch Z";
char const * const x_caption             		= "Ruch X";
char const * const y_caption             		= "Ruch Y";
char const * const preheat_abs_caption   		= "Ogrzej dla ABS";
char const * const preheat_pla_caption   		= "Ogrzej dla PLA";
char const * const manual_preheat_caption		= "Ustaw manualnie";
char const * const cool_down_caption     		= "Wychlodz";
char const * const home_z_caption        		= "Zeruj Z";
char const * const home_xy_caption       		= "Zeruj XY";
char const * const home_xyz_caption      		= "Zeruj XYZ";
char const * const hotend_temperature_caption 	= "Glowica";
char const * const hotbed_temperature_caption 	= "Platforma";
char const * const progress_caption 	 		= "Postep";
char const * const manual_extrusion_caption 	= "Manualna ekstruzja";
char const * const prime_printhead_caption 	 	= "Wstepna ekstruzja";
char const * const filament_change_caption 	 	= "Zmien filament";
char const * const level_bed_caption 	 		= "Poziomuj stol";
char const * const extrude_caption				= "Ekstruduj 5mm";
char const * const retract_caption				= "Cofnij 5mm";
char const * const menu_caption  				= "Menu";
char const * const no_file_caption				= "HBot gotowy";
char const * const abort_print_caption			= "Przerwij druk";
char const * const not_abort_print_caption		= "Wstecz";
char const * const version_caption				= "Wersja programu";
char const * const ip_caption					= "Adres IP";
char const * const init_home					= "Zeruj osie";
char const * const dont_home					= "Pomin zerowanie (!)";
char const * const turn_motors_off_caption      = "Wylacz silniki";
char const * const turn_motors_on_caption       = "Wlacz silniki";
char const * const motors_caption               = "Silniki";
char const * const leds_caption					= "Oswietlenie";
char const * const fans_caption                 = "Wentylacja";
char const * const turn_leds_on_caption			= "Wlacz oswietlenie";
char const * const turn_leds_off_caption		= "Wylacz oswietlenie";
char const * const turn_extruder_fans_on_caption = "Wlacz went. ekstrudera";
char const * const turn_extruder_fans_off_caption = "Wylacz went. ekstrudera";
char const * const turn_enclosure_fans_on_caption = "Wlacz went. obudowy";
char const * const turn_enclosure_fans_off_caption = "Wylacz went. obudowy";
char const * const cold_extrusion_prevented_caption	= "Rozgrzej glowice!";
char const * const scroll_bar_caption			= "";
char const * const printing_warning_caption		= "Uwaga! Trwa wydruk.";

char const * const bed_leveling_caption = "Upewnij sie, ze stol jest pusty. Aby przejsc do kolejnego kroku wcisnij OK.";

char const * const front_left_corner_caption 	= "Ustaw odstep stolu od glowicy w przednim lewym rogu na grubosc kartki papieru.";
char const * const front_right_corner_caption 	= "Ustaw odstep stolu od glowicy w przednim prawym rogu na grubosc kartki papieru.";
char const * const back_right_corner_caption 	= "Ustaw odstep stolu od glowicy w tylnym prawym rogu na grubosc kartki papieru.";
char const * const back_left_corner_caption 	= "Ustaw odstep stolu od glowicy w tylnym lewym rogu na grubosc kartki papieru.";

char const * const front_left_corner_again_caption 	= "Potwierdz odstep stolu od glowicy w przednim lewym rogu i wcisnij OK";
char const * const front_right_corner_again_caption	= "Potwierdz odstep stolu od glowicy w przednim prawym rogu i wcisnij OK";
char const * const back_right_corner_again_caption 	= "Potwierdz odstep stolu od glowicy w tylnym prawym rogu i wcisnij OK";
char const * const back_left_corner_again_caption 	= "Potwierdz odstep stolu od glowicy w tylnym lewym rogu i wcisnij OK";

char const * const finish_bed_leveling_caption 	= "Czy powtorzyc operacje?";
} // namespace i18n

#else

namespace i18n
{
char const * const move_caption          		= "Move";
char const * const heat_caption	         		= "Heat";
char const * const print_caption         		= "Print";
char const * const status_caption        		= "Status";
char const * const settings_caption      		= "Setup";
char const * const maintenance_caption   		= "Service";
char const * const back_caption          		= "Back";
char const * const home_caption          		= "Home Axes";
char const * const z_caption             		= "Move Z";
char const * const x_caption             		= "Move X";
char const * const y_caption             		= "Move Y";
char const * const preheat_abs_caption   		= "Preheat ABS";	
char const * const preheat_pla_caption   		= "Preheat PLA";
char const * const manual_preheat_caption		= "Manual Preheat";
char const * const cool_down_caption     		= "Cool Down";
char const * const home_z_caption        		= "Home Z";
char const * const home_xy_caption       		= "Home XY";
char const * const home_xyz_caption      		= "Home XYZ";
char const * const hotend_temperature_caption 	= "Hotend";
char const * const hotbed_temperature_caption 	= "Hotbed";
char const * const progress_caption 	 		= "Progress";
char const * const manual_extrusion_caption 	= "Manual Extrusion";
char const * const prime_printhead_caption 	 	= "Prime Printhead";
char const * const filament_change_caption 	 	= "Change Filament";
char const * const level_bed_caption 	 		= "Level Bed";
char const * const extrude_caption				= "Extrude 5mm";
char const * const retract_caption				= "Retract 5mm";
char const * const menu_caption  				= "Menu";
char const * const no_file_caption				= "HBot ready";
char const * const abort_print_caption			= "Abort print";
char const * const not_abort_print_caption		= "Back";
char const * const version_caption				= "Firmware version";
char const * const ip_caption					= "IP address";
char const * const init_home					= "Home axes";
char const * const dont_home					= "Ignore homing (!)";
char const * const turn_motors_off_caption      = "Turn motors off";
char const * const turn_motors_on_caption       = "Turn motors on";
char const * const motors_caption               = "Motors";
char const * const leds_caption					= "Lights";
char const * const fans_caption                 = "Fans";
char const * const turn_leds_on_caption			= "Turn lights on";
char const * const turn_leds_off_caption		= "Turn lights off";
char const * const turn_extruder_fans_on_caption = "Turn extruder fans on";
char const * const turn_extruder_fans_off_caption = "Turn extruder fans off";
char const * const turn_enclosure_fans_on_caption = "Turn enclosure fans on";
char const * const turn_enclosure_fans_off_caption = "Turn enclosure fans off";
char const * const cold_extrusion_prevented_caption = "Heat up hotend first!";
char const * const scroll_bar_caption			= "";
char const * const bed_leveling_caption 		= "";
char const * const printing_warning_caption		= "Warning! Printing in progress.";

char const * const front_left_corner_caption 	= "";
char const * const front_right_corner_caption 	= "";
char const * const back_right_corner_caption 	= "";
char const * const back_left_corner_caption 	= "";

char const * const front_left_corner_again_caption 	= "";
char const * const front_right_corner_again_caption	= "";
char const * const back_right_corner_again_caption 	= "";
char const * const back_left_corner_again_caption 	= "";

char const * const finish_bed_leveling_caption 	= "";
} // namespace i18n
#endif // LANG

#endif // __I18N_H__