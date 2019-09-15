#include "includes.h"

using namespace std;




int VerifyUniqueness(char* cUniqueMessage);
void SetRand(char* cBuf,int nSize);
string SuffixAppropirateUniqueIdentifier(string lcString,short nCommand);



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

///CreateUDPSocketIP
///Creates a Simple UDP Socket
int CreateUDPSocketIP();



void FillSockAddrin(long sin_family, unsigned short int sin_port, long long sin_addr, sockaddr_in* sockaddrin);



int SendUDPData(int nSockFD, const void* cData, size_t nSize, const struct sockaddr_in* pstSockAddr, long nSockAddrLen);

int RecvUDPData(int nSockFD, void* cData, size_t nSize, sockaddr_in* pstSockAddr, long pnSockAddrLen);


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




string SuffixAppropirateUniqueIdentifier(string lcString,short nCommand);


#endif
