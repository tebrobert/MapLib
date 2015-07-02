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

PVOID Allocate_Routine
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

LIB_NODE* addNode(LIB_NODE T)
{
    LIB_NODE *ptr;
    BOOLEAN ok;
    ptr = RtlInsertElementGenericTableAvl(CurrentTable, &T, sizeof(T), &ok);
    return ptr;
}

BOOLEAN existNode(LIB_BLOCK A)
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

BOOLEAN deleteNode(LIB_BLOCK A)
{
    BOOLEAN ok;
    LIB_NODE *P = FindNode(CurrentTable, A);
    ok = RtlDeleteElementGenericTableAvl(CurrentTable, P);
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
    temp = addNode(TempStruct);
    restart = (PVOID)((char*)(temp) - 0x20);
    ptr = (LIB_NODE*)RtlEnumerateGenericTableWithoutSplayingAvl(Table, &restart);
    deleteNode(A);
    return ptr;
}

VOID splitNode(LIB_BLOCK A, LIB_BLOCK *L, LIB_BLOCK *R)
{
}

LIB_PTABLE CopyTable(LIB_PTABLE T)
{
    LIB_PTABLE Table = CreateTable();
    LIB_NODE *elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(T, TRUE);
    
    while (elem != NULL)
    {
        RtlInsertElementGenericTableAvl(Table, elem, sizeof(*elem), NULL);
        elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(T, FALSE);
    }
    
    return Table;
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
