#include "Widget.h"
// #include "Layout.h"
// #include "Group.h"


namespace ui
{
	Widget::Widget(Layout* layout)
	:layout(layout)
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

	void Widget::set_group(Group* group)
	{
		this->group = group;
	}

} // namespace ui