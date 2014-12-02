#include "FileShiftRegister.h"

namespace ui
{
bool filter_filename(std::string const & name)
{
	return (	name.find(".g")		== 0 || 
				name.find(".gcode") == 0 ||
				name.find(".gc") 	== 0 ||
				name.find(".gcd") 	== 0);
}
}