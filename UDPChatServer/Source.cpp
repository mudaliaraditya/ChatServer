#include <deque>

#include "includes.h"
using namespace std;

#pragma pack(1)

long g_OS = 0;
bool g_bProgramShouldWork = true;
//UDPChatServer 20/12/2018 Aditya M.
typedef map<string, tagData*> CIdentiferDataStore;
typedef map<string, tagData*>::iterator CIteratotrIdentiferDataStore;

typedef  list<tagData*> CDataStore;
typedef  list<tagData*>::iterator CIteratorDataStore;



deque<string> g_cIdentifierStore;

int g_nTesting = 1;

CIdentiferDataStore g_cPortIdentifier;
CDataStore g_cResponseList;
CDataStore g_cProcessList;
//UDPChatServer 20/12/2018 Aditya M.

int CreateUDPSocketIP()
{
   int lnSockFD = socket(AF_INET, SOCK_DGRAM, 0);
   return lnSockFD;
}

struct sockaddr_in servaddr, cliaddr;

int CleanUp()
{
   g_bProgramShouldWork = false;
   for(CIteratotrIdentiferDataStore lcIter = g_cPortIdentifier.begin(); lcIter != g_cPortIdentifier.end();lcIter++)
   {
      tagData* lpstData = lcIter->second;
      if(lpstData != nullptr)
      {
         delete (lpstData);
         lpstData = nullptr;
      }
      //lcIter = g_cPortIdentifier.erase(lcIter);
   }
   
   for(CIteratorDataStore lcIter = g_cResponseList.begin();g_cResponseList.end() != lcIter; lcIter++)
   {
      tagData* lpstData = *lcIter;
      if(lpstData != nullptr)
      {
         delete (lpstData);
         lpstData = nullptr;
      }
   }
   
   for(CIteratorDataStore lcIter = g_cProcessList.begin(); g_cProcessList.end() != lcIter; lcIter++)
   {
      tagData* lpstData = *lcIter;
      if(lpstData != nullptr)
      {
         delete (lpstData);
         lpstData = nullptr;
      }
   }
   
   return 0;
}

void handle_signal(int signal) 
{
    const char *signal_name;
    sigset_t pending;

    // Find out which signal we're handling
    switch (signal) 
    {
        case SIGINT:
        {
            printf("Caught SIGINT, exiting now\n");
            CleanUp();
            exit(0);
        }
        break;
        
       default:
       {
          fprintf(stderr, "Caught wrong signal: %d\n", signal);
          return;
       }
    }
}


void FillSockAddrin(long sin_family, unsigned short int sin_port, long long sin_addr, sockaddr_in* sockaddrin)
{

   sockaddrin->sin_family = sin_family;
   sockaddrin->sin_port = sin_port;
   sockaddrin->sin_addr.s_addr = sin_addr;
}

int NetWorkInitialize(int& nSockfd)
{
   if ((nSockfd = CreateUDPSocketIP()) < 0)//socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
   {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
   }


   memset(&servaddr, 0, sizeof(servaddr));
   memset(&cliaddr, 0, sizeof(cliaddr));

   FillSockAddrin(AF_INET, htons(PORT), INADDR_ANY, &servaddr);

   if (bind(nSockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
   {
      perror("bind failed");
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
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

int ExecuteFunction(const tagData& stData)
{
   long lnMessageCode = stData.nMessageCode;
   tagData* lpstData = nullptr;
   switch (lnMessageCode)
   {
   case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER) :
   {
      lpstData = new tagData(stData);
      bool lbVal = (g_cPortIdentifier.insert(pair<string, tagData*>(stData.cIdentifier, lpstData))).second;
      if (lbVal == false)
      {
         printf("duplicate identifier insert to identifier(%s) store failed ", stData.cIdentifier);
            printf("%s %d", strerror(errno), __LINE__);
        // exit(-1);
      }
   }
   break;

   case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET) :
   {
      map<string, tagData*>::iterator lcIterator = g_cPortIdentifier.find(stData.cIdentifier);
      if (lcIterator == g_cPortIdentifier.end())
      {

      }
         if(lcIterator->second == nullptr)
         {
            cout << "invalid ptr" << endl;
            exit(1);
         }
      tagData& lcData = *(lcIterator->second);
      strncpy(lcData.cTarget, stData.cTarget, 20);
   }
                                                                                 break;

   case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT) :
   {
      char lcBuffer[sizeof(tagData)] = { 0 };

      tagData lstData = stData;
      lstData.nMessageCode = (long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MESSAGE;
      memcpy(&lcBuffer, (char*)&lstData, sizeof(tagData));
      map<string, tagData*>::iterator lcIter = g_cPortIdentifier.find(stData.cIdentifier);
      if (lcIter == g_cPortIdentifier.end())
      {
         cout << stData.cIdentifier << "not found";
            printf("%s %d", strerror(errno), __LINE__);
        // exit(1);
      }
      
      map<string, tagData*>::iterator lcIter1 = g_cPortIdentifier.find(lcIter->second->cTarget);
      if (lcIter1 == g_cPortIdentifier.end())
      {
         cout << "Target not found" << lcIter->second->cTarget <<  "By Identifier " << stData.cIdentifier <<endl;
            printf("%s %d", strerror(errno), __LINE__);
        // exit(1);
         //goto A;
      }
      int len, n;
      sendto(lcIter1->second->stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData), 0, (const struct sockaddr *) &(lcIter1->second->stNetWork.addr), lcIter1->second->stNetWork.restrict);
   }
                                                                      break;
   }

   return 0;
}

int GetResponseForFunction(tagData& stData)
{
   long& lnMessageCode = stData.nMessageCode;
   switch (lnMessageCode)
   {
      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER) :
      {

        stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_RESPONSE);
        return 0;
      }
                                                                          break;

   case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET) :
   {

      stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET_RESPONSE);
      return 0;
   }
                                                                                 break;

   case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT) :
   {

      stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_RESPONSE);
      return 0;
   }
                                                                      break;
   }

   return -1;
}



