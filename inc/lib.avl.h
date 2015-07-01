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

RTL_AVL_COMPARE_ROUTINE     CompareRoutine;
RTL_AVL_ALLOCATE_ROUTINE    Allocate_Routine;
RTL_AVL_FREE_ROUTINE        FreeRoutine;

LIB_NODE*     addNode(LIB_NODE T);
BOOLEAN         existNode(LIB_BLOCK A);
LIB_NODE*     findNode(LIB_BLOCK A);
BOOLEAN         deleteNode(LIB_BLOCK A);
LIB_NODE*     nextNode(LIB_BLOCK A);

LIB_PTABLE CopyTable(LIB_PTABLE T);

LIB_PNODE_ARRAY CreateNodeArray(int Cap);
VOID            DeleteNodeArray(LIB_PNODE_ARRAY*);
VOID            AppendNode(LIB_PNODE_ARRAY*, LIB_PNODE);

#endif //_LIB_AVL_H
