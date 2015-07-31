#ifndef _LIB_SNAP_H
#define _LIB_SNAP_H

#include "lib.a.h"

//GLOBAL
LIB_PTABLE_ARRAY TableStorage;
LIB_PTABLE ReadonlyNodes;
//end GLOBAL

int SnapshotCount();
int SnapshotCapacity();
int SnapshotMake();
BOOLEAN SnapshotSave(int n);
BOOLEAN SnapshotLoad(int n);
BOOLEAN SnapshotDelete(int n);

BOOLEAN         PrepareToWrite(LIB_BLOCK A, LIB_BLOCK k);

int             FindEmptySlot(LIB_PTABLE_ARRAY *TS);
LIB_NODE_ARRAY  FindAvailable(int amount, LIB_BLOCK limit);

#endif //_LIB_SNAP_H
