#include "pch.h"
#include "LockFreePool.h"

namespace lockfree_container
{
	void LockFreePool::Initialize(size_t capacity, size_t dataSize)
	{
		mDataSize = dataSize;
		mPaddedDataSize = 0;
		mCapacity = capacity;
		mNodes = (Node*)_aligned_malloc(sizeof(Node) * (capacity + 1), 16);

		for (size_t i = 0; i < capacity; ++i) {
			mNodes[i].next = &mNodes[i + 1];
			mNodes[i].data = 0;
		}

		if (dataSize > 8) {
			const size_t alignment = 16;
			while (mPaddedDataSize < mDataSize) {
				mPaddedDataSize += alignment;
			}

			mData = (BYTE*)_aligned_malloc(mPaddedDataSize * capacity, 16);
			for (size_t i = 0; i < capacity; ++i) {
				mNodes[i].data = (LONG64)&mData[mPaddedDataSize * i];
			}
		}

		mNodes[capacity].next = nullptr;
		mNodes[capacity].data = 0;

		mHead.ptr = &mNodes[0];
		mHead.tag = 0;
	}

	void LockFreePool::Finialize()
	{
		_aligned_free(mNodes);
		if (mDataSize > 8) {
			_aligned_free(mData);
		}
	}

	PVOID LockFreePool::Alloc()
	{
		TaggedPtr head;
		TaggedPtr next;

		while (true) {
			head.ptr = mHead.ptr;
			head.tag = mHead.tag;

			next.ptr = ((Node*)head.ptr)->next;
			next.tag = head.tag + 1;
			if (next.ptr == nullptr) {
				return nullptr;
			}

			if (_InterlockedCompareExchange128((volatile LONG64*)&mHead, (LONG64)next.ptr, next.tag, (LONG64*)&head)) {
				break;
			}

			for (int i = 0; i < 1024; ++i) {
				YieldProcessor();
			}
		}

		Node* pNode = (Node*)head.ptr;
		pNode->next = nullptr;
		if (mDataSize > 8) {
			return (PVOID)pNode->data;
		}
		return &pNode->data;
	}

	void LockFreePool::Free(PVOID ptr)
	{
		TaggedPtr prevHead;
		TaggedPtr newHead;

		Node* pNode;
		if (mDataSize > 8) {
			size_t nodeOffset = ((BYTE*)ptr - mData) / mPaddedDataSize;
			pNode = &mNodes[nodeOffset];
		}
		else {
			pNode = CONTAINING_RECORD(ptr, Node, data);
		}

		newHead.ptr = pNode;
		
		while (true) {
			prevHead.ptr = mHead.ptr;
			prevHead.tag = mHead.tag;
			newHead.tag = prevHead.tag + 1;

			pNode->next = (Node*)prevHead.ptr;
			if (_InterlockedCompareExchange128((volatile LONG64*)&mHead, (LONG64)newHead.ptr, newHead.tag, (LONG64*)&prevHead)) {
				break;
			}

			for (int i = 0; i < 1024; ++i) {
				YieldProcessor();
			}
		}
	}
}