#include <stdio.h>
#include <stdlib.h>
#include <ntddk.h>
#include <lib.avl.h>
#include <app.convstr.h>

BOOLEAN LibraryCalledFromUserMode;

PRTL_AVL_TABLE Table;
PRTL_AVL_TABLE *TableArray;
int TableCount;

PVOID MemoryAllocate(int NumberOfBytes)
{
    if (LibraryCalledFromUserMode)
    {
        return malloc(NumberOfBytes);
    }
    return ExAllocatePoolWithTag(PagedPool, NumberOfBytes, 1);
}

VOID MemoryFree(PVOID Buffer)
{
    if (LibraryCalledFromUserMode)
    {
        free(Buffer);
        return;
    }
    
    ExFreePool(Buffer);
}

PVOID MemoryReallocate(PVOID Buffer, int NumberOfBytes)
{
    PVOID temp;
    if (LibraryCalledFromUserMode)
    {
        return realloc(Buffer, NumberOfBytes);
    }
    temp = MemoryAllocate(NumberOfBytes);
    memcpy(temp, Buffer, NumberOfBytes);
    MemoryFree(Buffer);
    return temp;
}

void createTableArray()
{
    TableArray = (PRTL_AVL_TABLE*)MemoryAllocate(sizeof(PRTL_AVL_TABLE));
    TableArray[0] = NULL;
    TableCount = 0;
}

void appendTable(PRTL_AVL_TABLE T){
    TableCount++;
    TableArray = (PRTL_AVL_TABLE*)MemoryReallocate(TableArray, (TableCount + 1) * sizeof(PRTL_AVL_TABLE));
    TableArray[TableCount - 1] = T;
    TableArray[TableCount] = NULL;
}

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

    if (k1 == 1 && (A1 >= A2 && A1 < A2 + k2)) return GenericEqual;
    if (k2 == 1 && (A2 >= A1 && A2 < A1 + k1)) return GenericEqual;
    if (A1 > A2) return GenericGreaterThan;
    if (A1 < A2) return GenericLessThan;
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

appBLOCK checkNode(appBLOCK A, BOOLEAN *ok){
    appTRIPLET *P = findNode(A);
    if(P == NULL){
        *ok = FALSE;
        return (appBLOCK)NULL;
    }
    *ok = TRUE;
    return P->B + (A - P->A);
}

// Not finished
/*appTRIPLET** checkArrNode(appBLOCK A, appNUMBER k){
    ULONG       i;
    appBLOCK    checkPoint;
    appTRIPLET  PointLeft, PointRight,
                *IterLeft, *IterRight,
                *EdgeLeft, *EdgeRight,
                *EdgeInnerLeft, *EdgeInnerRight,
                *tmp, *ptr,
                **Buffer;
    PVOID       restart;
    BOOLEAN     ok;
    
    //Searching points
    PointLeft.A     = A;
    PointLeft.k     = 1;
    PointRight.A    = A + k - 1;
    PointRight.k    = 1;
    
    //Check edge points
    if(!existNode(PointLeft.A) || !existNode(PointRight.A - 1))
    {
        return NULL;
    }
    
    //Iterate crossing intervals, check interruptions
    IterLeft    = findNode(PointLeft.A);
    IterRight   = nextNode(PointRight.A);
    EdgeLeft    = IterLeft;
    EdgeRight   = findNode(PointRight.A);
    
    restart = (PVOID)((char*)(IterLeft) - 0x20);
    ptr = IterLeft;
    checkPoint = ptr->A;
    for(i = 0; ptr != IterRight; i++)
    {
        if(ptr->A != checkPoint)
        {
            return NULL;
        }
        checkPoint += ptr->k;
        ptr = nextNode(ptr->A);
    }
    
    //Allocate memory, insert pointers
    Buffer          = (appTRIPLET**) MemoryAllocate(sizeof(appTRIPLET*) * (i + 3));
    EdgeInnerLeft   = (appTRIPLET* ) MemoryAllocate(sizeof(appTRIPLET ));
    EdgeInnerRight  = (appTRIPLET* ) MemoryAllocate(sizeof(appTRIPLET ));
    
    //Left Edge
    restart = (PVOID)((char*)(IterLeft) - 0x20);
    ptr = IterLeft;
    if(IterLeft->A == A && IterLeft->k == k) // case 1
    {
        MemoryFree(EdgeInnerLeft);
        printf("Buffer[%d] = NULL\n", i); Buffer[i] = NULL;
        printf("Buffer[%d] = ptr\n", i+1); Buffer[i+1] = ptr;
        printf("Buffer[%d] = NULL\n", i+2); Buffer[i+2] = NULL;
        printf("Buffer[%d] = NULL\n", i+3); Buffer[i+3] = NULL;
        return Buffer;
    }
    else
    if(A < IterLeft->A + IterLeft->k || A + k < IterLeft->A + IterLeft->k) // case 2 3 4
    {
        EdgeInnerLeft->A = A;
        EdgeInnerLeft->B = IterLeft->B + (A - IterLeft->A);
        EdgeInnerLeft->k = k;
        printf("Buffer[%d] = NULL\n", i); Buffer[i] = EdgeInnerLeft;
        printf("Buffer[%d] = NULL\n", i+1); Buffer[i+1] = NULL;
        printf("Buffer[%d] = NULL\n", i+2); Buffer[i+2] = NULL;
        printf("Buffer[%d] = NULL\n", i+3); Buffer[i+3] = NULL;
        return Buffer;
    }

    // case 5 6 7 8
    EdgeInnerLeft->A = A;
    EdgeInnerLeft->B = IterLeft->B + (A - IterLeft->A);
    EdgeInnerLeft->k = IterLeft->k - (A - IterLeft->A);
    printf("Buffer[%d] = EdgeInnerLeft\n", i); Buffer[i] = EdgeInnerLeft;
    i++;
    printf("Buffer[%d] = NULL\n", i); Buffer[i] = NULL;

    
    //Middle
    for(i = 0; ptr != IterRight; i++, ptr = nextNode(ptr->A))
    {
        if(nextNode(ptr->A) == IterRight)
        {
            printf("Buffer[%d] = NULL\n", i); Buffer[i] = NULL;
            i++;
            if(A == ptr->A)
            {
                MemoryFree(EdgeInnerRight);
            }
            else
            if(A + k == ptr->A + ptr->k)
            {
                MemoryFree(EdgeInnerRight);
                printf("Buffer[%d] = ptr\n", i); Buffer[i] = ptr;
                i++;
            }
            else
            {
                EdgeInnerRight->A = ptr->A;
                EdgeInnerRight->B = ptr->B;
                EdgeInnerRight->k = A + k - ptr->A;
                printf("Buffer[%d] = EdgeInnerRight\n", i); Buffer[i] = EdgeInnerRight;
                i++;
            }
            printf("Buffer[%d] = NULL\n", i); Buffer[i] = NULL;
            break;
        }
        else
        {
            printf("Buffer[%d] = ptr\n", i); Buffer[i] = ptr;
        }
    }
    
    //Right Edge
    
    
    
    printf("---Buffer[0]->A = %d\n", Buffer[0]->A);
    printf("return Buffer\n\n"); return Buffer;
}*/

