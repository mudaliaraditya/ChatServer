#include <math.h>

#include "includes.h"

#pragma pack(1)

using namespace std;

struct tagData;
int ExecuteResponse(tagData& stData);


char g_cIdentifier[20 + 1] = {0};
map <string,tagData> g_cMessageHolder;



void SetRand(char* cBuf,int nSize)
{
    srand ((time(NULL)));
    //char lcVar[nSize + 1] = {0};
    nSize--;
    int lnCharHalf = ceil(nSize/(13.33));
    for(int lnCounter = 0;lnCounter<nSize ;lnCounter++)
    {
        if(lnCounter < (nSize - lnCharHalf))
        {
            cBuf[lnCounter] = '0' + rand()%10;
        }
        else
        {
            cBuf[lnCounter] = 'a' + rand()%26;
        }
    }
}

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



void FillSockAddrin(long sin_family, unsigned short int sin_port, long long sin_addr, sockaddr_in* sockaddrin)
{

   sockaddrin->sin_family = sin_family;
   sockaddrin->sin_port = sin_port;
   sockaddrin->sin_addr.s_addr = sin_addr;
}



int SendUDPData(int nSockFD, const void* cData, size_t nSize, const struct sockaddr_in* pstSockAddr, long nSockAddrLen)
{
#ifndef WIN32
   return sendto(nSockFD, (const char *)&cData, nSize, MSG_CONFIRM, (const struct sockaddr *) pstSockAddr, nSockAddrLen);
#endif

#ifdef WIN32
   return sendto(nSockFD, (const char *)&cData, nSize, 0, (const struct sockaddr *) pstSockAddr, nSockAddrLen);
#endif
}

int RecvUDPData(int nSockFD, void* cData, size_t nSize, sockaddr_in* pstSockAddr, long pnSockAddrLen)
{

#ifndef WIN32
   return recvfrom(nSockFD, (void*)cData, nSize, MSG_WAITALL, (struct sockaddr *) pstSockAddr, (socklen_t*)&pnSockAddrLen);
#endif
#ifdef WIN32
   return recvfrom(nSockFD, (char*)cData, nSize, 0, (struct sockaddr *) pstSockAddr, (int*)&pnSockAddrLen);
#endif

}


