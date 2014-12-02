#ifndef PANEL_H
#define PANEL_H

#include "Module.h"
#include "ui/UserInterface.h"
#include "ui/Button.h"

class Panel : public Module
{
public:
    Panel();
    //virtual ~Panel();
	void on_module_loaded();
	void on_idle(void* argument);
	void on_main_loop(void* argument);
	void on_gcode_received(void* argument){};
	uint32_t button_tick(uint32_t dummy);
	uint32_t refresh_tick(uint32_t dummy);

private:
	volatile ui::Button up_button;
	volatile ui::Button down_button;
	volatile ui::Button select_button;

	volatile bool refresh_flag;
	volatile uint8_t button_state;
	ui::Screen screen;
	ui::UserInterface user_interface;
};

template <typename IndexType>
size_t index(IndexType index)
{
	return static_cast<size_t>(index);
}

#endif // PANEL_H