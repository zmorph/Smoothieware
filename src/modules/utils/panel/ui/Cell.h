#ifndef __CELL_H__
#define __CELL_H__

namespace ui
{

struct Cell
{
	uint8_t x, y, w, h;
};

constexpr uint8_t x_center(Cell const & cell)
{
	return cell.x + cell.w / 2;
}

constexpr uint8_t y_center(Cell const & cell)
{
	return cell.y + cell.h / 2;
}

constexpr uint8_t will_fit_cell(Cell const & cell, uint8_t width, uint8_t height)
{
	return (cell.w >= width) && (cell.y >= height);
}

} // namespace ui

#endif // __CELL_H__