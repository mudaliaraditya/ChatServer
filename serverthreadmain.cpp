#include <errno.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <iostream>  
#include <string.h>
#include <map>
#include <list>
#include "structs.h"
#include "defines.h"
#include "enums.h"

#pragma pack(0)
using namespace std;

struct sockaddr_in servaddr, cliaddr; 


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

int SendUDPData(int nSockFD,const void* cData,size_t nSize,const struct sockaddr_in* pstSockAddr,socklen_t nSockAddrLen)
{
    return sendto(nSockFD, (const char *)&cData, nSize, MSG_CONFIRM, (const struct sockaddr *) pstSockAddr, nSockAddrLen); 
}

int RecvUDPData(int nSockFD,void* cData,size_t nSize, sockaddr_in* pstSockAddr,socklen_t* pnSockAddrLen)
{
    return recvfrom(nSockFD, ( void*)cData, nSize, MSG_WAITALL, (struct sockaddr *) pstSockAddr,pnSockAddrLen); 
}




map<string, tagData> g_cPortIdentifier;

list<tagData> g_cList;

int ExecuteFunction( const tagData& stData);

int GetResponseForFunction(tagData& stData);
#ifndef singlethread
void* SenderThread(void* pArg)
{
   // tagData lstRecvData = {0};
    tagData lstData = {0};
    int lnReturnVal = 0;
    
    while(true)
    {
       if(!g_cList.empty())
       {
           lstData = g_cList.back();
           g_cList.pop_back();
           if(lstData.nMessageCode == 1003)
           {
               cout << "prob";
           }
            int lnVal = ExecuteFunction(lstData);
           if(lnVal != 0)
           {
               exit(1); 
           }
           
           lnReturnVal = GetResponseForFunction(lstData);
           if(0 > lnReturnVal )
           {
               cout << "wrong response code" << endl;
               //printf("%s", strerror(errno));
              // exit(1);
           }
           int lnDataStructSize = sizeof(tagData);
           char lcBuffer[lnDataStructSize] = {0};
           
           memcpy(&lcBuffer, (char*)&lstData, lnDataStructSize);
           
           //sendto(sockfd, (const char *)&lcBuffer, sizeof(tagData),  MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
           lnReturnVal = sendto(lstData.stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData),  MSG_CONFIRM, (const struct sockaddr *) &(lstData.stNetWork.addr), lstData.stNetWork.restrict); 
           if(0 > lnReturnVal )
           {
               printf("%s", strerror(errno));
               exit(1);
           }
           printf("%s", strerror(errno));
           printf("Hello message sent.\n");
       }
    }
}
#endif
//    char buffer[MAXLINE]; 
//    char *hello = "Hello from server"; 
//    
//      
//    // Creating socket file descriptor 
//    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
//        perror("socket creation failed"); 
//        exit(EXIT_FAILURE); 
//    } 
//      
//    memset(&servaddr2, 0, sizeof(servaddr2)); 
//    memset(&cliaddr2, 0, sizeof(cliaddr2)); 
//      
//    // Filling server information 
//    servaddr2.sin_family    = AF_INET; // IPv4 
//    servaddr2.sin_addr.s_addr = INADDR_ANY; 
//    servaddr2.sin_port = htons(PORT1); 
//      
//    // Bind the socket with the server address 
//    if ( bind(sockfd, (const struct sockaddr *)&servaddr2,  
//            sizeof(servaddr2)) < 0 ) 
//    { 
//        perror("bind failed"); 
//        exit(EXIT_FAILURE); 
//    }
//}

int ExecuteFunction( const tagData& stData)
{
    long lnMessageCode = stData.nMessageCode;
    switch(lnMessageCode)
    {
        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER):
        {
            bool lbVal = (g_cPortIdentifier.insert(pair<string,tagData>(stData.cIdentifier,stData))).second;
            if(lbVal == false)
            {
                printf( "theres an error");
                exit(-1);
            }
        }
        break;
        
        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET):
        {
//            bool lbVal = (g_cPortIdentifier.insert(pair<string,tagData>(stData.cIdentifier,stData))).second;
//            if(lbVal == false)
//            {
//                printf( "theres an error");
//                exit(-1);
//            }
            map<string, tagData>::iterator lcIterator = g_cPortIdentifier.find(stData.cIdentifier);
            if(lcIterator == g_cPortIdentifier.end())
            {
                
            }
            
            tagData& lcData =  lcIterator->second;
            strncpy(lcData.cTarget, stData.cTarget, 20);
//            map<string, tagData>::iterator   lcitr =  g_cPortIdentifier.erase(lcIterator);
//            if(lcitr == g_cPortIdentifier.end())
//            {
//                
//            }
        }
        break;
        
        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT):
        {
            char lcBuffer[sizeof(tagData)] = {0};
      
            tagData lstData = stData;
            lstData.nMessageCode = (long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MESSAGE;
            memcpy(&lcBuffer, (char*)&lstData, sizeof(tagData));
            map<string, tagData>::iterator lcIter = g_cPortIdentifier.find(stData.cIdentifier);
            if(lcIter == g_cPortIdentifier.end())
            {
                cout << stData.cIdentifier << "not found";
                exit(1);
            }
            map<string, tagData>::iterator lcIter1 = g_cPortIdentifier.find(lcIter->second.cTarget);
            if(lcIter1 == g_cPortIdentifier.end())
            {
                cout << "Target not found";
                exit(1);
            }
            int len, n;
            sendto( lcIter1->second.stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData),  MSG_CONFIRM, (const struct sockaddr *) &(lcIter1->second.stNetWork.addr), lcIter1->second.stNetWork.restrict); 
        }
        break;
    }
    
    return 0;
}
  


