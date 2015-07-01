#ifndef _LIB_AVL_H
#define _LIB_AVL_H

#include <ntddk.h>

typedef unsigned int LIB_BLOCK;
typedef unsigned int LIB_NUMBER;
typedef RTL_AVL_TABLE LIB_TABLE;
typedef PRTL_AVL_TABLE LIB_PTABLE;

typedef struct
{
    LIB_BLOCK A;
    LIB_BLOCK B;
    LIB_NUMBER k;
    int BitmaskCount;
    int *UsedbyBitmask;
}
LIB_TRIPLET;

RTL_AVL_COMPARE_ROUTINE     CompareRoutine;
RTL_AVL_ALLOCATE_ROUTINE    Allocate_Routine;
RTL_AVL_FREE_ROUTINE        FreeRoutine;

LIB_TRIPLET*     addNode(LIB_TRIPLET T);
BOOLEAN         existNode(LIB_BLOCK A);
LIB_TRIPLET*     findNode(LIB_BLOCK A);
BOOLEAN         deleteNode(LIB_BLOCK A);
LIB_TRIPLET*     nextNode(LIB_BLOCK A);

LIB_PTABLE CopyTable(LIB_PTABLE T);

#endif //_LIB_AVL_H
