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
	Group(const CompositeItem(&items)[i], Link(&links)[i], Widget& widget)
	:widget(widget), _size(i), items(items), links(links), former(Link(0, this))
	{
		size_t index = 0;
		for(auto& link: links)
		{
			link = Link(index, this);
			++index;
		}
		widget.set_group(this);
	}

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
	
	CompositeItem const * begin()
	{
		return &(items[0]);
	}

	CompositeItem const * end()
	{
		return &(items[_size]);
	}

	void set_former_active(Link link)
	{
		former = link;
	}

	Link get_former_active()
	{
		return former;
	}

	CompositeItem const & operator[](size_t i)
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
	CompositeItem const * items;
	Link* links;
	Link former;
	
};

} // namespace ui

#endif // __GROUP_H__