#pragma once
#pragma warning(disable:4200)
#include "LinkedList.h"

enum class valid_t
{
	SLOT_UNUSE = 0,
	SLOT_USED = 0x7BABABAB,
	SLOT_DELETED = 0x7EDEDEDE,
};

struct Slot : public CLinkedList<Slot>::Node
{
	valid_t m_nValid = valid_t::SLOT_UNUSE;
	const void* m_pOwner = nullptr;
	size_t m_nSize = 0;
	size_t m_nActualSize = 0;
	char m_Mem[0];
};