#include "Renderer.h"
#include <algorithm>
#include <sstream>
#include <string>


namespace ui
{

void print_black_left(Screen& screen, Cell const & cell, std::string const & sequence)
{
	print_left_base<2, 5, 8, 1, true>(screen, cell, sequence);
}

void print_white_left(Screen& screen, Cell const & cell, std::string const & sequence)
{
	print_left_base<2, 5, 8, 1, false>(screen, cell, sequence);
}

void print_black_center(Screen& screen, Cell const & cell, std::string const & sequence)
{
	print_center_base<2, 5, 8, 1, true>(screen, cell, sequence);
}

void print_white_center(Screen& screen, Cell const & cell, std::string const & sequence)
{
	print_center_base<2, 5, 8, 1, false>(screen, cell, sequence);
}

void print_black_right(Screen& screen, Cell const & cell, std::string const & sequence)
{
	print_right_base<2, 5, 8, 1, true>(screen, cell, sequence);
}

void print_white_right(Screen& screen, Cell const & cell, std::string const & sequence)
{
	print_right_base<2, 5, 8, 1, false>(screen, cell, sequence);
}

size_t print_caption(Screen& screen, std::string const & name, uint16_t x_origin, uint16_t y_origin, uint16_t limit, int color, bool centered = false, bool right = false)
{
	uint8_t letter_offset = 6;
	uint8_t letter_height = 8;
	uint8_t letter_index  = 0;
	size_t max_number_of_letters = limit /letter_offset;
	uint8_t number_of_letters = std::min(name.size(), max_number_of_letters);
	uint8_t center_offset = centered?  (limit - number_of_letters * letter_offset) / 2 : 0;
	center_offset = right?  (limit - number_of_letters * letter_offset): center_offset;
	for(auto letter: name)
	{
		if(letter_index < max_number_of_letters)
		{
			screen.draw_char(x_origin + letter_offset * letter_index + center_offset, y_origin - letter_height / 2 + 1, letter, color);
		}
		else
		{
			break;
		}
		++letter_index;
	}
	return letter_offset*letter_index;
}


template <>
Cell ActiveRenderer::operator()(ui::GraphicalItem& item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 1);
	print_caption(screen, item.caption, cell.x+1, cell.y + cell.h/2, cell.w-1, 0, true);
	return cell;
}

template <>
Cell DefaultRenderer::operator()(ui::GraphicalItem& item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	screen.draw_picture(x_center(cell) - 12, y_center(cell) - 12, 24, 24, item.icon, false);
	return cell;
}

template <>
Cell ActiveRenderer::operator()(ui::File& item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 1);
	print_white_left(screen, cell, item.get_name());
	return cell;
}

template <>
Cell DefaultRenderer::operator()(ui::File& item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_black_left(screen, cell, item.get_name());
	return cell;
}


template <> 
Cell ActiveRenderer::operator()<ui::PositionControl >(ui::PositionControl&  item) const
{
	if(item.mode)
	{
		screen.draw_rectangle(cell.x +cell.w / 2 + 4, cell.y, cell.w - cell.w / 2 - 4 , cell.h, 1);
		print_black_left(screen, cell, item.caption);
	}
	else
	{
		screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 1);
		print_white_left(screen, cell, item.caption);
	}
	char buffer[10];
	snprintf(buffer, sizeof(buffer), "%.1f", item.get_data());
	//print_white_right();
	print_caption(screen, buffer, cell.w - 40, cell.y + cell.h/2, cell.w - 4, 0);
	return cell;
}

template <> 
Cell DefaultRenderer::operator()<ui::PositionControl >(ui::PositionControl&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_caption(screen, item.caption, cell.x + 4, cell.y + cell.h/2, cell.w - 4, 1);
	char buffer[10];
	snprintf(buffer, sizeof(buffer), "%.1f", item.get_data());
	print_caption(screen, buffer, cell.w - 40, cell.y + cell.h/2, cell.w - 4, 1);
	
	return cell;
}

template <> 
Cell ActiveRenderer::operator()<ui::HeatControl >(ui::HeatControl&  item) const
{
	if(item.mode)
	{
		screen.draw_rectangle(cell.x +cell.w / 2 , cell.y, cell.w - cell.w / 2 , cell.h, 1);
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
	snprintf(buffer, sizeof(buffer), "%.0f/%.0f%cC", current, target, 248);
	print_caption(screen, buffer, cell.x, y_center(cell), cell.w-4, 0, false, true);
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
	snprintf(buffer, sizeof(buffer), "%.0f/%.0f%cC", current, target, 248);
	print_caption(screen, buffer, cell.x, y_center(cell), cell.w-4, 1, false, true);
	
	return cell;
}

template <> 
Cell ActiveRenderer::operator()<ui::CharInfo >(ui::CharInfo&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 1);
	print_white_left(screen, cell, item.get_data());
	return cell;
}

template <> 
Cell DefaultRenderer::operator()<ui::CharInfo >(ui::CharInfo&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_black_left(screen, cell, item.caption);
	return cell;
}

