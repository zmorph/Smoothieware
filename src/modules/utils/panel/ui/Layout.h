#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include "Cell.h"

namespace ui
{

// UNIFY with Group
struct Layout
{
	template<size_t i>
	Layout(const Cell(&cells)[i])
	:_size(i), cells(cells)
	{
	}

	Cell const * begin()
	{
		return &(cells[0]);
	}

	Cell const * end()
	{
		return &(cells[_size]);
	}

	const Cell& operator[](size_t i)
	{
		return cells[i%_size];
	}

	size_t size()
	{
		return _size;
	}

private:
	size_t _size;
	Cell const * const cells;
};

} // namespace ui

#endif // __LAYOUT_H__