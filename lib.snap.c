#include "lib.a.h"

int SnapshotMake()
{
    int i, n, index;
    LIB_PTABLE NewTable;
    LIB_NODE New;
    LIB_PNODE elem, temp;
    LIB_BLOCK I, I_prev;
    LIB_PNODE_ARRAY Buffer;
    
    if (TableStorage.Count == TableStorage.Capacity) 
        return -2;
    
    index = FindEmptySlot(&TableStorage);
    //n = index + 1;
    n = index;
    
    SnapshotSave(n);
    
    return n;
}

int SnapshotCount()
{
    //return TableStorage.Count;
    return TableStorage.Count - 1;
}

int SnapshotCapacity()
{
    //return TableStorage.Capacity;
    return TableStorage.Capacity - 1;
}

int SnapshotZeroSave(int n)
{
    int i, k, index;
    LIB_PTABLE NewTable;
    LIB_NODE New;
    LIB_PNODE elem, temp;
    LIB_BLOCK I, I_prev;
    LIB_PNODE_ARRAY Buffer;
    
    //index = n - 1;
    index = n;
    if (index >= TableStorage.Capacity || index < 0) 
        return -2;
    
    NewTable = CopyTable(CurrentTable);
    
    if (GetBitValue(&TableStorage.SlotBitmask, index))
        SnapshotDelete(n);
    
    elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(CurrentTable, TRUE);
    while (elem != NULL)
    {
        temp = FindNode(ReadonlyNodes, elem->B);
        if (temp != NULL && elem->B > temp->A)
            SplitNode(ReadonlyNodes, temp, elem->B);
        temp = FindNode(ReadonlyNodes, elem->B + elem->k - 1);  // -1 !
        if (temp != NULL && elem->B + elem->k < temp->A + temp->k)
            SplitNode(ReadonlyNodes, temp, elem->B + elem->k);
        Buffer = CheckInterval(ReadonlyNodes, elem->B, elem->k);
        I = elem->B;
        I_prev = elem->B;
        for (i = 0; i < Buffer.Count; i++)
        {
            temp = Buffer.Data[i];
            if (I_prev < temp->A)
            {
                New.A = I_prev;
                New.k = temp->A - I_prev;
                New.UsedBy = MemoryAllocate(sizeof(LIB_BITMASK));
                *New.UsedBy = CreateBitmask(TableStorage.SlotBitmask.Capacity);
                SetBitValue(New.UsedBy, index, TRUE);
                AddNode(ReadonlyNodes, New);
                I = temp->A;
            }
            SetBitValue(temp->UsedBy, index, TRUE);
            I_prev = temp->A + temp->k;
        }
        if (Buffer.Count == 0)
        {
            New.A = elem->B;
            New.k = elem->k;
            New.UsedBy = MemoryAllocate(sizeof(LIB_BITMASK));
            *New.UsedBy = CreateBitmask(TableStorage.SlotBitmask.Capacity);
            SetBitValue(New.UsedBy, index, TRUE);
            AddNode(ReadonlyNodes, New);
        }
        if (i > 0 && (elem->B + elem->k > temp->A + temp->k))
        {
            New.A = temp->A + temp->k;
            New.k = (elem->B + elem->k) - (temp->A + temp->k);
            New.UsedBy = MemoryAllocate(sizeof(LIB_BITMASK));
            *New.UsedBy = CreateBitmask(TableStorage.SlotBitmask.Capacity);
            SetBitValue(New.UsedBy, index, TRUE);
            AddNode(ReadonlyNodes, New);
        }
        DeletePNodeArray(&Buffer);
        
        elem = (LIB_PNODE)RtlEnumerateGenericTableAvl(CurrentTable, FALSE);
    }
    
    TableStorage.Data[index] = NewTable;
    SetBitValue(&TableStorage.SlotBitmask, index, TRUE);
    TableStorage.Count++;
    
    return n;
}

int SnapshotSave(int n)
{
    if (n == 0)
        return -2;
    return SnapshotZeroSave(n);
}

int SnapshotLoad(int n)
{
    //int index = n - 1;
    int index = n;
    if (index >= TableStorage.Capacity || index < 0) 
        return -2;
    if (!GetBitValue(&TableStorage.SlotBitmask, index)) 
        return -1;
    DeleteTable(CurrentTable);
    CurrentTable = CopyTable(TableStorage.Data[index]);
    SnapshotZeroSave(0);
    return n;
}

int SnapshotDelete(int n)
{
    int i, index;
    LIB_PNODE elem, prev;
    
    //index = n - 1;
    index = n;
    if (index >= TableStorage.Capacity || index < 0) 
        return -2;
    if (!GetBitValue(&TableStorage.SlotBitmask, index)) 
        return -1;
    
    prev = NULL;
    elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(ReadonlyNodes, TRUE);
    while (elem != NULL)
    {
        SetBitValue(elem->UsedBy, index, FALSE);
        
        if (prev != NULL)
        {
            for (i = 0; i < prev->UsedBy->Capacity; i++)
            {
                if (GetBitValue(prev->UsedBy, i))
                    break;
            }
            if (i == prev->UsedBy->Capacity)
            {
                DeleteNode(ReadonlyNodes, prev->A);
            }
        }
        
        prev = elem;
        elem = (LIB_PNODE)RtlEnumerateGenericTableAvl(ReadonlyNodes, FALSE);
    }
    if (prev != NULL)
    {
        for (i = 0; i < prev->UsedBy->Capacity; i++)
        {
            if (GetBitValue(prev->UsedBy, i))
                break;
        }
        if (i == prev->UsedBy->Capacity)
        {
            DeleteNode(ReadonlyNodes, prev->A);
        }
    }
    
    DeleteTable(TableStorage.Data[index]);
    SetBitValue(&TableStorage.SlotBitmask, index, FALSE);
    TableStorage.Count--;
    
    return n;
}

int FindEmptySlot(LIB_PTABLE_ARRAY *TS)
{
    int i;
    
    for (i = 0; i < TS->Capacity; i++)
    {
        if (!GetBitValue(&TableStorage.SlotBitmask, i))
            break;
    }
    
    if (i == TS->Capacity)
        return -1;
    
    return i;
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

BOOLEAN PrepareToWrite(LIB_BLOCK A, LIB_BLOCK k)
{
    LIB_BLOCK I;
    LIB_PNODE_ARRAY Buffer;
    LIB_NODE_ARRAY Available;
    int i, j;
    
    Available = FindAvailable(k, PhysicalFileSize);
    if (Available.Count == 0)
    {
        DeleteNodeArray(&Available);
        return FALSE;
    }
    
    I = A;
    for (i = 0; i < Available.Count; i++)
    {
        mapNode(I, Available.Data[i].A, Available.Data[i].k);
        I += Available.Data[i].k;
    }
    DeleteNodeArray(&Available);
    
    SnapshotSave(0);
    
    return TRUE;
}