template <> 
Cell ActiveRenderer::operator()<ui::FloatFloatInfo >(ui::FloatFloatInfo&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	const uint8_t margin = 2;
	char buffer[16];
	float target, current;
	std::tie(target, current) = item.get_data();
	snprintf(buffer, sizeof(buffer), "%.0f/%.0f%cC", current, target, 248);
	print_caption(screen, item.caption, cell.x + margin, y_center(cell), cell.w-margin, 1);
	print_caption(screen, buffer, cell.x+ margin, y_center(cell), cell.w - margin, 1, false, true);
	
	return cell;
}

template <> 
Cell DefaultRenderer::operator()<ui::FloatFloatInfo >(ui::FloatFloatInfo&  item) const
{
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	const uint8_t margin = 2;
	char buffer[16];
	float target, current;
	std::tie(target, current) = item.get_data();
	snprintf(buffer, sizeof(buffer), "%.0f/%.0f%cC", current, target, 248);
	print_caption(screen, item.caption, cell.x + margin, y_center(cell), cell.w-margin, 1);
	print_caption(screen, buffer, cell.x+ margin, y_center(cell), cell.w - margin, 1, false, true);
	
	return cell;
}

void progress_bar(Cell const & cell, Screen& screen, uint8_t percent)
{
	uint32_t bar_width = 0;
	const uint8_t margin = 1;
	uint32_t max_bar_width = cell.w - 2 * margin;
	if(percent <= 100 && percent >= 0)
	{
		bar_width = (max_bar_width * percent) / 100;
	}

	screen.xor_rectangle(cell.x + margin, cell.y + margin, bar_width, cell.y - margin);
}

template <> 
Cell ActiveRenderer::operator()<ui::ProgressInfo >(ui::ProgressInfo&  item) const
{
	std::string filename;
	const uint8_t margin = 2;
	uint32_t percent;
	std::tie(percent, filename) = item.get_data();
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 1);
	print_caption(screen, filename, cell.x+margin, y_center(cell), cell.w-margin, 0, true);
	uint32_t bar_width = cell.w - 2*margin;
	bar_width *= percent;
	bar_width /= 100;
	screen.xor_rectangle(cell.x+margin, cell.y+margin, bar_width, cell.h-2*margin);
	return cell;
}

template <> 
Cell DefaultRenderer::operator()<ui::ProgressInfo >(ui::ProgressInfo&  item) const
{
	std::string filename;
	const uint8_t margin = 2;
	uint32_t percent;
	std::tie(percent, filename) = item.get_data();
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 1);
	print_caption(screen, filename, cell.x+margin, y_center(cell), cell.w-margin, 0, true);
	uint32_t bar_width = cell.w - 2*margin;
	bar_width *= percent;
	bar_width /= 100;
	screen.xor_rectangle(cell.x+margin, cell.y+margin, bar_width, cell.h-2*margin);
	return cell;
}

template <> 
Cell ActiveRenderer::operator()<ui::TimeInfo >(ui::TimeInfo&  item) const
{
	uint32_t elapsed, remaining;
	const uint8_t margin = 2;
	std::tie(elapsed, remaining) = item.get_data();
	char buffer_elapsed[14];
	char buffer_remaining[12];
	snprintf(buffer_elapsed, 14, "%d min %ds", elapsed/60, elapsed%60);
	snprintf(buffer_remaining, 12, "%d min", remaining/60);
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_caption(screen, buffer_elapsed, cell.x+margin, y_center(cell), cell.w-margin, 1, false);
	print_caption(screen, buffer_remaining, cell.x+margin, y_center(cell), cell.w-margin, 1, false, true);
	return cell;
}

template <> 
Cell DefaultRenderer::operator()<ui::TimeInfo >(ui::TimeInfo&  item) const
{
	uint32_t elapsed, remaining;
	const uint8_t margin = 2;
	std::tie(elapsed, remaining) = item.get_data();
	char buffer_elapsed[14];
	char buffer_remaining[12];
	snprintf(buffer_elapsed, 14, "%d min %ds", elapsed/60, elapsed%60);
	snprintf(buffer_remaining, 12, "%d min", remaining/60);
	screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
	print_caption(screen, buffer_elapsed, cell.x+margin, y_center(cell), cell.w-margin, 1, false);
	print_caption(screen, buffer_remaining, cell.x+margin, y_center(cell), cell.w-margin, 1, false, true);
	return cell;
}

template <> 
Cell ActiveRenderer::operator()<ui::LogoItem >(ui::LogoItem&  item) const
{
	screen.draw_picture(cell.x + 12, cell.y, 104, 64, item.icon, false);
	return cell;
}

template <> 
Cell DefaultRenderer::operator()<ui::LogoItem >(ui::LogoItem&  item) const
{
	screen.draw_picture(cell.x + 12, cell.y, 104, 64, item.icon, false);
	return cell;
}

} // namespace ui