#ifndef WIN32

pthread_mutex_t g_cProcessMutex;

pthread_mutex_t g_cResponseMutex;

pthread_mutex_t g_cIdentifierMutex;

pthread_mutex_t g_cDataStoreMutex;

pthread_cond_t g_cConditionalVar;


void* EventHandling(void* pEventHandlingArg)
{
   while(g_bProgramShouldWork)
   {
      pthread_mutex_lock(&g_cIdentifierMutex);
      if(!(g_cIdentifierStore.empty()))
      {
          sleep(10);
          g_cIdentifierStore.pop_front();
          pthread_mutex_unlock(&g_cIdentifierMutex);
      }
      else
      {
         
         pthread_mutex_unlock(&g_cIdentifierMutex);
      }
   }


}




void* ProcessThread(void* pArg)
{
   int lnReturnVal = 0;
   //tagData lstData = {0};
   tagData* lstData = nullptr;
   while (true == g_bProgramShouldWork)
   {

      lnReturnVal = pthread_mutex_lock(&g_cProcessMutex);
      if (lnReturnVal != 0)
         {
            printf("%s, %d ", strerror(errno), __LINE__);
            perror("unable to take lock");
            exit(1);
         }
      //if (g_cListLen > 0)
      {

#ifdef LOGGING
         printf("In funtion \nthread id = %d\n", pthread_self());
#endif

         
         pthread_cond_wait(&g_cConditionalVar, &g_cProcessMutex);
         if(!g_cProcessList.empty())
         {

#ifdef LOGGING
         cout << "process thread process mutex acquired" << endl;
#endif
            lstData = g_cProcessList.back();
            cout << "Process fired" << endl;
            g_cProcessList.pop_back();         
         }
         else
         {
            lnReturnVal = pthread_mutex_unlock(&g_cProcessMutex);
            if (lnReturnVal != 0)
            {
               printf("%s, %d", strerror(errno), __LINE__);
               perror("unable to take lock");
               exit(1);
            }
            continue;
         }
         lnReturnVal = pthread_mutex_unlock(&g_cProcessMutex);
         if (lnReturnVal != 0)
         {
            printf("%s, %d", strerror(errno), __LINE__);
            perror("unable to take lock");
            exit(1);
         }
#ifdef LOGGING           
         cout << "process thread process mutex acquired" << endl;
#endif
         int lnVal = ExecuteFunction(*lstData);
         if (lnVal != 0)
         {
         //   exit(1);
         }

         lnReturnVal = GetResponseForFunction(*lstData);
         if (0 > lnReturnVal)
         {
            cout << "wrong response code" << endl;
            //printf("%s", strerror(errno));
            // exit(1);
         }
         int lnDataStructSize = sizeof(tagData);
         char lcBuffer[lnDataStructSize] = { 0 };

         lnReturnVal = pthread_mutex_lock(&g_cResponseMutex);
         if (lnReturnVal != 0)
         {
            printf("%s, %d", strerror(errno), __LINE__);
            perror("unable to take lock");
            exit(1);
         }
#ifdef LOGGING
         cout << "processthread mutex lock acquired" << endl;
#endif
         g_cResponseList.push_back(lstData);
         lnReturnVal = pthread_mutex_unlock(&g_cResponseMutex);
         if (lnReturnVal != 0)
         {
            printf("%s, %d", strerror(errno), __LINE__);
            perror("unable to take lock");
            exit(1);
         }
#ifdef LOGGING
         cout << "processthread mutex lock released" << endl;
#endif
      }
   }
   return NULL;
}

