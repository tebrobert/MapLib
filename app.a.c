#include "app.a.h"

int main()
{
    VOID (*DoOnCommand[])(int, char**) =
    {
        DoOnCommandHelp,
        DoOnCommandInit,
        DoOnCommandCheck,
        DoOnCommandMap,
        DoOnCommandUnmap,
        DoOnCommandPrint,
        DoOnCommandRead,
        DoOnCommandWrite,
        DoOnCommandSnapshot,
    };
    BOOLEAN Exit;
    APP_COMMAND FirstCommand;
    LIB_MODE _LibraryMode;
    ULONG _VirtualFileSize, _PhysicalFileSize;
    int _MaxSlotCount;
    
    HardFileName = "hard";
    
    //_LogicalFileSize and _MaxSlotCount are random
    _LibraryMode = USER_MODE;
    _PhysicalFileSize = GetFileSize(HardFileName);
    _VirtualFileSize = 10;
    _MaxSlotCount = 10;
    InitLibrary(_LibraryMode, _VirtualFileSize, _PhysicalFileSize, _MaxSlotCount);
    
    InitCommandString();
    
    Exit = FALSE;
    while (!Exit)
    {
        printf(">");
        
        ReadCommandString();
        FirstCommand = CheckCommand(CommandString.comV[0]);
        
        switch (FirstCommand)
        {
            case COMMAND_EMPTY:
                break;
            case COMMAND_UNKNOWN:
                printf("No such command\n");
                break;
            case COMMAND_EXIT:
                Exit = TRUE;
                break;
            case COMMAND_CLS:
                system("cls");
                continue;
            default:
                DoOnCommand[FirstCommand](CommandString.comC, CommandString.comV);
                break;
        }
        
        printf("\n");
    }
    
    DeinitCommandString();
    DeinitLibrary();
    
    return 0;
}
