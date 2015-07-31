#include "lib.a.h"

extern int SnapshotZeroSave(int n);

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

VOID InitLibrary(BOOLEAN _LibraryMode, ULONG _VirtualFileSize, ULONG _PhysicalFileSize, int _MaxSnapshotCount)
{
    LibraryMode = _LibraryMode;
    VirtualFileSize = _VirtualFileSize;
    PhysicalFileSize = _PhysicalFileSize;
    
    TableStorage = CreatePTableArray(_MaxSnapshotCount + 1);
    CurrentTable = CreateTable();
    ReadonlyNodes = CreateTable();
    
    MapInterval(0, 0, VirtualFileSize);
    SnapshotZeroSave(0);
}

VOID DeinitLibrary()
{
    DeleteTable(CurrentTable);
    DeleteTable(ReadonlyNodes);
    DeletePTableArray(&TableStorage);
}
