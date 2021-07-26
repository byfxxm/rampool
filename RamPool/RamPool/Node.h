#pragma once

class CNode
{
public:
	CNode() :m_pPrevious(nullptr), m_pNext(nullptr) {}
	virtual ~CNode() {}

private:
	CNode* m_pPrevious;
	CNode* m_pNext;
	friend class CLinkedList;
};