BOOLEAN mapNode(appBLOCK A, appBLOCK B, appNUMBER k){
    BOOLEAN     ok;
    appTRIPLET  New,
                PointLeft, PointRight,
                *IterLeft, *IterRight,
                *EdgeLeft, *EdgeRight,
                EdgeOuterLeft, EdgeOuterRight,
                *tmp, *ptr;
    PVOID       restart;

    //Find the edge intervals
    PointLeft.A = A;
    PointLeft.k = 1;
    PointRight.A = A + k;
    PointRight.k = 1;

    if(existNode(PointLeft.A))
    {
        IterLeft = findNode(PointLeft.A);
        EdgeLeft = IterLeft;
    }
    else
    {
        IterLeft = nextNode(PointLeft.A);
        EdgeLeft = NULL;
    }

    if(existNode(PointRight.A))
    {
        EdgeRight = findNode(PointRight.A);
        IterRight = nextNode(PointRight.A);
    }
    else
    {
        IterRight = nextNode(PointRight.A);
        EdgeRight = NULL;
    }

    //Split edge intervals
    if(EdgeLeft != NULL)
    {
        EdgeOuterLeft.A = EdgeLeft->A;
        EdgeOuterLeft.B = EdgeLeft->B;
        EdgeOuterLeft.k = PointLeft.A - EdgeLeft->A;
        // EdgeLeft = &EdgeOuterLeft;
    }
    if(EdgeRight != NULL)
    {
        EdgeOuterRight.A = PointRight.A;
        EdgeOuterRight.B = EdgeRight->B + (PointRight.A - EdgeRight->A);
        EdgeOuterRight.k = (EdgeRight->A + EdgeRight->k) - PointRight.A;
        // EdgeRight = &PointRight;
    }

    // Delete crossing intervals
    restart = (PVOID)((char*)(IterLeft) - 0x20);
    for(ptr = IterLeft; ptr != IterRight; )
    {
        tmp = ptr;
        ptr = nextNode(ptr->A);
        RtlDeleteElementGenericTableAvl(Table, tmp);
    }

    //Add the main interval
    New.A = A;
    New.B = B;
    New.k = k;
    RtlInsertElementGenericTableAvl(Table, &New, sizeof(New), &ok);

    //Add the splitted intervals
    if(EdgeLeft != NULL) addNode(EdgeOuterLeft.A, EdgeOuterLeft.B, EdgeOuterLeft.k);
    if(EdgeRight != NULL) addNode(EdgeOuterRight.A, EdgeOuterRight.B, EdgeOuterRight.k);

    return ok;
}

BOOLEAN unmapNode(appBLOCK A, appNUMBER k){
    BOOLEAN ok;
    mapNode(A, (appBLOCK)NULL, k);
    ok = deleteNode(A);
    return ok;
}

VOID InitLibrary(BOOLEAN Mode)
{
    LibraryCalledFromUserMode = Mode;
    
    //??
    createTableArray();
    Table = (PRTL_AVL_TABLE)malloc(sizeof(RTL_AVL_TABLE));
    RtlInitializeGenericTableAvl(Table, CompareRoutine, Allocate_Routine, FreeRoutine, NULL);
    appendTable(Table);
    Table = (PRTL_AVL_TABLE)malloc(sizeof(RTL_AVL_TABLE));
    RtlInitializeGenericTableAvl(Table, CompareRoutine, Allocate_Routine, FreeRoutine, NULL);
    appendTable(Table);
    //printf("##############################\n");
    //for(i=0; i==0 || TableArray[i-1]!=NULL; i++) printf("TableArray[%d] = %x\n", i, TableArray[i]);
    //printf("TableArray = %x\n", TableArray);
    //printf("##############################\n");
}

VOID DeinitLibrary()
{
    freeArr(TableArray);
}
