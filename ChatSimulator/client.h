#pragma once
#ifndef CLIENT_H
#define CLIENT_H
#include "includes.h"





int VerifyUniqueness(char* cUniqueMessage);
void SetRand(char* cBuf,int nSize);
std::string SuffixAppropirateUniqueIdentifier(std::string lcString,short nCommand);



tagBufferData ConvertToNetworkBuffer(tagData& stData);


tagData ConvertToDataStruct(tagBufferData& stData);



void* CheckResponse(void*);



void SetRand(char* cBuf,int nSize);


int VerifyUniqueness(char* cUniqueMessage);


void SetUniqueRand(char* cBuf,int nSize);



///ExecuteResponse
///the endpoint of  a message
///this will be the last place where a message would end/cause effect
int ExecuteResponse(tagData& stData);

#ifndef WIN32

//The job of this function is to erase the sender request when the final response has arrived
int MakeReSender(tagData lstRecvData );


//this func is located for SenderThread if this message is unique then it is added to resender store
bool IsMessageUnique(tagData stData);




void* SenderThread(void* pVData);





void* RecieverThread(void* pVData);



//This functions does the job of which message code to be sent next
//UDpChatServer 21/12/2018 Aditya M.:START
int PreSender(tagData& stData);


int PostSender(tagData& stData);

void HandleSignal(int nSignal);

//UDpChatServer 21/12/2018 Aditya M.:END
//int main(int argc,char* argv[])




std::string SuffixAppropirateUniqueIdentifier(std::string lcString,short nCommand);
int ClearResenderStore();

#endif

#endif
