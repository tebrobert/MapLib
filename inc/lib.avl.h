#ifndef _LIB_AVL_H
#define _LIB_AVL_H

#include <ntddk.h>

typedef unsigned int appBLOCK;
typedef unsigned int appNUMBER;

typedef struct appTRIPLET_
{
    appBLOCK A;
    appBLOCK B;
    appNUMBER k;
}
appTRIPLET;

RTL_AVL_COMPARE_ROUTINE     CompareRoutine;
RTL_AVL_ALLOCATE_ROUTINE    Allocate_Routine;
RTL_AVL_FREE_ROUTINE        FreeRoutine;

appTRIPLET*     addNode(appBLOCK A, appBLOCK B, appNUMBER k);
BOOLEAN         existNode(appBLOCK A);
appTRIPLET*     findNode(appBLOCK A);
BOOLEAN         deleteNode(appBLOCK A);
appTRIPLET*     nextNode(appBLOCK A);

#endif //_LIB_AVL_H
