#ifndef __COMPOSITEITEM_H__
#define __COMPOSITEITEM_H__

#include <boost/variant/variant.hpp>
#include "Item.h"
#include "File.h"

typedef boost::variant<ui::Item, ui::GraphicalItem, ui::LogoItem, ui::Command, ui::PositionControl, ui::File, ui::HeatControl, ui::FloatFloatInfo, ui::TimeInfo, ui::ProgressInfo, ui::CharInfo> CompositeItem; 

#endif // __COMPOSITEITEM_H__