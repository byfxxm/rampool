#pragma once

class CNode;
using namespace std;

class CLinkedList
{
public:
	CLinkedList();
	~CLinkedList();
	bool PushBack(CNode*);
	CNode* PopFront();
	CNode* Front();
	CNode* Back();

private:
	bool IsEmpty();
	CNode* m_pHead;
	CNode* m_pTail;
};