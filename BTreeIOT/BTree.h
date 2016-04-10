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

