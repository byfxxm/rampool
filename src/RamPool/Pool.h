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
	volatile size_t& Total();
	void GC();

private:
	mutex m_Mutex;
	CLinkedList<Block> m_BlockList;
	CLinkedList<Slot> m_FreeList;
	volatile size_t m_nSize = 0;
	volatile size_t m_nCount = 0;
	volatile size_t m_nTotal = 0;
};