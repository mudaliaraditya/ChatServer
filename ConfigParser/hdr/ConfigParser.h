#pragma once
#include<string>
#include<cstring>
#include<map>
#include <errno.h>
#include<fstream>
#include<malloc.h>
#ifdef __cplusplus
extern "C" {
#endif

void*   CreateNewMap();


void DeleteNewMap(void* pMap);


char* GetValueForKey(char* cKey,char* cFileName , void* pcVMap);




int AddTokenToStore(std::string cString,std::map<std::string,std::string>& cMap, std::map< std::string,std::map < std::string,std::string > >* pcBigMap);

int  GetConfig(char* cFileName,void* pMap);

int DeleteKeyVal(char* pVal);
#ifdef __cplusplus
}
#endif

