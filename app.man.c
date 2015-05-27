#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app.convstr.h"
#include "lib.avl.h"

void printTriplet(appTRIPLET* I){
    printf("[%d, %d)->[%d, %d)\n",
        I->A, I->A + I->k,
        I->B, I->B + I->k
    );
}

void
printFile(const char *fileNeme){
    //Start vars
    ULONG i, fileSize;
    void *buffer;
    size_t result;
    FILE *filePtr;
    char *S;
    
    //Open file
    filePtr = fopen(fileNeme, "r+b");
    if(filePtr == NULL){
        printf("Can't open file!\n\n");
        return;
    }
    fseek(filePtr , 0, SEEK_END);
    fileSize = ftell(filePtr);
    if(fileSize == -1){
        printf("Can't detect file size!\n\n");
        return;
    }
    rewind(filePtr);
    
    //Allocate memory for file
    buffer = malloc(sizeof(char)*fileSize);
    if(buffer == NULL){
        printf("Can't allocate memory for file!\n\n");
        return;
    }

    //Read file
    result = fread(buffer, sizeof(char), fileSize, filePtr);
    if(result != fileSize){
        printf("Can't read file!\n\n");
        return;
    }
    
    //Print file
    printf("%s is %lld bytes:\n", fileNeme, fileSize);
    for(i = 0; i < fileSize; i++){
        printf("%02X ", ((unsigned char*)buffer)[i]);
        if(i % 16 == 7) printf("  ");
        if(i % 16 == 15) printf("\n");
    }
    i--;
    if(i % 16 != 15) printf("\n");
    printf("\n");
    
    //Close file
    fclose(filePtr);
}

void
readFile(appBLOCK A, appNUMBER k, const char *fileName)
{
    FILE    *filePtr;
    ULONG   i, fileSize;
    void    *Buf;
    
    filePtr = fopen(fileName, "rb");
    if (filePtr == NULL){
        printf("Can't read file!\n\n");
        return;
    }
    fseek(filePtr , 0, SEEK_END);
    fileSize = ftell(filePtr);
    rewind(filePtr);
    
    if (A + k > fileSize)
    {
        printf("Can't read out of file!\n\n");
        fclose(filePtr);
        return;
    }
    
    Buf = malloc(k);
    
    
    fseek(filePtr, A, 0);
    fread(Buf, 1, k, filePtr);
    
    for(i = 0; i < k; i++)
    {
        printf(hex2str(((unsigned char*)Buf)[i]));
        printf(" ");
    }
    printf("\n\n");
    
    free(Buf);
    fclose(filePtr);
}

void
writeFile(appBLOCK A, appNUMBER k, char **Bytes, const char *fileName)
{
    FILE    *filePtr;
    ULONG   i, fileSize;
    void    *Buf;
    char    *temp;
        
    //Open file
    filePtr = fopen(fileName, "r+b");
    if (filePtr == NULL)
    {
        printf("Can't open file!\n\n");
        return;
    }
    fseek(filePtr , 0, SEEK_END);
    fileSize = ftell(filePtr);
    rewind(filePtr);
    
    //Write file
    if (A + k > fileSize)
    {
        printf("Can't write out of file!\n\n");
        fclose(filePtr);
        return;
    }
    Buf = malloc(k);
    for(i = 0; i < k; i++)
    {
        ((unsigned char*)Buf)[i] = str2hex(Bytes[i]);
    }
    fseek(filePtr, A, 0);
    i = fwrite(Buf, 1, k, filePtr);
    
    //Check writing file
    if(i != k) printf("Error - written %d of %d bytes!\n\n", i, k);
    else printf("Write success!\n\n");
    
    free(Buf);
    fclose(filePtr);
}



