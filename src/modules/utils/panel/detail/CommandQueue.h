#ifndef __COMMAND_QUEUE_H__
#define __COMMAND_QUEUE_H__

#include "FixedQueue.h"
#include <functional>

template <uint8_t N> using CommandQueue = FixedQueue<std::function<void()>, N>;

template <uint8_t N>
bool execute_next(CommandQueue<N>& queue)
{
	if(queue.empty())
	{
		return false;
	}
	else
	{
		queue.peek()();
		queue.pop();
		return true;
	}
}

#endif // __COMMAND_QUEUE_H__