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

/*
        [                    ]                                                
        [    ]     [      ]       [       ]                                      
                                                                            
                                                                            
                                                                            
*/

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

BOOLEAN IsInBorders(LIB_BLOCK Left, LIB_BLOCK Right, LIB_BLOCK Point)
{
    return (Point >= Left && Point < Right);
}

BOOLEAN PrepareToWrite(LIB_BLOCK A, LIB_BLOCK k)
{
    LIB_PNODE_ARRAY Buffer, Reserved;
    LIB_NODE_ARRAY Available;
    BOOLEAN IsReserved;
    int i, j, w, CurrentAvailableIndex;
    LIB_BLOCK ToRedirect, FirstLeftVoidBorder, FirstRightVoidBorder, LastLeftVoidBorder, LastRightVoidBorder;
    LIB_BLOCK TempA, TempK, CurrentAvailableBlock, CurrentAvailableLength;
    
    ToRedirect = 0;
    
    Buffer = CheckInterval(CurrentTable, A, k);
    FirstLeftVoidBorder = Buffer.Data[0]->B;
    FirstRightVoidBorder = A + (Buffer.Data[0]->B - Buffer.Data[0]->A);
    LastLeftVoidBorder = (A + k) + (Buffer.Data[Buffer.Count - 1]->B - Buffer.Data[Buffer.Count - 1]->A);
    LastRightVoidBorder = Buffer.Data[Buffer.Count - 1]->B + Buffer.Data[Buffer.Count - 1]->k;
    
    printf("FirstLeftVoidBorder = %d\n", FirstLeftVoidBorder);
    printf("FirstRightVoidBorder = %d\n", FirstRightVoidBorder);
    printf("LastLeftVoidBorder = %d\n", LastLeftVoidBorder);
    printf("LastRightVoidBorder = %d\n", LastRightVoidBorder);
    
    printf("Buffer.Count = %d\n", Buffer.Count);
    for (i = 0; i < Buffer.Count; i++)
    {
        Reserved = CheckInterval(ReadonlyNodes, Buffer.Data[i]->B, Buffer.Data[i]->k);
        printf("r=c(%d,%d)\n", Buffer.Data[i]->B, Buffer.Data[i]->k);
        
        printf("Reserved.Count = %d\n", Reserved.Count);
        for (j = 0; j < Reserved.Count; j++)
        {
            IsReserved = FALSE;
            
            printf("LOOOL!!!!!!!!!\n");
            printf("Reserved.Data[i]->A = %d\n", Reserved.Data[j]->A);
            printf("Reserved.Data[i]->k = %d\n", Reserved.Data[j]->k);
            printf("Reserved.Data[i]->UsedBy->Capacity = %d\n", Reserved.Data[j]->UsedBy->Capacity);
            printf("L!!!!!!!!!\n");
            
            for (w = 1; w < Reserved.Data[j]->UsedBy->Capacity; w++)
            {
                if (GetBitValue(Reserved.Data[j]->UsedBy, w))
                {
                    IsReserved = TRUE;
                    break;
                }
            }
            
            if (IsReserved)
            {
                TempA = Reserved.Data[j]->A;
                TempK = Reserved.Data[j]->k;
                
                ToRedirect += TempK;
                
                if (IsInBorders(FirstLeftVoidBorder, FirstRightVoidBorder, Reserved.Data[j]->A))
                {
                    printf("NYYYYYYYYYYP1\n");
                    if (!IsInBorders(FirstLeftVoidBorder, FirstRightVoidBorder, Reserved.Data[j]->A + Reserved.Data[j]->k))
                    {
                        ToRedirect -= (FirstRightVoidBorder - TempA);
                    }
                    else
                    {
                        ToRedirect -= TempK;
                    }
                }
                if (IsInBorders(LastLeftVoidBorder, LastRightVoidBorder, Reserved.Data[j]->A + Reserved.Data[j]->k - 1))
                {
                    printf("NYYYYYYYYYYP2\n");
                    if (!IsInBorders(LastLeftVoidBorder, LastRightVoidBorder, Reserved.Data[j]->A))
                    {
                        ToRedirect -= (TempA + TempK - LastLeftVoidBorder);
                    }
                    else
                    {
                        ToRedirect -= TempK;
                    }
                }
                
                
            }
        }
        DeletePNodeArray(&Reserved);
    }
    DeletePNodeArray(&Buffer);
    
    printf("ToRedirect = %d\n", ToRedirect);
    if (ToRedirect == 0)
    {
        return TRUE;
    }
    
    
    Available = FindAvailable(ToRedirect, PhysicalFileSize);
    if (Available.Count == 0)
    {
        DeleteNodeArray(&Available);
        return FALSE;
    }
    
    printf("Available.Count = %d\n", Available.Count);
    printf("Available.Data[0].A = %d\n", Available.Data[0].A);
    printf("Available.Data[0].k = %d\n", Available.Data[0].k);
    
    CurrentAvailableIndex = 0;
    CurrentAvailableBlock = Available.Data[0].A;
    CurrentAvailableLength = Available.Data[0].k;
    
    Buffer = CheckInterval(CurrentTable, A, k);
    for (i = 0; i < Buffer.Count; i++)
    {
        Reserved = CheckInterval(ReadonlyNodes, Buffer.Data[i]->B, Buffer.Data[i]->k);
        
        for (j = 0; j < Reserved.Count; j++)
        {
            IsReserved = FALSE;
            for (w = 1; w < Reserved.Data[j]->UsedBy->Capacity; w++)
            {
                if (GetBitValue(Reserved.Data[j]->UsedBy, w))
                {
                    IsReserved = TRUE;
                    break;
                }
            }
            
            if (IsReserved)
            {
                TempA = Reserved.Data[j]->A;
                TempK = Reserved.Data[j]->k;
                
                if (IsInBorders(FirstLeftVoidBorder, FirstRightVoidBorder, Reserved.Data[j]->A))
                {
                    if (!IsInBorders(FirstLeftVoidBorder, FirstRightVoidBorder, Reserved.Data[j]->A + Reserved.Data[j]->k))
                    {
                        TempK -= FirstRightVoidBorder - TempA;
                        TempA = FirstRightVoidBorder;
                    }
                    else
                    {
                        TempK = 0;
                    }
                }
                if (IsInBorders(LastLeftVoidBorder, LastRightVoidBorder, Reserved.Data[j]->A + Reserved.Data[j]->k - 1))
                {
                    if (!IsInBorders(LastLeftVoidBorder, LastRightVoidBorder, Reserved.Data[j]->A))
                    {
                        TempK = LastLeftVoidBorder - TempA;
                    }
                    else
                    {
                        TempK = 0;
                    }
                }
                
                while (TempK > 0)
                {
                    printf("GRAPP> TempK = %d, CurrentAvailableLength = %d\n", TempK, CurrentAvailableLength);
                    if (TempK <= CurrentAvailableLength)
                    {
                        printf("here1\n");
                        printf("<%d, %d, %d>\n\n",Buffer.Data[i]->A+(TempA-Buffer.Data[i]->B), CurrentAvailableBlock, TempK);
                        mapNode(Buffer.Data[i]->A + (TempA - Buffer.Data[i]->B), CurrentAvailableBlock, TempK);
                        CurrentAvailableLength -= TempK;
                        CurrentAvailableBlock += TempK;
                        if (CurrentAvailableLength == 0)
                        {
                            CurrentAvailableIndex++;
                            CurrentAvailableBlock = Available.Data[CurrentAvailableIndex].A;
                            CurrentAvailableLength = Available.Data[CurrentAvailableIndex].k;
                        }
                        break;
                    }
                    
                    printf("here2\n");
                    printf("<%d, %d, %d>\n\n", Buffer.Data[i]->A + (TempA - Buffer.Data[i]->B), CurrentAvailableBlock, TempK);
                    mapNode(Buffer.Data[i]->A + (TempA - Buffer.Data[i]->B), CurrentAvailableBlock, CurrentAvailableLength);
                    TempA += CurrentAvailableLength;
                    TempK -= CurrentAvailableLength;
                    CurrentAvailableIndex++;
                    CurrentAvailableBlock = Available.Data[CurrentAvailableIndex].A;
                    CurrentAvailableLength = Available.Data[CurrentAvailableIndex].k;
                }
            }
        }
        
        DeletePNodeArray(&Reserved);
    }
    DeletePNodeArray(&Buffer);
    
    SnapshotZeroSave(0);
    return TRUE;
}
