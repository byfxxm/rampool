#pragma once
#pragma warning(disable:4200)

struct Slot: public Node<Slot*>
{
	Slot() : Node<Slot*>(this) {}
	int nSize;
	char Mem[];
};

#define POINTER_TO_SLOT(p) ((Slot*)((char*)(p) - sizeof(Slot)))