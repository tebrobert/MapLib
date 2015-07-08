﻿#ifndef _LIB_A_H
#define _LIB_A_H

// #include <stdio.h>
#include <stdlib.h>
#include <ntddk.h>

#include "lib.avl.h"
#include "lib.map.h"
#include "lib.snap.h"

typedef enum
{
    KERNEL_MODE = 0,
    USER_MODE   = 1,
}
LIB_MODE;

//GLOBAL
ULONG       LogicalFileSize;
ULONG       PhysicalFileSize;
//char*       PhysicalFileName;
LIB_PTABLE  CurrentTable;
LIB_MODE    LibraryMode;
//end GLOBAL

PVOID   MemoryAllocate(int NumberOfBytes);
VOID    MemoryFree(PVOID Buffer);
PVOID   MemoryReallocate(PVOID Buffer, int NumberOfBytes);

VOID    InitLibrary(BOOLEAN _LibraryMode, ULONG _LogicalFileSize, ULONG _PhysicalFileSize, int _MaxSlotCount);
VOID    DeinitLibrary();

#endif //_LIB_A_H
