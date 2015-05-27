#include "app.convstr.h"
#include "app.com.h"
#include "app.man.h"
#include "lib.avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <ntddk.h>

int main()
{
    int InputLimit;
    int comC;
    char *comS, **comV;
    BOOLEAN exit = FALSE;
    APP_COMMAND FirstCommand;
    VOID (*DoOnCommand[])(int, char**) =
    {
        DoOnCommandInit,
        DoOnCommandCheck,
        DoOnCommandMap,
        DoOnCommandUnmap,
        DoOnCommandPrint,
        DoOnCommandRead,
        DoOnCommandWrite,
    };
    
    InitLibrary(TRUE);
    
    InitCommandString(&comS, &InputLimit, &comC, &comV);
    while (!exit)
    {
        printf(">");
        
        ReadCommandString(&comS, &InputLimit, &comC, &comV);
        FirstCommand = CheckCommand(comV[0]);
        
        switch (FirstCommand)
        {
            case COMMAND_EMPTY:
                break;
            case COMMAND_UNKNOWN:
                printf("No such command\n");
                break;
            case COMMAND_EXIT:
                exit = TRUE;
                break;
            default:
                DoOnCommand[FirstCommand](comC, comV);
                break;
        }
        
        printf("\n");
    }
    DeinitCommandString(&comS, &InputLimit, &comC, &comV);
    
    DeinitLibrary();
    
    return 0;
}
