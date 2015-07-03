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

//GLOBAL
LIB_PTABLE_ARRAY TableStorage;
LIB_PTABLE ReadonlyNodes;
LIB_BITMASK SlotBitmask;
//end GLOBAL

void ROP();
int SnapshotCount();
int SnapshotMake();
int SnapshotLoad(int n);
int SnapshotSave(int n);
int SnapshotDelete(int n);

LIB_PTABLE_ARRAY    CreateTableArray(int Cap);
VOID                DeleteTableArray(LIB_PTABLE_ARRAY*);
int                 FindEmptySlot(LIB_PTABLE_ARRAY *TS);

LIB_BITMASK         CreateBitmask(int Cap);
LIB_BITMASK         CopyBitmask(LIB_BITMASK *BM);
VOID                DeleteBitmask(LIB_BITMASK*);
BOOLEAN             GetBitValue(LIB_BITMASK *Bitmask, int index);
VOID                SetBitValue(LIB_BITMASK *Bitmask, int index, BOOLEAN value);

LIB_NODE_ARRAY      FindAvailable(int amount, LIB_BLOCK limit);

#endif //_LIB_SNAP_H
