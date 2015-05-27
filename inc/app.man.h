#ifndef _APP_MAN_H
#define _APP_MAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib.avl.h>

void printTriplet(appTRIPLET* I);

char readByte(const char *fileName, appBLOCK A, BOOLEAN *ok);
void writeByte(const char *fileName, appBLOCK A, char Byte, BOOLEAN *ok);

void printFile(const char *fileName);
void readFile(appBLOCK A, appNUMBER k, const char *fileName);
void writeFile(appBLOCK A, appNUMBER k, char **Bytes, const char *fileName);

void printVirtualFile(const char *fileName, PRTL_AVL_TABLE Table);
void readVirtualFile(appBLOCK A, appNUMBER k, const char *fileName, PRTL_AVL_TABLE Table);
void writeVirtualFile(appBLOCK A, appNUMBER k, char **Bytes, const char *fileName, PRTL_AVL_TABLE Table);

#endif //_APP_MAN_H
