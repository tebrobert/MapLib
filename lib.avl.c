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

    A1 = ((LIB_TRIPLET*)FirstStruct)->A;
    k1 = ((LIB_TRIPLET*)FirstStruct)->k;
    A2 = ((LIB_TRIPLET*)SecondStruct)->A;
    k2 = ((LIB_TRIPLET*)SecondStruct)->k;

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

LIB_TRIPLET* addNode(LIB_TRIPLET T)
{
    LIB_TRIPLET *ptr;
    BOOLEAN ok;
    ptr = RtlInsertElementGenericTableAvl(Table, &T, sizeof(T), &ok);
    return ptr;
}

BOOLEAN existNode(LIB_BLOCK A)
{
    LIB_TRIPLET T, *P;
    T.A = A;
    T.k = 1;
    P = (LIB_TRIPLET*)RtlLookupElementGenericTableAvl(Table, &T);
    if (P == NULL)
        return FALSE;
    return TRUE;
}

LIB_TRIPLET* findNode(LIB_BLOCK A)
{
    LIB_TRIPLET T, *P;
    T.A = A;
    T.k = 1;
    P = RtlLookupElementGenericTableAvl(Table, &T);
    return P;
}

BOOLEAN deleteNode(LIB_BLOCK A)
{
    BOOLEAN ok;
    LIB_TRIPLET *P = findNode(A);
    ok = RtlDeleteElementGenericTableAvl(Table, P);
    return ok;
}

LIB_TRIPLET* nextNode(LIB_BLOCK A)
{
    LIB_TRIPLET *ptr, *temp, TempStruct;
    PVOID restart;
    temp = findNode(A);
    if(temp != NULL){
        restart = (PVOID)((char*)(temp) - 0x20);
        ptr = (LIB_TRIPLET*)RtlEnumerateGenericTableWithoutSplayingAvl(Table, &restart);
        return ptr;
    }
    TempStruct.A = A;
    TempStruct.B = (LIB_BLOCK)NULL;
    TempStruct.k = 1;
    temp = addNode(TempStruct);
    restart = (PVOID)((char*)(temp) - 0x20);
    ptr = (LIB_TRIPLET*)RtlEnumerateGenericTableWithoutSplayingAvl(Table, &restart);
    deleteNode(A);
    return ptr;
}

VOID splitNode(LIB_BLOCK A, LIB_BLOCK *L, LIB_BLOCK *R)
{
}

LIB_PTABLE CopyTable(LIB_PTABLE T)
{
    LIB_PTABLE Table = CreateTable();
    LIB_TRIPLET *elem = (LIB_TRIPLET*)RtlEnumerateGenericTableAvl(T, TRUE);
    
    while (elem != NULL)
    {
        RtlInsertElementGenericTableAvl(Table, elem, sizeof(*elem), NULL);
        elem = (LIB_TRIPLET*)RtlEnumerateGenericTableAvl(T, FALSE);
    }
    
    return Table;
}
