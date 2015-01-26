#include "Printing.h"

namespace ui
{

void print_black_left(Screen& screen, Dimensions const & cell, std::string const & sequence)
{
	print_left_base<2, 5, 8, 1, true>(screen, cell, sequence);
}

void print_white_left(Screen& screen, Dimensions const & cell, std::string const & sequence)
{
	print_left_base<2, 5, 8, 1, false>(screen, cell, sequence);
}

void print_black_center(Screen& screen, Dimensions const & cell, std::string const & sequence)
{
	print_center_base<2, 5, 8, 1, true>(screen, cell, sequence);
}

void print_white_center(Screen& screen, Dimensions const & cell, std::string const & sequence)
{
	print_center_base<2, 5, 8, 1, false>(screen, cell, sequence);
}

void print_black_right(Screen& screen, Dimensions const & cell, std::string const & sequence)
{
	print_right_base<2, 5, 8, 1, true>(screen, cell, sequence);
}

void print_white_right(Screen& screen, Dimensions const & cell, std::string const & sequence)
{
	print_right_base<2, 5, 8, 1, false>(screen, cell, sequence);
}

void print_multiline_black_center(Screen& screen, Dimensions const & cell, std::string const & sequence)
{
	print_multiline_base<2, 5, 8, 1, 4, true>(screen, cell, sequence);
}

void print_multiline_white_center(Screen& screen, Dimensions const & cell, std::string const & sequence)
{
	print_multiline_base<2, 5, 8, 1, 4, false>(screen, cell, sequence);
}

} // namespace ui
