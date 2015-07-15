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
int SnapshotLoad(int n);
int SnapshotSave(int n);
int SnapshotDelete(int n);

int             FindEmptySlot(LIB_PTABLE_ARRAY *TS);
LIB_NODE_ARRAY  FindAvailable(int amount, LIB_BLOCK limit);
BOOLEAN         PrepareToWrite(LIB_BLOCK A, LIB_BLOCK k);

#endif //_LIB_SNAP_H
