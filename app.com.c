#include "app.a.h"

VOID InitCommandString()
{
    CommandString.InputLimit = 200;
    CommandString.comS = (char*)malloc(CommandString.InputLimit + 1);
    if (CommandString.comS == NULL)
    {
        exit(1);
    }
    CommandString.comV = split("");
}

VOID DeinitCommandString()
{
    freeArr(CommandString.comV);
    free(CommandString.comS);
}

VOID ReadCommandString()
{
    freeArr(CommandString.comV);
    fgets(CommandString.comS, CommandString.InputLimit, stdin);
    CommandString.comV = split(CommandString.comS);
    CommandString.comC = lengthArr(CommandString.comV);
}

APP_COMMAND CheckCommand(char *S)
{
    if (S == NULL)
        return COMMAND_EMPTY;
    if (strcmp(S, "cls") == 0)
        return COMMAND_CLS;
    if (strcmp(S, "help") == 0)
        return COMMAND_HELP;
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
    if (strcmp(S, "snapshot") == 0)
        return COMMAND_SNAPSHOT;
    
    return COMMAND_UNKNOWN;
}

VOID DoOnCommandHelp(int comC, char **comV)
{
    // printf("init - \n");
    printf("\thelp     - print this help message\n");
    printf("\tcheck    - show blocks of hard storage related to virtual one\n");
    printf("\tmap      - ralate blocks from virtual storage to hard one\n");
    printf("\tunmap    - delete ralation for specified blocks\n");
    printf("\tprint    - print storage out (virtual or hard)\n");
    printf("\tread     - read data from storage (virtual or hard)\n");
    printf("\twrite    - write data to storage (virtual or hard)\n");
    printf("\tsnapshot - manage backup copies of virtual storage\n");
    printf("\t\n");
    printf("\tType \"[command] help\" to see details\n");
}

VOID DoOnCommandInit(int comC, char **comV)
{
    return;
    // if(comC < 2 || strcmp(comV[1], "help") == 0){
        // printf("Usage: init ... \n");
        // return;
    // }
    
}

VOID DoOnCommandCheck(int comC, char **comV)
{
        BOOLEAN ok;
        int i;
        LIB_BLOCK A, k, I, I_prev;
        LIB_PNODE temp;
        LIB_PNODE_ARRAY Buffer;
        
        if ((comC != 2 && comC != 3)  || strcmp(comV[1], "help") == 0)
        {
            printf("Usage: check [block]\n");
            printf("       check [block] [amount]\n");
            return;
        }
        
        switch (comC)
        {
            case 2:
                if (!isDec(comV[1]))
                {
                    printf("Invalid block format!\n");
                    return;
                }
                
                A = str2dec(comV[1]);
                if (!ExistNode(A))
                {
                    printf("%d not mapped!\n", A);
                    return;
                }
                
                printf("%d -> %d\n", A, CheckBlock(A, &ok));
                return;

            case 3:
                if (!isDec(comV[1]) || !isDec(comV[2]))
                {
                    printf("Invalid block or amount format!\n");
                    return;
                }
                
                A = str2dec(comV[1]);
                k = str2dec(comV[2]);
                
                Buffer = CheckInterval(CurrentTable, A, k);
                I_prev = A;
                I = A;
                for (i = 0; i < Buffer.Count; i++)
                {
                    temp = Buffer.Data[i];
                    if (I_prev < temp->A)
                    {
                        printf("[%d, %d) not mapped!\n", I_prev, temp->A);
                        I = temp->A;
                    }
                    if (I > temp->A && A + k <=  temp->A + temp->k)
                    {
                        printf("[%d, %d) -> [%d, %d)\n", I, A + k, temp->B + I - temp->A, temp->B + (A + k) - temp->A);
                        break;
                    }
                    if (I > temp->A)
                    {
                        printf("[%d, %d) -> [%d, %d)\n", I, temp->A + temp->k, temp->B + I - temp->A, temp->B + temp->k);
                        I_prev = temp->A + temp->k;
                        continue;
                    }
                    if (A + k <=  temp->A + temp->k)
                    {
                        printf("[%d, %d) -> [%d, %d)\n", temp->A, A + k, temp->B, temp->B + (A + k) - temp->A);
                        break;
                    }
                    printf("[%d, %d) -> [%d, %d)\n", temp->A, temp->A + temp->k, temp->B, temp->B + temp->k);
                    I_prev = temp->A + temp->k;
                }
                if (Buffer.Count == 0)
                    printf("[%d, %d) not mapped!\n", A, A + k);
                if (i > 0 && (A + k > temp->A + temp->k))
                    printf("[%d, %d) not mapped!\n", temp->A + temp->k, A + k);
                DeletePNodeArray(&Buffer);
                
                return;
        }
}

