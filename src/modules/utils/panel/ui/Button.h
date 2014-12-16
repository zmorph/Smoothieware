#ifndef __BUTTON_H__
#define __BUTTON_H__

namespace ui
{

class Button
{
	uint16_t pressed_ticks;
	const uint16_t dead_time;
	const uint16_t delay;
	bool long_pressed;
	bool pressed;

public:
	Button(uint16_t dead_time = 20, uint16_t delay = 10)
	:pressed_ticks(0), dead_time(dead_time), delay(delay),
		long_pressed(false), pressed(false)
	{}

	void tick() volatile
	{
		long_pressed = false;
		pressed_ticks = 0;
	}

	void press() volatile
	{
		pressed = (pressed_ticks == 0) || pressed;
		
		++pressed_ticks;
		
		if(pressed_ticks == dead_time)
		{
			long_pressed = true;
			pressed_ticks = 0;
		}
		if(long_pressed && pressed_ticks == delay)
		{
			pressed_ticks = 0;
		}
		
	}

	bool read() volatile
	{
		bool result = pressed;
		pressed      = false;
		return result;
	}
};

} // namespace ui

#endif // __BUTTON_H__