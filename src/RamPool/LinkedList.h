#pragma once
#include <functional>
#include <mutex>

class CNode;
using namespace std;

class CLinkedList
{
public:
	CLinkedList();
	~CLinkedList();
	void PushBack(CNode*);
	CNode* PopFront();
	CNode* Front();
	CNode* Back();

private:
	bool IsEmpty();
	CNode* m_pHead;
	CNode* m_pTail;
	mutex m_Mutex;
};