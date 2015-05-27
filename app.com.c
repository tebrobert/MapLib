#include "app.convstr.h"
#include "app.com.h"
#include "app.man.h"
#include "lib.avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <ntddk.h>

VOID InitCommandString(char **comS, int *InputLimit, int *comC, char ***comV)
{
    *InputLimit = 200;
    *comS = (char*)malloc(*InputLimit + 1);
    
    if (*comS == NULL)
        exit(1);
    
    *comV = split("");
}

VOID DeinitCommandString(char **comS, int *InputLimit, int *comC, char ***comV)
{
    freeArr(*comV);
    free(*comS);
}

VOID ReadCommandString(char **comS, int *InputLimit, int *comC, char ***comV)
{
    freeArr(*comV);
    fgets(*comS, *InputLimit, stdin);
    *comV = split(*comS);
    *comC = lengthArr(*comV);
}

APP_COMMAND CheckCommand(char *S)
{
    if (S == NULL)
        return COMMAND_EMPTY;
    if (strcmp(S, "exit") == 0)
        return COMMAND_EXIT;
    
    if (strcmp(S, "init") == 0)
        return COMMAND_INIT;
    
    if (strcmp(S, "check") == 0)
        return COMMAND_CHECK;
    if (strcmp(S, "map") == 0)
        return COMMAND_MAP;
    if (strcmp(S, "unmap") == 0)
        return COMMAND_UNMAP;
    
    if (strcmp(S, "print") == 0)
        return COMMAND_PRINT;
    if (strcmp(S, "read") == 0)
        return COMMAND_READ;
    if (strcmp(S, "write") == 0)
        return COMMAND_WRITE;
    
    return COMMAND_UNKNOWN;
}

VOID DoOnCommandInit(int comC, char **comV)
{
    printf("init\n");
}

VOID DoOnCommandCheck(int comC, char **comV)
{
        BOOLEAN ok;
        appBLOCK A, B;
        
        if (comC != 2 && comC != 3)
        {
            printf("Usage: check [block]\n");
            // printf("       check [block] [amount]\n");
            return;
        }
        
        switch (comC)
        {
            case 2:
                if(!isDec(comV[1])){
                    printf("Invalid block format!\n\n");
                    return;
                }
                
                A = str2dec(comV[1]);
                if(!existNode(A))
                {
                    printf("%d not mapped!\n\n", A);
                    return;
                }
                
                printf("%d -> %d\n\n", A, checkNode(A, &ok));
                return;

            case 3:
                if(!isDec(comV[1]) || !isDec(comV[2])){
                    printf("Invalid block or amount format!\n\n");
                    return;
                }
                
                /*
                A = str2dec(comV[1]);
                k = str2dec(comV[2]);
                Buffer = checkArrNode(A, k);
                if(Buffer == NULL) printf("Some blocks not mapped!\n\n");
                else {
                    nullCount = 0;
                    for(i = 0; nullCount < 3; i++) {
                        if(Buffer[i] != NULL && (nullCount == 0 || nullCount == 2)){
                            printTriplet(Buffer[i]);
                            //free(Buffer[i]);
                        } else
                        if(Buffer[i] == NULL) nullCount++;
                        else printTriplet(Buffer[i]);
                    }
                    printf("\n");
                    // free(Buffer);
                }
                */
                return;
        }
}

VOID DoOnCommandMap(int comC, char **comV)
{
    int k;
    appBLOCK A, B;
    
    if(comC != 4){
        printf("Usage: map [block] [block] [amount]\n");
        return;
    }
    
    if(!isDec(comV[1]) || !isDec(comV[2]) || !isDec(comV[3])){
        printf("Invalid block or amount format\n");
        return;
    }
    
    A = str2dec(comV[1]);
    B = str2dec(comV[2]);
    k = str2dec(comV[3]);
    mapNode(A, B, k);
    printf("Mapped [%d, %d) -> [%d, %d)\n\n", A, A + k, B, B + k);
}

VOID DoOnCommandUnmap(int comC, char **comV)
{
    int k;
    appBLOCK A;
    
    if(comC != 3){
        printf("Usage: unmap [block] [amount]\n\n");
        return;
    }
    
    if(!isDec(comV[1]) || !isDec(comV[2])){
        printf("Invalid block or amount format\n\n");
        return;
    }
    
    A = str2dec(comV[1]);
    k = str2dec(comV[2]);
    unmapNode(A, k);
    printf("Unmapped [%d, %d)\n\n", A, A + k);
}

VOID DoOnCommandPrint(int comC, char **comV)
{
    if (comC != 2){
        printf("Usage: print [disk]\n\n");
        return;
    }
    
    if (strcmp(comV[1], "H") == 0){
        printFile("hard");
        return;
    }
    
    if (strcmp(comV[1], "L") == 0){
        printVirtualFile("hard", Table);
        return;
    }
    
    printf("Invalid disk format (use L or H)\n\n");
}

VOID DoOnCommandRead(int comC, char **comV)
{
    int k;
    appBLOCK A;
    BOOLEAN ModeHard = FALSE;
    
    if(comC != 4){
        printf("Usage: read [disk] [block] [amount]\n\n");
        return;
    }
    
    if (strcmp(comV[1], "H") == 0)
        ModeHard = TRUE;
    
    if(!ModeHard && strcmp(comV[1], "L") != 0){
        printf("Invalid disk format (use L or H)\n\n");
        return;
    }
    
    if(!isDec(comV[2]) || !isDec(comV[3])){
        printf("Invalid block or amount format\n\n");
        return;
    }
    
    A = str2dec(comV[2]);
    k = str2dec(comV[3]);
    
    if (ModeHard)
    {
        readFile(A, k, "hard");
        return;
    }
    
    readVirtualFile(A, k, "hard", Table);
}

VOID DoOnCommandWrite(int comC, char **comV)
{
    int k, i;
    appBLOCK A;
    BOOLEAN DataIsCorrect = TRUE, ModeHard = FALSE;
    
    if(comC < 4){
        printf("Usage: write [disk] [block] [byte 1] ... [byte N]\n\n");
        return;
    }
    
    if(strcmp(comV[1], "H") == 0)
        ModeHard = TRUE;
    
    if(!ModeHard && strcmp(comV[1], "L") != 0){
        printf("Invalid disk format\n\n");
        return;
    }
    
    if(!isDec(comV[2])){
        printf("Invalid block format\n\n");
        return;
    }
    
    for(i = 3; DataIsCorrect && i < comC; i++)
    {
        if(!isHex(comV[i]))
            DataIsCorrect = FALSE;
    }
    
    if(!DataIsCorrect){
        printf("Invalid byte format\n\n");
        return;
    }
    
    A = str2dec(comV[2]);
    k = comC - 3;
    
    if(ModeHard)
    {
        writeFile(A, k, &comV[3], "hard");
        return;
    }
    
    writeVirtualFile(A, k, &comV[3], "hard", Table);
}