#ifndef WIN32
void* RecieverThread(void* pVData)
{
    int lnDataRecievedLen = 0;
    tagData* lpstData = (tagData*)pVData;
   // tagData lstData = *lpstData;
    //tagNetworkThread* lpstThread = &(lstData.stNetWork);
    tagNetworkThread& lstThread = lpstData->stNetWork;

    tagData lstRecvData = {0};
    
    int lnRetVal = 0;
    while(true)
    {
       memset(&lstRecvData, 0 ,sizeof(tagData));       
       //lnDataRecievedLen = RecvUDPData(lpstThread->fd, (char *)&lstRecvData, sizeof(tagData), (struct sockaddr *) &(lpstThread->addr));
//       lnDataRecievedLen  = 	   recvfrom(lpstThread->fd, (char *)&lstRecvData, sizeof(tagData), MSG_WAITALL, (struct sockaddr *) &(lpstThread->addr),(socklen_t*)&(lpstThread->restrict));
       lnDataRecievedLen  = 	   recvfrom(lstThread.fd, (char *)&lstRecvData, sizeof(tagData), MSG_WAITALL, (struct sockaddr *) &(lstThread.addr),(socklen_t*)&(lstThread.restrict));
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


int main(int argc,char* argv[])
{
    int lnTesting = 0;
    if(argc >= 2)
    {
       if(strncmp("TEST",argv[1],4) == 0)
       {
           lnTesting = 1;
       }
    }
    int lnSockFd                      = 0; 
    tagData* lpstThreadData = NULL;
    struct sockaddr_in     servaddr = {0};

    size_t lnLen = 0;
    if ( (lnSockFd = CreateUDPSocketIP()) < 0 )
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    FillSockAddrin(AF_INET, htons(PORT), (in_addr_t)(inet_addr("127.0.0.1")), &servaddr);

    tagData lstData = {0};

    lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER;

    cout << "Enter your Identifier ID" << endl;
    if(lnTesting == 1)
    {
       //strncpy( lstData.cTarget, "QWE", 3);
       strncpy( lstData.cIdentifier, argv[2], 4);
    }
    else
    {
       cin >> lstData.cIdentifier;
       //strncpy( lstData.cIdentifier, "ABC", 3);
    }

    memcpy(g_cIdentifier, lstData.cIdentifier, 20);
    
    lstData.stNetWork.fd        = lnSockFd;
    lstData.stNetWork.n         = sizeof(tagData);
    lstData.stNetWork.flags     = MSG_WAITALL;
    lstData.stNetWork.addr      = servaddr;
    lstData.stNetWork.restrict  = sizeof(servaddr);
    
    SetRand(lstData.cUniqueMessageIdentifier,30);

    lpstThreadData = new tagData();
    
   //memcpy(lpstThreadData, &lstData, sizeof(tagData));
   // cout << "sockfd is "<< lpstThreadData->stNetWork.fd << endl;
   // pthread_t lnPThread;
   
   //pthread_create(&lnPThread, NULL, RecieverThread, lpstThreadData);
   //tagData lstThreadData = {0};
   memcpy(lpstThreadData, &lstData, sizeof(tagData));
   cout << "sockfd is "<< lpstThreadData->stNetWork.fd << endl;
   pthread_t lnPThread;
   pthread_t lnPThread1;
   //sleep(1); 
   pthread_create(&lnPThread, NULL, RecieverThread, lpstThreadData);
  // pthread_create(&lnPThread1, NULL, RecieverThread, lpstThreadData);
   
   lnLen = sendto(lnSockFd, (const char *)&lstData, sizeof(tagData), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
   if(lnLen <= 0)
   {
       printf("error");
       exit(1);
   }
   sleep(5);
   printf("Hello message sent.\n");
 
   memset(&lstData, 0, sizeof(tagData));
   memcpy(lstData.cIdentifier, g_cIdentifier, 20);
   lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET;
 
   cout << "Enter Target Identifier" << endl;
   //cin >> lstData.cTarget; 
   if(lnTesting == 1)
   {
      // strncpy( lstData.cTarget, "QWE", 3);
      strncpy( lstData.cTarget, argv[3], 4);
   }
   else
   {
      cin >> lstData.cTarget;
   }
   //SendUDPData(sockfd, (const char *)&lstData, sizeof(tagData), &servaddr); 
   SetRand(lstData.cUniqueMessageIdentifier,30);
   lnLen = sendto(lnSockFd, (const char *)&lstData, sizeof(tagData), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
   if(lnLen <= 0)
   {
      printf("error");
      exit(1);
   }
   sleep(2);
   printf("SpeakData message sent.\n"); 

   while(true)
   {
      memset(&lstData, 0, sizeof(tagData));    
      lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT;
      strncpy(lstData.cIdentifier, g_cIdentifier, 20);
      cout << "Enter Chat Data" << endl;
      if(lnTesting == 1)
      {
         strncpy(lstData.cBuffer, g_cIdentifier, 20);
      }
      else
      {
        cin  >> lstData.cBuffer; 
      }
      SetRand(lstData.cUniqueMessageIdentifier,30);
      lnLen = sendto(lnSockFd, (const char *)&lstData, sizeof(tagData), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
      if(lnLen <= 0)
      {
          printf("error");
          exit(1);
      }
      printf("ChatSend message sent.\n");
   }

   pthread_join(lnPThread,NULL);
   delete lpstThreadData;
   
   close(lnSockFd);
   return 0;
}
#endif
#ifdef WIN32

DWORD WINAPI RecieverThread(LPVOID lpParameter)
{
   int lnDataRecievedLen = 0;
   tagData* lpstData = (tagData*)lpParameter;
   tagData lstData = *lpstData;
   tagNetworkThread* lpstThread = &(lstData.stNetWork);

   tagData lstRecvData = {0};

   int lnRetVal = 0;
   while (true)
   {
      memset(&lstRecvData, 0, sizeof(tagData));
     // lnDataRecievedLen = RecvUDPData(lpstThread->fd, (char *)&lstRecvData, sizeof(tagData), (struct sockaddr *) &(lpstThread->addr));
      lnDataRecievedLen = recvfrom(lpstThread->fd, (char *)&lstRecvData, sizeof(tagData), 0, (struct sockaddr *) &(lpstThread->addr), (int*)&(lpstThread->restrict));
      if (0 >= lnDataRecievedLen)
      {
         cout << "data reception error" << endl;
         exit(1);
      }

      lnRetVal = ExecuteResponse(lstRecvData);
      if (lnRetVal == -1)
      {
         cout << "response handling error " << endl;
         exit(1);
      }

      printf("Server : %d\n", lstRecvData.nMessageCode);
   }
      return 0;
}


int main()
{
   int n = 0, len = sizeof(sockaddr_in);
   WSADATA w = { 0 };						/* Used to open windows connection */
   unsigned short port_number = 0;			/* Port number to use */
   int a1 = 0, a2 = 0, a3 = 0, a4 = 0;
   a1 = 127;
   a2 = 0;
   a3 = 0;
   a4 = 1;

   port_number = PORT;
   if (WSAStartup(0x0101, &w) != 0)
   {
      fprintf(stderr, "Could not open Windows connection.\n");
      exit(0);
   }
   int lnRetVal = 0;

   int sockfd = 0;

   struct sockaddr_in     servaddr = { 0 };

   if ((sockfd = CreateUDPSocketIP()) < 0)
   {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
   }
   //(short sin_family, u_short sin_port, ULONG sin_addr, sockaddr_in* sockaddrin
   FillSockAddrin(AF_INET, htons(PORT), (inet_addr("127.0.0.1")), &servaddr);

   tagData lstData = { 0 };

   lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER;

   cout << "Enter your Identifier ID" << endl;
   cin >> lstData.cIdentifier;

   memcpy(g_cIdentifier, lstData.cIdentifier, 20);


   lstData.stNetWork.fd = sockfd;
   lstData.stNetWork.n = sizeof(tagData);
   lstData.stNetWork.flags = 0;
   lstData.stNetWork.addr = servaddr;
   lstData.stNetWork.restrict = sizeof(servaddr);

   tagData lstThreadData = { 0 };
   memcpy(&lstThreadData, &lstData, sizeof(tagData));
   cout << "sockfd is " << lstThreadData.stNetWork.fd << endl;
   //pthread_t lnPThread;

  // pthread_create(&lnPThread, NULL, RecieverThread, &lstThreadData);
   unsigned int myCounter = 0;
   DWORD myThreadID;
   HANDLE myHandle = CreateThread(0, 0, RecieverThread, &lstThreadData , 0, &myThreadID);

//    SendUDPData(sockfd, (const char *)&lstData, sizeof(tagData),  &servaddr); 
   sendto(sockfd, (const char *)&lstData, sizeof(tagData), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
   printf("Hello message sent.\n");
   memset(&lstData, 0, sizeof(tagData));
   memcpy(lstData.cIdentifier, g_cIdentifier, 20);
   
//   n = recvfrom(sockfd, (char *)&lstData, sizeof(tagData), 0, (struct sockaddr *) &servaddr, (int*)&len);
//   lnRetVal = ExecuteResponse(lstData);
//   if (lnRetVal < 0)
//   {
//      cout << "" << endl;
//      exit(1);
//   }

   lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET;
   cout << "Enter Target Identifier" << endl;
   cin >> lstData.cTarget;
   //    SendUDPData(sockfd, (const char *)&lstData, sizeof(tagData), &servaddr); 
   sendto(sockfd, (const char *)&lstData, sizeof(tagData), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
   printf("SpeakData message sent.\n");
//   n = recvfrom(sockfd, (char *)&lstData, sizeof(tagData), 0, (struct sockaddr *) &servaddr, (int*)&len);
//   lnRetVal = ExecuteResponse(lstData);
//   if (lnRetVal < 0)
//   {
//      cout << "" << endl;
//      exit(1);
//   }

 

      while (true)
      {
         memset(&lstData, 0, sizeof(tagData));

         lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT;

         strncpy(lstData.cIdentifier, g_cIdentifier, 20);
         cout << "Enter Chat Data" << endl;
         cin >> lstData.cBuffer;

         sendto(sockfd, (const char *)&lstData, sizeof(tagData), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
         printf("ChatSend message sent.\n");
         //n = recvfrom(sockfd, (char *)&lstData, sizeof(tagData),MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t*)&len);
//         n = RecvUDPData(sockfd, (char *)&lstData, sizeof(tagData), (struct sockaddr *)&servaddr, (int*)&len);
//         if (n < 0)
//         {
//            cout << "" << endl;
//            exit(1);

//         }
//         lnRetVal = ExecuteResponse(lstData);
//         if (lnRetVal < 0)
//         {
//            cout << "" << endl;
//            exit(1);

//         }
      }

      //    pthread_join(lnPThread,NULL);
      closesocket(sockfd);
      WSACleanup();
      return 0;
   }



#endif

