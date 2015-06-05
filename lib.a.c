#include "lib.a.h"

PVOID MemoryAllocate(int NumberOfBytes)
{
    if (LibraryMode == USER_MODE)
    {
        return malloc(NumberOfBytes);
    }
    return ExAllocatePoolWithTag(PagedPool, NumberOfBytes, 1);
}

VOID MemoryFree(PVOID Buffer)
{
    if (LibraryMode == USER_MODE)
    {
        free(Buffer);
        return;
    }
    
    ExFreePool(Buffer);
}

PVOID MemoryReallocate(PVOID Buffer, int NumberOfBytes)
{
    PVOID temp;
    if (LibraryMode == USER_MODE)
    {
        return realloc(Buffer, NumberOfBytes);
    }
    temp = MemoryAllocate(NumberOfBytes);
    memcpy(temp, Buffer, NumberOfBytes);
    MemoryFree(Buffer);
    return temp;
}

VOID InitLibrary(BOOLEAN Mode)
{
    LibraryMode = Mode;
    InitTableArray();
    Table = CreateTable();
}

VOID DeinitLibrary()
{
    DeleteTable(Table);
    DeinitTableArray();
}

PRTL_AVL_TABLE CreateTable()
{
    PRTL_AVL_TABLE Table = (PRTL_AVL_TABLE)MemoryAllocate(sizeof(RTL_AVL_TABLE));
    RtlInitializeGenericTableAvl(Table, CompareRoutine, Allocate_Routine, FreeRoutine, NULL);
    return Table;
}

VOID DeleteTable(PRTL_AVL_TABLE Table)
{
    MemoryFree(Table);
}

VOID InitTableArray()
{
    TableArray = NULL;
    TableCount = 0;
}

VOID DeinitTableArray()
{
    int i;
    for (i = 0; i < TableCount; i++)
        DeleteTable(TableArray[i]);
    MemoryFree(TableArray);
}

VOID appendTable(PRTL_AVL_TABLE T)
{
    TableCount++;
    TableArray = (PRTL_AVL_TABLE*)MemoryReallocate
    (
        TableArray, (TableCount) * sizeof(PRTL_AVL_TABLE)
    );
    TableArray[TableCount - 1] = T;
}
