#include "lib.a.h"

RTL_GENERIC_COMPARE_RESULTS
CompareRoutine(
    __in struct _RTL_AVL_TABLE  *T,
    __in PVOID  FirstStruct,
    __in PVOID  SecondStruct
)
{
    int i, j, k1, k2;
    appBLOCK A1, A2;

    A1 = ((appTRIPLET*)FirstStruct)->A;
    k1 = ((appTRIPLET*)FirstStruct)->k;
    A2 = ((appTRIPLET*)SecondStruct)->A;
    k2 = ((appTRIPLET*)SecondStruct)->k;

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

PVOID
Allocate_Routine(
    __in struct _RTL_AVL_TABLE  *T,
    __in CLONG  ByteSize
)
{
    return MemoryAllocate(ByteSize);
}

VOID
FreeRoutine(
    __in struct _RTL_AVL_TABLE  *T,
    __in PVOID  Buffer
)
{
    MemoryFree(Buffer);
}

appTRIPLET*
addNode(appBLOCK A, appBLOCK B, appNUMBER k)
{
    appTRIPLET T = {A, B, k}, *ptr;
    BOOLEAN ok;
    ptr = RtlInsertElementGenericTableAvl(Table, &T, sizeof(T), &ok);
    return ptr;
}

BOOLEAN
existNode(appBLOCK A)
{
    appTRIPLET T, *P;
    T.A = A;
    T.k = 1;
    P = (appTRIPLET*)RtlLookupElementGenericTableAvl(Table, &T);
    if(P == NULL) return FALSE;
    return TRUE;
}

appTRIPLET*
findNode(appBLOCK A)
{
    appTRIPLET T, *P;
    T.A = A;
    T.k = 1;
    P = RtlLookupElementGenericTableAvl(Table, &T);
    return P;
}

BOOLEAN
deleteNode(appBLOCK A)
{
    BOOLEAN ok;
    appTRIPLET *P = findNode(A);
    ok = RtlDeleteElementGenericTableAvl(Table, P);
    return ok;
}

appTRIPLET* nextNode(appBLOCK A){
    appTRIPLET *ptr, *temp;
    PVOID restart;
    temp = findNode(A);
    if(temp != NULL){
        restart = (PVOID)((char*)(temp) - 0x20);
        ptr = (appTRIPLET*)RtlEnumerateGenericTableWithoutSplayingAvl(Table, &restart);
        return ptr;
    }
    temp = addNode(A, (appBLOCK)NULL, 1);
    restart = (PVOID)((char*)(temp) - 0x20);
    ptr = (appTRIPLET*)RtlEnumerateGenericTableWithoutSplayingAvl(Table, &restart);
    deleteNode(A);
    return ptr;
}
