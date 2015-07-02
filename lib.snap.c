#include "lib.a.h"

int SnapshotMake()
{
    int k, index;
    LIB_PNODE elem;
    LIB_PTABLE NewTable;
    if (TableStorage.Count == TableStorage.Capacity) 
        return -2;
    NewTable = CopyTable(CurrentTable);
    index = FindEmptySlot(&TableStorage);
    
    
    elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(CurrentTable, TRUE);
    while (elem != NULL)
    {
        FindNode(ReadonlyNodes, elem->A);
        
        elem = (LIB_PNODE)RtlEnumerateGenericTableAvl(CurrentTable, FALSE);
    }
    
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
    DeleteTable(CurrentTable);
    CurrentTable = CopyTable(TableStorage.Data[index]);
    return n;
}

int SnapshotSave(int n)
{
    int k, index = n - 1;
    LIB_PTABLE NewTable;
    NewTable = CopyTable(CurrentTable);
    
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

LIB_NODE_ARRAY FindAvailable(int amount, LIB_BLOCK limit)
{
    int i, cap = 0, left;
    LIB_BLOCK I_prev;
    LIB_PNODE ptr;
    LIB_NODE_ARRAY Buffer;
    LIB_NODE nd;
    
    I_prev = 0;
    left = amount;
    ptr = FindNode(ReadonlyNodes, I_prev);
    if (ptr == NULL)
        ptr = NextNode(ReadonlyNodes, I_prev);
    
    while (ptr != NULL && left > 0)
    {
        if (ptr->A > I_prev)
        {
            left -= ptr->A - I_prev;
            cap++;
        }
        I_prev = ptr->A + ptr->k;
        ptr = NextNode(ReadonlyNodes, ptr->A);
    }
    
    if (left > 0 && I_prev < limit)
    {
        left -= limit - I_prev;
        cap++;
    }
    
    if (left > 0)
    {
        Buffer = CreateNodeArray(0);
        return Buffer;
    }
    
    Buffer = CreateNodeArray(cap);
    
    I_prev = 0;
    left = amount;
    ptr = FindNode(ReadonlyNodes, I_prev);
    if (ptr == NULL)
        ptr = NextNode(ReadonlyNodes, I_prev);
    
    while (ptr != NULL && left > 0)
    {
        if (ptr->A > I_prev)
        {
            left -= ptr->A - I_prev;
            nd.A = I_prev;
            nd.k = ptr->A - I_prev;
            if (left < 0)
                nd.k += left;
            AppendNode(&Buffer, nd);
        }
        I_prev = ptr->A + ptr->k;
        ptr = NextNode(ReadonlyNodes, ptr->A);
    }
    
    if (left > 0 && I_prev < limit)
    {
        left -= limit - I_prev;
            nd.A = I_prev;
            nd.k = limit - I_prev;
            if (left < 0)
                nd.k += left;
            AppendNode(&Buffer, nd);
    }
    
    return Buffer;
}
