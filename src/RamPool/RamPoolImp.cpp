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
	return m_Pools[_index].Malloc();
}

void CRamPoolImp::Free(void* p_)
{
	if (p_ == nullptr)
		return;

	auto _pSlot = POINTER_TO_SLOT(p_);
	auto _index = POOLINDEX(_pSlot->m_nSize);
	m_Pools[_index].Free(p_);
}

size_t CRamPoolImp::Leak()
{
	size_t _total = 0;
	for (auto& _pool : m_Pools)
	{
		_total += _pool.GetCount() * _pool.GetSize();
	}

	return _total;
}