#pragma once
#pragma warning(disable:4200)

class CSlot: public Node<CSlot*>
{
public:
	CSlot() : Node<CSlot*>(this) {}
	virtual ~CSlot() {}
	bool bUsed;
	int nSize;
	char Data[];
};

#define POINTER_TO_SLOT(p) ((CSlot*)((char*)(p) - sizeof(CSlot)))