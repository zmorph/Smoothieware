#ifndef __CELL_H__
#define __CELL_H__

#include "boost/variant.hpp"
#include "RenderPolicy.h"
#include "Dimensions.h"

namespace ui
{

struct Cell
{
	Cell(Dimensions dimensions, RenderPolicy render_policy)
	:dimensions(dimensions), default_render_policy(render_policy), 
		active_render_policy(render_policy)
	{}

	Cell(Dimensions dimensions, RenderPolicy default_render_policy, RenderPolicy active_render_policy)
	:dimensions(dimensions), default_render_policy(default_render_policy), 
		active_render_policy(active_render_policy)
	{}

	Dimensions dimensions;
	RenderPolicy default_render_policy;
	RenderPolicy active_render_policy;
};

} // namespace ui

#endif // __CELL_H__