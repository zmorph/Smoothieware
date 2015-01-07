#include "File.h"

namespace ui
{

File::File(FileShiftRegister& file_shift_register, size_t index)
:file_shift_register(file_shift_register), index(index)
{}

std::string File::get_name() const
{
	return file_shift_register.get_name(index);
};

size_t File::get_index()
{
	return index;
}

FileShiftRegister & File::get_file_register()
{
	return file_shift_register;
}

} // namespace ui