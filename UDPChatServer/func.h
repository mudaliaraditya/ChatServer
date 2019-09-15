#pragma once

#include<string>
#include "structs.h"
#include "defines.h"


std::string SuffixAppropirateUniqueIdentifier(std::string lcString,short nCommand);
tagBufferData ConvertToNetworkBuffer(tagData& stData);
tagData ConvertToDataStruct(tagBufferData& stData);
////////////////////FINIALIZINGyyyy FUNCTIONS/////////////////////////
int CleanUp();
////////////////////FINIALIZINGyyyy1 FUNCTIONS/////////////////////////
void HandleSignal(int nSignal); 
int ExecuteFunction(tagData& stData);
int GetResponseForFunction(tagData& stData);
#ifndef WIN32
void* RecieverThread(void*);
void* EventThread(void*);
int DeleteMsgFromResenderStoreByUniqueIdentifier(const tagData lstRecvData );
bool IsMessageUniqueSoAddToResenderStore(tagData stData);
void* EventHandling(void* pEventHandlingArg);
int RejectDummyMsgCode(long long nMessageCode);
void* ProcessThread(void* pArg);
void* SenderThread(void* pArg);






std::string GetValueForKey(std::string cKey);
int AddTokenToStore(std::string cString);
int  GetConfig(std::string cFileName);



#endif

#ifndef WIN32
int main();

#endif



