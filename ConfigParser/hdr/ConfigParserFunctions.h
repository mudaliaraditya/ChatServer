#pragma once


struct ConfigParserHandle;
typedef struct ConfigParserHandle ConfigParserHandle;




#ifdef __cplusplus


extern "C" {




#endif

/*
ReadMe

Simple Way to use this would be
1)call CreateNewMap to create a ConfigParserMapObj
2)Pass the map object and filename to function GetConfig,this will load the config from file to the internal map
3)to get Value from the Config file call the GetValueForKey function either call free on the returned variable after use or call DeleteKeyVal
4)just before exiting call DeleteNewMap on the ConfigParserMapObj
*/



void*   CreateNewMap();//Create a Handler
int  GetConfig(char* cFileName,void* pMap);//Pass The Handle and File name
char* GetValueForKey(char* cKey,char* cFileName , void* pcVMap);
int DeleteKeyVal(char* pVal);
void DeleteNewMap(void* pMap);
//int AddTokenToStore(string cString,map<string,string>& cMap,map< string,map < string,string > >* pcBigMap);//Dont Call this directly not threadsafe



#ifdef __cplusplus
}
#endif

