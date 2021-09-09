#include "stdafx.h"
#include "RamPoolImp.h"
#include "Block.h"
#include "Slot.h"

CRamPoolImp::CRamPoolImp()
{
	int _size = 0;
	for (int _i = 0; _i < _countof(m_Pools); _i++)
	{
		_size += GRANULARITY;
		m_Pools[_i].SetSize(_size);
	}
}

CRamPoolImp::~CRamPoolImp()
{
	Destroy();
}

CRamPoolImp* CRamPoolImp::Instance()
{
	static CRamPoolImp s_Inst;
	return &s_Inst;
}

void CRamPoolImp::Destroy()
{
	for (auto& _pool : m_Pools)
	{
		_pool.Destroy();
	}
}

void* CRamPoolImp::Malloc(size_t nSize_)
{
	if (nSize_ == 0 || nSize_ > MAXSIZE)
		return nullptr;

	auto _index = POOLINDEX(nSize_);
	return m_Pools[_index].Malloc(nSize_);
}

void CRamPoolImp::Free(void* p_)
{
	if (p_ == nullptr)
		return;

	auto _pSlot = POINTER_TO_SLOT(p_);
	auto _index = POOLINDEX(_pSlot->m_nActualSize);
	m_Pools[_index].Free(p_);
}

void* CRamPoolImp::Realloc(void* p_, size_t nSize_)
{
	if (p_ == nullptr)
		return Malloc(nSize_);

	auto _p = Malloc(nSize_);
	memcpy(_p, p_, min(POINTER_TO_SLOT(p_)->m_nActualSize, nSize_));
	Free(p_);
	return _p;
}

void CRamPoolImp::Leak(size_t* pCount_, size_t* pTotalSize_)
{
	!!pCount_ && (*pCount_ = 0, 0);
	!!pTotalSize_ && (*pTotalSize_ = 0, 0);

	for (auto& _pool : m_Pools)
	{
		!!pCount_ && (*pCount_ += _pool.GetCount());
		!!pTotalSize_ && (*pTotalSize_ += _pool.GetCount() * _pool.GetSize());
	}
}

size_t CRamPoolImp::Size(void* p_)
{
	auto _pSlot = POINTER_TO_SLOT(p_);

	if (_pSlot->m_nValid != valid_t::SLOT_USED)
		return 0;

	return _pSlot->m_nActualSize;
}