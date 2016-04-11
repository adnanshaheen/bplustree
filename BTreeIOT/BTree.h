#pragma once


/*
 * CNode class
 */
class CNode
{
public:
	void** m_ppPointer;		/* N-number of pointers */
	int* m_pKeys;			/* N-1 number of keys */
	int m_nKeys;			/* key values */
	bool m_bLeaf;			/* Is leaf node */
	CNode* m_pParent;		/* Pointer to parent node */
	CNode* m_pNext;			/* Pointer to next node */

public:
	CNode();
	~CNode();
};

/*
 * CBTree class
 */
class CBTree
{
private:
	CNode* m_pRoot;			/* root node of btree */
	uint32_t m_uOrder;		/* order of btree */

public:
	CBTree();				/* constructor */
	~CBTree();				/* destructor */

	/* get/set order */
	inline uint32_t GetOrder() const { return m_uOrder; }
	inline void SetOrder(const uint32_t uOrder) { m_uOrder = uOrder; }

	/* Insert a node */
	CNode* Insert(int nKey, int nValue);

private:
	/* Insert node */
	CNode* Insert(CNode* pNode, int nKey, int nValue);					/* insert helper */
	CNode* NewTree(int nKey, int* pPointer);							/* create a new tree */
	int* MakeRecord(int nValue);										/* make a record */
	int GetLeftIndex(CNode* pParent, CNode* pLeft);						/* get the left most index */
	CNode* MakeNode();													/* make a normal node */
	CNode* MakeRoot();													/* make a root node */
	CNode* InsertInLeaf(CNode* pNode, int nKey, int* pPointer);			/* insert at leaf */
	CNode* InsertInLeafSplit(CNode* pNode, int nKey, int* pPointer);	/* insert at leaf after split */
	CNode* InsertInNode(CNode* pParent, int nIndex, int nKey, CNode* pRight);		/* insert in node */
	CNode* InsertInNodeSplit(CNode* pParent, int nIndex, int nKey, CNode* pRight);	/* insert in node after split */
	CNode* InsertInRoot(int nKey, CNode* pLeft, CNode* pRight);			/* insert at root */
	CNode* InsertInParent(int nKey, CNode* pLeft, CNode* pRight);		/* insert at parent */
};
