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
    int SlotCount = 20;
    LibraryMode = Mode;
    SlotBitmask = CreateBitmask(SlotCount);
    TableStorage = CreateTableArray(SlotCount);
    Table = CreateTable();
    Readonly = CreateTable();
    mapNode(0, 0, 10);
}

VOID DeinitLibrary()
{
    DeleteTable(Table);
    DeleteTable(Readonly);
    DeleteBitmask(&SlotBitmask);
    DeleteTableArray(&TableStorage);
}
