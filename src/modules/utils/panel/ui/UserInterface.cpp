#include "UserInterface.h"
#include "Layout.h"
#include "Group.h"
//#include "Widget.h"

namespace ui
{
	void UserInterface::render()
	{
		Group& active_group 	= *active.get_group();
		Layout& active_layout 	= *active_group.get_widget().get_layout();
		size_t active_item_index = active.get_index();
		size_t group_size = active_group.size();
		size_t layout_size = active_layout.size();

		if(layout_size == group_size)
		{
			size_t index = 0;
			for(auto& cell: active_layout)
			{
				if(index == active_item_index)
				{
					render_active(active_group[index], cell, screen);
				}
				else
				{
					render_blocks.push_back(render(active_group[index], cell, screen));
				}
				++index;
			}
		}
		else if(layout_size > group_size)
		{
			size_t index = 0;
			for(auto& cell: active_layout)
			{
				if(index < group_size)
				{
					if(index == active_item_index)
					{
						render_blocks.push_back(render_active(active_group[index], cell, screen));
					}
					else
					{
						render_blocks.push_back(render(active_group[index], cell, screen));
					}
				}
				else
				{
					render_blocks.push_back(clear(cell.dimensions, screen));
				}
				++index;
			}
		}
		else
		{
			render_blocks.push_back(render_active(active_group[active_item_index], active_layout[0], screen));
			for(size_t cell_index = 1, item_index = active_item_index + 1; cell_index < layout_size; ++cell_index, ++item_index)
			{
				render_blocks.push_back(render(active_group[item_index], active_layout[cell_index], screen));
			}
		}
	}

	void UserInterface::refresh()
	{
		screen.refresh();
		// while(!render_blocks.is_empty())
		// {
		// 	Cell cell = render_blocks.pop_front();
		// 	screen.refresh(cell.x, cell.y, cell.w, cell.h);
		// }
	}

	Dimensions UserInterface::render_active(CompositeItem const & item, const Cell& cell, Screen& screen)
	{
		return boost::apply_visitor(Renderer(cell.dimensions, screen), item, cell.active_render_policy);
	}

	Dimensions UserInterface::render(CompositeItem const & item, const Cell& cell, Screen& screen)
	{
		return boost::apply_visitor(Renderer(cell.dimensions, screen), item, cell.default_render_policy);
	}

	Dimensions UserInterface::clear(const Dimensions& cell, Screen& screen)
	{
		screen.clear(cell.x, cell.y, cell.w, cell.h);
		return cell;
	}

} // namespace ui