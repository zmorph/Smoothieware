#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "boost/variant/apply_visitor.hpp"
#include <algorithm>
#include "screen/Screen.h"
#include "Renderer.h"
#include "RenderBlockQueue.h"
#include "CompositeItem.h"
#include "Link.h"
#include "libs/Kernel.h"		// !!!
#include "StreamOutputPool.h"   // !!!

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
	{
    	THEKERNEL->streams->printf("UI initialized\r\n");
	}

	template <typename EventType>
	void dispatch(EventType event)
	{
		event.set_active_link(active);
		active = boost::apply_visitor(event, *active.fetch());
	}

	void render();
	void refresh();
private:
	Cell render_active(CompositeItem &item, const Cell& cell, Screen& screen);
	Cell render(CompositeItem &item, const Cell& cell, Screen& screen);
	Cell clear(const Cell& cell, Screen& screen);
	
};

} // namespace ui

#endif // __USER_INTERFACE_H__