#pragma once

class CNode
{
public:
	CNode() : m_pPrev(nullptr), m_pNext(nullptr) {}
	virtual ~CNode() {}

private:
	CNode* m_pPrev;
	CNode* m_pNext;
	friend class CLinkedList;
};