#ifndef __GLOBAL_RENDERER_H__
#define __GLOBAL_RENDERER_H__

#include "boost/variant/static_visitor.hpp"
#include "screen/Screen.h"
#include "Cell.h"
#include "Item.h"

namespace ui
{

class GlobalRenderer: public boost::static_visitor<Dimensions>
{
public:
	GlobalRenderer(const Dimensions& dimensions, Screen& screen, size_t number_of_items, size_t active_item_index)
	:dimensions(dimensions), screen(screen), number_of_items(number_of_items), active_item_index(active_item_index)
	{

	}

	template <typename ItemType, typename RenderPolicy>
	Dimensions operator()(ItemType& item, RenderPolicy& render_policy) const
	{
		render_policy.template apply_to<ItemType>(item, dimensions, screen, number_of_items, active_item_index);
		return dimensions;
	}

protected:
	const Dimensions& dimensions;
	Screen& screen;
	size_t number_of_items;
	size_t active_item_index;
};

} // namespace ui

#endif // __GLOBAL_RENDERER_H__
