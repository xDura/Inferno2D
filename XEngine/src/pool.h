//#pragma once
//#include <vector>
//#include <stack>
//#include "debug.h"
//
//constexpr auto POOL_AUTO_ALLOCATION_SIZE = 100;
//
//template<class T>
//class Pool
//{
//public:
//	std::vector<T> items;
//	unsigned int last;
//
//	void prewarm(int size)
//	{
//		last = 0;
//		items.resize();
//	}
//
//	template<class T>
//	T* spawn()
//	{
//		T* ret = &items[last];
//		ASSERT(last < items.size())
//		last++;
//		if (last >= items.size()) 
//		{
//			unsigned int newSize = items.size() + POOL_AUTO_ALLOCATION_SIZE;
//			LOG("max items on pool reached: had %d, resizing to %d", items.size, newSize);
//			items.resize(newSize);
//		}
//		return ret;
//	}
//
//	template<class T>
//	void despawn(T* item)
//	{
//		for (size_t i = 0; i < items.size(); i++)
//		{
//			if(&items[i] == item) 
//		}
//	}
//
//	void release()
//	{
//		items.clear();
//	}
//};