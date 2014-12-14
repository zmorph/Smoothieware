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

	// size_t get_number_of_files()
	// {	
	// 	size_t counter = 0;
	// 	while(dirent* d = read_directory())
	// 	{
			//THEKERNEL->streams->printf("counting: %s. \r\n", d->d_name);
	// 		++counter;
	// 	}
	// 	return counter;
	// }

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
		// dirent* dir;
		// do
		// {
		// 	dir = directory->readdir();
		// }
		// while(dir && !filter_filename(dir->d_name));
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
		// //THEKERNEL->streams->printf("rewind directory\r\n");
		// directory->rewinddir();

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
			size_t index_to_get = main_index-2;
			// if(number_of_files)
			// {
			// 	index_to_get = number_of_files - 1;
			// }

			// //THEKERNEL->streams->printf("index to get: %d\r\n", index_to_get);
			// //THEKERNEL->streams->printf("number_of_files: %d\r\n", number_of_files);
			// //THEKERNEL->streams->printf("last_valid_head: %d\r\n", last_valid_head);
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
				// //THEKERNEL->streams->printf("fuck\r\n");
				// what has happened??
				return "";
			}
		}
	}

	bool is_begin()
	{
		// //THEKERNEL->streams->printf("is_begin telldir: %d\r\n", directory->telldir());
		// //THEKERNEL->streams->printf("is_begin main_index: %d\r\n", main_index);
		return main_index == 0;
	}

	bool is_end()
	{
		// //THEKERNEL->streams->printf("is_end telldir: %d\r\n", directory->telldir());
		// //THEKERNEL->streams->printf("is_end main_index: %d\r\n", main_index);
		return main_index == last_valid_main_index;
	}

	void rewind_to_beginning()
	{
		head  = 0;
		main_index = 0;

		//directory->rewinddir();
		closedir(directory);
		directory = opendir(path.c_str());


		bool first = true;
		for(auto& entry: entries)
		{
			// //THEKERNEL->streams->printf("rewind dir iteration\r\n");
			if(!first)
			{
				// //THEKERNEL->streams->printf("not first iteration\r\n");
				entry = get_next();
				if(!entry.empty())
				{
					// //THEKERNEL->streams->printf("entry: %s \r\n", entry.c_str());
				}
			}
			else
			{
				// //THEKERNEL->streams->printf("first iteration\r\n");
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
			//read_directory();
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
		//THEKERNEL->streams->printf("get %d: %d \r\n", i ,(head + i) % N);
		return entries[(head + i) % N];
	}

public:
	FileShiftRegisterBase()
	:head(0), main_index(0), path("")
	{
	}

	bool open_directory(std::string const & new_path)
	{

		// CLOSE PREVIOUS DIRECTORY
		// //THEKERNEL->streams->printf("open directory: %s %d\r\n", new_path.c_str(), new_path.size());
		if(directory)
		{
			// //THEKERNEL->streams->printf("close previous directory \r\n");
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

		// //THEKERNEL->streams->printf("number of files in the directory %d\r\n", number_of_files);

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
		// //THEKERNEL->streams->printf("s\r\n");
		// //THEKERNEL->streams->printf("head: %d\r\n", head);
		// //THEKERNEL->streams->printf("number_of_files: %d\r\n", number_of_files);
		// //THEKERNEL->streams->printf("last_valid_head: %d\r\n", last_valid_head);
		if(!is_end())
		{
			// //THEKERNEL->streams->printf("not end\r\n");
			//assert current_index == 0
			entries[head] = get_next();
			head = increment_modulo_N(head);
			main_index++;
			return current_index;
		}

		if(!is_begin() && is_end())
		{
			// //THEKERNEL->streams->printf("end\r\n");

			if(is_last(current_index))
			{
				// //THEKERNEL->streams->printf("is last\r\n");
				rewind_to_beginning();
				return 0;
			}
			else
			{
				// //THEKERNEL->streams->printf("is not last\r\n");
				return current_index + 1;
			}
		}

		if(is_begin() && is_end())
		{
			// //THEKERNEL->streams->printf("begin and end \r\n");
			// //THEKERNEL->streams->printf("next valid: %d\r\n", next_valid(current_index));
			return next_valid(current_index);
		}
	}

	size_t shift_backward(size_t current_index)
	{
		

		// //THEKERNEL->streams->printf("entry\r\n");
		// //THEKERNEL->streams->printf("head: %d\r\n", head);
		// //THEKERNEL->streams->printf("number_of_files: %d\r\n", number_of_files);
		// //THEKERNEL->streams->printf("last_valid_head: %d\r\n", last_valid_head);

		if(!is_begin() && !is_end())
		{
			// //THEKERNEL->streams->printf("not end and not begin\r\n");
			head = decrement_modulo_N(head);
			entries[head] = get_previous();
			main_index--;
			return 0;
		}

		if(is_begin() && !is_end())
		{
			// //THEKERNEL->streams->printf("begin\r\n");
			//assert current_index == 0
			rewind_to_end();
			return last_valid_head-1;
		}

		if(!is_begin() && is_end())
		{
			// //THEKERNEL->streams->printf("end\r\n");
			if(is_first(current_index))
			{
				// //THEKERNEL->streams->printf("is first\r\n");
				head = decrement_modulo_N(head);
				entries[head] = get_previous();
				main_index--;
				return 0;
			}
			else
			{
				// //THEKERNEL->streams->printf("is not first\r\n");
				return current_index - 1;
			}
		}

		if(is_begin() && is_end())
		{
			// //THEKERNEL->streams->printf("begin and end \r\n");
			// //THEKERNEL->streams->printf("previous_valid valid: %d\r\n", previous_valid(current_index));
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
		// if(dirent* entry = get(i))
		// {
		// 	return entry->d_name;
		// }
		// else
		// {
		// 	return empty_string;
		// }
	}

	std::string const & get_current_directory()
	{
		return path;
	}

	bool is_directory(size_t i)
	{

		// if(0 == main_index && 0 == i)
		// {
		// 	if(path)
		// }
		// //THEKERNEL->streams->printf("is_directory? %s %d\r\n", path.c_str(), path.size());

		std::string current_path = path;

		//THEKERNEL->streams->printf("is_directory %s %d\r\n", current_path.c_str(), current_path.size());

        if ( current_path.compare("/") == 0 ) {
        	//THEKERNEL->streams->printf("compare current_path: %s \r\n", current_path.c_str());
            current_path = "";
        }
        
        //THEKERNEL->streams->printf("post current_path: %s \r\n", current_path.c_str());
        std::string entry = get(i);
		if(!entry.empty())
		{
			current_path = current_path + "/" + entry;
			//THEKERNEL->streams->printf("current_path: %s \r\n", current_path.c_str());
			if(DIR* temp_dir = opendir(current_path.c_str()))
			{
				closedir(temp_dir);
				return true;
			}
			//return (entry->fattrib & 0x10) != 0;
		}
		//THEKERNEL->streams->printf("false \r\n");
		return false;
	}
};

using FileShiftRegister = FileShiftRegisterBase<3>;

} // namespace ui

#endif //__FILE_BROWSER_H__

