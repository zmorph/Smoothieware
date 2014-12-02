#ifndef __RENDER_BLOCK_QUEUE_BASE_H__
#define __RENDER_BLOCK_QUEUE_BASE_H__

namespace ui
{

template <size_t N, typename RenderBlockType>
struct RenderBlockQueueBase
{	
	RenderBlockQueueBase()
	:bounding_block{255,255,0,0}, end(0)
	{}

	RenderBlockType bounding_block;
	RenderBlockType blocks[N];
	size_t end;

	bool is_fool(){return true;}
	bool is_full(){return end == N;}
	bool is_empty(){return end == 0;}

	RenderBlockType superset(RenderBlockType a, RenderBlockType b)
	{
		return RenderBlockType{std::min(a.x, b.x), std::min(a.y, b.y), std::max(a.w, b.w), std::max(a.h, b.h)};
	}

	void push_back(RenderBlockType block)
	{
		bounding_block = superset(bounding_block, block);
		if(is_full())
		{
			blocks[0] = bounding_block;
			end = 1;
		}
		else
		{
			blocks[end] = block;
			++end;
		}
	}

	RenderBlockType pop_front()
	{
		if(is_empty())
		{
			bounding_block = RenderBlockType{255,255,0,0};
			return RenderBlockType{0,0,0,0};
		}
		else
		{
			--end; // DO NOT CHECK EMPTINESS, JUST RETURN 0 ELEMENT IF EMPTY
			return blocks[end];	
		}
	}
};

} // namespace ui

#endif //__RENDER_BLOCK_QUEUE_BASE_H__