void* SenderThread(void* pArg)
{
   tagData* lstData = nullptr;
   int lnReturnVal = 0;

   while (true == g_bProgramShouldWork)
   {
      //if(!g_cResponseList.empty())

      pthread_mutex_lock(&g_cResponseMutex);
      if (g_cResponseList.empty())
      {
         pthread_mutex_unlock(&g_cResponseMutex);
         continue;
      }
      
      lstData = g_cResponseList.back();
      if(nullptr == lstData)
      {
         cout << "error nullptr" << endl;
         exit(1);
      }
      
      g_cResponseList.pop_back();
      lnReturnVal = pthread_mutex_unlock(&g_cResponseMutex);
      if (lnReturnVal != 0)
      {
         printf("%s, %d", strerror(errno), __LINE__);
         perror("unable to take lock");
         exit(1);
      }

      int lnDataStructSize = sizeof(tagData);
      char lcBuffer[lnDataStructSize] = { 0 };

      memcpy(&lcBuffer, (char*)lstData, lnDataStructSize);

      lnReturnVal = sendto(lstData->stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData), MSG_CONFIRM, (const struct sockaddr *) &(lstData->stNetWork.addr), lstData->stNetWork.restrict);
      if (0 > lnReturnVal)
      {
         printf("%s", strerror(errno));
         exit(1);
      }
      delete (lstData);
      lstData = nullptr;
      //#define LOGGING
#ifdef LOGGING        
      printf("%s", strerror(errno));
      printf("Hello message sent.\n");
#endif
   }
   return NULL;
}



#endif

