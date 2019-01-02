#include <math.h>
#include <deque>
#include <list>
#include "includes.h"

#pragma pack(1)

using namespace std;




void* CheckResponse(void*);

void SetRand(char* cBuf,int nSize);

int ExecuteResponse(tagData& stData);

int CreateUDPSocketIP();

void FillSockAddrin(long sin_family, unsigned short int sin_port, long long sin_addr, sockaddr_in* sockaddrin);

int SendUDPData(int nSockFD, const void* cData, size_t nSize, const struct sockaddr_in* pstSockAddr, long nSockAddrLen);

int RecvUDPData(int nSockFD, void* cData, size_t nSize, sockaddr_in* pstSockAddr, long pnSockAddrLen);

void* SenderThread(void* pVData);

void* RecieverThread(void* pVData);

int PreSender(tagData& stData);

int PostSender(tagData& stData);




struct tagData;

int    g_nTesting = 0;
char** g_pcParam = nullptr;
short  g_nArgs = 0;

char                    g_cIdentifier[20 + 1] = {0};

map <string,tagData>    g_cMessageHolder;

deque<tagData>          g_cSenderDataStore;

list<tagTimeData>       g_cEventResender;

pthread_mutex_t         g_SenderMutex;

pthread_mutex_t         g_ReSenderMutex;

struct sockaddr_in      servaddr = {0};




void* CheckResponse(void*)
{
   while(true)
   {
      pthread_mutex_lock(&g_ReSenderMutex);
      if(!g_cEventResender.empty())
      {
         for(list<tagTimeData>::iterator lcIter =  g_cEventResender.begin();lcIter !=  g_cEventResender.end();)
         {
            if(lcIter->m_nTime <= time(NULL))
            {
               //cout << "hello" << endl;
               if(lcIter->m_nCounter <= 0)
               {
                  lcIter = g_cEventResender.erase(lcIter);
               }
               else
               {
                  pthread_mutex_lock(&g_SenderMutex);
                  cout << "resending data" << endl;
                  g_cSenderDataStore.push_front(lcIter->stData);
                  pthread_mutex_unlock(&g_SenderMutex);
                  //lcIter->stData;
                  lcIter->m_nCounter--;
                  lcIter->m_nTime = time(NULL) + rand()%10;
                  lcIter++;
               }
            }
            else
            {
               lcIter++;
            }
         }
      }
      pthread_mutex_unlock(&g_ReSenderMutex);
      sleep(1);
     // else
     // {
     ///    break;
      //}
   }
}

