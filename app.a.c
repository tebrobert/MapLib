#include "app.a.h"

int main()
{
    int i, j;
    char c;
    BOOLEAN Exit;
    APP_COMMAND FirstCommand;
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
    LIB_MODE _LibraryMode;
    ULONG _LogicalFileSize, _PhysicalFileSize;
    int _MaxSlotCount;
    
    HardFileName = "hard";
    _LibraryMode = USER_MODE;
    _LogicalFileSize = 10;
    _PhysicalFileSize = GetFileSize(HardFileName);
    _MaxSlotCount = 10;
    InitLibrary(_LibraryMode, _LogicalFileSize, _PhysicalFileSize, _MaxSlotCount);
    
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

/*
 *  default mapping [0 10) [0 10)
 *  
 *  snapshot make                   L[1]: [0 10)
 *  
 *  read
 *  write [10 15]
 *  
 *  snapshot make                   L[2]: [0 15)
 *  
 */


/*
 *  
 *  L[0]: avl[(...........)]
 *  L[1]: avl[(.)..........]
 *  L[2]: avl[(.)(.).......]
 *  L[3]: avl[(.)(.).......]
 *  
 *  H:    avl[(.)(.)(.....)............(.)(.)..(.)(.)............]
 *             0  0  0                  1  2    3  3
 *                1  1                  2
 *                   2
 *                   3
 *  
 *  
 *  L[0]: avl[(...........)]
 *  L[3]: avl[(.)(.).......]
 *  
 *  H:    avl[(.)(.)(.....)............(.)(.)..(.)(.)............]
 *             0  0  0                          3  3
 *                   3                   
 *                    
 *                    
 *  
 */
