#pragma once
#include "LinkedList.h"

class CPool
{
public:
	CPool();
	~CPool();
	void SetSize(int);
	void* Malloc();
	void Free(void*);
	void Destroy();

private:
	CLinkedList m_BlockList;
	CLinkedList m_FreeList;
	size_t m_nSize;
	size_t m_nCount;
};