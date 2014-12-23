#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "boost/variant/apply_visitor.hpp"
#include <algorithm>
#include "screen/Screen.h"
#include "Renderer.h"
#include "RenderBlockQueue.h"
#include "CompositeItem.h"
#include "Link.h"

namespace ui
{

class Layout;

class UserInterface
{
	
	ui::Link active;
	ui::Screen& screen;
	RenderBlockQueue render_blocks;

public:
	UserInterface(ui::Link active, ui::Screen& screen)
	:active(active), screen(screen)
	{}

	template <typename EventType>
	void dispatch(EventType event)
	{
		event.set_active_link(active);
		Link former_active = active;
		active = boost::apply_visitor(event, *active.fetch());
		active.get_group()->set_former_active(former_active);
	}

	void render();
	void refresh();
private:
	Dimensions render_active(CompositeItem const & item, const Cell& cell, Screen& screen);
	Dimensions render(CompositeItem const & item, const Cell& cell, Screen& screen);
	Dimensions clear(const Dimensions& cell, Screen& screen);
	
};

} // namespace ui

#endif // __USER_INTERFACE_H__