void SetRand(char* cBuf,int nSize)
{
   
	
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
void* SenderThread(void* pVData)
{
    int lnDataRecievedLen = 0;
    tagData* lpstData = (tagData*)pVData;
 
    tagNetworkThread& lstThread = lpstData->stNetWork;
    int lnSockFd = 0,lnLen = 0;
    tagData lstRecvData = {0};
    
    int lnRetVal = 0;
    bool lbToResend = false;
    while(true)
    {
      lbToResend = true;
	   
      memset(&lstRecvData, 0 ,sizeof(tagData));       
	   
      pthread_mutex_lock(&g_SenderMutex);
       
	   if(!g_cSenderDataStore.empty())
      {
         lstRecvData = g_cSenderDataStore.back();
         g_cSenderDataStore.pop_back();
         pthread_mutex_unlock(&g_SenderMutex);
        
		   pthread_mutex_lock(&g_ReSenderMutex);

		  for(auto& lcTemp : g_cEventResender)
        {
            if(strcmp(lcTemp.stData.cUniqueMessageIdentifier, lstRecvData.cUniqueMessageIdentifier) == 0)
            {
               cout << "sending data";
               lbToResend = false;
            }
        }
         
        if(lbToResend == true)
        {
           cout << "this is a new unique message";
           tagTimeData lstTimeData((time(NULL) + rand()%10), lstRecvData);
           g_cEventResender.push_back(lstTimeData);
        }
        else
        {
           
        }
         pthread_mutex_unlock(&g_ReSenderMutex);
         
         lnLen = sendto(lstThread.fd, (const char *)&lstRecvData, sizeof(tagData), MSG_CONFIRM, (const struct sockaddr *) &(lstThread.addr), sizeof((lstThread.addr))); 
         if(lnLen <= 0)
         {
            printf("error");
            exit(1);
         }
         printf("ChatSend message sent.\n");
       }
       else
       {
          pthread_mutex_unlock(&g_SenderMutex);
       } 
    }
    
}




void* RecieverThread(void* pVData)
{
    int lnDataRecievedLen = 0;
    tagData* lpstData = (tagData*)pVData;

    tagNetworkThread& lstThread = lpstData->stNetWork;

    tagData lstRecvData = {0};
    
    int lnRetVal = 0;
    bool lbToResend =false;
	
    while(true)
    {
       lbToResend = true;
       memset(&lstRecvData, 0 ,sizeof(tagData));       
	   
       lnDataRecievedLen  = recvfrom(lstThread.fd, (char *)&lstRecvData, sizeof(tagData), MSG_WAITALL, (struct sockaddr *) &(lstThread.addr),(socklen_t*)&(lstThread.restrict));
       if(0 >= lnDataRecievedLen)
       {
           cout << "data reception error" << endl;
           cout << strerror(errno) << endl;
           //exit(1);
       }
       
       lnRetVal = ExecuteResponse(lstRecvData);
       if(lnRetVal == -1)
       {
           cout << "response handling error " << endl;
          //exit(1);
       }
        pthread_mutex_lock(&g_ReSenderMutex);
        for(list<tagTimeData>::iterator lcIter =  g_cEventResender.begin();lcIter != g_cEventResender.end();)
        {
             tagTimeData& lstData = *lcIter;
            if(strcmp(lstData.stData.cUniqueMessageIdentifier,lstRecvData.cUniqueMessageIdentifier) == 0)
            {
               cout << "response recieved now erasing" << endl;
               lcIter = g_cEventResender.erase( lcIter);
            }
            else
            {
               lcIter++;
            }
         }
         pthread_mutex_unlock(&g_ReSenderMutex);
        
       printf("Server : %d\n", lstRecvData.nMessageCode);
    }
    
}
//UDpChatServer 21/12/2018 Aditya M.:START
int PreSender(tagData& stData)
{
   switch(stData.nMessageCode)
   {
      case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER:
      {
         cout << "Enter your Identifier ID" << endl;
         if(g_nTesting == 1)
         {
             //strncpy( lstData.cTarget, "QWE", 3);
             strncpy( stData.cIdentifier, g_pcParam[2], 4);
         }
         else
         {
             cin >> stData.cIdentifier;
             //strncpy( lstData.cIdentifier, "ABC", 3);
         }

          memcpy(g_cIdentifier, stData.cIdentifier, 20);
          sleep(5);
      }
      break;
      case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET:
      {
         

         printf("Hello message sent.\n");
         
         memcpy(stData.cIdentifier, g_cIdentifier, 20);
         stData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET;

         cout << "Enter Target Identifier" << endl;
         //cin >> stData.cTarget; 
         if(g_nTesting == 1)
         {
            // strncpy( stData.cTarget, "QWE", 3);
            strncpy( stData.cTarget, g_pcParam[3], 4);
         }
         else
         {
            cin >> stData.cTarget;
         }
         sleep(2);
      }
      break;
      case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT:
      {
         stData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT;
         strncpy(stData.cIdentifier, g_cIdentifier, 20);
         
         if(g_nTesting == 1)
         {
            strncpy(stData.cBuffer, g_cIdentifier, 20);
         }
         else
         {
           cout << "Enter Chat Data" << endl;
           cin  >> stData.cBuffer; 
         }
      }
      break;
   }
   return 0;
}

int PostSender(tagData& stData)
{
   switch(stData.nMessageCode)
   {
      case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER:
      {
         stData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET;
      }
      break;
      case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET:
      {
         stData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT;
      }
      break;
      case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT:
      {
         stData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT;
      }
      break;
   }
   return 0;
}
//UDpChatServer 21/12/2018 Aditya M.:END
int main(int argc,char* argv[])
{
   g_nArgs = argc;
   g_pcParam = argv;
   srand ((time(NULL)));
   
   int lnRetVal = 0;
    
    if(argc == 4)
    {
       if( strncmp("TEST", g_pcParam[1], 4) == 0)
       {
           g_nTesting = 1;
       }
    }
    int lnSockFd                      = 0; 
    tagData* lpstThreadData = NULL;
    tagData* lpstThreadData1 = NULL;
  

    size_t lnLen = 0;
    if ( (lnSockFd = CreateUDPSocketIP()) < 0 )
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    //FillSockAddrin(AF_INET, htons(PORT), (in_addr_t)(inet_addr("127.0.0.1")), &servaddr);
    FillSockAddrin(AF_INET, htons(PORT), (in_addr_t)(inet_addr("172.25.1.8")), &servaddr);

    tagData lstData = {0};
    
    lstData.stNetWork.fd        = lnSockFd;
    lstData.stNetWork.n         = sizeof(tagData);
    lstData.stNetWork.flags     = MSG_WAITALL;
    lstData.stNetWork.addr      = servaddr;
    lstData.stNetWork.restrict  = sizeof(servaddr);
   
   lpstThreadData = new tagData();
   lpstThreadData1 = new tagData();
   
   cout << "sockfd is "<< lpstThreadData->stNetWork.fd << endl;
   
   pthread_t lnPThread;
   pthread_t lnPThread1;
   pthread_t lnPThread2;
   //sleep(1);
   memcpy(lpstThreadData, &lstData, sizeof(tagData));
   memcpy(lpstThreadData1, &lstData, sizeof(tagData));
   pthread_create(&lnPThread, NULL, RecieverThread, lpstThreadData);
   pthread_create(&lnPThread1, NULL, SenderThread, lpstThreadData1);
   pthread_create(&lnPThread2, NULL,  CheckResponse,NULL);
   
   lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER;
   while(true)
   {
      //memset(&lstData, 0, sizeof(tagData));    
      
      SetRand(lstData.cUniqueMessageIdentifier,30);
      
      lnRetVal = PreSender(lstData);
      if(0 != lnRetVal)
      {
         cout << "error occured at Presender" << endl;
         exit(1);  
      }
      
      pthread_mutex_lock(&g_SenderMutex);
      g_cSenderDataStore.push_front(lstData);
      pthread_mutex_unlock(&g_SenderMutex);
      cout << lstData.cUniqueMessageIdentifier << endl;
      memset (lstData.cBuffer, 0,sizeof(lstData.cBuffer));
      memset (lstData.cUniqueMessageIdentifier, 0,sizeof(lstData.cUniqueMessageIdentifier));
    
      lnRetVal = PostSender(lstData);
      if(0 != lnRetVal)
      {
         cout << "error occured at PostSender" << endl;
         exit(1);  
      }
   }

   pthread_join(lnPThread1,NULL);
   pthread_join(lnPThread,NULL);
   delete lpstThreadData;
   delete lpstThreadData1;
   
   close(lnSockFd);
   return 0;
}
#endif
