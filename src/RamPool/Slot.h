#pragma once
#pragma warning(disable:4200)
#include "LinkList.h"

enum class valid_t
{
	SLOT_UNUSE = 0,
	SLOT_USED = 0x7BABABAB,
	SLOT_DELETED = 0x7EDEDEDE,
};

struct Slot : public CLinkList<Slot>::Node
{
	const void* m_pOwner = nullptr;
	valid_t m_nValid = valid_t::SLOT_UNUSE;
	size_t m_nSize = 0;
	size_t m_nActualSize = 0;
	char m_Mem[0];
};