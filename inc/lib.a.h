#ifndef _LIB_A_H
#define _LIB_A_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ntddk.h>
#include <conio.h>

#include "lib.avl.h"
#include "lib.map.h"
#include "lib.snap.h"

typedef enum
{
    KERNEL_MODE = 0,
    USER_MODE   = 1,
}
LIB_MODE;

PRTL_AVL_TABLE Table;
PRTL_AVL_TABLE *TableArray;
int TableCount;
int TableArrayCapacity;

LIB_MODE LibraryMode;

PVOID MemoryAllocate(int NumberOfBytes);
VOID MemoryFree(PVOID Buffer);
PVOID MemoryReallocate(PVOID Buffer, int NumberOfBytes);

VOID InitLibrary(BOOLEAN Mode);
VOID DeinitLibrary();

PRTL_AVL_TABLE CreateTable();
VOID DeleteTable(PRTL_AVL_TABLE Table);
VOID InitTableArray();
VOID DeinitTableArray();
VOID appendTable(PRTL_AVL_TABLE T);

#endif //_LIB_A_H