#ifndef WIN32
int main()
{
   struct sigaction lstSigAction;
   lstSigAction.sa_flags = 0;
   lstSigAction.sa_handler  = handle_signal;
   
   sigaction(SIGINT, &lstSigAction, NULL);
   
   g_OS = 1;

   int lnRetVal = 0;
   int sockfd = 0;

   pthread_t lnSenderPThread;
   
   pthread_t lnProcessPThread[NO_OF_PROC_THREADS];
   
   pthread_create(&lnSenderPThread, NULL, SenderThread, NULL);
   
   for (int lnCounter = 0; lnCounter< NO_OF_PROC_THREADS; lnCounter++)
   {
      pthread_create(((pthread_t*)&(lnProcessPThread[lnCounter])), NULL, ProcessThread, NULL);
   }

   if (pthread_cond_init(&g_cConditionalVar,NULL) != 0)
   {
      printf("%s %d", strerror(errno), __LINE__);
      printf("\n conditional var init has failed\n");
      return 1;
   }
   
   if (pthread_mutex_init(&g_cProcessMutex, NULL) != 0)
   {
      printf("%s %d", strerror(errno), __LINE__);
      printf("\n mutex init has failed\n");
      return 1;
   }
   
   if (pthread_mutex_init(&g_cResponseMutex, NULL) != 0)
   {
      printf("%s %d", strerror(errno), __LINE__);
      printf("\n mutex init has failed\n");
      return 1;
   }
       
   lnRetVal = NetWorkInitialize(sockfd);
   if (lnRetVal != EXIT_SUCCESS)
   {
      perror("Network Initialize failed");
      exit(EXIT_FAILURE);
   }

   long long lnSockAddrlen = 0, lnNoOfBytes = 0;
   tagData* lpstData = nullptr;
   bool lbDiscardPacket = false;
   while (true == g_bProgramShouldWork)
   {
      lpstData = new tagData();
      lnSockAddrlen = sizeof(cliaddr);
      lnNoOfBytes = RecvUDPData(sockfd, (char *)lpstData, sizeof(tagData), &cliaddr, lnSockAddrlen);
      if (lnNoOfBytes <= 0)
      {
         cout << "" << endl;
         exit(1);
      }
      cout << lpstData->cUniqueMessageIdentifier << endl;
      pthread_mutex_lock( &g_cIdentifierMutex);
      for(auto& lstIdentifiers : g_cIdentifierStore)
      {
        // cout << lstIdentifiers.c_str() << " ";
         if(0 == strcmp(lstIdentifiers.c_str(),lpstData->cUniqueMessageIdentifier))
         {
            cout << "duplicate packet" << endl;
            cout << lstIdentifiers.c_str() << " duplicated" << endl;;
           lbDiscardPacket = true;
            // continue;
         }
      }
      if(true == lbDiscardPacket)
      {
         delete lpstData;
         lbDiscardPacket = false;
         lpstData = nullptr;
         continue;
      }
       //cout << endl;
      g_cIdentifierStore.push_back(lpstData->cUniqueMessageIdentifier);
      pthread_mutex_unlock( &g_cIdentifierMutex);
      
      if (lpstData->nMessageCode == (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER)
      {
         lpstData->stNetWork.fd = sockfd;
      }
      memcpy(&(lpstData->stNetWork.addr), &cliaddr, sizeof(sockaddr_in));
      lpstData->stNetWork.fd = sockfd;
      lpstData->stNetWork.restrict = sizeof(sockaddr_in);
      lpstData->stNetWork.flags = MSG_CONFIRM;
#ifdef LOGGING        
      cout << "chat data " << lpstData->cBuffer << endl;
      cout << "identifier " << lpstData->cIdentifier << endl;
      cout << "target " << lpstData->cTarget << endl;
#endif        
      lnRetVal = pthread_mutex_lock(&g_cProcessMutex);
      if (lnRetVal != 0)
      {
         printf("%s %d", strerror(errno), __LINE__);
         exit(1);
      }

#ifdef LOGGING
      cout << "mtex acquired process main" << endl;
#endif
      g_cProcessList.push_back(lpstData);
      
      pthread_cond_signal(&g_cConditionalVar);
      lnRetVal = pthread_mutex_unlock(&g_cProcessMutex);
      if (lnRetVal != 0)
      {
         printf("%s %d", strerror(errno), __LINE__);
         exit(1);
      }
#ifdef LOGGING
      cout << "mtex Released process main" << endl;
#endif

   }



   close(sockfd);
   if(CleanUp() != 0)
   {
      //printf("%s, %d", strerror(errno), __LINE__);
      perror("cleanup dailed");
      exit(1);
   }
   
   lnRetVal = pthread_mutex_destroy(&g_cProcessMutex);
   if (lnRetVal != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to take lock");
      exit(1);
   }

   lnRetVal = pthread_mutex_destroy(&g_cResponseMutex);
   if (lnRetVal != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to take lock");
      exit(1);
   }
   return 0;
}
#endif



#ifdef WIN32
CRITICAL_SECTION g_cProcessMutex;
CRITICAL_SECTION g_cProcessMutexCounter;
CRITICAL_SECTION g_cResponseMutex;

DWORD WINAPI ProcessThread(LPVOID pArg)
{
   tagData lstData = { 0 };
   int lnReturnVal = 0;
   int n = 0;
   while (true)
   {
      if (g_cListLen > 0)
      {
         EnterCriticalSection(&g_cProcessMutex);
         while (g_cLockerInt == 1) {}
         if (g_cLockerInt == 0)
         {
            g_cLockerInt = 1;
         }
         if (g_cListLen <= 0)
         {
            if (g_cLockerInt == 1)
            {
               g_cLockerInt = 0;
            }
            LeaveCriticalSection(&g_cProcessMutex);
            continue;
         }
         lstData = g_cProcessList.back();
         g_cProcessList.pop_back();
         EnterCriticalSection(&g_cProcessMutexCounter);
         g_cListLen--;
         LeaveCriticalSection(&g_cProcessMutexCounter);
         LeaveCriticalSection(&g_cProcessMutex);
         if (g_cLockerInt == 1)
         {
            g_cLockerInt = 0;
         }
         int lnVal = ExecuteFunction(lstData);
         if (lnVal != 0)
         {
            exit(1);
         }

         lnReturnVal = GetResponseForFunction(lstData);
         if (0 > lnReturnVal)
         {
            cout << "wrong response code" << endl;
            //printf("%s", strerror(errno));
            // exit(1);
         }
         EnterCriticalSection(&g_cResponseMutex);
         g_cResponseList.push_back(lstData);

         LeaveCriticalSection(&g_cResponseMutex);
      }
   }
}

DWORD WINAPI SenderThread(LPVOID pArg)
{
   tagData lstData = { 0 };
   int lnReturnVal = 0;
   int n = 0;
   while (true)
   {
      //if(!g_cResponseList.empty())
      //{
      EnterCriticalSection(&g_cResponseMutex);
      if (g_cResponseList.empty())
      {
         LeaveCriticalSection(&g_cResponseMutex);
         continue;
      }
      lstData = g_cResponseList.back();
      g_cResponseList.pop_back();
      LeaveCriticalSection(&g_cResponseMutex);
      if (lnReturnVal != 0)
      {
         printf("%s, %d", strerror(errno), __LINE__);
         perror("unable to take lock");
         // exit(1);
      }

      int lnDataStructSize = sizeof(tagData);
      char lcBuffer[sizeof(tagData)] = { 0 };

      memcpy(&lcBuffer, (char*)&lstData, lnDataStructSize);

      //sendto(sockfd, (const char *)&lcBuffer, sizeof(tagData),  MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
      n = sendto(lstData.stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData), 0, (const struct sockaddr *) &(lstData.stNetWork.addr), lstData.stNetWork.restrict);
      //sendto(lstData.stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData),  MSG_CONFIRM, (const struct sockaddr *) &(lstData.stNetWork.addr), lstData.stNetWork.restrict); 
      // lnReturnVal = SendUDPData(lstData.stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData),  &(lstData.stNetWork.addr), (lstData.stNetWork.restrict));
      //   (int nSockFD,const void* cData,size_t nSize,const struct sockaddr_in* pstSockAddr,socklen_t nSockAddrLen)
      if (0 > n)
      {
         printf("%s", strerror(errno));
         exit(1);
      }
      //#define LOGGING
#ifdef LOGGING        
      printf("%s", strerror(errno));
      printf("Hello message sent.\n");
#endif
      //#undefine LOGGING
      //}
   }
}
#endif 
#ifdef WIN32
#pragma comment (lib,"wsock32.lib")

