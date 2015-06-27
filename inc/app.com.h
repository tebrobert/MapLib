#ifndef _APP_COM_H
#define _APP_COM_H

#include <ntddk.h>

typedef struct APP_COMMAND_STRING_
{
    int InputLimit;
    int comC;
    char *comS;
    char **comV;
}
APP_COMMAND_STRING;

typedef enum
{
    COMMAND_CLS     = -4,
    COMMAND_EXIT    = -3,
    COMMAND_EMPTY   = -2,
    COMMAND_UNKNOWN = -1,
    
    COMMAND_HELP    = 0,
    COMMAND_INIT    = 1,
    COMMAND_CHECK   = 2,
    COMMAND_MAP     = 3,
    COMMAND_UNMAP   = 4,
    COMMAND_PRINT   = 5,
    COMMAND_READ    = 6,
    COMMAND_WRITE   = 7,
    COMMAND_SNAPSHOT= 8,
}
APP_COMMAND;

APP_COMMAND_STRING CommandString;

VOID InitCommandString();
VOID DeinitCommandString();
VOID ReadCommandString();

APP_COMMAND CheckCommand(char *S);

VOID DoOnCommandHelp(int comC, char **comV);

VOID DoOnCommandInit(int comC, char **comV);

VOID DoOnCommandCheck(int comC, char **comV);
VOID DoOnCommandMap(int comC, char **comV);
VOID DoOnCommandUnmap(int comC, char **comV);

VOID DoOnCommandPrint(int comC, char **comV);
VOID DoOnCommandRead(int comC, char **comV);
VOID DoOnCommandWrite(int comC, char **comV);

VOID DoOnCommandSnapshot(int comC, char **comV);


#endif //_APP_COM_H