int GetResponseForFunction(tagData& stData)
{
    long& lnMessageCode = stData.nMessageCode;
    switch(lnMessageCode)
    {
        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER):
        {
         
            stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_RESPONSE);
            return 0;
        }
        break;
        
        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET):
        {
         
            stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET_RESPONSE);
            return 0;
        }
        break;
        
         case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT):
         {
         
            stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_RESPONSE);
            return 0;
        }
        break;
    }
    
    return -1;
}

// Driver code 
int main() 
{ 
    int sockfd = 0;  
    // Creating socket file descriptor 
    //if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
    if ( (sockfd = CreateUDPSocketIP()) < 0)//socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
    {
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
//    servaddr.sin_family    = AF_INET; // IPv4 
//    servaddr.sin_addr.s_addr = INADDR_ANY; 
//    servaddr.sin_port = htons(PORT); 
    FillSockAddrin( AF_INET, htons(PORT), INADDR_ANY, &servaddr );
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int len, n;
    tagData lstData = {0}; 
    //n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t*)&len); 
#ifndef singlethread
    pthread_t lnPthread;
    pthread_create(&lnPthread, NULL, SenderThread,NULL);
#endif
    while(true)
    {
        memset(&lstData, 0, sizeof(lstData));
        n = recvfrom(sockfd, (char *)&lstData, sizeof(tagData),MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t*)&len);
      //  n = RecvUDPData(sockfd, (char *)&lstData, sizeof(tagData),  &cliaddr);
        if(n <= 0)
        {
            cout << "" << endl;
            exit(1);
            
        }
     	printf("%s", strerror(errno));
        if(lstData.nMessageCode == (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER)
        {
           lstData.lnSockFD = sockfd;
        }
        cout << cliaddr.sin_addr.s_addr << endl;
        cout << cliaddr.sin_family<< endl;
        cout << cliaddr.sin_port<< endl;
        cout << cliaddr.sin_zero << endl;
        //printf("Client : %s\n", lstData.cIdentifier);
//             if(lstData.nMessageCode == 1003)
//           {
//               cout << "prob";
//           }
#ifndef singlethread
        memcpy(&(lstData.stNetWork.addr), &cliaddr, sizeof(sockaddr_in));
        lstData.stNetWork.fd = sockfd;
        //lstData.stNetWork.addr
        lstData.stNetWork.restrict = sizeof(sockaddr_in);
        lstData.stNetWork.flags = MSG_CONFIRM;
        cout << "chat data " << lstData.cBuffer << endl;
        cout << "identifier " << lstData.cIdentifier << endl;
        cout << "target " <<lstData.cTarget << endl;
        ///cout << lstData.cBuffer << endl;
        g_cList.push_back(lstData);
        
#endif
#ifdef singlethread
        int lnVal = ExecuteFunction(lstData);
        if(lnVal != 0)
        {
            exit(1);
        }
        printf("%s", strerror(errno));
        tagData lstMajorData = GetResponseForFunction(lstData);
        //sendto(sockfd, (const char *)hello, strlen(hello),  MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
        int lnDataStructSize = sizeof(tagData);
        char lcBuffer[lnDataStructSize] = {0};
       //lstMajorData.nMessageCode = (long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_RESPONSE;
        memcpy(&lcBuffer, (char*)&lstMajorData, lnDataStructSize);
        len = sizeof(sockaddr_in);
        sendto(sockfd, (const char *)&lcBuffer, sizeof(tagData),  MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
        printf("%s", strerror(errno));
        printf("Hello message sent.\n");
#endif
        
    }
    

    
    close(sockfd);     
    return 0;
}


