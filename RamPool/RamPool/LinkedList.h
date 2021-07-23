#pragma once

template <class T>
struct Node
{
	T Data;
	Node* pNext;
	Node* pPrevious;
};

class CLinkedList
{
public:
	CLinkedList();
	~CLinkedList();

private:
	Node<void*>* m_pHead;
};