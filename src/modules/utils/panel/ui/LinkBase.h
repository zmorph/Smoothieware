#ifndef __LINKBASE_H__
#define __LINKBASE_H__

namespace ui
{

template <typename ContainerType>
struct LinkBase
{
	LinkBase(size_t index = 0, ContainerType* group = nullptr)
	:index(index), group(group)
	{}

	// RAII
	LinkBase<ContainerType>& set_index(size_t index)
	{
		this->index = index;
		return *this;
	}

	size_t get_index()
	{
		return index;
	}

	// RAII
	LinkBase<ContainerType>& set_group(ContainerType* group)
	{
		this->group = group;
		return *this;
	}

	ContainerType* get_group()
	{
		return group;
	}

	typename ContainerType::ElementType& operator*()
	{
		return &(this->get());
	}

	// CBFF
	typename ContainerType::ElementType* get()
	{
		return &(*group)[index];
	}

	// CBFF
	typename ContainerType::ElementType* fetch()
	{
		return get();
	}
private:
	size_t index;
	ContainerType* group;
};

} // namespace ui

#endif // __LINKBASE_H__