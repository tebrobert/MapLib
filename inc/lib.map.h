#ifndef _LIB_MAP_H
#define _LIB_MAP_H

#include "lib.a.h"

LIB_BLOCK       checkNode(LIB_BLOCK A, BOOLEAN *ok);
LIB_PNODE_ARRAY CheckNodeArr(LIB_PTABLE Table, LIB_BLOCK A, LIB_NUMBER k);
BOOLEAN         mapNode(LIB_BLOCK A, LIB_BLOCK B, LIB_NUMBER k);
BOOLEAN         unmapNode(LIB_BLOCK A, LIB_NUMBER k);

#endif //_LIB_MAP_H
