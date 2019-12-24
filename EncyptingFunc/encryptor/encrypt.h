#ifndef ENCRYPT_H
#define ENCRYPT_H
#ifdef _cplusplus
extern "C" {
#endif
#include <stdlib.h>

#define ONE_KB 1024
struct tagBuffer;

void* dofunc(void* pvData);

struct tagBuffer
{
    long long nSize;
    char      cKey[ONE_KB];
    char*     cBuffer;
};

typedef struct tagBuffer Data;

#ifdef _cplusplus
}
#endif

#endif