VOID DoOnCommandMap(int comC, char **comV)
{
    int k;
    LIB_BLOCK A, B;
    
    if(comC != 4 || strcmp(comV[1], "help") == 0){
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
    MapInterval(A, B, k);
    printf("Mapped [%d, %d) -> [%d, %d)\n", A, A + k, B, B + k);
}

VOID DoOnCommandUnmap(int comC, char **comV)
{
    int k;
    LIB_BLOCK A;
    
    if(comC != 3 || strcmp(comV[1], "help") == 0){
        printf("Usage: unmap [block] [amount]\n");
        return;
    }
    
    if(!isDec(comV[1]) || !isDec(comV[2])){
        printf("Invalid block or amount format\n");
        return;
    }
    
    A = str2dec(comV[1]);
    k = str2dec(comV[2]);
    UnmapInterval(A, k);
    printf("Unmapped [%d, %d)\n", A, A + k);
}

VOID DoOnCommandPrint(int comC, char **comV)
{
    int ArgOffset;
    BOOLEAN ModeHard;
    
    // Checking args amount
    if ((comC > 2) || (comC >= 2 && strcmp(comV[1], "help") == 0))
    {
        printf("Usage: print [-disk]\n");
        printf("       disk - \"L\" (default) or \"H\"\n");
        return;
    }
    
    // Checking disk presence
    ArgOffset = 0;
    ModeHard = FALSE;
    
    if (comC > 1 && strcmp(comV[1], "L") != 0 && strcmp(comV[1], "H") != 0)
        ArgOffset++;
    else
    {
        if (comC > 1 && strcmp(comV[1], "H") == 0)
            ModeHard = TRUE;
    }
    
    // Printing
    if (ModeHard)
    {
        printFile(HardFileName);
        return;
    }
    
    printVirtualFile(HardFileName, CurrentTable);
}

VOID DoOnCommandRead(int comC, char **comV)
{
    int k, ArgOffset;
    LIB_BLOCK A;
    BOOLEAN ModeHard;
    
    // Checking args amount
    if((comC < 3 || comC > 4) || (comC >= 2 && strcmp(comV[1], "help") == 0))
    {
        printf("Usage: read [-disk] [block] [amount]\n");
        printf("       disk - \"L\" (default) or \"H\"\n");
        return;
    }
    
    // Checking disk presense
    ArgOffset = 0;
    ModeHard = FALSE;
    
    if (comC > 1 && strcmp(comV[1], "L") != 0 && strcmp(comV[1], "H") != 0)
        ArgOffset++;
    else
    {
        if (comC > 1 && strcmp(comV[1], "H") == 0)
            ModeHard = TRUE;
    }
    
    // Checking block format
    if(!isDec(comV[2 - ArgOffset]))
    {
        printf("Invalid block format\n");
        return;
    }
    
    // Checking amount format
    if(!isDec(comV[3 - ArgOffset]))
    {
        printf("Invalid amount format\n");
        return;
    }
    
    // Reading
    A = str2dec(comV[2 - ArgOffset]);
    k = str2dec(comV[3 - ArgOffset]);
    
    if (ModeHard)
    {
        readFile(A, k, HardFileName);
        return;
    }
    
    readVirtualFile(A, k, HardFileName, CurrentTable);
}

VOID DoOnCommandWrite(int comC, char **comV)
{
    int k, i, ArgOffset;
    LIB_BLOCK A;
    BOOLEAN DataIsCorrect, ModeHard;
    
    // Checking args amount
    if ((comC < 3) || (comC >= 2 && strcmp(comV[1], "help") == 0))
    {
        printf("Usage: write [-disk] [block] [byte 1] ... [byte N]\n");
        printf("       disk - \"L\" (default) or \"H\"\n");
        return;
    }
    
    // Checking disk presense
    ArgOffset = 0;
    ModeHard = FALSE;
    
    if (comC > 1 && strcmp(comV[1], "L") != 0 && strcmp(comV[1], "H") != 0)
        ArgOffset++;
    else
    {
        if(comC > 1 && strcmp(comV[1], "H") == 0)
            ModeHard = TRUE;
    }
    
    // Checking block format
    if(!isDec(comV[2 - ArgOffset]))
    {
        printf("Invalid block format\n");
        return;
    }
    
    // Checking bytes format
    DataIsCorrect = TRUE;
    for (i = 3; DataIsCorrect && i < comC; i++)
    {
        if(!isHex(comV[i - ArgOffset]))
        {
            DataIsCorrect = FALSE;
            break;
        }
    }
    
    if (!DataIsCorrect)
    {
        printf("Invalid byte format (#%d: %s)\n", i - 2, comV[i - ArgOffset]);
        return;
    }
    
    //Writing data
    A = str2dec(comV[2 - ArgOffset]);
    k = comC - (3 - ArgOffset);
    
    if (ModeHard)
    {
        writeFile(A, k, &comV[3 - ArgOffset], HardFileName);
        return;
    }
    
    writeVirtualFile(A, k, &comV[3 - ArgOffset], HardFileName, CurrentTable);
}

void DoOnCommandSnapshot(int comC, char **comV)
{
    int k;
    LIB_BLOCK A;
    BOOLEAN ok;
    LIB_PNODE elem;
    LIB_NODE_ARRAY Buffer;
    
    if (comC < 2 || strcmp(comV[1], "help") == 0)
    {
        printf("Usage: snapshot count\n");
        printf("       snapshot make\n");
        printf("       snapshot save [slot]\n");
        printf("       snapshot load [slot]\n");
        printf("       snapshot delete [slot]\n");
        return;
    }
    
    if (strcmp(comV[1], "count") == 0)
    {
        printf("%d/%d  snapshots recorded.\n", SnapshotCount(), SnapshotCapacity());
        return;
    }
    
    if (strcmp(comV[1], "make") == 0)
    {
        k = SnapshotMake();
        if (k == -2)
        {
            printf("Snapshot was not recorded! Slot storage is full.\n");
            return;
        }
        
        printf("Snapshot recorded to slot #%d\n", k);
        
        return;
    }
    
    if (strcmp(comV[1], "load") == 0)
    {
        if (comC < 3)
        {
            printf("Usage: snapshot load [slot]\n");
            return;
        }
            
        if(!isDec(comV[2]))
        {
            printf("Invalid slot number format!\n");
            return;
        }
        
        k = str2dec(comV[2]);
        ok = SnapshotLoad(k);
        
        if (!ok)
        {
            printf("Snapshot was not loaded!\n");
            return;
        }
        
        printf("Snapshot #%d loaded.\n", k);
        
        return;
    }
    
    if (strcmp(comV[1], "delete") == 0)
    {
        if (comC < 3)
        {
            printf("Usage: snapshot delete [slot]\n");
            return;
        }
            
        if(!isDec(comV[2]))
        {
            printf("Invalid slot number format!\n");
            return;
        }
        
        k = str2dec(comV[2]);
        ok = SnapshotDelete(k);
        
        if (!ok)
        {
            printf("Snapshot was not deleted!\n");
            return;
        }
        
        printf("Snapshot #%d deleted.\n", k);
        
        return;
    }
    
    if (strcmp(comV[1], "save") == 0)
    {
        if (comC < 3)
        {
            printf("Usage: snapshot save [slot]\n");
            return;
        }
            
        if(!isDec(comV[2]))
        {
            printf("Invalid slot number format!\n");
            return;
        }
        
        k = str2dec(comV[2]);
        ok = SnapshotSave(k);
        
        if (!ok)
        {
            printf("Snapshot was not saved! Slot number is out of range.\n");
            return;
        }
        
        printf("Snapshot saved to slot #%d.\n", k);
        
        return;
    }
    
    if (strcmp(comV[1], "av") == 0)
    {
        if (comC < 3)
        {
            printf("Usage: snapshot av [amount]\n");
            return;
        }
            
        if(!isDec(comV[2]))
        {
            printf("Invalid amount format!\n");
            return;
        }
        
        k = str2dec(comV[2]);
        Buffer = FindAvailable(k, PhysicalFileSize);
        
        for (k = 0; k < Buffer.Count; k++)
        {
            printf("<%d, %d>\n", Buffer.Data[k].A, Buffer.Data[k].A + Buffer.Data[k].k);
        }
        return;
    }
    
    if (strcmp(comV[1], "rop") == 0)
    {
        elem = (LIB_NODE*)RtlEnumerateGenericTableAvl(ReadonlyNodes, TRUE);
        while (elem != NULL)
        {
            printf("<%d, %d> :", elem->A, elem->A + elem->k);
            for (k = 0; k < elem->UsedBy->Capacity; k++)
            {
                if (GetBitValue(elem->UsedBy, k))
                    printf(" #%d", k);
            }
            printf("\n");
            elem = (LIB_PNODE)RtlEnumerateGenericTableAvl(ReadonlyNodes, FALSE);
        }
        return;
    }
    
    if (strcmp(comV[1], "p") == 0)
    {
        if (comC < 4)
        {
            printf("Usage: snapshot p [block] [length]\n");
            return;
        }
        
        if(!isDec(comV[2]))
        {
            printf("Invalid block format!\n");
            return;
        }
        
        if(!isDec(comV[3]))
        {
            printf("Invalid length format!\n");
            return;
        }
        
        A = str2dec(comV[2]);
        k = str2dec(comV[3]);
        
        ok = PrepareToWrite(A, k);
        
        if (ok)
            printf("Redirected.\n");
        else
            printf("Unable to redirect! Not enough available space.\n");
        
        return;
    }
    
    printf("Unknown args\n");
}
