#ifndef _LIB_AVL_H
#define _LIB_AVL_H

#include <ntddk.h>

typedef unsigned int LIB_BLOCK;
typedef unsigned int LIB_NUMBER;
typedef RTL_AVL_TABLE LIB_TABLE;
typedef PRTL_AVL_TABLE LIB_PTABLE;

typedef struct
{
    int Capacity;
    int CharCount;
    char *Data;
}
LIB_BITMASK;

typedef struct
{
    LIB_BLOCK A;
    LIB_BLOCK B;
    LIB_NUMBER k;
    LIB_BITMASK *UsedbyBitmask;
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
RTL_AVL_ALLOCATE_ROUTINE    Allocate_Routine;
RTL_AVL_FREE_ROUTINE        FreeRoutine;

LIB_PNODE       AddNode(LIB_PTABLE Table, LIB_NODE T);
BOOLEAN         existNode(LIB_BLOCK A);
LIB_PNODE       FindNode(LIB_PTABLE, LIB_BLOCK A);
BOOLEAN         DeleteNode(LIB_PTABLE Table, LIB_BLOCK A);
LIB_PNODE       NextNode(LIB_PTABLE, LIB_BLOCK A);
VOID            SplitNode(LIB_PTABLE, LIB_PNODE, LIB_BLOCK Point);

LIB_PTABLE      CreateTable();
LIB_PTABLE      CopyTable(LIB_PTABLE T);
VOID            DeleteTable(LIB_PTABLE Table);

LIB_PNODE_ARRAY CreatePNodeArray(int Cap);
VOID            DeletePNodeArray(LIB_PNODE_ARRAY*);
VOID            AppendPNode(LIB_PNODE_ARRAY*, LIB_PNODE);
LIB_NODE_ARRAY  CreateNodeArray(int Cap);
VOID            DeleteNodeArray(LIB_NODE_ARRAY*);
VOID            AppendNode(LIB_NODE_ARRAY*, LIB_NODE);

#endif //_LIB_AVL_H
