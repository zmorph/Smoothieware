#include "RenderPolicy.h"

namespace ui
{

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
Dimensions DefaultMultilineRenderPolicy::apply_to<ui::File const>(ui::File const & item, Dimensions const & dimensions, Screen &screen) const
{
	return dimensions;
}

template <> 
Dimensions ActiveMultilineRenderPolicy::apply_to<ui::File const>(ui::File const & item, Dimensions const & dimensions, Screen &screen) const
{
	return dimensions;
}

template <>
Dimensions ActiveRenderPolicy::apply_to<ui::GraphicalItem const>(ui::GraphicalItem const & item, Dimensions const & dimensions, Screen &screen) const
{
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 1);
	print_caption(screen, item.caption, dimensions.x+1, dimensions.y + dimensions.h/2, dimensions.w-1, 0, true);
	return dimensions;
}

template <>
Dimensions DefaultRenderPolicy::apply_to<ui::GraphicalItem const>(ui::GraphicalItem const & item, Dimensions const & dimensions, Screen &screen) const
{
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
	screen.draw_picture(x_center(dimensions) - 12, y_center(dimensions) - 12, 24, 24, item.icon, false);
	return dimensions;
}

template <>
ui::Dimensions ActiveRenderPolicy::apply_to<ui::File const>(ui::File const & item, ui::Dimensions const & dimensions, ui::Screen &screen) const
{
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 1);
	print_white_left(screen, dimensions, item.get_name());
	return dimensions;
}

template <>
ui::Dimensions DefaultRenderPolicy::apply_to<ui::File const>(ui::File const & item, ui::Dimensions const & dimensions, ui::Screen &screen) const
{
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
	print_black_left(screen, dimensions, item.get_name());
	return dimensions;
}


template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::PositionControl const>(ui::PositionControl const & item, Dimensions const & dimensions, Screen &screen) const
{
	if(item.mode)
	{
		screen.draw_rectangle(dimensions.x +dimensions.w / 2 + 4, dimensions.y, dimensions.w - dimensions.w / 2 - 4 , dimensions.h, 1);
		print_black_left(screen, dimensions, item.caption);
	}
	else
	{
		screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 1);
		print_white_left(screen, dimensions, item.caption);
	}
	char buffer[10];
	snprintf(buffer, sizeof(buffer), "%.1f", item.get_data());
	//print_white_right();
	print_caption(screen, buffer, dimensions.w - 40, dimensions.y + dimensions.h/2, dimensions.w - 4, 0);
	return dimensions;
}

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::PositionControl const>(ui::PositionControl const &  item, Dimensions const & dimensions, Screen &screen) const
{
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
	print_caption(screen, item.caption, dimensions.x + 4, dimensions.y + dimensions.h/2, dimensions.w - 4, 1);
	char buffer[10];
	snprintf(buffer, sizeof(buffer), "%.1f", item.get_data());
	print_caption(screen, buffer, dimensions.w - 40, dimensions.y + dimensions.h/2, dimensions.w - 4, 1);
	
	return dimensions;
}

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::HeatControl const>(ui::HeatControl const & item, Dimensions const & dimensions, Screen &screen) const
{
	if(item.mode)
	{
		screen.draw_rectangle(dimensions.x +dimensions.w / 2 , dimensions.y, dimensions.w - dimensions.w / 2 , dimensions.h, 1);
		print_caption(screen, item.caption, dimensions.x + 4, dimensions.y + dimensions.h/2, dimensions.w - 4, 1);
	}
	else
	{
		screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 1);
		print_caption(screen, item.caption, dimensions.x + 4, dimensions.y + dimensions.h/2, dimensions.w - 4, 0);
	}
	char buffer[16];
	float target, current;
	std::tie(target, current) = item.get_data();
	snprintf(buffer, sizeof(buffer), "%.0f/%.0f%cC", current, target, 248);
	print_caption(screen, buffer, dimensions.x, y_center(dimensions), dimensions.w-4, 0, false, true);
	return dimensions;
}

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::HeatControl const>(ui::HeatControl const & item, Dimensions const & dimensions, Screen &screen) const
{
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
	print_caption(screen, item.caption, dimensions.x + 4, dimensions.y + dimensions.h/2, dimensions.w - 4, 1);
	char buffer[16];
	float target, current;
	std::tie(target, current) = item.get_data();
	snprintf(buffer, sizeof(buffer), "%.0f/%.0f%cC", current, target, 248);
	print_caption(screen, buffer, dimensions.x, y_center(dimensions), dimensions.w-4, 1, false, true);
	
	return dimensions;
}

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::CharInfo const>(ui::CharInfo const & item, Dimensions const & dimensions, Screen &screen) const
{
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 1);
	print_white_left(screen, dimensions, item.get_data());
	return dimensions;
}

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::CharInfo const>(ui::CharInfo const & item, Dimensions const & dimensions, Screen &screen) const
{
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
	print_black_left(screen, dimensions, item.caption);
	return dimensions;
}

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::FloatFloatInfo const>(ui::FloatFloatInfo const & item, Dimensions const & dimensions, Screen &screen) const
{
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
	const uint8_t margin = 2;
	char buffer[16];
	float target, current;
	std::tie(target, current) = item.get_data();
	snprintf(buffer, sizeof(buffer), "%.0f/%.0f%cC", current, target, 248);
	print_caption(screen, item.caption, dimensions.x + margin, y_center(dimensions), dimensions.w-margin, 1);
	print_caption(screen, buffer, dimensions.x+ margin, y_center(dimensions), dimensions.w - margin, 1, false, true);
	
	return dimensions;
}

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::FloatFloatInfo const>(ui::FloatFloatInfo const & item, Dimensions const & dimensions, Screen &screen) const
{
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
	const uint8_t margin = 2;
	char buffer[16];
	float target, current;
	std::tie(target, current) = item.get_data();
	snprintf(buffer, sizeof(buffer), "%.0f/%.0f%cC", current, target, 248);
	print_caption(screen, item.caption, dimensions.x + margin, y_center(dimensions), dimensions.w-margin, 1);
	print_caption(screen, buffer, dimensions.x+ margin, y_center(dimensions), dimensions.w - margin, 1, false, true);
	
	return dimensions;
}

