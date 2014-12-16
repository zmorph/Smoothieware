#ifndef __EVENT_H__
#define __EVENT_H__

#include "boost/variant/static_visitor.hpp"

#include "Link.h"
#include "File.h"

namespace ui
{

class Event: public boost::static_visitor<Link>
{
public:
	Event& set_active_link(Link link)
	{
		this->active_link = link;
		return *this;
	}

protected:
	Link active_link;

	Link next()
	{
		return active_link.get_group()->get_link_to(active_link.get_index() + 1);
	}

	Link previous() // Test for comming back
	{
		Group* group = active_link.get_group();
		return group->get_link_to((active_link.get_index() == 0) ? group->size() - 1: active_link.get_index() - 1);
	}

	Link linked()
	{
		return active_link.get_group()->get_link_for(active_link.get_index());
	}

	Link back()
	{
		return active_link.get_group()->get_former_active();
	}

	Link same()
	{
		return active_link;
	}
};

struct UpEvent: public Event
{
	template <typename ItemType>
	Link operator()(ItemType& active_item)
	{
		return previous();
	}
};

struct DownEvent: public Event
{
	template <typename ItemType>
	Link operator()(ItemType& active_item)
	{
		return next();
	}
};

struct OkEvent: public Event
{
	template <typename ItemType>
	Link operator()(ItemType& active_item)
	{
		return linked();
	}
};

struct TickEvent: public Event
{
	template <typename ItemType>
	Link operator()(ItemType& active_item)
	{
		return same();
	}
};

void play_file(std::string const & path);

template <>
Link OkEvent::operator()<ui::File>(ui::File& active_item);

template <>
Link DownEvent::operator()<ui::File>(ui::File& active_item);

template <>
Link UpEvent::operator()<ui::File>(ui::File& active_item);

template <>
Link OkEvent::operator()<ui::Command>(ui::Command& active_item);

template <>
Link OkEvent::operator()<ui::ConditionalCommand>(ui::ConditionalCommand& active_item);

template <>
Link OkEvent::operator()<ui::PositionControl >(ui::PositionControl&  active_item);

template <>
Link DownEvent::operator()<ui::PositionControl >(ui::PositionControl&  active_item);

template <>
Link UpEvent::operator()<ui::PositionControl >(ui::PositionControl&  active_item);

template <>
Link OkEvent::operator()<ui::HeatControl >(ui::HeatControl&  active_item);

template <>
Link DownEvent::operator()<ui::HeatControl >(ui::HeatControl&  active_item);

template <>
Link UpEvent::operator()<ui::HeatControl >(ui::HeatControl&  active_item);

template <>
Link TickEvent::operator()<ui::LogoItem >(ui::LogoItem&  active_item);

} // namespace ui

#endif // __EVENT_H__