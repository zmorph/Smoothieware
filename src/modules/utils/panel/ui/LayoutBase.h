#ifndef __LAYOUT_BASE_H__
#define __LAYOUT_BASE_H__

#include <cstddef>

namespace ui
{

// UNIFY with Group
template <typename CellType>
struct LayoutBase
{
	template<size_t i>
	LayoutBase(const CellType(&cells)[i])
	:_size(i), cells(cells)
	{
	}

	CellType const * begin()
	{
		return &(cells[0]);
	}

	CellType const * end()
	{
		return &(cells[_size]);
	}

	const CellType& operator[](size_t i)
	{
		return cells[i%_size];
	}

	size_t size()
	{
		return _size;
	}

private:
	size_t _size;
	CellType const * const cells;
};

} // namespace ui

#endif // __LAYOUT_BASE_H__