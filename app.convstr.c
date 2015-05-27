#include <stdlib.h>
#include <string.h>
#include <ntddk.h>
#include <stdio.h>/////////////

BOOLEAN isDec(char *S){
    int i;
    for(i = 0; S[i] != '\0'; i++){
        if(S[i] < '0' || S[i] > '9') return FALSE;
    }
    return TRUE;
}

BOOLEAN isHex(char *S){
    int L = strnlen(S, 4);
    if(L != 2){
        return FALSE;
    }
    if((S[0] < '0' || S[0] > '9') && (S[0] < 'A' || S[0] > 'F')){
        return FALSE;
    }
    if((S[1] < '0' || S[1] > '9') && (S[1] < 'A' || S[1] > 'F')){
        return FALSE;
    }
    return TRUE;
}

unsigned char str2hexSingle(char S){
    if ((S >= '0') && (S <= '9')) return S - '0';
    if ((S >= 'A') && (S <= 'F')) return S - 'A' + 0xA;
    return 0;
}

char hex2srtSingle(unsigned char C){
    if ((C >= 0  ) && (C <= 9  )) return C + '0';
    if ((C >= 0xA) && (C <= 0xF)) return C + 'A' - 0xA;
    return '0';
}

unsigned char str2hex(char* S){
    unsigned char result;
    const int MAX_HEX_STR_LEN = 2;
    int len = strnlen(S, MAX_HEX_STR_LEN);
    result = str2hexSingle(S[0]);
    if(len > 1){
        result *= 0x10;
        result += str2hexSingle(S[1]);
    }
    return result;
}

char* hex2str(unsigned char C){
    char *S = (char*)malloc(3);
    S[0] = hex2srtSingle(C / 0x10);
    S[1] = hex2srtSingle(C % 0x10);
    S[2] = 0;
    return S;
}

unsigned int str2dec(char* S){
    unsigned int result;
    const int MAX_INT_STR_LEN = 10;
    int len = strnlen(S, MAX_INT_STR_LEN);
    int i;
    result = 0;
    for(i = 0; i < len; i++){
        result *= 10;
        result += S[i] - '0';
    }
    return result;
}

char* dec2str(unsigned int N){
    int i, j, k;
    char *S;
    if(N == 0){
        S = (char*)malloc(2);
        S[0] = '0';
        S[1] = 0;
        return S;
    }
    j = 1;
    k = N;
    for(i = 0; k / 10 > 0; i++){
        j *= 10;
        k /= 10;
    }
    S = (char*)malloc(i + 1);
    for(i = 0; j > 0; j /= 10, i++){
        S[i] = (N / j) % 10 + '0';
    }
    S[i] = 0;
    return S;
}

char** split(char* S){
    int i, j, N = 0, w = 0;
    char **SArr;
    for(i = 0; S[i] != '\0'; i++){
        if (S[i] == '\n'){
            S[i] = '\0';
            continue;
        }
        if (S[i] != ' ' && w == 0){
            N++;
            w = 1;
            continue;
        }
        if (S[i] == ' ' && w == 1){
            w = 0;
            continue;
        }
    }
    SArr = (char**)malloc(sizeof(char*) * (N + 1));
    SArr[N] = NULL;
    N = 0;
    for(i = 0; S[i] != '\0'; i++){
        if (S[i] == ' ') continue;
        for(w = 0; S[i + w] != ' ' && S[i + w] != '\0'; w++);
        SArr[N] = malloc(w);
        for(j = 0; j < w; j++){
            SArr[N][j] = S[i + j];
        }
        SArr[N][w] = '\0';
        N++;
        i = i + w - 1;
    }
    return SArr;
}

void freeArr(void **SArr){
    int i = 0;
    for(i = 0; SArr[i] != NULL; i++){
        // printf("%d[1]\n",i);
        // printf("SArr[%d] = %x\n", i, SArr[i]);
        free(SArr[i]);
        // printf("%d[2]\n",i);
    }
    // printf("[1]\n");
    // printf("SArr = %x\n", SArr);
    free(SArr);
    // printf("[2]\n");
}

char lengthArr(char **SArr) {
    char i;
    for(i = 0; SArr[i] != NULL; i++);
    return i;
}
