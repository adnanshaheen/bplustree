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

/*
* Insert
*
* Insert a new node in tree
*/
CNode * CBTree::Insert(int nKey, int nValue)
{
	CNode* pNode = m_pRoot;
	try {
		/* find the node, duplicates are not allowed */
		if (!Find(nKey))
			pNode = Insert(m_pRoot, nKey, nValue);
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return pNode;
}

/*
* Insert
*
* Insert a new node in tree - helper function
*/
CNode * CBTree::Insert(CNode * pNode, int nKey, int nValue)
{
	CNode* pNewNode = NULL;
	try {
		int* pPointer = MakeRecord(nValue);				/* create a new record */
		if (pPointer == NULL)
			throw std::bad_alloc();

		if (pNode == NULL)								/* create new tree */
			pNewNode = NewTree(nKey, pPointer);

		CNode* pLeaf = FindLeaf(nKey);					/* find the leaf node */
		if (pLeaf == NULL)
			throw std::bad_alloc();

		if (pLeaf->m_nKeys > GetOrder() - 1)			/* space is available, insert here */
			pLeaf = InsertInLeaf(pLeaf, nKey, pPointer);
		else {
			pNewNode = SplitInsertLeaf(pLeaf, nKey, pPointer);
		}
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return pNewNode;
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

/*
* InsertInLeaf
*
* Insert a new node in leaf
*/
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

/*
* SplitInsertLeaf
*
* Split and then Insert a new node in leaf
*/
CNode * CBTree::SplitInsertLeaf(CNode * pNode, int nKey, int * pPointer)
{
	CNode* pNewNode = NULL;
	try {
		if (pNode == NULL || pPointer == NULL)		/* check parameters */
			throw std::exception("Invalid parameters!!!");

		void** ppNodePointers = new void*[GetOrder()];		/* create temporary pointers */
		if (ppNodePointers == NULL)
			throw std::bad_alloc();

		int* pKeys = new int[GetOrder()];					/* create temporary keys */
		if (pKeys == NULL)
			throw std::bad_alloc();

		uint32_t nIndex = 0;
		while (nIndex < GetOrder() - 1 && pNode->m_pKeys[nIndex] < nKey)
			++ nIndex;

		for (uint32_t i = 0, j = 0; i < pNode->m_nKeys; ++ i, ++ j) {	/* copy pointers */
			if (nIndex == j)
				++ j;
			ppNodePointers[j] = pNode->m_ppPointer[i];
		}

		for (uint32_t i = 0, j = 0; i < pNode->m_nKeys; ++ i, ++ j) {		/* copy keys */
			if (nIndex == j)
				++ j;
			pKeys[j] = pNode->m_pKeys[i];
		}

		ppNodePointers[nIndex] = pPointer;
		pKeys[nIndex] = nKey;

		/* Split in half */
		uint32_t nHalf = Half();
		pNode->m_nKeys = 0;
		uint32_t nPrevKeys = 0;
		for (nPrevKeys = 0; nPrevKeys < nHalf; ++ nPrevKeys) {		/* update the parent pointers and keys */
			pNode->m_ppPointer[nPrevKeys] = ppNodePointers[nPrevKeys];
			pNode->m_pKeys[nPrevKeys] = pKeys[nPrevKeys];
			pNode->IncKeys();
		}

		pNewNode = MakeNode();		/* create new node */
		if (pNewNode == NULL)
			throw std::bad_alloc();

		for (uint32_t nNextKeys = 0, nPrevKeys = nHalf; nPrevKeys < GetOrder(); ++ nPrevKeys, ++ nNextKeys) {	/* copy remaining half */
			pNewNode->m_ppPointer[nNextKeys] = ppNodePointers[nPrevKeys];
			pNewNode->m_pKeys[nNextKeys] = pKeys[nPrevKeys];
			pNewNode->IncKeys();
		}

		/* free the temporary pointers and keys */
		delete[] ppNodePointers;
		delete[] pKeys;

		pNewNode->m_ppPointer[GetOrder() - 1] = pNode->m_ppPointer[GetOrder() - 1];
		pNode->m_ppPointer[GetOrder() - 1] = pNewNode;

		for (uint32_t nNullKey = pNode->m_nKeys; nNullKey < GetOrder() - 1; ++ nNullKey)	/* set old pointers to null */
			pNode->m_ppPointer[nNullKey] = NULL;

		for (uint32_t nNullKey = pNewNode->m_nKeys; nNullKey < GetOrder() - 1; ++ nNullKey)	/* set pointers to null */
			pNewNode->m_ppPointer[nNullKey] = NULL;

		pNewNode->m_pParent = pNode->m_pParent;

		/* insert new key in to parent */
		CNode* pLeaf = InsertInParent(pNewNode->m_pKeys[0], pNode, pNewNode);	/* pNode = pLeft, pNewNode = pRight */
		if (pLeaf)
			pNewNode = pLeaf;
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return pNewNode;
}

/*
* InsertInNode
*
* Insert a new node
*/
CNode * CBTree::InsertInNode(CNode * pParent, const uint32_t nIndex, const int nKey, CNode * pRight)
{
	CNode* pNode = NULL;
	try {
		if (pParent == NULL || pRight == NULL)		/* check parameters */
			throw std::exception("Invalid parameters!!!");

		for (uint32_t nKeyPos = pParent->m_nKeys; nKeyPos > 0; -- nKeyPos) {		/* update the pointers and keys */
			pParent->m_ppPointer[nKeyPos + 1] = pParent->m_ppPointer[nKeyPos];
			pParent->m_pKeys[nKeyPos] = pParent->m_pKeys[nKeyPos - 1];
		}

		/* set the new values */
		pParent->m_ppPointer[nIndex + 1] = pRight;
		pParent->m_pKeys[nIndex] = nKey;
		pParent->IncKeys();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return m_pRoot;
}

/*
* SplitInsertNode
*
* Split and insert a new node
*/
CNode * CBTree::SplitInsertNode(CNode * pParent, const uint32_t nIndex, const int nKey, CNode * pRight)
{
	CNode* pNode = NULL;
	try {
		if (pParent == NULL || pRight == NULL)		/* check parameters */
			throw std::exception("Invalid parameters!!!");

		void** ppNodePointers = new void*[GetOrder() + 1];	/* create temporary pointers */
		if (ppNodePointers == NULL)
			throw std::bad_alloc();

		int* pKeys = new int[GetOrder()];					/* create temporary keys */
		if (pKeys == NULL)
			throw std::bad_alloc();

		for (uint32_t i = 0, j = 0; i < pParent->m_nKeys + 1; ++ i, ++ j) {	/* copy pointers */
			if (nIndex + 1 == j)
				++ j;
			ppNodePointers[j] = pParent->m_ppPointer[i];
		}

		for (uint32_t i = 0, j = 0; i < pParent->m_nKeys; ++ i, ++ j) {		/* copy keys */
			if (nIndex == j)
				++ j;
			pKeys[j] = pParent->m_pKeys[i];
		}

		ppNodePointers[nIndex + 1] = pParent;
		pKeys[nIndex] = nKey;

		/* Split in half */
		uint32_t nHalf = Half();
		pParent->m_nKeys = 0;
		uint32_t nPrevKeys = 0;
		for (nPrevKeys = 0; nPrevKeys < nHalf - 1; ++ nPrevKeys) {		/* update the parent pointers and keys */
			pParent->m_ppPointer[nPrevKeys] = ppNodePointers[nPrevKeys];
			pParent->m_pKeys[nPrevKeys] = pKeys[nPrevKeys];
			pParent->IncKeys();
		}
		//pParent->m_ppPointer[nPrevKeys] = ppNodePointers[nPrevKeys];

		pNode = MakeNode();		/* create new node */
		if (pNode == NULL)
			throw std::bad_alloc();

		int nMiddleKey = pKeys[nHalf - 1];
		for (uint32_t nNextKeys = 0; nPrevKeys < GetOrder(); ++ nPrevKeys, ++ nNextKeys) {	/* copy remaining half */
			pNode->m_ppPointer[nNextKeys] = ppNodePointers[nPrevKeys];
			pNode->m_pKeys[nNextKeys] = pKeys[nPrevKeys];
			pNode->IncKeys();
		}
		pNode->m_pParent = pParent->m_pParent;
		//pNode->m_ppPointer[nNextKeys] = ppNodePointers->m_ppPointer[nPrevKeys];

		CNode* pChild = NULL;
		for (uint32_t nNewKeys = 0; nNewKeys < pNode->m_nKeys; ++ nNewKeys) {
			pChild = (CNode*) pNode->m_ppPointer[nNewKeys];					// FIXME: Use CNode** but not void**
			pChild->m_pParent = pNode;
		}

		/* free the temporary pointers and keys */
		delete[] ppNodePointers;
		delete[] pKeys;

		/* insert new key in to parent */
		CNode* pNewNode = InsertInParent(nMiddleKey, pParent, pNode);	/* pParent = pLeft, pNode = pRight */
		if (pNewNode)
			pNode = pNewNode;
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return pNode;
}

/*
* InsertInRoot
*
* insert a new node in root
*/
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
			pNode = InsertInNode(pParent, GetLeftIndex(pParent, pLeft), nKey, pRight);		/* insert in the same node */
		else
			pNode = SplitInsertNode(pParent, GetLeftIndex(pParent, pLeft), nKey, pRight);	/* insert in splitted node */
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return nullptr;
}

/*
* Find
*
* Find a key
*/
bool CBTree::Find(int nKey)
{
	bool bRes = false;
	try {
		if (Find(m_pRoot, nKey))
			bRes = true;
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return bRes;
}

/*
* Find
*
* Find a key, starting from CNode
*/
CNode * CBTree::Find(CNode * pNode, int nKey)
{
	try {
		pNode = FindLeaf(pNode, nKey);
		if (pNode != NULL) {
			uint32_t nIndex = 0;
			for (nIndex = 0; nIndex < pNode->m_nKeys; ++ nIndex)
				if (pNode->m_pKeys[nIndex] == nKey)
					break;
			if (nIndex == pNode->m_nKeys)
				pNode = NULL;
			else
				pNode = (CNode*) pNode->m_ppPointer[nIndex];
		}
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return pNode;
}

/*
* FindLeaf
*
* Find a node with nKey
*/
CNode * CBTree::FindLeaf(int nKey)
{
	CNode* pNode = NULL;
	try {
		if (m_pRoot == NULL)
			throw std::exception("Empty tree!!!");

		pNode = FindLeaf(m_pRoot, nKey);
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return pNode;
}

/*
* FindLeaf
*
* Find a node, starting from pNode with nKey
*/
CNode * CBTree::FindLeaf(CNode * pNode, int nKey)
{
	try {
		while (!pNode->IsLeaf()) {
			int nIndex = 0;
			while (nIndex < pNode->m_nKeys) {
				if (nKey >= pNode->m_pKeys[nIndex])
					++ nIndex;
				else
					break;
			}

			pNode = (CNode*) pNode->m_ppPointer[nIndex];
		}
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return nullptr;
}

/*
* Half
*
* Find the middle of order
*/
uint32_t CBTree::Half() const
{
	if (GetOrder() % 2)
		return GetOrder() / 2;
	return GetOrder() / 2 + 1;
}

