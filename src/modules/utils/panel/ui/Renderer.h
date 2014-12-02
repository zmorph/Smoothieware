#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "boost/variant/static_visitor.hpp"
#include "screen/Screen.h"
#include "File.h"
#include "Cell.h"
#include "Item.h"

namespace ui
{

class Renderer: public boost::static_visitor<Cell>
{
public:
	Renderer(const Cell& cell, Screen& screen)
	:cell(cell), screen(screen)
	{

	}
protected:
	const Cell& cell;
	Screen& screen;
};

class ActiveRenderer: public Renderer
{
public:
	ActiveRenderer(const Cell& cell, Screen& screen)
	:Renderer(cell, screen)
	{
	}

	template <typename ItemType>
	Cell operator()(ItemType& item) const
	{
		screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 1);
		uint8_t offset = 6;
		uint8_t letter_height = 8;
		const char* letter = item.caption;
		uint8_t letter_index = 0;
		while(*letter != 0)
		{
			if(offset*letter_index < cell.w - 4)
			{
				screen.draw_char(cell.x + offset*letter_index + 4, cell.y + cell.h/2 - letter_height/2 + 1, *letter, 0);
			}
			++letter;
			++letter_index;
		}
		return cell;
	}
};

class DefaultRenderer: public Renderer
{
public:
	DefaultRenderer(const Cell& cell, Screen& screen)
	:Renderer(cell, screen)
	{
	}

	template <typename ItemType>
	Cell operator()(ItemType& item) const
	{
		screen.draw_rectangle(cell.x, cell.y, cell.w, cell.h, 0);
		uint8_t offset = 6;
		uint8_t letter_height = 8;
		const char* letter = item.caption;
		uint8_t letter_index = 0;
		while(*letter != 0)
		{
			if(offset*letter_index < cell.w - 4)
			{		
				screen.draw_char(cell.x + offset*letter_index + 4, cell.y + cell.h/2 - letter_height/2 + 1, *letter, 1);
			}
			++letter;
			++letter_index;
		}
		return cell;
	}
};

template <> 
Cell ActiveRenderer::operator()<ui::File>(ui::File& item) const;

template <> 
Cell DefaultRenderer::operator()<ui::File>(ui::File& item) const;

template <> 
Cell ActiveRenderer::operator()<ui::PositionControl >(ui::PositionControl&  item) const;

template <> 
Cell DefaultRenderer::operator()<ui::PositionControl >(ui::PositionControl&  item) const;

template <> 
Cell ActiveRenderer::operator()<ui::HeatControl >(ui::HeatControl&  item) const;

template <> 
Cell DefaultRenderer::operator()<ui::HeatControl >(ui::HeatControl&  item) const;

template <> 
Cell ActiveRenderer::operator()<ui::FloatInfo >(ui::FloatInfo&  item) const;

template <> 
Cell DefaultRenderer::operator()<ui::FloatInfo >(ui::FloatInfo&  item) const;

template <> 
Cell ActiveRenderer::operator()<ui::FloatFloatInfo >(ui::FloatFloatInfo&  item) const;

template <> 
Cell DefaultRenderer::operator()<ui::FloatFloatInfo >(ui::FloatFloatInfo&  item) const;

template <> 
Cell ActiveRenderer::operator()<ui::PlayInfo >(ui::PlayInfo&  item) const;

template <> 
Cell DefaultRenderer::operator()<ui::PlayInfo >(ui::PlayInfo&  item) const;

} // namespace ui

#endif // __RENDERER_H__