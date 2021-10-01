#pragma once
#include "LinkedList.h"
#include "Block.h"
#include "Slot.h"

class CPool
{
public:
	CPool() = default;
	~CPool() = default;
	void SetSize(size_t);
	size_t GetSize();
	void* Malloc(size_t);
	void Free(void*);
	void Destroy();
	size_t GetCount();
	atomic<size_t>& Total();
	void GC();
	bool NeedGC();

private:
	mutex m_Mutex;
	CLinkList<Block> m_BlockList;
	CLinkList<Slot> m_FreeList;
	atomic<size_t> m_nSize = 0;
	atomic<size_t> m_nCount = 0;
	atomic<size_t> m_nTotal = 0;
};