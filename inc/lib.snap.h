#ifndef _LIB_SNAP_H
#define _LIB_SNAP_H

#include <ntddk.h>

#include "lib.avl.h"

typedef struct
{
    LIB_PTABLE *Data;
    int Count;
    int Capacity;
}
LIB_PTABLE_ARRAY;

typedef struct
{
    int Capacity;
    int CharCount;
    char *Data;
}
LIB_BITMASK;

//GLOBAL
LIB_PTABLE_ARRAY TableStorage;
LIB_PTABLE Readonly;
LIB_BITMASK SlotBitmask;
//end GLOBAL

int     SnapshotCount();
int     SnapshotMake();
BOOLEAN SnapshotLoad();

LIB_PTABLE CreateTable();
VOID DeleteTable(LIB_PTABLE Table);

LIB_PTABLE_ARRAY CreateTableArray(int Cap);
VOID DeleteTableArray(LIB_PTABLE_ARRAY*);
int appendTable(LIB_PTABLE_ARRAY *TS, LIB_PTABLE T);

LIB_BITMASK CreateBitmask(int Cap);
VOID DeleteBitmask(LIB_BITMASK*);
BOOLEAN GetBitValue(LIB_BITMASK *Bitmask, int index);
VOID SetBitValue(LIB_BITMASK *Bitmask, int index, BOOLEAN value);

#endif //_LIB_SNAP_H
