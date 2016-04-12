#include <iostream>

#include "Support.h"
#include "BTree.h"

/*
* Constructor
*
* Create a node
*/
CNode::CNode()
{
	m_ppPointer = NULL;
	m_pKeys = NULL;
	m_nKeys = 0;
	m_bLeaf = false;
	m_pParent = NULL;
	m_pNext = NULL;
}

/*
* Destructor
*
* Clean all memory for a node
*/
CNode::~CNode()
{
	if (m_ppPointer) {
		delete m_ppPointer;
		m_ppPointer = NULL;
	}

	if (m_pKeys) {
		delete m_pKeys;
		m_pKeys = NULL;
	}
}

/*
 * Constructor
 *
 * Create new tree memory
 */
CBTree::CBTree()
{
}

/*
 * Destructor
 *
 * Clean all memory
 */
CBTree::~CBTree()
{
	DeleteTree();
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
		if (GetOrder() < nMinOrder && GetOrder() > nMaxOrder)
			throw std::exception("Order of tree not supported!!!");

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
		uint32_t nHalf = Half(GetOrder() - 1);
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
		uint32_t nHalf = Half(GetOrder());
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
		if (m_pRoot != NULL && Find(m_pRoot, nKey))
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
		if (pNode == NULL)
			throw std::exception("Invalid parameter!!!");

		while (pNode != NULL && !pNode->IsLeaf()) {
			uint32_t nIndex = 0;
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

	return pNode;
}

/*
 * Delete
 *
 * Delete the node
 */
bool CBTree::Delete(int nKey)
{
	bool bRes = false;
	try {
		if (Delete(m_pRoot, nKey))
			bRes = true;
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return false;
}

/*
 * Delete
 *
 * Delete the node
 */
CNode * CBTree::Delete(CNode * pNode, int nKey)
{
	try {
		CNode* pRecord = Find(m_pRoot, nKey);
		CNode* pLeaf = FindLeaf(m_pRoot, nKey);
		if (pRecord && pLeaf) {
			pNode = DeleteEntry(pLeaf, pRecord, nKey);
			delete pRecord;
		}
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return nullptr;
}

/*
 * DeleteEntry
 *
 * Delete an entry from tree
 */
CNode * CBTree::DeleteEntry(CNode * pNode, CNode * pRecord, int nKey)
{
	CNode* pResult = NULL;
	try {
		pNode = RemoveEntry(pNode, pRecord, nKey);		/* remove the pointer and key */
		if (pNode == m_pRoot)							/* Deletion from root */
			pResult = AdjustRoot();						/* adjust the root */
		else {
			uint32_t nMinKeys = pNode->IsLeaf() ? Half(GetOrder() - 1) : Half(GetOrder()) - 1;
			if (pNode->m_nKeys >= nMinKeys)				/* there is enough space */
				pResult = m_pRoot;
			else {
				int nNextIndex = GetNextIndex(pNode);							/* find the next node */
				int nMiddleIndex = nNextIndex == -1 ? 0 : nNextIndex;
				if (pNode == NULL || pNode->m_pParent == NULL)					/* check if parent is available or not */
					throw std::exception("Invalid parent!!!");

				int nNewMiddleIndex = pNode->m_pParent->m_pKeys[nMiddleIndex];	/* find the middle for adjustment */
				CNode* pNext = (CNode*) ((nNextIndex == -1) ?
					pNode->m_pParent->m_ppPointer[1] : pNode->m_pParent->m_ppPointer[nNextIndex]);
				uint32_t nCapacity = pNode->IsLeaf() ? GetOrder() : GetOrder() - 1;
				if (pNext->m_nKeys + pNode->m_nKeys < nCapacity)				/* space is available adjust nodes */
					pResult = AdjustNodes(pNode, pNext, nNewMiddleIndex, nNextIndex);
				else															/* distribute the space between two nodes */
					pResult = DistributeNodes(pNode, pNext, nMiddleIndex, nNewMiddleIndex, nNextIndex);
			}
		}
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return pResult;
}

/*
 * RemoveEntry
 *
 * Remove key and pointer from node
 */
CNode * CBTree::RemoveEntry(CNode * pNode, CNode * pRecord, int nKey)
{
	try {
		if (pNode == NULL)
			throw std::exception("Invalid parameter!!!");

		/* remove and shift the keys */
		uint32_t nIndex = 0;
		while (nKey != pNode->m_pKeys[nIndex])
			++ nIndex;
		for (; nIndex = pNode->m_nKeys; ++ nIndex)							/* shift the keys */
			pNode->m_pKeys[nIndex - 1] = pNode->m_pKeys[nIndex];

		/* remove and shift the pointer */
		uint32_t nPointers = pNode->IsLeaf() ? pNode->m_nKeys : pNode->m_nKeys + 1;
		nIndex = 0;
		while (pRecord != pNode->m_ppPointer[nIndex])
			++ nIndex;
		for (; nIndex < nPointers; ++nIndex)
			pNode->m_ppPointer[nIndex - 1] = pNode->m_ppPointer[nIndex];	/* shift the pointers */

		pNode->m_nKeys --;													/* update the keys */

		/* update the remaining pointers */
		if (pNode->IsLeaf())
			for (nIndex = pNode->m_nKeys; nIndex < GetOrder() - 1; ++nIndex)
				pNode->m_ppPointer[nIndex] = NULL;
		else
			for (nIndex = pNode->m_nKeys + 1; nIndex < GetOrder(); ++nIndex)
				pNode->m_ppPointer[nIndex] = NULL;
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return nullptr;
}

/*
 * AdjustRoot
 *
 * adjust the root, if there is any deletion from root
 */
CNode * CBTree::AdjustRoot()
{
	CNode* pNode = NULL;
	try {
		if (m_pRoot->m_nKeys > 0)		/* root has space, just return it */
			pNode = m_pRoot;
		else {
			/* no more space in root */
			if (m_pRoot->IsLeaf()) {
				pNode = (CNode*) m_pRoot->m_ppPointer[0];		/* set the new root */
				pNode->m_pParent = NULL;						/* update the parent */
			}
		}

		delete m_pRoot;											/* delete root, we have adjusted it */
		m_pRoot = NULL;
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return nullptr;
}

/*
 * GetNextIndex
 *
 * Get the index of closest node
 */
int CBTree::GetNextIndex(CNode * pNode)
{
	int nResult = 0;
	try {
		if (pNode == NULL || pNode->m_pParent == NULL)
			throw std::exception("Invalid parameter!!!");

		for (uint32_t nIndex = 0; nIndex < pNode->m_pParent->m_nKeys; ++ nIndex)
			if (pNode->m_pParent->m_ppPointer[nIndex] == pNode) {
				nResult = nIndex - 1;
				break;
			}
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return nResult;
}

/*
 * AdjustNodes
 *
 * If a node become too small after deletion, adjust with closest node
 */
CNode * CBTree::AdjustNodes(CNode * pNode, CNode * pNext, int nMiddle, int nNext)
{
	try {
		if (pNode == NULL || pNext == NULL)
			throw std::exception("Invalid parameter!!!");

		/* swap the closest node with the node if they are on extreame left and right */
		if (nNext == -1) {
			CNode* pTemp = pNode;
			pNode = pNext;
			pNext = pTemp;
		}

		uint32_t nNextKeys = pNext->m_nKeys;
		if (!pNode->IsLeaf()) {
			/* not a leaf node */
			pNext->m_pKeys[nNextKeys] = nMiddle;
			pNext->m_nKeys ++;

			/* append the keys and pointers to next */
			uint32_t nIndex = nNextKeys + 1;
			uint32_t nNodeIndex = 0;
			for (; nNodeIndex < pNode->m_nKeys;) {
				pNext->m_pKeys[nIndex ++] = pNode->m_pKeys[nNodeIndex ++];		/* append keys */
				pNext->m_ppPointer[nIndex] = pNode->m_ppPointer[nNodeIndex];	/* append pointers */
				pNext->m_nKeys ++;												/* increment keys in next */
				pNode->m_nKeys --;												/* decrement keys in current */
			}

			/* we have one more pointer then keys, append that pointer too */
			pNext->m_ppPointer[nIndex] = pNode->m_ppPointer[nNodeIndex];

			/* set the parent pointer */
			for (nIndex = 0; nIndex < pNext->m_nKeys + 1; ++ nIndex) {			/* m_nKeys + 1, for last pointer */
				CNode* pTemp = (CNode*) pNext->m_ppPointer[nIndex];
				pTemp->m_pParent = pNext;
			}
		}
		else {
			/* node is a leaf */
			/* append the keys and pointers to next */
			uint32_t nIndex = nNextKeys + 1;
			uint32_t nNodeIndex = 0;
			for (; nNodeIndex < pNode->m_nKeys;) {
				pNext->m_pKeys[nIndex ++] = pNode->m_pKeys[nNodeIndex ++];		/* append the keys */
				pNext->m_ppPointer[nIndex] = pNode->m_ppPointer[nNodeIndex];	/* append the pointers */
				pNext->m_nKeys ++;												/* increment the keys */
			}
			pNext->m_ppPointer[nIndex] = pNode->m_ppPointer[nNodeIndex];		/* copy last pointer */
		}
		m_pRoot = DeleteEntry(pNode->m_pParent, pNode, nMiddle);
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return nullptr;
}

/*
 * DistributeNodes
 *
 * If a node become too small and closest one is too big
 * Just distribute the nodes amongst two nodes
 */
CNode * CBTree::DistributeNodes(CNode * pNode, CNode * pNext, int nMiddle, int nNewMiddle, int nNext)
{
	try {

	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}

	return nullptr;
}

/*
 * DeleteTree
 *
 * Delete the whole tree
 */
void CBTree::DeleteTree()
{
	DeleteTree(m_pRoot);
}

/*
 * DeleteTree
 *
 * Delete the whole tree
 */
void CBTree::DeleteTree(CNode * pNode)
{
	try {
		if (pNode) {
			if (pNode->IsLeaf())
				for (uint32_t nKey = 0; pNode->m_nKeys; ++nKey)
					delete pNode->m_ppPointer[nKey];
			else
				for (uint32_t nKey = 0; pNode->m_nKeys + 1; ++nKey)
					DeleteTree((CNode*)pNode->m_ppPointer[nKey]);
			delete pNode;
			pNode = NULL;
		}
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
}

/*
 * Half
 *
 * Find the middle of order
 */
uint32_t CBTree::Half(uint32_t nOrder) const
{
	if (nOrder % 2)
		return nOrder / 2;
	return nOrder / 2 + 1;
}

