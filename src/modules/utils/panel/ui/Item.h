#ifndef __ITEM_H__
#define __ITEM_H__

#include <functional>
#include <tuple>

namespace ui
{

template <class DataType> using DataGetter = DataType(*)(void);
template <class DataType> using DataSetter = void(*)(DataType);
using CommandSender = void(*)(void);

class Item
{
public:
	constexpr Item(char const * const caption)
	:caption(caption)
	{}

	char const * const caption;
};


class GraphicalItem : public Item
{
public:
	constexpr GraphicalItem(char const * const caption, uint8_t const * const icon)
	:Item(caption), icon(icon)
	{}

	uint8_t const * const icon;
};

class LogoItem : public GraphicalItem
{
public:
	constexpr LogoItem(char const * const caption, uint8_t const * const picture, uint8_t duration)
	:GraphicalItem(caption, picture), duration(duration), elapsed(0)
	{}

	uint8_t duration;
	uint8_t elapsed;
};

class TimedItem : public Item
{
public:
	constexpr TimedItem(char const * const caption, uint8_t duration)
	:Item(caption), duration(duration), elapsed(0)
	{}

	uint8_t duration;
	uint8_t elapsed;
};

/*
	Control item should be able to present and manipulate some state property.
*/
template <typename DataType>
class Control : public Item
{
public:
	constexpr Control(char const * const caption, DataGetter<DataType> getter, DataSetter<DataType> setter)
	: Item(caption), get_data(getter), set_data(setter)
	{

	}
	DataGetter<DataType> get_data;
	DataSetter<DataType> set_data;
};

/*
	Switch item is a simple bool control.
*/
typedef Control<bool> Switch;

/*
	Stateful control is a control that can store data.
*/
template <typename StateType, typename DataType>
class StatefulControl : public Control<DataType>
{
public:
	StateType state;
};

/*
	Command item sends command.
*/
class Command : public Item
{
public:
	constexpr Command(char const * const caption, CommandSender sender, bool return_after_action = false)
	: Item(caption), send_command(sender), return_after_action(return_after_action)
	{

	}
	CommandSender send_command;
	bool return_after_action;
};


class ConditionalCommand : public Item
{
public:
	using Condition = DataGetter<bool>;
	constexpr ConditionalCommand(char const * const caption, Condition condition, CommandSender sender, bool return_after_action = false)
	: Item(caption), send_command(sender), condition(condition), return_after_action(return_after_action)
	{
	}

	CommandSender send_command;
	Condition condition;
	bool return_after_action;
};

/*
	Command item sends command.
*/
template <typename DataType>
class ComplexControl : public Item
{
public:
	constexpr ComplexControl(char const * const caption, DataGetter<DataType> getter, DataSetter<DataType> setter)
	: Item(caption), get_data(getter), set_data(setter), mode(false)
	{

	}
	DataGetter<DataType> get_data;
	DataSetter<DataType> set_data;
	bool mode;
};


template <typename DataType>
class PositionControlBase : public Item
{
public:
	constexpr PositionControlBase(char const * const caption, DataGetter<DataType> getter, DataSetter<DataType> setter)
	: Item(caption), get_data(getter), set_data(setter), mode(false)
	{

	}
	DataGetter<DataType> get_data;
	DataSetter<DataType> set_data;
	bool mode;
};

using PositionControl = PositionControlBase<float>;

template <typename DataType>
class InfoBase : public Item
{
public:
	constexpr InfoBase(char const * const caption, DataGetter<DataType> getter)
	: Item(caption), get_data(getter)
	{

	}
	DataGetter<DataType> get_data;
};

using CharInfo = InfoBase<std::string>;
using FloatFloatInfo = InfoBase<std::tuple<float, float> >;
using ProgressInfo = InfoBase<std::tuple<uint32_t, std::string> >;
using TimeInfo = InfoBase<std::tuple<uint32_t, uint32_t> >;

template <typename DataType>
class HeatControlBase : public Item
{
public:
	constexpr HeatControlBase(char const * const caption, DataGetter<std::tuple<DataType, DataType> > getter, DataSetter<DataType> setter, DataType minimum_settable)
	: Item(caption), get_data(getter), set_data(setter), minimum_settable(minimum_settable), mode(false)
	{

	}
	DataGetter<std::tuple<DataType, DataType> > get_data;
	DataSetter<DataType> set_data;
	DataType minimum_settable;
	bool mode;
};

using HeatControl = HeatControlBase<float>;
/*
// 	Notification window is meant to work as a splash screen.
// */
// class Notification : public Item
// {
// public:
// 	char const * const notification;
// };

} // namespace ui

#endif // __ITEM_H__