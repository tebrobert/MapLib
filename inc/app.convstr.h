#ifndef _APP_CONVSTR_H
#define _APP_CONVSTR_H

#include <stdlib.h>
#include <string.h>
#include <ntddk.h>

BOOLEAN isDec(char *S);
BOOLEAN isHex(char *S);

unsigned char str2hexSingle(char S);
char hex2srtSingle(unsigned char C);
unsigned char str2hex(char* S);
char* hex2str(unsigned char C);
unsigned int str2dec(char* S);
char* dec2str(unsigned int N);
char** split(char* S);

void freeArr(void **SArr);
char lengthArr(char **SArr);

#endif //_APP_CONVSTR_H