#pragma pack(1)
using namespace std;

//struct sockaddr_in servaddr, cliaddr;

//
//int CreateUDPSocketIP()
//{
//   int lnSockFD = socket(AF_INET, SOCK_DGRAM, 0);
//   return lnSockFD;
//}
//
//void FillSockAddrin(short sin_family, u_short sin_port, ULONG sin_addr, sockaddr_in* sockaddrin)
//{
//   sockaddrin->sin_family = sin_family;
//   sockaddrin->sin_port = sin_port;
//   sockaddrin->sin_addr.s_addr = sin_addr;
//}
//
//int SendUDPData(int nSockFD, const void* cData, size_t nSize, const struct sockaddr_in* pstSockAddr, int nSockAddrLen)
//{
//   return sendto(nSockFD, (const char *)&cData, nSize, 0, (const struct sockaddr *) pstSockAddr, nSockAddrLen);
//}
//
//int RecvUDPData(int nSockFD, void* cData, size_t nSize, sockaddr_in* pstSockAddr, int* pnSockAddrLen)
//{
//   return recvfrom(nSockFD, (char*)cData, nSize, 0, (struct sockaddr *) pstSockAddr, pnSockAddrLen);
//}


//
//
//map<string, tagData> g_cPortIdentifier;
//
//list<tagData> g_cResponseList;
//
//list<tagData> g_cProcessList;
//CRITICAL_SECTION g_cProcessMutex;
//CRITICAL_SECTION g_cProcessMutexCounter;
//CRITICAL_SECTION g_cResponseMutex;
//
//
//int g_cLockerInt = 0;
//int g_cListLen = 0;
//
//
//int ExecuteFunction(const tagData& stData);
//
//int GetResponseForFunction(tagData& stData);

