#ifndef __LINKBASE_H__
#define __LINKBASE_H__

#include <cstddef>
#include <functional>

namespace ui
{

struct DupaLink
{
	void* dupa;
	size_t index;
};

bool always();

template <typename ContainerType>
struct LinkBase
{
	LinkBase(size_t index = 0, ContainerType* group = nullptr)
	:condition(always), index(index), group(group), index_false(index), group_false(group)
	{}

	LinkBase(std::function<bool()> condition, size_t index = 0, ContainerType* group = nullptr, size_t index_false = 0, ContainerType* group_false = nullptr)
	:condition(condition), index(index), group(group), index_false(index_false), group_false(group_false)
	{}

	size_t get_index() const
	{
		if(condition())
		{
			return index;
		}
		else
		{
			
			return index_false;
		}
	}

	ContainerType * get_group() const
	{
		if(condition())
		{
			return group;
		}
		else
		{
			
			return group_false;
		}
	}

	typename ContainerType::ElementType const & operator*()
	{
		return &(this->get());
	}

	// CBFF
	typename ContainerType::ElementType const * get()
	{
		if(condition())
		{
			return &(*group)[index];
		}
		else
		{
			return &(*group_false)[index];
		}
	}

	// CBFF
	typename ContainerType::ElementType const * fetch()
	{
		return get();
	}

protected:
	std::function<bool()> condition;
	size_t index;
	ContainerType* group;
	size_t index_false;
	ContainerType* group_false;
};

} // namespace ui

#endif // __LINKBASE_H__