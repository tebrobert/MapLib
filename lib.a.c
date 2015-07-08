#include "lib.a.h"

PVOID MemoryAllocate(int NumberOfBytes)
{
    if (LibraryMode == USER_MODE)
    {
        return malloc(NumberOfBytes);
    }
    // return ExAllocatePool(PagedPool, NumberOfBytes);
    return ExAllocatePoolWithTag(PagedPool, NumberOfBytes, '1gaT');
}

VOID MemoryFree(PVOID Buffer)
{
    if (LibraryMode == USER_MODE)
    {
        free(Buffer);
        return;
    }
    ExFreePool(Buffer);
    // ExFreePoolWithTag(Buffer, '1gaT');
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

VOID InitLibrary(BOOLEAN _LibraryMode, ULONG _LogicalFileSize, ULONG _PhysicalFileSize, int _MaxSlotCount)
{
    LibraryMode = _LibraryMode;
    LogicalFileSize = _LogicalFileSize;
    PhysicalFileSize = _PhysicalFileSize;
    SlotBitmask = CreateBitmask(_MaxSlotCount);
    TableStorage = CreateTableArray(_MaxSlotCount);
    CurrentTable = CreateTable();
    ReadonlyNodes = CreateTable();
    mapNode(0, 0, LogicalFileSize);
}

VOID DeinitLibrary()
{
    DeleteTable(CurrentTable);
    DeleteTable(ReadonlyNodes);
    DeleteBitmask(&SlotBitmask);
    DeleteTableArray(&TableStorage);
}