//
//DWORD WINAPI ProcessThread(LPVOID pArg)
//{
//   tagData lstData = { 0 };
//   int lnReturnVal = 0;
//   int n = 0;
//   while (true)
//   {
//      if(g_cListLen > 0)
//      {
//         EnterCriticalSection(&g_cProcessMutex);
//         while(g_cLockerInt == 1){}
//         if(g_cLockerInt == 0)
//         {
//            g_cLockerInt = 1;
//         }
//         if(g_cListLen <= 0)
//         {
//            if(g_cLockerInt == 1)
//            {
//               g_cLockerInt = 0;
//            }
//            LeaveCriticalSection(&g_cProcessMutex);
//            continue;
//         }
//         lstData = g_cProcessList.back();
//         g_cProcessList.pop_back();
//         EnterCriticalSection(&g_cProcessMutexCounter); 
//         g_cListLen--;
//         LeaveCriticalSection(&g_cProcessMutexCounter);
//         LeaveCriticalSection(&g_cProcessMutex);
//         if (g_cLockerInt == 1)
//         {
//            g_cLockerInt = 0;
//         }
//         int lnVal = ExecuteFunction(lstData);
//         if (lnVal != 0)
//         {
//            exit(1);
//         }
//
//         lnReturnVal = GetResponseForFunction(lstData);
//         if (0 > lnReturnVal)
//         {
//            cout << "wrong response code" << endl;
//            //printf("%s", strerror(errno));
//            // exit(1);
//         }
//         EnterCriticalSection(&g_cResponseMutex);
//         g_cResponseList.push_back(lstData);
//         
//         LeaveCriticalSection(&g_cResponseMutex);
//      }
//   }
//}
//
//DWORD WINAPI SenderThread(LPVOID pArg)
//{
//    tagData lstData = {0};
//    int lnReturnVal = 0;
//    int n = 0;
//    while(true)
//    {
//       //if(!g_cResponseList.empty())
//       //{
//           EnterCriticalSection(&g_cResponseMutex);
//           if(g_cResponseList.empty())
//           {
//              LeaveCriticalSection(&g_cResponseMutex);
//               continue;
//           }
//           lstData = g_cResponseList.back();
//           g_cResponseList.pop_back();
//           LeaveCriticalSection(&g_cResponseMutex);
//           if(lnReturnVal != 0)
//           {
//                printf("%s, %d",strerror(errno),__LINE__);
//                perror("unable to take lock");
//               // exit(1);
//           }
//           
//           int lnDataStructSize = sizeof(tagData);
//           char lcBuffer[sizeof(tagData)] = {0};
//           
//           memcpy(&lcBuffer, (char*)&lstData, lnDataStructSize);
//           
//           //sendto(sockfd, (const char *)&lcBuffer, sizeof(tagData),  MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
//           n = sendto(lstData.stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData),  0, (const struct sockaddr *) &(lstData.stNetWork.addr), lstData.stNetWork.restrict); 
//           //sendto(lstData.stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData),  MSG_CONFIRM, (const struct sockaddr *) &(lstData.stNetWork.addr), lstData.stNetWork.restrict); 
//          // lnReturnVal = SendUDPData(lstData.stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData),  &(lstData.stNetWork.addr), (lstData.stNetWork.restrict));
//                                 //   (int nSockFD,const void* cData,size_t nSize,const struct sockaddr_in* pstSockAddr,socklen_t nSockAddrLen)
//           if(0 > n)
//           {
//               printf("%s", strerror(errno));
//               exit(1);
//           }
////#define LOGGING
//#ifdef LOGGING        
//           printf("%s", strerror(errno));
//           printf("Hello message sent.\n");
//#endif
////#undefine LOGGING
//       //}
//    }
//}
//
//int ExecuteFunction(const tagData& stData)
//{
//   long lnMessageCode = stData.nMessageCode;
//   switch (lnMessageCode)
//   {
//      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER) :
//      {
//        bool lbVal = (g_cPortIdentifier.insert(pair<string, tagData>(stData.cIdentifier, stData))).second;
//        if (lbVal == false)
//         {
//            printf("theres an error");
//            exit(-1);
//         }
//        return 0;
//      }
//                                                                          break;
//
//       case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET) :
//       {
//         //            bool lbVal = (g_cPortIdentifier.insert(pair<string,tagData>(stData.cIdentifier,stData))).second;
//         //            if(lbVal == false)
//         //            {
//         //                printf( "theres an error");
//         //                exit(-1);
//         //            }
//         map<string, tagData>::iterator lcIterator = g_cPortIdentifier.find(stData.cIdentifier);
//         if (lcIterator == g_cPortIdentifier.end())
//         {
//
//         }
//
//         tagData& lcData = lcIterator->second;
//         strncpy(lcData.cTarget, stData.cTarget, 20);
//         //            map<string, tagData>::iterator   lcitr =  g_cPortIdentifier.erase(lcIterator);
//         //            if(lcitr == g_cPortIdentifier.end())
//         //            {
//         //                
//         //            }
//         return 0;
//      }
//                                                                                 break;
//
//      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT) :
//      {
//         char lcBuffer[sizeof(tagData)] = { 0 };
//
//         tagData lstData = stData;
//         lstData.nMessageCode = (long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MESSAGE;
//         memcpy(&lcBuffer, (char*)&lstData, sizeof(tagData));
//         map<string, tagData>::iterator lcIter = g_cPortIdentifier.find(stData.cIdentifier);
//         if (lcIter == g_cPortIdentifier.end())
//         {
//            cout << stData.cIdentifier << "not found";
//            exit(1);
//         }
//         map<string, tagData>::iterator lcIter1 = g_cPortIdentifier.find(lcIter->second.cTarget);
//         if (lcIter1 == g_cPortIdentifier.end())
//         {
//            cout << "Target not found";
//            exit(1);
//         }
//         int len, n;
//         n = sendto(lcIter1->second.stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData), 0, (const struct sockaddr *) &(lcIter1->second.stNetWork.addr), lcIter1->second.stNetWork.restrict);
//         if (n <= 0)
//         {
//            printf("%s", strerror(errno));
//            exit(1);
//         }
//         return 0;
//      }
//                                                                      break;
//   }
//
//   return -1;
//}
//
//
//
//int GetResponseForFunction(tagData& stData)
//{
//   long& lnMessageCode = stData.nMessageCode;
//   switch (lnMessageCode)
//   {
//   case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER) :
//   {
//
//      stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_RESPONSE);
//      return 0;
//   }
//                                                                          break;
//
//   case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET) :
//   {
//
//      stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET_RESPONSE);
//      return 0;
//   }
//                                                                                 break;
//
//   case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT) :
//   {
//
//      stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_RESPONSE);
//      return 0;
//   }
//                                                                      break;
//   }
//
//   return -1;
//}
//
//
//int NetWorkInitialize(int& nSockfd)
//{
//   if ((nSockfd = CreateUDPSocketIP()) < 0)//socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
//   {
//      perror("socket creation failed");
//      exit(EXIT_FAILURE);
//   }
//
//
//   memset(&servaddr, 0, sizeof(servaddr));
//   memset(&cliaddr, 0, sizeof(cliaddr));
//
//   FillSockAddrin(AF_INET, htons(PORT), INADDR_ANY, &servaddr);
//
//   if (bind(nSockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
//   {
//      perror("bind failed");
//      exit(EXIT_FAILURE);
//   }
//
//   return EXIT_SUCCESS;
//}

