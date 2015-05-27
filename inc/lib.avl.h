#ifndef _APP_AVL_H
#define _APP_AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <ntddk.h>
#include <lib.avl.h>
#include <app.convstr.h>

typedef PRTL_AVL_TABLE TABLE;
typedef unsigned int appBLOCK;
typedef unsigned int appNUMBER;
typedef struct appTRIPLET_ {appBLOCK A; appBLOCK B; appNUMBER k;} appTRIPLET;

BOOLEAN LibraryCalledFromUserMode;
PRTL_AVL_TABLE Table;
PRTL_AVL_TABLE *TableArray;
int TableCount;

VOID InitLibrary(BOOLEAN Mode);
VOID DeinitLibrary();

void createTableArray();
void appendTable(PRTL_AVL_TABLE T);

RTL_AVL_COMPARE_ROUTINE     CompareRoutine;
RTL_AVL_ALLOCATE_ROUTINE    Allocate_Routine;
RTL_AVL_FREE_ROUTINE        FreeRoutine;

appTRIPLET*     addNode(appBLOCK A, appBLOCK B, appNUMBER k);
BOOLEAN         existNode(appBLOCK A);
appTRIPLET*     findNode(appBLOCK A);
BOOLEAN         deleteNode(appBLOCK A);
appTRIPLET*     nextNode(appBLOCK A);

appBLOCK        checkNode(appBLOCK A, BOOLEAN *ok);
//appTRIPLET**    checkArrNode(appBLOCK A, appNUMBER k);
BOOLEAN         mapNode(appBLOCK A, appBLOCK B, appNUMBER k);
BOOLEAN         unmapNode(appBLOCK A, appNUMBER k);

#endif //_APP_AVL_H
