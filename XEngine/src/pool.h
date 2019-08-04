#pragma once
#include <vector>
#include <stack>
#include <iostream>
#include <typeinfo>
#include "debug.h"

constexpr auto POOL_AUTO_ALLOCATION_SIZE = 200;

template<class T>
class Pool
{
private:

	void resize(int extra_size)
	{
		int current_size = memory.size();
		int new_size = current_size + extra_size;
		LOG("resizing pool of items: %s, lastSize: %d, newSize: %d", typeid(T).name(), current_size, new_size);

		memory.resize(new_size);
		for (int i = current_size; i < new_size; i++)
			available.push(&memory[i]);
	}

public:
	//TODO: try to make a big chung of memory and pick memory from there
	//instead of vector resize
	std::vector<T> memory;
	//TODO: replace stack by a fake stack using a vector that
	//pushes items fast
	std::stack<T*> available;

	void prewarm(int size)
	{
		ASSERT(available.size() == 0);
		resize(size);
	}

	T* spawn()
	{
		if (available.size() == 0) 
			resize(POOL_AUTO_ALLOCATION_SIZE);
		T* ret = available.top();
		available.pop();
		return ret;
	}

	void despawn(T* item)
	{
		//TODO: ensure that memory contains item
		available.push(item);
	}

	void release()
	{
		available.clear();
		memory.clear();
	}
};