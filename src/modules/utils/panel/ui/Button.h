#ifndef __BUTTON_H__
#define __BUTTON_H__

namespace ui
{

class Button
{
	uint16_t idle_time;
	const uint16_t dead_time;
	bool pressed;
	bool dead;

public:
	Button(uint16_t dead_time = 5)
	:idle_time(0), dead_time(dead_time), 
		pressed(false), dead(false)
	{}

	void tick() volatile
	{
		if(!pressed)
		{
			dead = false;
		}

		if(dead)
		{
			++idle_time;
			dead = idle_time < dead_time;
		}
		else
		{
			idle_time = 0;
		}	
	}

	void press() volatile
	{
		if(dead)
		{
			return;
		}
		pressed = true;
		dead = true;
	}

	bool read() volatile
	{
		bool result = pressed;
		pressed = false;
		return result;
	}
};

} // namespace ui

#endif // __BUTTON_H__