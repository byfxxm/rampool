#include "stdafx.h"
#include "pool.h"
#include "block.h"
#include "slot.h"

void Pool::Initialize(size_t size, const void* owner) {
	size_ = size;
	owner_ = owner;
}

size_t Pool::GetSize() {
	return size_;
}

void* Pool::Malloc(size_t size) {
	Lock lock(mutex_);
	++count_;
	total_ += size;

	auto slot = free_stack_.Pop();
	if (slot) {
		assert(slot->valid == Slot::Valid::kDeleted);
		slot->valid = Slot::Valid::kUsed;
		slot->actual_size = size;
		return slot->mem;
	}

	auto block = block_stack_.Top();
	if (!block || block->IsFull()) {
		block = new Block(size_, owner_);
		block_stack_.Push(block);
	}

	return block->Alloc(size);
}

void Pool::Free(void* p) {
	Lock lock(mutex_);

	auto slot = POINTER_TO_SLOT(p);
	slot->valid = Slot::Valid::kDeleted;
	free_stack_.Push(slot);
	--count_;
	total_ -= slot->actual_size;
	assert((int)count_ >= 0);
}

void Pool::Destroy() {
	Lock lock(mutex_);

	Block* block = nullptr;
	while (block = block_stack_.Top()) {
		block_stack_.Pop();
		delete block;
	}

	new(&block_stack_) Stack<Block>();
	new(&free_stack_) Stack<Slot>();
	count_ = 0;
	total_ = 0;
}

size_t Pool::Count() {
	return count_;
}

size_t Pool::Total() {
	return total_;
}

void Pool::Gc() {
	Lock lock(mutex_);

	Block* next = nullptr;
	for (auto block = block_stack_.Top(); block; block = next) {
		next = block->next;
		size_t index = 0;

		for (; index < block->cur_slot; ++index) {
			assert(block->slots[index]->valid != Slot::Valid::kUnuse);
			if (block->slots[index]->valid == Slot::Valid::kUsed)
				break;
		}

		if (index == block->cur_slot) {
			for (size_t i = 0; i < block->cur_slot; ++i)
				block->slots[i]->valid = Slot::Valid::kUnuse;

			for (auto slot = free_stack_.Top(); slot; slot = slot->next)
				if (slot->valid == Slot::Valid::kUnuse)
					free_stack_.Erase(slot);

			block_stack_.Erase(block);
			delete block;
		}
	}
}

bool Pool::NeedGc() {
	return free_stack_.Count() >= AUTOGC_THRESHOLD;
}