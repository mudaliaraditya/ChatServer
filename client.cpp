#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <iostream>
#include <memory>
#include <pthread.h>
#include "structs.h"
#include "defines.h"
#include "enums.h"
#pragma pack(0)

using namespace std;

struct tagData;
int ExecuteResponse(tagData& stData);

void FillSockAddrin(sa_family_t sin_family, in_port_t sin_port, in_addr_t sin_addr, sockaddr_in* sockaddrin);

int SendUDPData(int nSockFD,const void* cData,size_t nSize,const struct sockaddr* pstSockAddr);

int RecvUDPData(int nSockFD,void* cData,size_t nSize, sockaddr* pstSockAddr);



void* RecieverThread(void* pVData)
{
    int lnDataRecievedLen = 0;
    tagData* lpstData = (tagData*)pVData;
    tagData lstData = *lpstData;
    tagNetworkThread* lpstThread = &(lstData.stNetWork);

    tagData lstRecvData = {0};
    
    int lnRetVal = 0;
    while(true)
    {
       memset(&lstRecvData, 0 ,sizeof(tagData));       
       //lnDataRecievedLen = RecvUDPData(lpstThread->fd, (char *)&lstRecvData, sizeof(tagData), (struct sockaddr *) &(lpstThread->addr));
       lnDataRecievedLen  = 	   recvfrom(lpstThread->fd, (char *)&lstRecvData, sizeof(tagData), MSG_WAITALL, (struct sockaddr *) &(lpstThread->addr),(socklen_t*)&(lpstThread->restrict));
       if(0 >= lnDataRecievedLen)
       {
           cout << "data reception error" << endl;
          // exit(1);
       }
       
       lnRetVal = ExecuteResponse(lstRecvData);
       if(lnRetVal == -1)
       {
           cout << "response handling error " << endl;
          //exit(1);
       }
       printf("Server : %d\n", lstRecvData.nMessageCode);
    }
    
}

char g_cIdentifier[20 + 1] = {0};


int ExecuteResponse(tagData& stData)
{
    long lnMessageCode = stData.nMessageCode;
    switch(lnMessageCode)
    {

        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_RESPONSE):
        {
            cout << "response recieved" << endl;
        }
        break;
        
        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET_RESPONSE):
        {
            cout << "target fixed" << endl;
        }
        break;
        
        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MESSAGE):
        {
            cout << stData.cBuffer << endl;
        }
        break;
        
        
        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_RESPONSE):
        {
            cout << "chat sent" << endl;    
        }
        break;
        
        default :
        {
            return -1;
        }
        break;
            
     }
    
    return 0;
}


int CreateUDPSocketIP()
{
    int lnSockFD = socket(AF_INET, SOCK_DGRAM, 0);
    return lnSockFD;
}

void FillSockAddrin(sa_family_t sin_family, in_port_t sin_port, in_addr_t sin_addr, sockaddr_in* sockaddrin)
{
   sockaddrin->sin_family = sin_family; 
   sockaddrin->sin_port = sin_port;
   sockaddrin->sin_addr.s_addr = sin_addr;
}

int SendUDPData(int nSockFD,const void* cData,size_t nSize,const struct sockaddr_in* pstSockAddr)
{
    return sendto(nSockFD, (const char *)&cData, nSize, MSG_CONFIRM, (const struct sockaddr *) pstSockAddr, sizeof(*pstSockAddr)); 
}

int RecvUDPData(int nSockFD,void* cData,size_t nSize, sockaddr* pstSockAddr)
{
    socklen_t lnLen = sizeof(*pstSockAddr);
    return recvfrom(nSockFD, ( void*)cData, nSize, MSG_WAITALL, (struct sockaddr *) pstSockAddr, &(lnLen)); 
}


int main() 
{
    int sockfd                      = 0; 

    struct sockaddr_in     servaddr = {0}; 
  
    if ( (sockfd = CreateUDPSocketIP()) < 0 ) 
    {
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }
  
    FillSockAddrin(AF_INET, htons(PORT), (in_addr_t)(inet_addr("127.0.0.1")), &servaddr);
      
    tagData lstData = {0}; 
    
    lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER;
    
    cout << "Enter your Identifier ID" << endl;
    cin >> lstData.cIdentifier;
    
    memcpy(g_cIdentifier, lstData.cIdentifier, 20);
    
    
    lstData.stNetWork.fd        = sockfd;   
    lstData.stNetWork.n         = sizeof(tagData);
    lstData.stNetWork.flags     = MSG_WAITALL;
    lstData.stNetWork.addr      = servaddr;
    lstData.stNetWork.restrict  = sizeof(servaddr);
    
    tagData lstThreadData = {0};
    memcpy(&lstThreadData, &lstData, sizeof(tagData));
    cout << "sockfd is "<< lstThreadData.stNetWork.fd << endl;
    pthread_t lnPThread;
   
    pthread_create(&lnPThread, NULL, RecieverThread, &lstThreadData);
   
    
//    SendUDPData(sockfd, (const char *)&lstData, sizeof(tagData),  &servaddr); 
   sendto(sockfd, (const char *)&lstData, sizeof(tagData), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    printf("Hello message sent.\n");
   
 
    memset(&lstData, 0, sizeof(tagData));
    memcpy(lstData.cIdentifier, g_cIdentifier, 20);
    lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET;
 
    cout << "Enter Target Identifier" << endl;
    cin >> lstData.cTarget;
    
//    SendUDPData(sockfd, (const char *)&lstData, sizeof(tagData), &servaddr); 
    sendto(sockfd, (const char *)&lstData, sizeof(tagData), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    printf("SpeakData message sent.\n"); 

    while(true)
    {
       memset(&lstData, 0, sizeof(tagData));
       
       lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT;

       strncpy(lstData.cIdentifier, g_cIdentifier, 20);
       cout << "Enter Chat Data" << endl;
       cin  >> lstData.cBuffer;

       sendto(sockfd, (const char *)&lstData, sizeof(tagData), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
       printf("ChatSend message sent.\n");
    }
    
    pthread_join(lnPThread,NULL);
    close(sockfd); 
    return 0; 
}


