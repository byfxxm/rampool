#include "pch.h"
#include "RamPoolImp.h"
#include "Block.h"


CRamPoolImp::CRamPoolImp()
{
}

CRamPoolImp::~CRamPoolImp()
{
	for (auto _block : m_Blocks)
	{
		_block.Find([](NODE* p_)->bool
		{
			_ASSERT(p_ != nullptr);
			delete p_;

			return false;
		});
	}
}

void* CRamPoolImp::Malloc(int nSize_)
{
	int _index = BLOCKINDEX(nSize_);

	auto _pBlock = m_Blocks[_index].Find([](NODE* p_)->bool
	{
		_ASSERT(p_ != nullptr);
		
		if (p_->Data->Alloc() == nullptr)
			return false;

		return true;
	});

	if (_pBlock != nullptr)
	{
		m_Blocks[_index].PushBack(new CBlock(nSize_));
	}

	return nullptr;
}

void CRamPoolImp::Free(void*)
{

}