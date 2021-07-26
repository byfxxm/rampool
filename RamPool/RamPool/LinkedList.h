#pragma once
#include <functional>
#include <condition_variable>

class CNode;
using namespace std;

class CLinkedList
{
public:
	CLinkedList();
	~CLinkedList();
	void PushBack(CNode*);
	CNode* PopFront();
	CNode* Find(std::function<bool(CNode*)>);

private:
	bool IsEmpty();
	CNode* m_pHead;
	CNode* m_pTail;
	mutex m_Mutex;
};
