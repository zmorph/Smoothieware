#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "ST7565.h"
#include "libs/Kernel.h"
#include "StreamOutputPool.h"
namespace ui
{

class Screen
{
public:
	Screen()
	:width(128), height(64), screen(new ST7565())
	{
		THEKERNEL->streams->printf("Screen initialized\r\n");
		screen->init();
		screen->clear();
	}
	
	void draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
	{
		if(w+x > width)
		{
			return;
		}
		if(h+y > height)
		{
			return;
		}
		for(size_t xi = x; xi < x+w; ++xi)
		{
			for(size_t yi = y; yi < y+h; ++yi)
			{
				screen->pixel(xi, yi, color);
			}
		}
	}

	void draw_char(uint16_t x, uint16_t y, char letter, uint16_t color)
	{
		screen->drawChar(x, y, letter, color);
	}

	void set_pixel(uint16_t x, uint16_t y)
	{
		screen->pixel(x, y, 1);
	}

	void clear_pixel(uint16_t x, uint16_t y)
	{
		screen->pixel(x, y, 0);
	}

	void refresh()
	{
		screen->on_refresh();
	}

	void refresh(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
	{
		screen->on_refresh();
	}

	void clear()
	{
		screen->clear();
	}
	
	void clear(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
	{
		draw_rectangle(x, y, w, h, 0);
	}

	uint8_t read_buttons(void) 
	{
		return screen->readButtons();
	}

	const uint16_t width;
	const uint16_t height;

private:
	ST7565* screen;
};

} // namespace ui

#endif // __SCREEN_H__