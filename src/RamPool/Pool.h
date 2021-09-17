#pragma once
#include "LinkedList.h"
#include "Block.h"
#include "Slot.h"

class CPool
{
public:
	CPool();
	~CPool();
	void SetSize(size_t);
	size_t GetSize();
	void* Malloc(size_t);
	void Free(void*);
	void Destroy();
	size_t GetCount();
	size_t& Total();

private:
	CLinkedList<Block> m_BlockList;
	CLinkedList<Slot> m_FreeList;
	volatile size_t m_nSize;
	volatile size_t m_nCount;
	size_t m_nTotal;
	mutex m_Mutex;
};