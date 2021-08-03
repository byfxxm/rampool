#pragma once
#include "LinkedList.h"

class CPool
{
public:
	CPool();
	~CPool();
	void SetSize(size_t);
	size_t GetSize();
	void* Malloc();
	void Free(void*);
	void Destroy();
	size_t GetCount();

private:
	CLinkedList m_BlockList;
	CLinkedList m_FreeList;
	size_t m_nSize;
	size_t m_nCount;
};