#ifndef __DIMENSIONS_H__
#define __DIMENSIONS_H__

#include <cstdint>

namespace ui
{

struct Dimensions
{
	uint8_t x, y, w, h;
};

template <typename DimensionsType>
constexpr uint8_t x_center(DimensionsType const & dimensions)
{
	return dimensions.x + dimensions.w / 2;
}
template <typename DimensionsType>
constexpr uint8_t y_center(DimensionsType const & dimensions)
{
	return dimensions.y + dimensions.h / 2;
}
template <typename DimensionsType>
constexpr uint8_t will_fit_cell(DimensionsType const & dimensions, uint8_t width, uint8_t height)
{
	return (dimensions.w >= width) && (dimensions.y >= height);
}

} // namespace ui

#endif // __DIMENSIONS_H__