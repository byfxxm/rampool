#include "stdafx.h"
#include "RamPoolImp.h"
#include "Block.h"
#include "Slot.h"

CRamPoolImp::CRamPoolImp()
{
	size_t _size = 0;
	for (size_t _i = 0; _i < _countof(m_Pools); _i++)
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
		_pool.Destroy();
}

void* CRamPoolImp::Malloc(size_t nSize_)
{
	if (nSize_ == 0 || nSize_ > MAXSIZE)
		throw std::bad_alloc();

	auto _index = POOLINDEX(nSize_);
	return m_Pools[_index].Malloc(nSize_);
}

void CRamPoolImp::Free(void* p_)
{
	if (!p_)
		return;

	auto _pSlot = POINTER_TO_SLOT(p_);
	m_Pools[POOLINDEX(_pSlot->m_nSize)].Free(p_);
}

void* CRamPoolImp::Realloc(void* p_, size_t nSize_)
{
	if (!p_)
		return Malloc(nSize_);

	auto _pSlot = POINTER_TO_SLOT(p_);
	assert(_pSlot->m_pOwner == &m_Pools[POOLINDEX(_pSlot->m_nSize)]);
	assert(_pSlot->m_nValid == valid_t::SLOT_USED);

	if (nSize_ <= _pSlot->m_nSize)
	{
		m_Pools[POOLINDEX(_pSlot->m_nSize)].Total() += nSize_ - _pSlot->m_nActualSize;
		_pSlot->m_nActualSize = nSize_;
		return p_;
	}

	auto _p = Malloc(nSize_);
	memmove(_p, p_, _pSlot->m_nActualSize);
	Free(p_);
	return _p;
}

void CRamPoolImp::Leak(LeakInfo* pLeakInfo_)
{
	if (!pLeakInfo_)
		return;

	memset(pLeakInfo_, 0, sizeof(LeakInfo));
	for (auto& _pool : m_Pools)
	{
		pLeakInfo_->nCount += _pool.GetCount();
		pLeakInfo_->nTotalSize += _pool.GetCount() * _pool.GetSize();
		pLeakInfo_->nTotalActualSize += _pool.Total();
	}

	assert(pLeakInfo_->nTotalActualSize <= pLeakInfo_->nTotalSize);
}

size_t CRamPoolImp::Size(void* p_)
{
	auto _pSlot = POINTER_TO_SLOT(p_);

	if (_pSlot->m_nValid != valid_t::SLOT_USED)
		return 0;

	return _pSlot->m_nActualSize;
}

void CRamPoolImp::GC()
{
	for (auto& _pool : m_Pools)
		_pool.GC();
}