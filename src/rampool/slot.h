#pragma once
#pragma warning(disable:4200)
#include "stack.h"

namespace rampool {
	struct Slot : public Stack<Slot>::Node {
		enum class Valid {
			kUnuse = 0,
			kUsed = 'used',
			kDeleted = 'dele',
		};

		const void* owner{ nullptr };
		Valid valid{ Valid::kUnuse };
		size_t normalize_size{ 0 };
		size_t actual_size{ 0 };
		char mem[0];
	};
}