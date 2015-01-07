#ifndef __LINK_H__
#define __LINK_H__

#include "Group.h"
#include "LinkBase.h"
#include "boost/variant/variant.hpp"

namespace ui
{
	typedef LinkBase<Group> Link;
} // namespace ui

#endif // __LINK_H__