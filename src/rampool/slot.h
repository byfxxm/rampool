#pragma once
#pragma warning(disable:4200)
#include "stack.h"

namespace rampool
{
	enum class valid_t
	{
		slot_s_UNUSE = 0,
		slot_s_USED = 'used',
		slot_s_DELETED = 'dele',
	};

	struct slot_s : public stack_c<slot_s>::node_s
	{
		const void* owner{ nullptr };
		valid_t valid{ valid_t::slot_s_UNUSE };
		size_t normalize_size{ 0 };
		size_t actual_size{ 0 };
		char mem[0];
	};
}