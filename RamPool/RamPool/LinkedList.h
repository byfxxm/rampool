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
	void PushBack(CNode* p_);
	CNode* PopFront();
	CNode* Find(std::function<bool(CNode*)> func_);

private:
	bool IsEmpty();
	CNode* m_pHead;
	CNode* m_pTail;
	mutex m_Mutex;
};
