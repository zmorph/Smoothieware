#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "boost/variant/static_visitor.hpp"
#include "screen/Screen.h"
#include "File.h"
#include "Cell.h"
#include "Item.h"

namespace ui
{

class Renderer: public boost::static_visitor<Dimensions>
{
public:
	Renderer(const Dimensions& dimensions, Screen& screen)
	:dimensions(dimensions), screen(screen)
	{

	}

	template <typename ItemType, typename RenderPolicy>
	Dimensions operator()(ItemType& item, RenderPolicy& render_policy) const
	{
		render_policy.template apply_to<ItemType>(item, dimensions, screen);
		return dimensions;
	}

protected:
	const Dimensions& dimensions;
	Screen& screen;
};


// maybe make Dimensions with drawing abilities already
// I mean sth like this
// Frame slice = screen.get<10, 32, 32, 100>(); //proper screen fit is compile time checked
// slice.pixel(1, 3); //local reference frame


} // namespace ui

#endif // __RENDERER_H__