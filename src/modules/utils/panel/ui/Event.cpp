#include "Event.h"
#include "Kernel.h"
#include "SerialMessage.h"
#include "StreamOutput.h"

namespace ui
{

void play_file(std::string const & path)
{
    SerialMessage message;
    message.message = std::string("play ") + path;
    message.stream = &(StreamOutput::NullStream);
    THEKERNEL->call_event(ON_CONSOLE_LINE_RECEIVED, &message);
}



template <>
Link OkEvent::operator()<ui::Command>(ui::Command& active_item)
{
	active_item.send_command();
	return linked();
}

template <>
Link OkEvent::operator()<ui::PositionControl >(ui::PositionControl&  active_item)
{
	active_item.mode = !active_item.mode;
	return same();
}

template <>
Link DownEvent::operator()<ui::PositionControl >(ui::PositionControl&  active_item)
{
	if(active_item.mode)
	{
		active_item.set_data(active_item.get_data()-10.0);
		return same();
	}
	else
	{
		return next();
	}
}

template <>
Link UpEvent::operator()<ui::PositionControl >(ui::PositionControl&  active_item)
{
	if(active_item.mode)
	{
		active_item.set_data(active_item.get_data()+10.0);
		return same();
	}
	else
	{
		return previous();
	}
}


template <>
Link OkEvent::operator()<ui::HeatControl >(ui::HeatControl&  active_item)
{
	active_item.mode = !active_item.mode;
	return same();
}

template <>
Link DownEvent::operator()<ui::HeatControl >(ui::HeatControl&  active_item)
{
	float target, current;
	std::tie(target, current) = active_item.get_data();
	if(active_item.mode)
	{
		if(target > active_item.minimum_settable)
		{
			active_item.set_data(target - 5.0);
		}
		else
		{
			active_item.set_data(0);
		}
		return same();
	}
	else
	{
		return next();
	}
}

template <>
Link UpEvent::operator()<ui::HeatControl >(ui::HeatControl&  active_item)
{
	float target, current;
	std::tie(target, current) = active_item.get_data();
	if(active_item.mode)
	{
		if(target < active_item.minimum_settable)
		{
			active_item.set_data(active_item.minimum_settable);
		}
		else
		{
			active_item.set_data(target + 5.0);
		}
		return same();
	}
	else
	{
		return previous();
	}
}

template <>
Link OkEvent::operator()<ui::File>(ui::File& active_item)
{
	FileShiftRegister& file_register = active_item.get_file_register();
	std::string cwd = file_register.get_current_directory();
	
	size_t index = same().get_index();
	std::string new_path = file_register.get_name(index);
	
	if((cwd.compare("/") == 0) && (new_path.compare("..") == 0)) 
	{
		return linked();
	}
	else if(new_path.compare("..") == 0)
	{
		new_path = cwd.substr(0, cwd.find_last_of('/'));
		if(new_path.size() == 0)
		{
			file_register.open_directory("/");
		}
		else
		{
			file_register.open_directory(new_path);
		}
		return Link(0,	same().get_group());
	}

	if(file_register.is_directory(index))
	{
		if(cwd.compare("/"))
		{			
			new_path = cwd + "/" + new_path;
		}
		else
		{	
			new_path = cwd + new_path;	
		}

		file_register.open_directory(new_path);
		return Link( 0,	same().get_group());
	}
	else
	{
		std::string file_path = cwd + "/" + new_path;
		play_file(file_path);
		return linked();
	}

    return same();
}

template <>
Link DownEvent::operator()<ui::File>(ui::File& active_item)
{
	size_t index = active_item.get_file_register().shift_forward(same().get_index());
	return Link( index,	same().get_group());
}

template <>
Link UpEvent::operator()<ui::File>(ui::File& active_item)
{	
	size_t index = active_item.get_file_register().shift_backward(same().get_index());	
    return Link( index,	same().get_group());
}

template <>
Link TickEvent::operator()<ui::LogoItem >(ui::LogoItem&  active_item)
{
	active_item.elapsed = (active_item.elapsed + 1) % active_item.duration;
	if(active_item.elapsed == 0)
	{
		return linked();
	}
	else
	{
		return same();
	}
}

} 
