#include "HeapRing.h"

#include <cstdlib>

#include "cmsis.h"

/*
 * constructors
 */

template<class kind> HeapRing<kind>::HeapRing()
{
    head_i = tail_i = length = 0;
}

template<class kind> HeapRing<kind>::HeapRing(unsigned int length) : 
ring{new kind[length]}
{
    head_i = tail_i = 0;
    // TODO: handle allocation failure
    this->length = length;
}

/*
 * destructor
 */

template<class kind> HeapRing<kind>::~HeapRing()
{
}

/*
 * index accessors (protected)
 */

template<class kind> unsigned int HeapRing<kind>::next(unsigned int item)
{
    if (length == 0)
        return 0;

    if (++item >= length)
        return 0;

    return item;
}

template<class kind> unsigned int HeapRing<kind>::prev(unsigned int item)
{
    if (length == 0)
        return 0;

    if (item == 0)
        return (length - 1);
    else
        return (item - 1);
}

/*
 * reference accessors
 */

template<class kind> kind& HeapRing<kind>::head()
{
    return ring[head_i];
}

template<class kind> kind& HeapRing<kind>::tail()
{
    return ring[tail_i];
}

template<class kind> kind& HeapRing<kind>::item(unsigned int i)
{
    return ring[i];
}

template<class kind> void HeapRing<kind>::push_front(kind& item)
{
    ring[head_i] = item;
    head_i = next(head_i);
}

template<class kind> kind& HeapRing<kind>::pop_back()
{
    kind& r = ring[tail_i];
    tail_i = next(tail_i);
    return r;
}

/*
 * pointer accessors
 */

template<class kind> kind* HeapRing<kind>::head_ref()
{
    return &ring[head_i];
}

template<class kind> kind* HeapRing<kind>::tail_ref()
{
    return &ring[tail_i];
}

template<class kind> kind* HeapRing<kind>::item_ref(unsigned int i)
{
    return &ring[i];
}

template<class kind> void HeapRing<kind>::produce_head()
{
    while (is_full());
    head_i = next(head_i);
}

template<class kind> void HeapRing<kind>::consume_tail()
{
    if (!is_empty())
        tail_i = next(tail_i);
}

/*
 * queue status accessors
 */

template<class kind> bool HeapRing<kind>::is_full()
{
    __disable_irq();
    bool r = (next(head_i) == tail_i);
    __enable_irq();

    return r;
}

template<class kind> bool HeapRing<kind>::is_empty()
{
    __disable_irq();
    bool r = (head_i == tail_i);
    __enable_irq();

    return r;
}

/*
 * resize
 */

template<class kind> bool HeapRing<kind>::resize(unsigned int length)
{
    if (is_empty())
    {
        if (length == 0)
        {
            __disable_irq();

            if (is_empty()) // check again in case something was pushed
            {
                head_i = tail_i = this->length = 0;

                __enable_irq();

                ring.reset();

                return true;
            }

            __enable_irq();

            return false;
        }

        // Note: we don't use realloc so we can fall back to the existing ring if allocation fails
        std::unique_ptr<kind[]> newring { new kind[length] };

        if (newring)
        {
            __disable_irq();

            if (is_empty()) // check again in case something was pushed while malloc did its thing
            {
                ring = std::move(newring);
                this->length = length;
                head_i = tail_i = 0;

                __enable_irq();

                return true;
            }

            __enable_irq();
        }
    }

    return false;
}

template<class kind> bool HeapRing<kind>::provide(std::unique_ptr<kind[]> buffer, unsigned int length)
{
    __disable_irq();

    if (is_empty())
    {
        if ((buffer) && (length > 0))
        {
            ring = std::move(buffer);
            this->length = length;
            head_i = tail_i = 0;

            __enable_irq();
            
            return true;
        }
    }

    __enable_irq();

    return false;
}
