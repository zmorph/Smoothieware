#ifndef __FILE_H__
#define __FILE_H__

#include "FileShiftRegister.h"

namespace ui
{

class File
{
	FileShiftRegister& file_shift_register;
	const size_t index;
public:
	File(FileShiftRegister& file_shift_register, size_t index);

	size_t get_index();
	std::string get_name() const;
	FileShiftRegister & get_file_register();
};

} // namespace ui

#endif // __FILE_H__