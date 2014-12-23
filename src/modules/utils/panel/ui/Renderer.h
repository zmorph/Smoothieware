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
		render_policy.apply_to(item, dimensions, screen);
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
// then Renderers would be:
//

/*
class ActiveRenderer: public Renderer
{
public:
	ActiveRenderer(Frame& frame)
	:Renderer(frame)
	{
	}

	template <typename ItemType, typename RenderPolicy>
	Cell operator()(ItemType& item, RenderPolicy& render_policy) const
	{
		render_policy.apply_to(item, frame);
		return dimensions;
	}
};

*/

/*
	ui.render_active
		ActiveRenderer(), item, cell
*/

// template <> 
// Cell ActiveRenderer::operator()<ui::GraphicalItem>(ui::GraphicalItem& item) const;

// template <> 
// Cell DefaultRenderer::operator()<ui::GraphicalItem>(ui::GraphicalItem& item) const;

// template <> 
// Cell ActiveRenderer::operator()<ui::File>(ui::File& item) const;

// template <> 
// Cell DefaultRenderer::operator()<ui::File>(ui::File& item) const;

// template <> 
// Cell ActiveRenderer::operator()<ui::PositionControl >(ui::PositionControl&  item) const;

// template <>
// Cell DefaultRenderer::operator()<ui::PositionControl >(ui::PositionControl&  item) const;

// template <> 
// Cell ActiveRenderer::operator()<ui::HeatControl >(ui::HeatControl&  item) const;

// template <> 
// Cell DefaultRenderer::operator()<ui::HeatControl >(ui::HeatControl&  item) const;

// template <> 
// Cell ActiveRenderer::operator()<ui::CharInfo >(ui::CharInfo&  item) const;

// template <> 
// Cell DefaultRenderer::operator()<ui::CharInfo >(ui::CharInfo&  item) const;

// template <> 
// Cell ActiveRenderer::operator()<ui::FloatFloatInfo >(ui::FloatFloatInfo&  item) const;

// template <> 
// Cell DefaultRenderer::operator()<ui::FloatFloatInfo >(ui::FloatFloatInfo&  item) const;

// template <> 
// Cell ActiveRenderer::operator()<ui::ProgressInfo >(ui::ProgressInfo&  item) const;

// template <> 
// Cell DefaultRenderer::operator()<ui::ProgressInfo >(ui::ProgressInfo&  item) const;

// template <> 
// Cell ActiveRenderer::operator()<ui::TimeInfo >(ui::TimeInfo&  item) const;

// template <> 
// Cell DefaultRenderer::operator()<ui::TimeInfo >(ui::TimeInfo&  item) const;

// template <> 
// Cell ActiveRenderer::operator()<ui::LogoItem >(ui::LogoItem&  item) const;

// template <> 
// Cell DefaultRenderer::operator()<ui::LogoItem >(ui::LogoItem&  item) const;
} // namespace ui

#endif // __RENDERER_H__