#ifndef __FILE_BROWSER_H__
#define __FILE_BROWSER_H__

#include "DirHandle.h"
#include "ff.h"
#include <string>

/*

CLOSE THE DIRECTORY!!!!

*/
namespace ui
{

bool filter_filename(std::string const & name);

template <size_t N>
class FileShiftRegisterBase
{

	size_t head;
	size_t main_index;
	size_t last_valid_head; // constant for directory
	size_t last_valid_main_index;
	size_t number_of_files; // constant for directory
	char const * empty_string = "";
	
	std::string path;
	std::string entries[N];

	DIR* 	directory;

	size_t get_number_of_files()
	{	
		size_t counter = 0;
		while(!get_next().empty())
		{
			++counter;
		}
		return counter;
	}

	dirent* read_directory()
	{
		return directory->readdir();
	}

	std::string get_next()
	{
		if(dirent* dir = read_directory())
		{
			return dir->d_name;
		}
		else
		{
			return "";
		}
	}

	std::string get_previous()
	{
		closedir(directory);
		directory = opendir(path.c_str());
		

		if(main_index == 1)
		{
			for(size_t i = 0; i < N-1; ++i)
			{
				read_directory();
			}
			return "..";
		}
		else
		{
			size_t index_to_get = main_index - 2;
			for(size_t i = 0; i < index_to_get; ++i)
			{
				read_directory();
			}
			
			
			if(dirent* entry = read_directory())
			{
				std::string name = entry->d_name;
				for(size_t i = 0; i < N-1; ++i)
				{
					read_directory();
				}
				return name;
			}
			else
			{
				return "";
			}
		}
	}

	bool is_begin()
	{
		return main_index == 0;
	}

	bool is_end()
	{
		return main_index == last_valid_main_index;
	}

	void rewind_to_beginning()
	{
		head  = 0;
		main_index = 0;

		closedir(directory);
		directory = opendir(path.c_str());

		bool first = true;
		for(auto& entry: entries)
		{
			if(!first)
			{
				entry = get_next();
			}
			else
			{
				first = false;
			}
		}
	}

	void rewind_to_end()
	{
		head = 0;
		main_index = last_valid_main_index;


		closedir(directory);
		directory = opendir(path.c_str());


		for(size_t i = 0; i < last_valid_main_index - 1; ++i)
		{
			get_next();
		}
		for(auto& entry: entries)
		{
			entry = get_next();
		}
	}

	bool is_last(size_t index)
	{
		return index == (N - 1);
	}

	bool is_first(size_t index)
	{
		return index == 0;
	}

	size_t next_valid(size_t index) const
	{
		return (index >= last_valid_head) ? 0 : index + 1;
	}
	
	size_t previous_valid(size_t index) const
	{
		return (index > 0) ? index - 1 : last_valid_head;
	}


// VALID

	size_t increment_modulo_N(size_t index)
	{
		return (index + 1) % N;
	}

// VALID
	size_t decrement_modulo_N(size_t index)
	{
		return (index + N - 1) % N;
	}

	template<size_t I>
	std::string get()
	{
		static_assert(I >= N, 
			"Trying to access out of bounds element of FileShiftRegister.");
		return entries[(head + I) % N];
	}

	std::string get(size_t i)
	{
		return entries[(head + i) % N];
	}

public:
	FileShiftRegisterBase()
	:head(0), main_index(0), path("")
	{
	}

	bool open_directory(std::string const & new_path)
	{

		if(directory)
		{
			closedir(directory);
		}
		else
		{
			//return false;
		}

		// SET STATE

		head = 0;
		main_index = 0;
		path = new_path;
		
		this->directory = opendir(path.c_str());
		
		number_of_files = get_number_of_files();

		if(number_of_files < N)
		{
			last_valid_main_index = 0;
			last_valid_head = number_of_files;
		}
		else		
		{
			last_valid_main_index = number_of_files - N + 1;
			last_valid_head = N;
		}

		rewind_to_beginning();
		
		return true;
	}

	size_t shift_forward(size_t current_index)
	{
		if(!is_end())
		{
			entries[head] = get_next();
			head = increment_modulo_N(head);
			main_index++;
			return current_index;
		}

		if(!is_begin() && is_end())
		{

			if(is_last(current_index))
			{
				rewind_to_beginning();
				return 0;
			}
			else
			{
				return current_index + 1;
			}
		}

		if(is_begin() && is_end())
		{
			return next_valid(current_index);
		}
	}

	size_t shift_backward(size_t current_index)
	{
		if(!is_begin() && !is_end())
		{
			head = decrement_modulo_N(head);
			entries[head] = get_previous();
			main_index--;
			return 0;
		}

		if(is_begin() && !is_end())
		{
			rewind_to_end();
			return last_valid_head-1;
		}

		if(!is_begin() && is_end())
		{
			if(is_first(current_index))
			{
				head = decrement_modulo_N(head);
				entries[head] = get_previous();
				main_index--;
				return 0;
			}
			else
			{
				return current_index - 1;
			}
		}

		if(is_begin() && is_end())
		{
			return previous_valid(current_index);
		}
	}


	std::string get_name(size_t i)
	{
		if(0 == main_index && 0 == i)
		{
			return "..";
		}
		else
		{
			return get(i);
		}
	}

	std::string const & get_current_directory()
	{
		return path;
	}

	bool is_directory(size_t i)
	{
		std::string current_path = path;

        if ( current_path.compare("/") == 0 ) 
        {
            current_path = "";
        }
        
        std::string entry = get(i);
		if(!entry.empty())
		{
			current_path = current_path + "/" + entry;
			if(DIR* temp_dir = opendir(current_path.c_str()))
			{
				closedir(temp_dir);
				return true;
			}
		}
		return false;
	}
};

using FileShiftRegister = FileShiftRegisterBase<3>;

} // namespace ui

#endif //__FILE_BROWSER_H__