// // void progress_bar(dimensions const & dimensions, Screen& screen, uint8_t percent)
// // {
// // 	uint32_t bar_width = 0;
// // 	const uint8_t margin = 1;
// // 	uint32_t max_bar_width = dimensions.w - 2 * margin;
// // 	if(percent <= 100 && percent >= 0)
// // 	{
// // 		bar_width = (max_bar_width * percent) / 100;
// // 	}

// // 	screen.xor_rectangle(dimensions.x + margin, dimensions.y + margin, bar_width, dimensions.y - margin);
// // }

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::ProgressInfo const>(ui::ProgressInfo const & item, Dimensions const & dimensions, Screen &screen) const
{
	std::string filename;
	const uint8_t margin = 2;
	uint32_t percent;
	std::tie(percent, filename) = item.get_data();
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 1);
	print_caption(screen, filename, dimensions.x+margin, y_center(dimensions), dimensions.w-margin, 0, true);
	uint32_t bar_width = dimensions.w - 2*margin;
	bar_width *= percent;
	bar_width /= 100;
	screen.xor_rectangle(dimensions.x+margin, dimensions.y+margin, bar_width, dimensions.h-2*margin);
	return dimensions;
}

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::ProgressInfo const>(ui::ProgressInfo const & item, Dimensions const & dimensions, Screen &screen) const
{
	std::string filename;
	const uint8_t margin = 2;
	uint32_t percent;
	std::tie(percent, filename) = item.get_data();
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 1);
	print_caption(screen, filename, dimensions.x+margin, y_center(dimensions), dimensions.w-margin, 0, true);
	uint32_t bar_width = dimensions.w - 2*margin;
	bar_width *= percent;
	bar_width /= 100;
	screen.xor_rectangle(dimensions.x+margin, dimensions.y+margin, bar_width, dimensions.h-2*margin);
	return dimensions;
}

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::TimeInfo const>(ui::TimeInfo const & item, Dimensions const & dimensions, Screen &screen) const
{
	uint32_t elapsed, remaining;
	const uint8_t margin = 2;
	std::tie(elapsed, remaining) = item.get_data();
	char buffer_elapsed[14];
	char buffer_remaining[12];
	snprintf(buffer_elapsed, 14, "%d min %ds", elapsed/60, elapsed%60);
	snprintf(buffer_remaining, 12, "%d min", remaining/60);
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
	print_caption(screen, buffer_elapsed, dimensions.x+margin, y_center(dimensions), dimensions.w-margin, 1, false);
	print_caption(screen, buffer_remaining, dimensions.x+margin, y_center(dimensions), dimensions.w-margin, 1, false, true);
	return dimensions;
}

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::TimeInfo const>(ui::TimeInfo const & item, Dimensions const & dimensions, Screen &screen) const
{
	uint32_t elapsed, remaining;
	const uint8_t margin = 2;
	std::tie(elapsed, remaining) = item.get_data();
	char buffer_elapsed[14];
	char buffer_remaining[12];
	snprintf(buffer_elapsed, 14, "%d min %ds", elapsed/60, elapsed%60);
	snprintf(buffer_remaining, 12, "%d min", remaining/60);
	screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
	print_caption(screen, buffer_elapsed, dimensions.x+margin, y_center(dimensions), dimensions.w-margin, 1, false);
	print_caption(screen, buffer_remaining, dimensions.x+margin, y_center(dimensions), dimensions.w-margin, 1, false, true);
	return dimensions;
}

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::LogoItem const>(ui::LogoItem const & item, Dimensions const & dimensions, Screen &screen) const
{
	screen.draw_picture(dimensions.x + 12, dimensions.y, 104, 64, item.icon, false);
	return dimensions;
}

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::LogoItem const>(ui::LogoItem const &  item, Dimensions const & dimensions, Screen &screen) const
{
	screen.draw_picture(dimensions.x + 12, dimensions.y, 104, 64, item.icon, false);
	return dimensions;
}

} // namespace ui
