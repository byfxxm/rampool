#pragma once
#pragma warning(disable:4200)
#include "stack.h"

enum class valid_t
{
	SLOT_UNUSE = 0,
	SLOT_USED = 0x7BABABAB,
	SLOT_DELETED = 0x7EDEDEDE,
};

struct slot : public stack<slot>::node
{
	const void* owner = nullptr;
	valid_t valid = valid_t::SLOT_UNUSE;
	size_t normalize_size = 0;
	size_t actual_size = 0;
	char mem[0];
};