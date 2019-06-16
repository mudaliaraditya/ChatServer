#pragma once
//#include<iostream>
//#include<string>
//#include<string.h>
//#include<map>
//#include<fstream>
//using namespace std;
//map<string,string> g_cMap;
//map< string,map < string,string > > g_cBigMap;
#ifdef __cplusplus
extern "C" {
#endif

void*   CreateNewMap();


void DeleteNewMap(void* pMap);


char* GetValueForKey(char* cKey,char* cFileName , void* pcVMap);



//int AddTokenToStore(string cString,map<string,string>& cMap,map< string,map < string,string > >* pcBigMap);


int  GetConfig(char* cFileName,void* pMap);

int DeleteKeyVal(char* pVal);
#ifdef __cplusplus
}
#endif

