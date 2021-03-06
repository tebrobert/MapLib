﻿#ifndef _LIB_AVL_H
#define _LIB_AVL_H

#include "lib.a.h"

typedef unsigned int LIB_BLOCK;
typedef RTL_AVL_TABLE LIB_TABLE, *LIB_PTABLE;

typedef struct
{
    int Capacity;
    int CharCount;
    char *Data;
}
LIB_BITMASK;

typedef struct
{
    LIB_PTABLE *Data;
    int Count;
    int Capacity;
    LIB_BITMASK SlotBitmask;
}
LIB_PTABLE_ARRAY;

typedef struct
{
    LIB_BLOCK A;
    LIB_BLOCK B;
    LIB_BLOCK k;
    LIB_BITMASK *UsedBy;
}
LIB_NODE, *LIB_PNODE;

typedef struct
{
    LIB_PNODE *Data;
    int Count;
    int Capacity;
}
LIB_PNODE_ARRAY;

typedef struct
{
    LIB_NODE *Data;
    int Count;
    int Capacity;
}
LIB_NODE_ARRAY;

RTL_AVL_COMPARE_ROUTINE     CompareRoutine;
RTL_AVL_ALLOCATE_ROUTINE    AllocateRoutine;
RTL_AVL_FREE_ROUTINE        FreeRoutine;

LIB_PNODE           AddNode(LIB_PTABLE Table, LIB_NODE T);
BOOLEAN             ExistNode(LIB_BLOCK A);
LIB_PNODE           FindNode(LIB_PTABLE, LIB_BLOCK A);
BOOLEAN             DeleteNode(LIB_PTABLE Table, LIB_BLOCK A);
LIB_PNODE           NextNode(LIB_PTABLE, LIB_BLOCK A);
VOID                SplitNode(LIB_PTABLE, LIB_PNODE, LIB_BLOCK Point);

LIB_PTABLE          CreateTable();
LIB_PTABLE          CopyTable(LIB_PTABLE T);
VOID                DeleteTable(LIB_PTABLE Table);

LIB_PTABLE_ARRAY    CreatePTableArray(int Cap);
VOID                DeletePTableArray(LIB_PTABLE_ARRAY*);

LIB_PNODE_ARRAY     CreatePNodeArray(int Cap);
VOID                DeletePNodeArray(LIB_PNODE_ARRAY*);
VOID                AppendPNode(LIB_PNODE_ARRAY*, LIB_PNODE);
LIB_NODE_ARRAY      CreateNodeArray(int Cap);
VOID                DeleteNodeArray(LIB_NODE_ARRAY*);
VOID                AppendNode(LIB_NODE_ARRAY*, LIB_NODE);

LIB_BITMASK         CreateBitmask(int Cap);
LIB_BITMASK         CopyBitmask(LIB_BITMASK *BM);
VOID                DeleteBitmask(LIB_BITMASK*);
BOOLEAN             GetBitValue(LIB_BITMASK *Bitmask, int index);
VOID                SetBitValue(LIB_BITMASK *Bitmask, int index, BOOLEAN value);

#endif //_LIB_AVL_H
