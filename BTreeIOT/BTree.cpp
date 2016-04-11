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

/*
* NewTree
*
* Make a new tree
*/
CNode * CBTree::NewTree(int nKey, int * pPointer)
{
	CNode* pNode = NULL;
	try {
		pNode = MakeLeaf();
		if (pNode) {
			pNode->m_pKeys[0] = nKey;
			pNode->m_ppPointer[0] = pPointer;
			pNode->IncKeys();
		}
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return pNode;
}

/*
 * MakeRecord
 *
 * Make a record with value
 */
int * CBTree::MakeRecord(int nValue)
{
	int* pRecord = NULL;
	try {
		pRecord = new int(nValue);
		if (pRecord == NULL)
			throw std::bad_alloc();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return pRecord;
}

/*
* GetLeftIndex
*
* find the left most index
*/
uint32_t CBTree::GetLeftIndex(CNode * pParent, CNode * pLeft)
{
	uint32_t nIndex = 0;
	try {
		while (nIndex <= pParent->m_nKeys && pParent->m_ppPointer[nIndex] != pLeft)
			++ nIndex;
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return nIndex;
}

/*
 * MakeNode
 *
 * Create a new node
 */
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
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return pNode;
}

/*
 * MakeLeaf
 *
 * make a leaf node
 */
CNode * CBTree::MakeLeaf()
{
	CNode* pNode = NULL;
	try {
		pNode = MakeNode();
		pNode->SetLeaf();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return pNode;
}

CNode * CBTree::InsertInLeaf(CNode * pNode, int nKey, int * pPointer)
{
	CNode* pResult = NULL;
	try {
		uint32_t nPos = 0;
		while (nPos < pNode->m_nKeys && pNode->m_pKeys[nPos] < nKey)			/* find the position to insert in leaf */
			++ nPos;

		for (uint32_t nKeyPos = pNode->m_nKeys; nKeyPos > nPos; ++ nKeyPos) {	/* adjust the previous keys and pointers */
			pNode->m_pKeys[nKeyPos] = pNode->m_pKeys[nKeyPos - 1];
			pNode->m_ppPointer[nKeyPos] = pNode->m_ppPointer[nKeyPos - 1];
		}

		/* update the values */
		pNode->m_pKeys[nPos] = nKey;
		pNode->m_ppPointer[nPos] = pPointer;
		pNode->IncKeys();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return pResult;
}

CNode * CBTree::InsertInLeafSplit(CNode * pNode, int nKey, int * pPointer)
{
	return nullptr;
}

CNode * CBTree::InsertInNode(CNode * pParent, const uint32_t nIndex, const int nKey, CNode * pRight)
{
	return nullptr;
}

CNode * CBTree::InsertInNodeSplit(CNode * pParent, const uint32_t nIndex, const int nKey, CNode * pRight)
{
	return nullptr;
}

CNode * CBTree::InsertInRoot(const int nKey, CNode * pLeft, CNode * pRight)
{
	CNode* pNode = NULL;
	try {
		pNode = MakeNode();
		pNode->m_pKeys[0] = nKey;
		pNode->m_ppPointer[0] = pLeft;
		pNode->m_ppPointer[1] = pRight;
		pNode->IncKeys();
		pLeft->m_pParent = pNode;
		pRight->m_pParent = pNode;
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return nullptr;
}

/*
* InsertInParent
*
* insert a new node in parent
*/
CNode * CBTree::InsertInParent(const int nKey, CNode * pLeft, CNode * pRight)
{
	CNode* pNode = NULL;
	try {
		if (pLeft == NULL || pRight == NULL)
			throw std::exception("Invalid parameters!!!");
		CNode* pParent = pLeft->m_pParent;
		if (pParent == NULL)
			pNode = InsertInRoot(nKey, pLeft, pRight);		/* create new root */
		else if (pParent->m_nKeys < GetOrder() - 1)
			pNode = InsertInNode(pParent, GetLeftIndex(pParent, pLeft), nKey, pRight);
			
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return nullptr;
}

