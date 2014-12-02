#ifndef __WIDGET_H__
#define __WIDGET_H__

namespace ui
{

class Group;
class Layout;

class Widget
{
public:
	Widget(Layout* layout);
	Layout* get_layout();
	Group* get_group();
	void set_group(Group * const group);
private:
	Layout * layout;
	Group * group;
};

} // namespace ui
#endif // __WIDGET_H__