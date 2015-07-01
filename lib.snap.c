#include "lib.a.h"

int SnapshotMake()
{
    int k, index;
    LIB_PTABLE NewTable;
    if (TableStorage.Count == TableStorage.Capacity) 
        return (int)NULL;
    NewTable = CopyTable(Table);
    index = FindEmptySlot(&TableStorage);
    
    TableStorage.Data[index] = NewTable;
    SetBitValue(&SlotBitmask, index, TRUE);
    TableStorage.Count++;
    
    k = index + 1;
    return k;
}

int SnapshotCount()
{
    return TableStorage.Count;
}

int SnapshotLoad(int n)
{
    int index = n - 1;
    if (index >= TableStorage.Capacity || index < 0) 
        return -2;
    if (!GetBitValue(&SlotBitmask, index)) 
        return -1;
    DeleteTable(Table);
    Table = CopyTable(TableStorage.Data[index]);
    return n;
}

int SnapshotSave(int n)
{
    int k, index = n - 1;
    LIB_PTABLE NewTable;
    NewTable = CopyTable(Table);
    
    if (index >= TableStorage.Capacity || index < 0) 
        return -2;
    
    if (GetBitValue(&SlotBitmask, index))
    {
        DeleteTable(TableStorage.Data[index]);
        TableStorage.Count--;
    }
    
    TableStorage.Data[index] = NewTable;
    SetBitValue(&SlotBitmask, index, TRUE);
    TableStorage.Count++;
    
    return n;
}

int SnapshotDelete(int n)
{
    int index = n - 1;
    if (index >= TableStorage.Capacity || index < 0) 
        return -2;
    if (!GetBitValue(&SlotBitmask, index)) 
        return -1;
    
    DeleteTable(TableStorage.Data[index]);
    SetBitValue(&SlotBitmask, index, FALSE);
    TableStorage.Count--;
    
    return n;
}

LIB_PTABLE CreateTable()
{
    LIB_PTABLE Table = (LIB_PTABLE)MemoryAllocate(sizeof(LIB_TABLE));
    RtlInitializeGenericTableAvl(Table, CompareRoutine, Allocate_Routine, FreeRoutine, NULL);
    return Table;
}

VOID DeleteTable(LIB_PTABLE Table)
{
    MemoryFree(Table);
}

LIB_PTABLE_ARRAY CreateTableArray(int Cap)
{
    LIB_PTABLE_ARRAY Table;
    Table.Capacity = Cap;
    Table.Data = (LIB_PTABLE*)MemoryAllocate(Cap * sizeof(LIB_PTABLE));
    Table.Count = 0;
    return Table;
}

VOID DeleteTableArray(LIB_PTABLE_ARRAY *TA)
{
    int i;
    for (i = 0; i < TA->Count; i++)
        DeleteTable(TA->Data[i]);
    MemoryFree(TA->Data);
}

int FindEmptySlot(LIB_PTABLE_ARRAY *TS)
{
    int i;
    
    for (i = 0; i < TS->Capacity; i++)
    {
        if (!GetBitValue(&SlotBitmask, i))
            break;
    }
    
    if (i == TS->Capacity)
        return -1;
    
    return i;
    
    // TS->Count++;
    // TS->Data[TS->Count - 1] = T;
    // return TS->Count - 1;
}

LIB_BITMASK CreateBitmask(int Cap)
{
    int i;
    LIB_BITMASK Bitmask;
    Bitmask.Capacity = Cap;
    Bitmask.CharCount = (Cap - 1) / 8 + 1;
    Bitmask.Data = (char*)MemoryAllocate(Bitmask.CharCount);
    for (i = 0; i < Cap; i++)
        SetBitValue(&Bitmask, i, FALSE);
    return Bitmask;
}

VOID DeleteBitmask(LIB_BITMASK *Bitmask)
{
    MemoryFree(Bitmask->Data);
}

BOOLEAN GetBitValue(LIB_BITMASK *Bitmask, int index)
{
    int order = index / 8;
    int offset = index % 8;
    return ((Bitmask->Data[order]) & (1 << offset)) >> offset;
}

VOID SetBitValue(LIB_BITMASK *Bitmask, int index, BOOLEAN value)
{
    int order = index / 8;
    int offset = index % 8;
    char mask = 1 << offset;

    if (value)
    {
        Bitmask->Data[order] |= mask;
        return;
    }
    
    Bitmask->Data[order] &= ~mask;
}
