#pragma once
#pragma warning(disable:4200)

class CSlot: public Node<CSlot*>
{
public:
	CSlot() : Node<CSlot*>(this) {}
	virtual ~CSlot() {}
	int m_nSize;
	char m_Mem[];
};

#define POINTER_TO_SLOT(p) ((CSlot*)((char*)(p) - sizeof(CSlot)))