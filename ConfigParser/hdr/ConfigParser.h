#pragma once
#include<string>
#include<cstring>
#include<map>
#include <errno.h>
#include<fstream>
#include<malloc.h>
#include <thread>
#include <mutex>

extern std::mutex g_i_ConfigParser_mutex;

#ifdef __cplusplus
extern "C" {
#endif


void*   CreateNewMap();


void DeleteNewMap(void* pMap);


char* GetValueForKey(char* cKey,char* cFileName , void* pcVMap);





int  GetConfig(char* cFileName,void* pMap);

int DeleteKeyVal(char* pVal);
#ifdef __cplusplus
}
#endif

