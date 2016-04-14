#include <iostream>

#include "Support.h"
#include "BTree.h"

int main()
{
	CBTree cBTreeObj;

	cBTreeObj.SetOrder(nMinOrder);
	cBTreeObj.Insert(20, 20);
	cBTreeObj.Insert(20, 20);
	cBTreeObj.Insert(10, 10);
	cBTreeObj.Insert(40, 40);
	cBTreeObj.Insert(30, 30);
	cBTreeObj.Find(10);

	return 0;
}