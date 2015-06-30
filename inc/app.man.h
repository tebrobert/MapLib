#ifndef _APP_MAN_H
#define _APP_MAN_H


void printTriplet(appTRIPLET* I);

char readByte(const char *fileName, appBLOCK A, BOOLEAN *ok);
void writeByte(const char *fileName, appBLOCK A, char Byte, BOOLEAN *ok);

void printFile(const char *fileName);
void readFile(appBLOCK A, appNUMBER k, const char *fileName);
void writeFile(appBLOCK A, appNUMBER k, char **Bytes, const char *fileName);

void printVirtualFile(const char *fileName, LIB_PTABLE Table);
void readVirtualFile(appBLOCK A, appNUMBER k, const char *fileName, LIB_PTABLE Table);
void writeVirtualFile(appBLOCK A, appNUMBER k, char **Bytes, const char *fileName, LIB_PTABLE Table);

#endif //_APP_MAN_H
