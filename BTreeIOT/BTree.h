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
};

/*
 * CBTree class
 */
class CBTree
{
private:
	CNode* m_pRoot;			/* root node of btree */

public:
	CBTree();				/* constructor */
	~CBTree();				/* destructor */

	/* Insert a node */
	CNode* Insert(int nKey, int nValue);

private:
	CNode* Insert(CNode* pNode, int nKey, int nValue);			/* insert helper */
	int* MakeRecord(int nValue);								/* make a record */
	int GetLeftIndex(CNode* pParent, CNode* pLeft);				/* get the left most index */
	CNode* MakeNode();											/* make a normal node */
	CNode* MakeRoot();											/* make a root node */
	CNode* InsertInLeaf(CNode* pNode, int nKey, int* pPointer);	/* insert at leaf */

};
