#ifndef _LIB_AVL_H
#define _LIB_AVL_H

#include <ntddk.h>

typedef unsigned int appBLOCK;
typedef unsigned int appNUMBER;
typedef RTL_AVL_TABLE LIB_TABLE;
typedef PRTL_AVL_TABLE LIB_PTABLE;

typedef struct
{
    appBLOCK A;
    appBLOCK B;
    appNUMBER k;
    int BitmaskCount;
    int *UsedbyBitmask;
}
appTRIPLET;

RTL_AVL_COMPARE_ROUTINE     CompareRoutine;
RTL_AVL_ALLOCATE_ROUTINE    Allocate_Routine;
RTL_AVL_FREE_ROUTINE        FreeRoutine;

appTRIPLET*     addNode(appTRIPLET T);
BOOLEAN         existNode(appBLOCK A);
appTRIPLET*     findNode(appBLOCK A);
BOOLEAN         deleteNode(appBLOCK A);
appTRIPLET*     nextNode(appBLOCK A);

LIB_PTABLE CopyTable(LIB_PTABLE T);

#endif //_LIB_AVL_H
