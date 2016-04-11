#include <iostream>

#include "Support.h"
#include "BTree.h"

int main()
{
	CBTree cBTreeObj;
	cBTreeObj.SetOrder(nMinOrder);
	cBTreeObj.Insert(20, 20);
	return 0;
}