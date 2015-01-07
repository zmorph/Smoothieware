#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "Layout.h"
#include "GlobalLayout.h"

namespace ui
{

class Group;

/*
Widget
Ultimately it would be a list of pairs (layout, group).
Maybe additional explicit mapping behaviour for each pair would be beneficial.
*/

class Widget
{
public:
	Widget(Layout* layout);
	Widget(Layout* layout, GlobalLayout* global_layout);
	Layout* get_layout();
	Group* get_group();
	GlobalLayout* get_global_layout();
	Group* get_global_group();
	void set_group(Group * group);
	void set_global_group(Group * group);
private:
	Layout * layout;
	Group  * group;
	GlobalLayout * global_layout;
	Group  * global_group;
};

} // namespace ui
#endif // __WIDGET_H__