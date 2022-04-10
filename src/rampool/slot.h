#pragma once
#pragma warning(disable:4200)
#include "stack.h"

namespace rampool
{
	struct slot : public stack<slot>::node_s
	{
		enum class valid_t
		{
			UNUSE = 0,
			USED = 'used',
			DELETED = 'dele',
		};

		const void* owner{ nullptr };
		valid_t valid{ valid_t::UNUSE };
		size_t normalize_size{ 0 };
		size_t actual_size{ 0 };
		char mem[0];
	};
}