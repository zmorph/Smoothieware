#ifndef __GLOBAL_CELL_H__
#define __GLOBAL_CELL_H__

#include "boost/variant.hpp"
#include "RenderPolicy.h"
#include "Dimensions.h"

namespace ui
{

struct GlobalCell
{
	GlobalCell(Dimensions dimensions, GlobalRenderPolicy render_policy)
	:dimensions(dimensions), render_policy(render_policy) 
	{}

	Dimensions dimensions;
	GlobalRenderPolicy render_policy;
};

} // namespace ui

#endif // __GLOBAL_CELL_H__