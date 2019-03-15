#include "CounterHeap.h"

CounterHeap::CounterHeap(double percent):percentage(percent)
{}

CounterHeap::CounterHeap(double percent, vector<int> init_elements):CounterHeap(percent)
{
    insert(init_elements);
}

int CounterHeap::getBreakPoint()
{
    return ceil(size() * percentage);
}

void CounterHeap::check()
{
    if (maxHeap.size() < getBreakPoint())
    {
        maxHeap.insert(minHeap.pop());
    }
    else if (maxHeap.size() > getBreakPoint())
    {
        minHeap.insert(maxHeap.pop());
    }
}

void CounterHeap::insert(int element)//condition: o1  insert: lgn  -> lgn
{
    if (size() == 0 || element <= target())
        maxHeap.insert(element);
        else
            minHeap.insert(element);
            check();
    
}

void CounterHeap::insert(vector<int> new_elements)//mlgn apparently
{
    for (auto &x : new_elements)
        insert(x);
}

bool CounterHeap::erase(int val)//condition o1 , so the same as Heap.erase()
{
    int mid = target();
    if (mid >= val)
    {
        bool isSuc = maxHeap.erase(val);
        if (isSuc)
        {
            if (maxHeap.size() < getBreakPoint())
                maxHeap.insert(minHeap.pop());
        }
        return isSuc;
    }
        else
        {
            bool isSuc = minHeap.erase(val);
            if (isSuc)
            {
                if (maxHeap.size() > getBreakPoint())
                    minHeap.insert(maxHeap.pop());
            }
            return isSuc;
        }
}


int CounterHeap::size()//o1
{
    return minHeap.size() + maxHeap.size();
}