#ifndef __COMPOSITEITEM_H__
#define __COMPOSITEITEM_H__

#include <boost/variant/variant.hpp>
#include "Item.h"
#include "File.h"

typedef boost::variant<ui::Item, ui::Command, ui::PositionControl, ui::File, ui::HeatControl, ui::FloatInfo, ui::FloatFloatInfo, ui::PlayInfo> CompositeItem; 

#endif // __COMPOSITEITEM_H__