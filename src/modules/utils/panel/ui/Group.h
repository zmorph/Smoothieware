#ifndef __GROUP_H__
#define __GROUP_H__

#include "CompositeItem.h"
#include "LinkBase.h"
#include "Widget.h"

namespace ui
{

/**
	Group is a zip of list of items and a list of links.
*/

// maybe make it parametrised with enum class
// UNIFY with Layout
struct Group
{
	typedef LinkBase<Group> Link;
	typedef CompositeItem ElementType;

	template<size_t i>
	Group(CompositeItem(&items)[i], Link(&links)[i], Widget& widget)
	:widget(widget), _size(i), items(items), links(links), former(Link(0, this))
	{
		size_t index = 0;
		for(auto& link: links)
		{
			link.set_group(this).set_index(index);
			++index;
		}
		widget.set_group(this);
	}

	// Group& init_links()
	// {
	// 	size_t index = 0;
	// 	for(auto& link: links)
	// 	{
	// 		link.set_group(this).set_index(index);
	// 		++index;
	// 	}
	// 	return *this;
	// }

	Widget& get_widget()
	{
		return widget;
	}

	Group& set_link_for(size_t index, Link link)
	{
		if(index >= _size) //  || index < 0 gives warning
		{
			return *this;
		}
		
		links[index] = link;
		return *this;
	}

	Link get_link_to(size_t index)
	{
		return Link(index%_size, this);
	}

	Link get_link_for(size_t index)
	{
		return links[index];
	}
	
	CompositeItem* begin()
	{
		return &(items[0]);
	}

	CompositeItem* end()
	{
		return &(items[_size]);
	}

	Link get_former_active()
	{
		return former;
	}

	CompositeItem& operator[](size_t i)
	{
		return items[i%_size];
	}

	size_t size()
	{
		return _size;
	}
private:
	Widget& widget; // :(
		
	size_t _size;
	CompositeItem* items;
	Link* links;
	Link former;
	
};

} // namespace ui

#endif // __GROUP_H__