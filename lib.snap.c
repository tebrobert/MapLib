#include "lib.a.h"

VOID SnapshotMake()
{
    // appendTable(Table);
    // Table = CreateTable();
    if (TableCount == TableArrayCapacity) 
        return;
    appendTable(Table);
    Table = CreateTable();
}

int SnapshotCount()
{
    return TableCount;
}
