#pragma once
#include "LinkedList.h"
#include "Slot.h"

struct Block : public CLinkedList<Block>::Node
{
	Block(size_t, void*);
	~Block();
	void* Alloc(size_t);
	bool IsFull();

	char* m_pMem;
	size_t m_nMemSize;
	Slot** m_ppSlots;
	size_t m_nSlotNum;
	size_t m_nCurSlot;
};