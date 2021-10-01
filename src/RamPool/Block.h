#pragma once
#include "LinkedList.h"
#include "Slot.h"

struct Block : public CLinkList<Block>::Node
{
	Block(size_t, void*);
	~Block();
	void* Alloc(size_t);
	bool IsFull();

	char* m_pMem = nullptr;
	size_t m_nMemSize = 0;
	Slot** m_ppSlots = nullptr;
	size_t m_nSlotNum = 0;
	size_t m_nCurSlot = 0;
};