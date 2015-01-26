#ifndef __RENDER_POLICY_H__
#define __RENDER_POLICY_H__

#include "boost/variant.hpp"

#include "screen/Screen.h"
#include "Dimensions.h"
#include "Printing.h"
#include "File.h"
#include "Item.h"

namespace ui
{

struct DefaultRenderPolicy
{
	template <typename ItemType>
	Dimensions apply_to(ItemType const & item, Dimensions const & dimensions, Screen &screen) const
	{
		screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
		//print_multiline_black_center(screen, dimensions, item.caption);
		print_black_left(screen, dimensions, item.caption);
		return dimensions;
	}
};

struct ActiveRenderPolicy
{
	template <typename ItemType>
	Dimensions apply_to(ItemType const & item, Dimensions const & dimensions, Screen &screen) const
	{
		screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 1);
		print_white_left(screen, dimensions, item.caption);
		return dimensions;
	}
};

struct DefaultMultilineRenderPolicy
{
	template <typename ItemType>
	Dimensions apply_to(ItemType const & item, Dimensions const & dimensions, Screen &screen) const
	{
		screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
		print_multiline_black_center(screen, dimensions, item.caption);
		return dimensions;
	}
};

struct ActiveMultilineRenderPolicy
{
	template <typename ItemType>
	Dimensions apply_to(ItemType const & item, Dimensions const & dimensions, Screen &screen) const
	{
		screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 1);
		print_multiline_white_center(screen, dimensions, item.caption);
		return dimensions;
	}
};

struct DefaultGlobalRenderPolicy
{
	template <typename ItemType>
	Dimensions apply_to(ItemType const & item, Dimensions const & dimensions, Screen &screen, size_t number_of_items, size_t active_item_index) const
	{
		size_t bar_height 	= dimensions.h/number_of_items; //number_of_items should never be 0
		size_t remainder	= dimensions.h%number_of_items;
		size_t bar_y		= bar_height*active_item_index + ((active_item_index+1)*remainder)/number_of_items;
		screen.draw_rectangle(dimensions.x, dimensions.y, dimensions.w, dimensions.h, 0);
		screen.draw_rectangle(dimensions.x+1, bar_y, dimensions.w-1, bar_height, 1);
		return dimensions;
	}
};

template <> 
Dimensions DefaultMultilineRenderPolicy::apply_to<ui::File const>(ui::File const & item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions ActiveMultilineRenderPolicy::apply_to<ui::File const>(ui::File const & item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::File const>(ui::File const & item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::File const>(ui::File const & item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::GraphicalItem const>(const ui::GraphicalItem& item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::GraphicalItem const>(const ui::GraphicalItem& item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::PositionControl const>(const ui::PositionControl&  item, Dimensions const & dimensions, Screen &screen) const;

template <>
Dimensions DefaultRenderPolicy::apply_to<ui::PositionControl const>(const ui::PositionControl&  item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::HeatControl const>(const ui::HeatControl&  item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::HeatControl const>(const ui::HeatControl&  item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::CharInfo const>(const ui::CharInfo&  item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::CharInfo const>(const ui::CharInfo&  item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::FloatFloatInfo const>(const ui::FloatFloatInfo&  item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::FloatFloatInfo const>(const ui::FloatFloatInfo&  item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::ProgressInfo const>(const ui::ProgressInfo&  item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::ProgressInfo const>(const ui::ProgressInfo&  item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions ActiveRenderPolicy::apply_to<ui::TimeInfo const>(const ui::TimeInfo&  item, Dimensions const & dimensions, Screen &screen) const;

template <> 
Dimensions DefaultRenderPolicy::apply_to<ui::TimeInfo const>(const ui::TimeInfo&  item, Dimensions const & dimensions, Screen &screen) const;

template <> 
ui::Dimensions ActiveRenderPolicy::apply_to<ui::LogoItem const>(const ui::LogoItem&  item, ui::Dimensions const & dimensions, ui::Screen &screen) const;

template <> 
ui::Dimensions DefaultRenderPolicy::apply_to<ui::LogoItem const>(const ui::LogoItem&  item, ui::Dimensions const & dimensions, ui::Screen &screen) const;

using RenderPolicy = boost::variant<DefaultRenderPolicy, ActiveRenderPolicy, DefaultMultilineRenderPolicy, ActiveMultilineRenderPolicy>;
using GlobalRenderPolicy = boost::variant<DefaultGlobalRenderPolicy>;

} // namespace

#endif // __RENDER_POLICY_H__
