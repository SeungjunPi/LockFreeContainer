#ifndef LOCK_FREE_POOL
#define LOCK_FREE_POOL

#include "pch.h"

namespace lockfree_container
{
	struct Node
	{
		LONG64 data;
		Node* next;
	};

	struct alignas(16) TaggedPtr
	{
		LONG64 tag;
		PVOID ptr;
	};

	class LockFreePool
	{
	public:
		void Initialize(size_t capacity, size_t dataSize);
		void Finialize();

		PVOID Alloc();
		void Free(PVOID ptr);

	private:
		volatile TaggedPtr mHead;
		Node* mNodes;
		size_t mCapacity;

		BYTE* mData;
		size_t mDataSize;
		size_t mPaddedDataSize;
	};
}



#endif // !LOCK_FREE_POOL