char
readByte(const char *fileName, appBLOCK A, BOOLEAN *ok)
{
    //Start vars
    ULONG i, fileSize;
    char   buffer;
    size_t result;
    FILE *filePtr;
    
    //Open file
    filePtr = fopen(fileName, "r+b");
    if(filePtr == NULL)
    {
        printf("Can't open file!\n\n");
        *ok = FALSE;
        return (char)NULL;
    }
    fseek(filePtr , 0, SEEK_END);
    fileSize = ftell(filePtr);
    rewind(filePtr);
    
    //Check
    if(A >= fileSize)
    {
        printf("Can't read out of file!\n\n");
        *ok = FALSE;
        fclose(filePtr);
        return (char)NULL;
    }
    
    //Read byte
    fseek(filePtr , A, SEEK_SET);
    result = fread(&buffer, sizeof(char), 1, filePtr);
    if (result != 1)
    {
        printf("Can't read byte!\n\n");
        *ok = FALSE;
        fclose(filePtr);
        return (char)NULL;
    }
    
    //Close file
    fclose(filePtr);
    *ok = TRUE;
    return buffer;
}



void
writeByte(const char *fileName, appBLOCK A, char Byte, BOOLEAN *ok)
{
    FILE    *filePtr;
    ULONG   i, fileSize;
    void    *Buf;
    char    *temp;
        
    //Open file
    filePtr = fopen(fileName, "r+b");
    if (filePtr == NULL)
    {
        printf("Can't open file!\n\n");
        *ok = FALSE;
        return;
    }
    fseek(filePtr , 0, SEEK_END);
    fileSize = ftell(filePtr);
    rewind(filePtr);
    
    //Write file
    if (A >= fileSize)
    {
        printf("Can't write out of file!\n\n");
        fclose(filePtr);
        *ok = FALSE;
        return;
    }
    fseek(filePtr, A, SEEK_SET);
    i = fwrite(&Byte, sizeof(char), 1, filePtr);
    
    //Check writing file
    if(i != 1)
    {
        printf("Can't write byte!\n\n");
        *ok = FALSE;
    }
    else
    {
        *ok = TRUE;
    }
    fclose(filePtr);
}




void
printVirtualFile(const char *fileName, PRTL_AVL_TABLE Table)
{
    //Start vars
    ULONG i;
    char *S, Byte;
    appBLOCK B;
    BOOLEAN ok;
    int fileSize = 10;
    
    //Print file
    printf("virtual is %lld bytes:\n", fileSize);
    for(i = 0; i < 10; i++)
    {
        B = checkNode(i, &ok);
        if(ok)
        {
            Byte = readByte(fileName, B, &ok);
            S = hex2str(Byte);
            printf("%s ", S);
            free(S);
        }
        else
        {
            printf("xx ");
        }
        if(i % 16 == 7) printf("  ");
        if(i % 16 == 15) printf("\n");
    }
    i--;
    if(i % 16 != 15)
    {
        printf("\n");
    }
    printf("\n");
}

void
readVirtualFile(appBLOCK A, appNUMBER k, const char *fileName, PRTL_AVL_TABLE Table)
{
    //Start vars
    ULONG i;
    char *S, Byte;
    appBLOCK B;
    BOOLEAN ok;
    int fileSize = 10;
    
    //Read file
    for(i = 0; i < k; i++)
    {
        B = checkNode(A + i, &ok);
        if(ok)
        {
            Byte = readByte(fileName, B, &ok);
            S = hex2str(Byte);
            printf("%s ", S);
            free(S);
        }
        else
        {
            printf("xx ");
        }
    }
    printf("\n");
}

void
writeVirtualFile(appBLOCK A, appNUMBER k, char **Bytes, const char *fileName, PRTL_AVL_TABLE Table)
{
    //Start vars
    ULONG i, fileSize = 10;
    char *S, Byte;
    appBLOCK B;
    BOOLEAN ok;
    
    //Write file
    for(i = 0; i < k; i++)
    {
        B = checkNode(A + i, &ok);
        if(ok)
        {
            writeByte(fileName, B, str2hex(Bytes[i]), &ok);
        }
    }
    if(!ok) printf("Can't write bytes\n\n");
    else printf("Write success!\n\n");
}
