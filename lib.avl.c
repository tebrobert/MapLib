#include "lib.a.h"

RTL_GENERIC_COMPARE_RESULTS CompareRoutine
(
    __in struct _RTL_AVL_TABLE  *T,
    __in PVOID  FirstStruct,
    __in PVOID  SecondStruct
)
{
    int i, j, k1, k2;
    LIB_BLOCK A1, A2;
    
    A1 = ((LIB_NODE*)FirstStruct)->A;
    k1 = ((LIB_NODE*)FirstStruct)->k;
    A2 = ((LIB_NODE*)SecondStruct)->A;
    k2 = ((LIB_NODE*)SecondStruct)->k;
    
    if (k1 == 1 && (A1 >= A2 && A1 < A2 + k2))
        return GenericEqual;
    if (k2 == 1 && (A2 >= A1 && A2 < A1 + k1))
        return GenericEqual; 
    if (A1 > A2)
        return GenericGreaterThan;
    if (A1 < A2)
        return GenericLessThan;
    return GenericEqual;
}

PVOID AllocateRoutine
(
    __in struct _RTL_AVL_TABLE  *T,
    __in CLONG  ByteSize
)
{
    return MemoryAllocate(ByteSize);
}

VOID FreeRoutine
(
    __in struct _RTL_AVL_TABLE  *T,
    __in PVOID  Buffer
)
{
    MemoryFree(Buffer);
}

LIB_PNODE AddNode(LIB_PTABLE Table, LIB_NODE T)
{
    LIB_NODE *ptr;
    BOOLEAN ok;
    ptr = RtlInsertElementGenericTableAvl(Table, &T, sizeof(T), &ok);
    return ptr;
}

BOOLEAN ExistNode(LIB_BLOCK A)
{
    LIB_NODE T, *P;
    T.A = A;
    T.k = 1;
    P = (LIB_NODE*)RtlLookupElementGenericTableAvl(CurrentTable, &T);
    if (P == NULL)
        return FALSE;
    return TRUE;
}

LIB_PNODE FindNode(LIB_PTABLE Table, LIB_BLOCK A)
{
    LIB_NODE T, *P;
    T.A = A;
    T.k = 1;
    P = RtlLookupElementGenericTableAvl(Table, &T);
    return P;
}

BOOLEAN DeleteNode(LIB_PTABLE Table, LIB_BLOCK A)
{
    BOOLEAN ok;
    LIB_PNODE P;
    
    P = FindNode(Table, A);
    if (P->UsedBy != NULL)
        DeleteBitmask(P->UsedBy);
    ok = RtlDeleteElementGenericTableAvl(Table, P);
    return ok;
}

LIB_PNODE NextNode(LIB_PTABLE Table, LIB_BLOCK A)
{
    LIB_NODE *ptr, *temp, TempStruct;
    PVOID restart;
    temp = FindNode(Table, A);
    
    if (temp != NULL)
    {
        restart = (PVOID)((char*)(temp) - 0x20);
        ptr = (LIB_NODE*)RtlEnumerateGenericTableWithoutSplayingAvl(Table, &restart);
        return ptr;
    }
    
    TempStruct.A = A;
    TempStruct.k = 1;
    TempStruct.UsedBy = NULL;
    temp = AddNode(Table, TempStruct);
    restart = (PVOID)((char*)(temp) - 0x20);
    ptr = (LIB_NODE*)RtlEnumerateGenericTableWithoutSplayingAvl(Table, &restart);
    DeleteNode(Table, A);
    return ptr;
}

VOID SplitNode(LIB_PTABLE Table, LIB_PNODE Node, LIB_BLOCK Point)
{
    LIB_NODE T1, T2;
    T1.A = Node->A;
    T1.B = Node->B;
    T1.k = Point - T1.A;
    T2.A = Point;
    T2.B = T1.B + T1.k;
    T2.k = Node->k - T1.k;
    
    if (Node->UsedBy != NULL)
    {
        T1.UsedBy = MemoryAllocate(sizeof(LIB_BITMASK));
        *T1.UsedBy = CopyBitmask(Node->UsedBy);
        T2.UsedBy = MemoryAllocate(sizeof(LIB_BITMASK));
        *T2.UsedBy = CopyBitmask(Node->UsedBy);
        DeleteBitmask(Node->UsedBy);
    }
    
    RtlDeleteElementGenericTableAvl(Table, Node);
    RtlInsertElementGenericTableAvl(Table, &T1, sizeof(T1), NULL);
    RtlInsertElementGenericTableAvl(Table, &T2, sizeof(T2), NULL);
}

LIB_PTABLE CreateTable()
{
    LIB_PTABLE Table = (LIB_PTABLE)MemoryAllocate(sizeof(LIB_TABLE));
    RtlInitializeGenericTableAvl(Table, CompareRoutine, AllocateRoutine, FreeRoutine, NULL);
    return Table;
}

LIB_PTABLE CopyTable(LIB_PTABLE T)
{
    LIB_PTABLE Table = CreateTable();
    LIB_NODE *elem;
    
    elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(T, TRUE);
    while (elem != NULL)
    {
        RtlInsertElementGenericTableAvl(Table, elem, sizeof(*elem), NULL);
        elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(T, FALSE);
    }
    
    return Table;
}

VOID DeleteTable(LIB_PTABLE Table)
{
    LIB_NODE *elem;
    
    elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(Table, TRUE);
    while (elem != NULL)
    {
        if (elem->UsedBy != NULL)
            DeleteBitmask(elem->UsedBy);
        elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(Table, FALSE);
    }
    MemoryFree(Table);
}

LIB_PTABLE_ARRAY CreatePTableArray(int Cap)
{
    LIB_PTABLE_ARRAY Table;
    Table.Capacity = Cap;
    Table.Data = (LIB_PTABLE*)MemoryAllocate(Cap * sizeof(LIB_PTABLE));
    Table.Count = 0;
    Table.SlotBitmask = CreateBitmask(Cap);
    return Table;
}

VOID DeletePTableArray(LIB_PTABLE_ARRAY *TA)
{
    int i;
    for (i = 0; i < TA->Count; i++)
    {
        if (GetBitValue(&TA->SlotBitmask, i))
            DeleteTable(TA->Data[i]);
    }
    DeleteBitmask(&TA->SlotBitmask);
    MemoryFree(TA->Data);
}

LIB_PNODE_ARRAY CreatePNodeArray(int Cap)
{
    LIB_PNODE_ARRAY NodeArray;
    NodeArray.Capacity = Cap;
    NodeArray.Data = (LIB_PNODE*)MemoryAllocate(Cap * sizeof(LIB_PNODE));
    NodeArray.Count = 0;
    return NodeArray;
}

VOID DeletePNodeArray(LIB_PNODE_ARRAY* NA)
{
    MemoryFree(NA->Data);
}

VOID AppendPNode(LIB_PNODE_ARRAY* NA, LIB_PNODE Node)
{
    NA->Data[NA->Count] = Node;
    NA->Count++;
}

LIB_NODE_ARRAY CreateNodeArray(int Cap)
{
    LIB_NODE_ARRAY NodeArray;
    NodeArray.Capacity = Cap;
    NodeArray.Data = (LIB_NODE*)MemoryAllocate(Cap * sizeof(LIB_NODE));
    NodeArray.Count = 0;
    return NodeArray;
}

VOID DeleteNodeArray(LIB_NODE_ARRAY* NA)
{
    MemoryFree(NA->Data);
}

VOID AppendNode(LIB_NODE_ARRAY* NA, LIB_NODE Node)
{
    NA->Data[NA->Count] = Node;
    NA->Count++;
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