// Driver code 
int main()
{
   g_OS = 2;
   WSADATA w = { 0 };						/* Used to open windows connection */
   unsigned short port_number = 0;			/* Port number to use */
   int a1 = 0, a2 = 0, a3 = 0, a4 = 0;					/* Components of address in xxx.xxx.xxx.xxx form */
                                                   //int client_length = 0;					/* Length of client struct */
                                                   //int bytes_received = 0;					/* Bytes received from client */
                                                   //SOCKET sd = 0;							/* Socket descriptor of server */
                                                   //struct sockaddr_in server = { 0 };			/* Information about the server */
                                                   //struct sockaddr_in client = { 0 };			/* Information about the client */
                                                   //char buffer[BUFFER_SIZE] = { 0 };			/* Where to store received data */
                                                   ///struct hostent *hp = NULL;					/* Information about this computer */
                                                   //char host_name[256] = { 0 };				/* Name of the server */
                                                   //time_t current_time = 0;				/* Current time */
                                                   //CRITICAL_SECTION g_cProcessMutex;
                                                   //CRITICAL_SECTION g_cProcessMutexCounter;
                                                   //CRITICAL_SECTION g_cResponseMutex;

   InitializeCriticalSection(&g_cProcessMutex);



   InitializeCriticalSection(&g_cProcessMutexCounter);



   InitializeCriticalSection(&g_cResponseMutex);

   /* Interpret command line */
   a1 = 127;
   a2 = 0;
   a3 = 0;
   a4 = 1;

   //if (argc == 2)
   //{
   //   /* Use local address */
   //   if (sscanf(argv[1], "%u", (unsigned int*)&port_number) != 1)
   //   {
   //      usage();
   //   }
   //}
   //else if (argc == 3)
   //{
   //   /* Copy address */
   //  if (sscanf(argv[1], "%d.%d.%d.%d", &a1, &a2, &a3, &a4) != 4)
   //   {
   //      usage();
   //   }
   //   if (sscanf(argv[2], "%u", (unsigned int*)&port_number) != 1)
   //   {
   //      usage();
   //   }
   //}
   //else
   // {
   //   usage();
   // }

   ///* Open windows connection */
   if (WSAStartup(0x0101, &w) != 0)
   {
      fprintf(stderr, "Could not open Windows connection.\n");
      exit(0);
   }


   int lnRetVal = 0;
   int sockfd = 0;
   DWORD myThreadID1;
   DWORD myThreadID2;
   //HANDLE myHandle = CreateThread(0, 0, SenderThread, &lstThreadData, 0, &myThreadID);
   HANDLE myHandle1 = CreateThread(0, 0, SenderThread, NULL, 0, &myThreadID1);
   HANDLE myHandle2 = CreateThread(0, 0, ProcessThread, NULL, 0, &myThreadID2);
   // Creating socket file descriptor 
   //if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
   //    if ( (sockfd = CreateUDPSocketIP()) < 0)//socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
   //    {
   //        perror("socket creation failed"); 
   //        exit(EXIT_FAILURE); 
   //    } 
   //      
   //    memset(&servaddr, 0, sizeof(servaddr)); 
   //    memset(&cliaddr, 0, sizeof(cliaddr)); 
   //      
   // Filling server information 
   //    servaddr.sin_family    = AF_INET; // IPv4 
   //    servaddr.sin_addr.s_addr = INADDR_ANY; 
   //    servaddr.sin_port = htons(PORT); 

   //    FillSockAddrin( AF_INET, htons(PORT), INADDR_ANY, &servaddr );

   // Bind the socket with the server address 
   //    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
   //            sizeof(servaddr)) < 0 )
   //    {
   //        perror("bind failed"); 
   //        exit(EXIT_FAILURE); 
   //    } 
   lnRetVal = NetWorkInitialize(sockfd);
   if (lnRetVal != EXIT_SUCCESS)
   {
      perror("Network Initialize failed");
      exit(EXIT_FAILURE);
   }

   int len = 0, n = 0;
   tagData lstData = { 0 };
   //n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t*)&len); 

   while (true)
   {
      memset(&lstData, 0, sizeof(lstData));
      len = sizeof(cliaddr);
      n = recvfrom(sockfd, (char *)&lstData, sizeof(tagData), 0, (struct sockaddr *) &cliaddr, (int*)&len);
      //n = RecvUDPData(sockfd, (char *)&lstData, sizeof(tagData), &cliaddr, (int*)&len);
      if (n <= 0)
      {
         cout << "" << endl;
         exit(1);

      }
#ifdef LOGGING
      printf("%s", strerror(errno));
#endif
      if (lstData.nMessageCode == (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER)
      {
         lstData.lnSockFD = sockfd;
         memcpy(&(lstData.stNetWork.addr), &cliaddr, sizeof(sockaddr_in));
         lstData.stNetWork.fd = sockfd;
         //lstData.stNetWork.addr
         lstData.stNetWork.restrict = sizeof(sockaddr_in);
         lstData.stNetWork.flags = 0;
         // memcpy(&(lstData.stNetWork), &cliaddr, sizeof(cliaddr));
      }
#ifdef LOGGING
      cout << cliaddr.sin_addr.s_addr << endl;
      cout << cliaddr.sin_family << endl;
      cout << cliaddr.sin_port << endl;
      cout << cliaddr.sin_zero << endl;
#endif
#ifndef singlethread
      memcpy(&(lstData.stNetWork.addr), &cliaddr, sizeof(sockaddr_in));
      lstData.stNetWork.fd = sockfd;
      //lstData.stNetWork.addr
      lstData.stNetWork.restrict = sizeof(sockaddr_in);
      lstData.stNetWork.flags = 0;
#ifdef LOGGING        
      cout << "chat data " << lstData.cBuffer << endl;
      cout << "identifier " << lstData.cIdentifier << endl;
      cout << "target " << lstData.cTarget << endl;
#endif        
      EnterCriticalSection(&g_cProcessMutex);
      if (lnRetVal != 0)
      {
         printf("%s %d", strerror(errno), __LINE__);
         exit(1);
      }
      while (g_cLockerInt == 1)
      {

      }
      if (g_cLockerInt == 0)
      {
         g_cLockerInt = 1;
      }
#ifdef LOGGING
      cout << "mtex acquired process main" << endl;
#endif
      g_cProcessList.push_back(lstData);

      EnterCriticalSection(&g_cProcessMutexCounter);
      g_cListLen++;
      LeaveCriticalSection(&g_cProcessMutexCounter);
      if (lnRetVal != 0)
      {
         printf("%s %d", strerror(errno), __LINE__);
         exit(1);
      }

      if (g_cLockerInt == 1)
      {
         g_cLockerInt = 0;
      }
      LeaveCriticalSection(&g_cProcessMutex);
      if (lnRetVal != 0)
      {
         printf("%s %d", strerror(errno), __LINE__);
         exit(1);
      }
#ifdef LOGGING
      cout << "mtex Released process main" << endl;
#endif
#endif
#ifdef singlethread
      int lnVal = ExecuteFunction(lstData);
      if (lnVal != 0)
      {
         exit(1);
      }
      printf("%s", strerror(errno));
      int lnReturnVal = GetResponseForFunction(lstData);
      if (lnReturnVal != 0)
      {
         exit(1);
      }
      //sendto(sockfd, (const char *)hello, strlen(hello),  MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
      int lnDataStructSize = sizeof(tagData);
      char lcBuffer[sizeof(tagData)] = { 0 };
      //lstMajorData.nMessageCode = (long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_RESPONSE;
      memcpy(&lcBuffer, (char*)&lstData, lnDataStructSize);
      len = sizeof(sockaddr_in);
      sendto(sockfd, (const char *)&lcBuffer, sizeof(tagData), 0, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
      printf("%s", strerror(errno));
      printf("Hello message sent.\n");
#endif

   }



   closesocket(sockfd);
   DeleteCriticalSection(&g_cProcessMutex);
   DeleteCriticalSection(&g_cResponseMutex);
   DeleteCriticalSection(&g_cProcessMutexCounter);
   WSACleanup();
   system("pause");
   return 0;
}




#endif
