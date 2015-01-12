#ifndef __PRINTING_H__
#define __PRINTING_H__

#include "Dimensions.h"
#include "screen/Screen.h"
#include <string>

namespace ui
{

constexpr size_t letter_and_whitespace(size_t letter_width, size_t letter_offset)
{
	return letter_width + letter_offset;
}

template <size_t letter_width, size_t letter_offset, bool color>
void print(Screen& screen, std::string const & sequence, size_t x_origin, size_t y_origin)
{
	size_t letter_index = 0;
	for(auto letter: sequence)
	{
		screen.draw_char(x_origin +  letter_and_whitespace(letter_width, letter_offset) * letter_index, y_origin, letter, color);
		++letter_index;
	}
}

template <size_t letter_width, size_t letter_offset, bool color>
void print_n(size_t n, Screen& screen, std::string const & sequence, size_t x_origin, size_t y_origin)
{
	print<letter_width, letter_offset, color>(screen, sequence.substr(0, n), x_origin, y_origin);
}

template <size_t margin, size_t letter_width, size_t letter_height, size_t letter_offset, bool color>
void print_left_base(Screen& screen, Dimensions const & cell, std::string const & sequence)
{
	if(letter_height + 2 * margin > cell.h)
	{
		return;
	}

	size_t sequence_width = sequence.size() * letter_and_whitespace(letter_width, letter_offset) + 2 * margin;
	size_t x_origin = cell.x + margin; 
	size_t y_origin = y_center(cell) - letter_height / 2;

	if(sequence_width < cell.w)
	{
		print<letter_width, letter_offset, color>(screen, sequence, x_origin, y_origin);
	}
	else
	{
		size_t number_of_letters_to_write = (cell.w - 2 * margin) / letter_and_whitespace(letter_width, letter_offset);
		print_n<letter_width, letter_offset, color>(number_of_letters_to_write, screen, sequence, x_origin, y_origin);
	}
}



template <size_t margin, size_t letter_width, size_t letter_height, size_t letter_offset, bool color>
void print_center_base(Screen& screen, Dimensions const & cell, std::string const & sequence)
{
	if(letter_height + 2 * margin > cell.h)
	{
		return;
	}
	
	size_t sequence_width = sequence.size() * letter_and_whitespace(letter_width, letter_offset) + 2 * margin;
	size_t y_origin = y_center(cell) - letter_height / 2;
	

	if(sequence_width < cell.w)
	{
		size_t x_origin = cell.x + (cell.w - sequence_width) / 2;
		print<letter_width, letter_offset, color>(screen, sequence, x_origin, y_origin);
	}
	else
	{
		size_t number_of_letters_to_write = (cell.w - 2 * margin) / letter_and_whitespace(letter_width, letter_offset);
		size_t truncated_sequence_width   = number_of_letters_to_write * letter_and_whitespace(letter_width, letter_offset) + 2 * margin;
		size_t x_origin = cell.x + (cell.w - truncated_sequence_width) / 2;
		print_n<letter_width, letter_offset, color>(number_of_letters_to_write, screen, sequence, x_origin, y_origin);
	}
}

template <size_t margin, size_t letter_width, size_t letter_height, size_t letter_offset, bool color>
void print_right_base(Screen& screen, Dimensions const & cell, std::string const & sequence)
{
	if(letter_height + 2 * margin > cell.h)
	{
		return;
	}
	
	size_t sequence_width = sequence.size() * letter_and_whitespace(letter_width, letter_offset) + 2 * margin;
	size_t y_origin = y_center(cell) - letter_height / 2;

	if(sequence_width < cell.w)
	{
		size_t x_origin = cell.x + (cell.w - sequence_width);
		print<letter_width, letter_offset, color>(screen, sequence, x_origin, y_origin);
	}
	else
	{
		size_t number_of_letters_to_write = (cell.w - 2 * margin) / letter_and_whitespace(letter_width, letter_offset);
		size_t truncated_sequence_width   = number_of_letters_to_write * letter_and_whitespace(letter_width, letter_offset) + 2 * margin;
		size_t x_origin = cell.x + (cell.w - truncated_sequence_width);
		print_n<letter_width, letter_offset, color>(number_of_letters_to_write, screen, sequence, x_origin, y_origin);
	}
}


void print_black_left(Screen& screen, Dimensions const & cell, std::string const & sequence);
void print_white_left(Screen& screen, Dimensions const & cell, std::string const & sequence);
void print_black_center(Screen& screen, Dimensions const & cell, std::string const & sequence);
void print_white_center(Screen& screen, Dimensions const & cell, std::string const & sequence);
void print_black_right(Screen& screen, Dimensions const & cell, std::string const & sequence);
void print_white_right(Screen& screen, Dimensions const & cell, std::string const & sequence);
void print_multiline_black_center(Screen& screen, Dimensions const & cell, std::string const & sequence);
void print_multiline_white_center(Screen& screen, Dimensions const & cell, std::string const & sequence);

} // namespace ui

#endif // __PRINTING_H__