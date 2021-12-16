#pragma once
#pragma warning(disable:4200)
#include "stack.h"

namespace rampool
{
	enum class slot_valid_t
	{
		UNUSE = 0,
		USED = 'used',
		DELETED = 'dele',
	};

	struct slot_s : public stack_c<slot_s>::node_s
	{
		const void* owner{ nullptr };
		slot_valid_t valid{ slot_valid_t::UNUSE };
		size_t normalize_size{ 0 };
		size_t actual_size{ 0 };
		char mem[0];
	};
}