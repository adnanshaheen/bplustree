#include <iostream>

#include "BTree.h"

CNode::CNode()
{
	m_ppPointer = NULL;
	m_pKeys = NULL;
	m_nKeys = 0;
	m_bLeaf = false;
	m_pParent = NULL;
	m_pNext = NULL;
}

CNode::~CNode()
{
}

CBTree::CBTree()
{
}

CBTree::~CBTree()
{
}

CNode * CBTree::Insert(int nKey, int nValue)
{
	return nullptr;
}

CNode * CBTree::Insert(CNode * pNode, int nKey, int nValue)
{
	return nullptr;
}

CNode * CBTree::NewTree(int nKey, int * pPointer)
{
	return nullptr;
}

int * CBTree::MakeRecord(int nValue)
{
	return nullptr;
}

int CBTree::GetLeftIndex(CNode * pParent, CNode * pLeft)
{
	return 0;
}

CNode * CBTree::MakeNode()
{
	CNode* pNode = NULL;
	try {
		pNode = new CNode();						/* create a new node */
		if (pNode == NULL)
			throw std::bad_alloc();

		pNode->m_pKeys = new int[GetOrder() - 1];		/* initialize the keys */
		if (pNode->m_pKeys == NULL)
			throw std::bad_alloc();

		pNode->m_ppPointer = new void*[GetOrder()];		/* create pointers */
		if (pNode->m_ppPointer == NULL)
			throw std::bad_alloc();
	}
	catch (std::exception ex) {
		std::cerr << ex.what() << std::endl;
	}
	return pNode;
}

CNode * CBTree::MakeRoot()
{
	return nullptr;
}

CNode * CBTree::InsertInLeaf(CNode * pNode, int nKey, int * pPointer)
{
	return nullptr;
}

CNode * CBTree::InsertInLeafSplit(CNode * pNode, int nKey, int * pPointer)
{
	return nullptr;
}

CNode * CBTree::InsertInNode(CNode * pParent, int nIndex, int nKey, CNode * pRight)
{
	return nullptr;
}

CNode * CBTree::InsertInNodeSplit(CNode * pParent, int nIndex, int nKey, CNode * pRight)
{
	return nullptr;
}

CNode * CBTree::InsertInRoot(int nKey, CNode * pLeft, CNode * pRight)
{
	return nullptr;
}

CNode * CBTree::InsertInParent(int nKey, CNode * pLeft, CNode * pRight)
{
	return nullptr;
}

