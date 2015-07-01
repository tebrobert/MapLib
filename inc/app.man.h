#ifndef _APP_MAN_H
#define _APP_MAN_H


void printTriplet(LIB_TRIPLET* I);

char readByte(const char *fileName, LIB_BLOCK A, BOOLEAN *ok);
void writeByte(const char *fileName, LIB_BLOCK A, char Byte, BOOLEAN *ok);

void printFile(const char *fileName);
void readFile(LIB_BLOCK A, LIB_NUMBER k, const char *fileName);
void writeFile(LIB_BLOCK A, LIB_NUMBER k, char **Bytes, const char *fileName);

void printVirtualFile(const char *fileName, LIB_PTABLE Table);
void readVirtualFile(LIB_BLOCK A, LIB_NUMBER k, const char *fileName, LIB_PTABLE Table);
void writeVirtualFile(LIB_BLOCK A, LIB_NUMBER k, char **Bytes, const char *fileName, LIB_PTABLE Table);

#endif //_APP_MAN_H
