#pragma once


/*
 * CNode class
 */
class CNode
{
public:
	void** m_ppPointer;		/* N-number of pointers */ /* FIXME: Change it to CNode** */
	int* m_pKeys;			/* N-1 number of keys */
	uint32_t m_nKeys;		/* key values */
	bool m_bLeaf;			/* Is leaf node */
	CNode* m_pParent;		/* Pointer to parent node */
	CNode* m_pNext;			/* Pointer to next node */

public:
	CNode();
	~CNode();

	inline void SetLeaf(bool bLeaf = true) { m_bLeaf = bLeaf; }
	inline bool IsLeaf() const { return m_bLeaf; }
	inline void IncKeys() { ++ m_nKeys; }
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
	CNode* Insert(CNode* pNode, int nKey, int nValue);												/* insert helper */
	CNode* NewTree(int nKey, int* pPointer);														/* create a new tree */
	int* MakeRecord(int nValue);																	/* make a record */
	uint32_t GetLeftIndex(CNode* pParent, CNode* pLeft);											/* get the left most index */
	CNode* MakeNode();																				/* make a normal node */
	CNode* MakeLeaf();																				/* make a leaf node */
	CNode* InsertInLeaf(CNode* pNode, int nKey, int* pPointer);										/* insert at leaf */
	CNode* SplitInsertLeaf(CNode* pNode, int nKey, int* pPointer);									/* insert at leaf after split */
	CNode* InsertInNode(CNode* pParent, const uint32_t nIndex, const int nKey, CNode* pRight);		/* insert in node */
	CNode* SplitInsertNode(CNode* pParent, const uint32_t nIndex, const int nKey, CNode* pRight);	/* insert in node after split */
	CNode* InsertInRoot(const int nKey, CNode* pLeft, CNode* pRight);								/* insert at root */
	CNode* InsertInParent(const int nKey, CNode* pLeft, CNode* pRight);								/* insert at parent */

	/* Search node */
	bool Find(int nKey);																			/* find the key */
	CNode* Find(CNode* pNode, int nKey);															/* find the key starting from node */
	CNode* FindLeaf(int nKey);																		/* find a leaf */
	CNode* FindLeaf(CNode* pNode, int nKey);														/* find a leaf starting from node */

	/* delete node */
	bool Delete(int nKey);																			/* delete a node */
	CNode* Delete(CNode* pNode, int nKey);															/* delete a node with key */
	CNode* DeleteEntry(CNode* pNode, CNode* pRecord, int nKey);										/* delete an entry */
	CNode* RemoveEntry(CNode* pNode, CNode* pRecord, int nKey);										/* remove an entry from node */
	CNode* AdjustRoot();																			/* adjust the root */
	int GetNextIndex(CNode* pNode);																	/* get next index */
	CNode* AdjustNodes(CNode* pNode, CNode* pNext, int nMiddle, int nNext);							/* adjust the nodes, if they are too small */
	CNode* DistributeNodes(CNode* pNode, CNode* pNext, int nMiddle, int nNewMiddle, int nNext);		/* distribute the nodes across two nodes */

	/* delete the tree */
	void DeleteTree();																				/* delete the complete tree */
	void DeleteTree(CNode* pNode);																	/* delete one node */

	/* utility */
	uint32_t Half(uint32_t nOrder) const;
};
