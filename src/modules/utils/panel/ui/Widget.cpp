#include "Widget.h"
// #include "Layout.h"
// #include "Group.h"


namespace ui
{
	Widget::Widget(Layout* layout)
	:layout(layout), global_layout(nullptr)
	{
	}

	Widget::Widget(Layout* layout, GlobalLayout * global_layout)
	:layout(layout), global_layout(global_layout)
	{
	}

	Layout* Widget::get_layout()
	{
		return layout;
	}

	Group* Widget::get_group()
	{
		return group;
	}

	void Widget::set_group(Group * group)
	{
		this->group = group;
	}

	void Widget::set_global_group(Group * group)
	{
		this->global_group = group;
	}

	GlobalLayout* Widget::get_global_layout()
	{
		return global_layout;
	}

	Group* Widget::get_global_group()
	{
		return global_group;
	}
} // namespace ui