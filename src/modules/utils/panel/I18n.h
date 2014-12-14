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
char const * const hotbed_temperature_caption 	= "Stol";
char const * const progress_caption 	 		= "Postep";
char const * const manual_extrusion_caption 	= "Manualna ekstruzja";
char const * const prime_printhead_caption 	 	= "Wstepna ekstruzja";
char const * const filament_change_caption 	 	= "Zmien filament";
char const * const level_bed_caption 	 		= "Poziomuj stol";
char const * const extrude_caption				= "Ekstruduj 5mm";
char const * const retract_caption				= "Wsun 5mm";
char const * const menu_caption  				= "Menu";
char const * const no_file_caption				= "HBot gotowy";
char const * const abort_print_caption			= "Przerwij druk";
char const * const not_abort_print_caption		= "Wstecz";
char const * const version_caption				= "Wersja programu";
char const * const ip_caption					= "Adres IP";
char const * const init_home					= "Zeruj osie";
char const * const dont_home					= "Pomin zerowanie (!)";
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
char const * const no_file_caption				= "No file";
char const * const abort_print_caption			= "Abort print";
char const * const not_abort_print_caption		= "Back";
char const * const version_caption				= "Firmware version";
char const * const ip_caption					= "IP address";
char const * const init_home					= "Home axes";
char const * const dont_home					= "Ignore homing (!)";

} // namespace i18n
#endif // LANG

#endif // __I18N_H__