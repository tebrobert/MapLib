#include "lib.a.h"

void ROP()
{
    int i;
    LIB_PNODE elem;
    
    elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(ReadonlyNodes, TRUE);
    while (elem != NULL)
    {
        printf("<%d, %d> :", elem->A, elem->A + elem->k);
        for (i = 0; i < elem->UsedbyBitmask->Capacity; i++)
        {
            if (GetBitValue(elem->UsedbyBitmask, i))
                printf(" #%d", i + 1);
        }
        printf("\n");
        elem = (LIB_PNODE)RtlEnumerateGenericTableAvl(ReadonlyNodes, FALSE);
    }
}

int SnapshotMake()
{
    int i, k, index;
    LIB_NODE New;
    LIB_PNODE elem, temp;
    LIB_PTABLE NewTable;
    LIB_BLOCK I, I_prev;
    LIB_PNODE_ARRAY Buffer;
    
    if (TableStorage.Count == TableStorage.Capacity) 
        return -2;
    
    NewTable = CopyTable(CurrentTable);
    index = FindEmptySlot(&TableStorage);
    
    elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(CurrentTable, TRUE);
    while (elem != NULL)
    {
        //OLD
        // temp = FindNode(ReadonlyNodes, elem->B);
        // if (temp == NULL)
        // {
            // New.A = elem->B;
            // New.k = elem->k;
            // New.UsedbyBitmask = MemoryAllocate(sizeof(LIB_BITMASK));
            // *New.UsedbyBitmask = CreateBitmask(TableStorage.Capacity);
            // SetBitValue(New.UsedbyBitmask, index, TRUE);
            // AddNode(ReadonlyNodes, New);
            // continue;
        // }
        // if (temp->A + temp->k > elem->B + elem->k)
        // {
            // SplitNode(ReadonlyNodes, temp, elem->B + elem->k);
            // temp = FindNode(ReadonlyNodes, elem->B);
        // }
        // if (elem->B > temp->A)
        // {
            // SplitNode(ReadonlyNodes, temp, elem->B);
            // temp = FindNode(ReadonlyNodes, elem->B);
        // }
        // SetBitValue(temp->UsedbyBitmask, index, TRUE);
        //end OLD
        
        //NEW
        temp = FindNode(ReadonlyNodes, elem->B);
        if (temp != NULL && elem->B > temp->A)
            SplitNode(ReadonlyNodes, temp, elem->B);
        temp = FindNode(ReadonlyNodes, elem->B + elem->k - 1);  // -1 !
        if (temp != NULL && elem->B + elem->k < temp->A + temp->k)
            SplitNode(ReadonlyNodes, temp, elem->B + elem->k);
        Buffer = CheckNodeArr(ReadonlyNodes, elem->B, elem->k);
        I = elem->B;
        I_prev = elem->B;
        for (i = 0; i < Buffer.Count; i++)
        {
            temp = Buffer.Data[i];
            if (I_prev < temp->A)
            {
                //printf("[%d, %d) not mapped!\n", I_prev, temp->A);
                New.A = I_prev;
                New.k = temp->A - I_prev;
                New.UsedbyBitmask = MemoryAllocate(sizeof(LIB_BITMASK));
                *New.UsedbyBitmask = CreateBitmask(SlotBitmask.Capacity);
                SetBitValue(New.UsedbyBitmask, index, TRUE);
                AddNode(ReadonlyNodes, New);
                I = temp->A;
            }
            // printf("[%d, %d) -> [%d, %d)\n", temp->A, temp->A + temp->k, temp->B, temp->B + temp->k);
            SetBitValue(temp->UsedbyBitmask, index, TRUE);
            I_prev = temp->A + temp->k;
        }
        if (Buffer.Count == 0)
        {
            //printf("[%d, %d) not mapped!\n", elem->B, elem->B + elem->k);
            New.A = elem->B;
            New.k = elem->k;
            New.UsedbyBitmask = MemoryAllocate(sizeof(LIB_BITMASK));
            *New.UsedbyBitmask = CreateBitmask(SlotBitmask.Capacity);
            SetBitValue(New.UsedbyBitmask, index, TRUE);
            AddNode(ReadonlyNodes, New);
        }
        if (i > 0 && (elem->B + elem->k > temp->A + temp->k))
        {
            //printf("[%d, %d) not mapped!\n", temp->A + temp->k, elem->B + elem->k);
            New.A = temp->A + temp->k;
            New.k = (elem->B + elem->k) - (temp->A + temp->k);
            New.UsedbyBitmask = MemoryAllocate(sizeof(LIB_BITMASK));
            *New.UsedbyBitmask = CreateBitmask(SlotBitmask.Capacity);
            SetBitValue(New.UsedbyBitmask, index, TRUE);
            AddNode(ReadonlyNodes, New);
        }
        DeletePNodeArray(&Buffer);
        
        //end NEW
        
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
    int i, index;
    LIB_PNODE elem, prev;
    
    index = n - 1;
    if (index >= TableStorage.Capacity || index < 0) 
        return -2;
    if (!GetBitValue(&SlotBitmask, index)) 
        return -1;
    
    prev = NULL;
    elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(ReadonlyNodes, TRUE);
    while (elem != NULL)
    {
        SetBitValue(elem->UsedbyBitmask, index, FALSE);
        
        if (prev != NULL)
        {
            for (i = 0; i < prev->UsedbyBitmask->Capacity; i++)
            {
                if (GetBitValue(prev->UsedbyBitmask, i))
                    break;
            }
            if (i == prev->UsedbyBitmask->Capacity)
            {
                DeleteNode(ReadonlyNodes, prev->A);
            }
        }
        
        prev = elem;
        elem = (LIB_PNODE)RtlEnumerateGenericTableAvl(ReadonlyNodes, FALSE);
    }
    if (prev != NULL)
    {
        for (i = 0; i < prev->UsedbyBitmask->Capacity; i++)
        {
            if (GetBitValue(prev->UsedbyBitmask, i))
                break;
        }
        if (i == prev->UsedbyBitmask->Capacity)
        {
            DeleteNode(ReadonlyNodes, prev->A);
        }
    }
    
    DeleteTable(TableStorage.Data[index]);
    SetBitValue(&SlotBitmask, index, FALSE);
    TableStorage.Count--;
    
    return n;
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

LIB_BITMASK CopyBitmask(LIB_BITMASK *BM)
{
    int i;
    LIB_BITMASK Bitmask;
    
    Bitmask.Capacity = BM->Capacity;
    Bitmask.CharCount = BM->CharCount;
    Bitmask.Data = (char*)MemoryAllocate(BM->CharCount);
    for (i = 0; i < BM->CharCount; i++)
         Bitmask.Data[i] = BM->Data[i];
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
