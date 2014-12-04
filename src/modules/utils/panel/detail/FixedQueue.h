#ifndef __FIXED_QUEUE_H__
#define __FIXED_QUEUE_H__

#include <cstdint>
#include <array>

template <typename ElementType, uint8_t N>
class FixedQueue
{
public:
	FixedQueue()
	:_size(0), head(0), tail(0)
	{}

	bool push(ElementType e)
	{
		uint8_t new_head = increment_modulo_N(head);
		if(full())
		{
			return false;
		}
		else
		{
			++_size;
			container[head] = e;
			head = new_head;
			return true;
		}
	};

	uint8_t size() const
	{
		return _size;
	}

	bool full() const
	{
		return _size == N;
	};

	bool empty() const
	{
		return _size == 0;
	};

	ElementType& peek()
	{
		return container[tail];
	}

	bool pop()
	{
		if(_size)
		{
			--_size;
			tail = increment_modulo_N(tail);
			return true;
		}
		else
		{
			return false;
		}
	};

private:
	uint8_t increment_modulo_N(uint8_t n)
	{
		return (n+1) % N;
	}

	std::array<ElementType, N> container;
	uint8_t _size;
	uint8_t head;
	uint8_t tail;
};

#endif // __FIXED_QUEUE_H__