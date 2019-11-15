#ifndef ENCRYPT_H
#define ENCRYPT_H

#define ONE_KB 1024
struct tagData;

struct tagData
{
    long long nSize;
    char      cKey[ONE_KB];
    char*     cBuffer;
};

typedef struct tagData Data;

#endif
