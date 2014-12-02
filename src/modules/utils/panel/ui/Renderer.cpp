#include "Renderer.h"
#include <sstream>
#include <string>


namespace ui
{

size_t print_caption(Screen& screen, std::string const & name, uint16_t x, uint16_t y, uint16_t limit, int color)
{
	uint8_t offset        = 6;
	uint8_t letter_height = 8;
	uint8_t letter_index  = 0;
	for(auto letter: name)
	{
		if(offset*letter_index < limit)
		{
			screen.draw_char(x + offset*letter_index + 4, y - letter_height/2 + 1, letter, color);
		}
		else
		{
			break;
		}
		++letter_index;
	}
	return offset*letter_index;
}

template <>
Cell ActiveRenderer::operator()(ui::File& item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 1);
	uint8_t offset = 6;
	uint8_t letter_height = 8;
	std::string name = item.get_name();
	uint8_t letter_index = 0;
	for(auto letter: name)
	{
		if(offset*letter_index < cell.w - 4)
		{
			screen.draw_char(cell.x + offset*letter_index + 4, cell.y + cell.h/2 - letter_height/2 + 1, letter, 0);
		}
		++letter_index;		
	}
	return cell;
}

template <>
Cell DefaultRenderer::operator()(ui::File& item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
    uint8_t offset = 6;
	uint8_t letter_height = 8;
	std::string name = item.get_name();
	uint8_t letter_index = 0;
	for(auto letter: name)
	{
		if(offset*letter_index < cell.w - 4)
		{
			screen.draw_char(cell.x + offset*letter_index + 4, cell.y + cell.h/2 - letter_height/2 + 1, letter, 1);
		}
		++letter_index;		
	}
	return cell;
}

// std::string to_string(const int value)
// {
//     std::ostringstream oss;
//     oss << value;
//     return oss.str();
// }

template <> 
Cell ActiveRenderer::operator()<ui::PositionControl >(ui::PositionControl&  item) const
{
	if(item.mode)
	{
		screen.draw_rectangle(cell.x +cell.w / 2 + 4, cell.y, cell.w - cell.w / 2 - 4 , cell.h, 1);
		print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	}
	else
	{
		screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 1);
		print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 0);
	}
	char buffer[10];
	sprintf(buffer, "%.1f", item.get_data());
	print_caption(screen, buffer, cell.w - 40, cell.y + cell.h/2, cell.w - 4, 0);
	return cell;
}

template <> 
Cell DefaultRenderer::operator()<ui::PositionControl >(ui::PositionControl&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	char buffer[10];
	sprintf(buffer, "%.1f", item.get_data());
	print_caption(screen, buffer, cell.w - 40, cell.y + cell.h/2, cell.w - 4, 1);
	
	return cell;
}

template <> 
Cell ActiveRenderer::operator()<ui::HeatControl >(ui::HeatControl&  item) const
{
	if(item.mode)
	{
		screen.draw_rectangle(cell.x +cell.w / 2 + 4, cell.y, cell.w - cell.w / 2 - 4 , cell.h, 1);
		print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	}
	else
	{
		screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 1);
		print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 0);
	}
	char buffer[16];
	float target, current;
	std::tie(target, current) = item.get_data();
	sprintf(buffer, "%.0f/%.0f", target, current);
	print_caption(screen, buffer, cell.w - 50, cell.y + cell.h/2, 60, 0);
	return cell;
}

template <> 
Cell DefaultRenderer::operator()<ui::HeatControl >(ui::HeatControl&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	char buffer[16];
	float target, current;
	std::tie(target, current) = item.get_data();
	sprintf(buffer, "%.0f/%.0f", target, current);
	print_caption(screen, buffer, cell.w - 50, cell.y + cell.h/2, 60, 1);
	
	return cell;
}

template <> 
Cell ActiveRenderer::operator()<ui::FloatInfo >(ui::FloatInfo&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	char buffer[10];
	sprintf(buffer, "%.1f", item.get_data());
	print_caption(screen, buffer, cell.w - 40, cell.y + cell.h/2, cell.w - 4, 1);
	
	return cell;
}

template <> 
Cell DefaultRenderer::operator()<ui::FloatInfo >(ui::FloatInfo&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	char buffer[10];
	sprintf(buffer, "%.1f", item.get_data());
	print_caption(screen, buffer, cell.w - 40, cell.y + cell.h/2, cell.w - 4, 1);
	
	return cell;
}

template <> 
Cell ActiveRenderer::operator()<ui::FloatFloatInfo >(ui::FloatFloatInfo&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	char buffer[16];
	float target, current;
	std::tie(target, current) = item.get_data();
	sprintf(buffer, "%.0f/%.0f", target, current);
	print_caption(screen, buffer, cell.w - 50, cell.y + cell.h/2, 60, 1);
	
	return cell;
}

template <> 
Cell DefaultRenderer::operator()<ui::FloatFloatInfo >(ui::FloatFloatInfo&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	char buffer[16];
	float target, current;
	std::tie(target, current) = item.get_data();
	sprintf(buffer, "%.0f/%.0f", target, current);
	print_caption(screen, buffer, cell.w - 50, cell.y + cell.h/2, 60, 1);
	
	return cell;
}

template <> 
Cell ActiveRenderer::operator()<ui::PlayInfo >(ui::PlayInfo&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	//print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	char buffer[16];
	uint32_t percent, elapsed;
	std::string filename;
	std::tie(percent, elapsed, filename) = item.get_data();
	sprintf(buffer, "%d%%    %dmin %ds", percent, elapsed/60, elapsed%60);
	print_caption(screen, buffer, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	
	return cell;
}

template <> 
Cell DefaultRenderer::operator()<ui::PlayInfo >(ui::PlayInfo&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	char buffer[16];
	uint32_t percent, elapsed;
	std::string filename;
	std::tie(percent, elapsed, filename) = item.get_data();
	sprintf(buffer, "%d %d", percent, elapsed);
	print_caption(screen, buffer, cell.w - 50, cell.y + cell.h/2, 60, 1);
	
	return cell;
}

} // namespace ui