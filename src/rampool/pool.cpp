#include "stdafx.h"
#include "pool.h"
#include "Block.h"
#include "Slot.h"

void pool::Initialize(size_t Size, const void* owner) {
	size_ = Size;
	owner_ = owner;
}

size_t pool::GetSize() {
	return size_;
}

void* pool::Malloc(size_t Size) {
	Lock lck(mutex_);
	++count_;
	total_ += Size;

	auto slt = free_Stack_.pop();
	if (slt) {
		assert(slt->valid == Slot::Valid::DELETED);
		slt->valid = Slot::Valid::USED;
		slt->actual_size = Size;
		return slt->mem;
	}

	auto blk = block_Stack_.top();
	if (!blk || blk->IsFull()) {
		blk = new Block(size_, owner_);
		block_Stack_.push(blk);
	}

	return blk->Alloc(Size);
}

void pool::Free(void* p) {
	Lock lck(mutex_);

	auto slt = POINTER_TO_slot_s(p);
	slt->valid = Slot::Valid::DELETED;
	free_Stack_.push(slt);
	--count_;
	total_ -= slt->actual_size;
	assert((int)count_ >= 0);
}

void pool::Destroy() {
	Lock lck(mutex_);

	Block* blk = nullptr;
	while (blk = block_Stack_.top()) {
		block_Stack_.pop();
		delete blk;
	}

	new(&block_Stack_) Stack<Block>();
	new(&free_Stack_) Stack<Slot>();
	count_ = 0;
	total_ = 0;
}

size_t pool::Count()
{
	return count_;
}

size_t pool::Total()
{
	return total_;
}

void pool::Gc()
{
	Lock lck(mutex_);

	Block* next = nullptr;
	for (auto blk = block_Stack_.top(); blk; blk = next) {
		next = blk->next;

		size_t index = 0;
		for (; index < blk->cur_slot; ++index) {
			assert(block_->slots[index]->valid != Slot::Valid::UNUSE);
			if (blk->slots[index]->valid == Slot::Valid::USED)
				break;
		}

		if (index == blk->cur_slot) {
			for (size_t i = 0; i < blk->cur_slot; ++i)
				blk->slots[i]->valid = Slot::Valid::UNUSE;

			for (auto Slot = free_Stack_.top(); Slot; Slot = Slot->next) {
				if (Slot->valid == Slot::Valid::UNUSE)
					free_Stack_.erase(Slot);
			}

			block_Stack_.erase(blk);
			delete blk;
		}
	}
}

bool pool::NeedGc() {
	return free_Stack_.Count() >= AUTOGC_THRESHOLD;
}