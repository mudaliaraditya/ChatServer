#include "includes.h"

using namespace std;
#pragma pack(1)




#ifdef WIN32
int InitMutex(LPCRITICAL_SECTION  pcThreadSync)
{
   InitializeCriticalSection(pcThreadSync);
   return 0;
}
#endif


#ifndef WIN32
int InitMutex(pthread_mutex_t* pcThreadSync)
{
   pthread_mutex_init(pcThreadSync, NULL);
   return 0;
}
#endif



#ifdef WIN32
#define CREATE_THREAD(cThreado)\
{\
    cThread* lpstThreadObj = (cThread*)cThreado;\
    HANDLE  lnData = CreateThread(lpstThreadObj->pstThreadAttri, 0, lpstThreadObj->pFunctionPtr, lpstThreadObj->pvThreadParam,0, &(lpstThreadObj->nThreadID));\
    lpstThreadObj->nThreadWaitObject = lnData ;;\
}
#endif

#ifndef WIN32
#define CREATE_THREAD(cThreado)\
{\
   cThread* lpstThreadObj = cThreado;\
   pthread_create(&(lpstThreadObj->nThreadID), lpstThreadObj->pstThreadAttri, lpstThreadObj->pFunctionPtr, lpstThreadObj->pvThreadParam);\
   lpstThreadObj->nThreadWaitObject = lpstThreadObj->nThreadID;\
}

#endif
#ifdef WIN32
#define JOIN_THREAD(cThreado)\
{\
   cThread* lpstThreadObj = cThreado;\
   WaitForSingleObject(lpstThreadObj->nThreadWaitObject,INFINITE);                          \
}
#endif

#ifndef WIN32
#define JOIN_THREAD(cThreado)\
{\
      cThread* lpstThreadObj = cThreado;\
      pthread_join(lpstThreadObj->nThreadWaitObject,NULL);\
}
#endif

#ifdef WIN32
DWORD WINAPI SendThread(LPVOID lpParameter)
{
   SenderThread(lpParameter);
   return 0;
}
#endif
#ifndef WIN32

void* SendThread(void *pData)
{
   SenderThread(pData);
}
#endif


#ifdef WIN32
DWORD WINAPI CheckRespond(LPVOID lpParameter)
{
   CheckResponse(lpParameter);
   return 0;
}
#endif
#ifndef WIN32

void* CheckRespond(void *pData)
{
   CheckResponse(pData);
}
#endif

#ifdef WIN32
#define CLOSESOCKFD(SOCK_FD)\
{\
   closesocket(SOCK_FD);\
}
#endif

#ifndef WIN32
#define CLOSESOCKFD(SOCK_FD)\
{\
   close(SOCK_FD);\
}
#endif

#ifdef WIN32
DWORD WINAPI RecvThread(LPVOID lpParameter)
{
   RecieverThread(lpParameter);
   return 0;
}
#endif
#ifndef WIN32

void* RecvThread(void *pData)
{
   RecieverThread(pData);
}
#endif


#ifdef WIN32

INITIALIZE()\
{\
   WSADATA w = { 0 };\
   if (WSAStartup(0x0202, &w) != 0)\
   {\
      fprintf(stderr, "Could not open Windows connection.\n");\
      exit(0);\
   }\
}
#endif




          


#define COREGEN
int g_nConnectedTest = 0;
long getMicrotime()
{
   struct timeval currentTime;
   gettimeofday(&currentTime, NULL);
   //return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
   return currentTime.tv_usec;
}

#define LOG_LOGGER(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      \
       LOGGINGI(g_cfstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}

#define LOGGINGI(cFileStream,cToBeLogged, ...)\
{\
    \
   {                                                \
      \
      char lcBufferMessage[700] = {0};                       \
      char lcTimeLogBuffer[700] = {0};\
      GETTIMEBUFFER(lcTimeLogBuffer);\
      snprintf(lcBufferMessage,500,cToBeLogged, ##__VA_ARGS__);         \
      (cFileStream <<  lcTimeLogBuffer << " | " << g_PID << " | "<< lcBufferMessage <<  " | "<< __func__ <<"() | " << "thread id :" << pthread_self() << " | "<<__FILE__<< ":"<<__LINE__ <<endl);           \
   }               \
}



#define GETTIMEBUFFER(cTimeBuffer)\
{\
    {\
      time_t g_nTime = 0;\
      g_nTime = time(NULL);\
      long long lnMicro = getMicrotime();\
      struct tm*  lpsttm =  localtime(&g_nTime);\
      char lcBufferTime[200] = {0};                       \
      snprintf(lcBufferTime,200,"%02d:%02d:%02d.%06d %02d-%02d-%04d",lpsttm->tm_hour, lpsttm->tm_min, lpsttm->tm_sec, lnMicro,lpsttm->tm_mday,lpsttm->tm_mon,(lpsttm->tm_year + 1900) );\
      snprintf(cTimeBuffer,500,"%s",lcBufferTime);\
   }\
}

#define TESTLOG(cToBeLogged, ...)             \
{                                                 \
   {                                                \
     LOGGINGI(g_cfstreamDataLogger,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}




int VerifyUniqueness(char* cUniqueMessage);
void SetRand(char* cBuf,int nSize);
string SuffixAppropirateUniqueIdentifier(string lcString,short nCommand);



#ifdef TESTLOGGING
/*
#define TESTLOG(STRING, ...) \
{\*/
  /*printf(STRING, ##__VA_ARGS__);*/ /*DATA_LOGGER(STRING, ##__VA_ARGS__)\
}*/
#endif

#ifndef TESTLOGGING
/*
#define TESTLOG(STRING, ...) \
{\
   \
}*/

#endif



struct tagSharedMem
{
   pthread_mutex_t m_cSharedMemMutex;
   int             nState;
   char            cBuffer[1024 + 1];
};
enum CState {
   STATE_INVALID= 0,
   STATE_INITIALIZE= 1,
   STATE_WRITING_READY= 2,
   STATE_WRITING_DONE= 3,
   STATE_READING_READY= 4,
   STATE_READING_DONE= 5,
};


//int g_nSeqNo = 0;



tagTimeData  GetTimeTag(tagData stData,long long nTime,int lnNo)
{
   tagTimeData stTimeData(nTime,stData,lnNo);
   //stTimeData.m_nTime = lnNo;
   return stTimeData;
}



long GetResendingTime()
{
   if(pConfigObject == NULL)
   {      
          return -1;
   }
   int lnRetVal = GetConfig(CNF_RESEND_TIME_TAG,pConfigObject);
   if(0 != lnRetVal)
   {      
          return -1;
   }
   const char* lcResendTime = ((GetValueForKey(CNF_RESEND_TIME_TAG, CNF_FILE_NAME , pConfigObject)));
   if(lcResendTime == NULL)
   {      
          return -1;
   }
   int lnSize = strlen(lcResendTime);
   char lcBufferResendTime[lnSize + 1];
   memset(lcBufferResendTime, 0, lnSize);
   {
          strncpy(lcBufferResendTime, lcResendTime, lnSize);
   }
   
   if(0 !=DeleteKeyVal((char*)lcResendTime))
   {
          return -1;
   }
   
   long lnResendTime = atol(lcBufferResendTime);
   return lnResendTime;
}

int AddResendingMessageToEventStoreWithCount(tagData stData,int nCount)
{  
   long lnResendTime = GetResendingTime() + time(NULL);
   tagTimeData  lstTimeData = GetTimeTag(stData, lnResendTime, nCount);
   TESTLOG("adding to resender store : resend time %d count %d message code %ld",lnResendTime,nCount,stData.nMessageCode);
   //lstTimeData.m_nTime = lnResendTime ;
   //tagTimeData lstTimeData(time(NULL) + lnResendTime,stData, INT_MAX);
   g_cEventResender.insert(pair<time_t,tagTimeData>(lstTimeData.m_nTime, lstTimeData)); //to Uncomment
   return 0;
   
}


int AddResendingMessageToEventStoreDummy(tagData stData)
{
   return AddResendingMessageToEventStoreWithCount(stData,INT_MAX);
}

int AddResendingMessageToEventStore(tagData stData)
{
   return AddResendingMessageToEventStoreWithCount(stData,DEFAULT_RESEND_COUNT);
}



void GetDateTimeFormat(char* cBuffer,long nLength)
{
   time_t lnTime;
   struct tm*  lpsttm = NULL;
   char lcErrorLogFileName[200] = {0};
   lnTime = time(NULL);
   lpsttm  = gmtime(&lnTime);
   snprintf(cBuffer,nLength,"%02d-%02d-%04d_%02d%02d%02d.%s",lpsttm->tm_mday,lpsttm->tm_mon +1  ,lpsttm->tm_year + 1900,lpsttm->tm_hour,lpsttm->tm_min,lpsttm->tm_sec,"log");
}


void AddErrorLogSuffix(char* cBuffer,long nLength)
{
     // char lcBuffer[nLength + 1];
     // snprintf(cBuffer,nLength,"%s",cBuffer);
}

int InitiateLogging()
{
   //LOG File Handling START
   time_t lnTime;
   struct tm*  lpsttm = NULL;
   char lcErrorLogFileName[200] = {0};
   lnTime = time(NULL);
   lpsttm  = gmtime(&lnTime);
   //snprintf(lcBuffera,200,"%d:%d:%d%d-%d-%d",psttm->tm_hour,psttm->tm_min,psttm->tm_sec,psttm->tm_mday,psttm->tm_mon,psttm->tm_year );
   snprintf(lcErrorLogFileName,200,"%s/%s-%02d-%02d-%04d_%02d%02d%02d.%s","Logs","log",lpsttm->tm_mday,lpsttm->tm_mon +1  ,lpsttm->tm_year + 1900,lpsttm->tm_hour,lpsttm->tm_min,lpsttm->tm_sec,"log");
   TESTLOG( "%s %s"," log file name ", lcErrorLogFileName );
   g_cfstream.open(lcErrorLogFileName,ios::in|ios::out | ios::app);
   if(g_cfstream.fail())
   {
     LOG_LOGGER("%s","g_cfstream failed truncating or creating new file");
      LOG_LOGGER( "%s","g_cfstream failed truncating or creating new file");
      g_cfstream.open(lcErrorLogFileName,ios::in|ios::out|ios::trunc);
      if(g_cfstream.fail())
      {
          return EXIT_FAILURE;
      }
      g_cfstream.clear();
   }
   g_cfstream.seekp(ios::end); //output
   g_cfstream.seekg(ios::end);

   char lcDataLogFileName[200] = {0};
   snprintf(lcDataLogFileName, 200, "%s/%s-%02d-%02d-%04d_%02d%02d%02d.%s", "Logs", "data", lpsttm->tm_mday, lpsttm->tm_mon + 1  , lpsttm->tm_year + 1900, lpsttm->tm_hour, lpsttm->tm_min, lpsttm->tm_sec,"log");
   g_cfstreamDataLogger.open(lcDataLogFileName,ios::in|ios::out | ios::app);
   if(g_cfstreamDataLogger.fail())
   {
      LOG_LOGGER("%s","g_cfstreamDataLogger failed truncating or creating new file");
      LOG_LOGGER( "g_cfstreamDataLogger failed truncating or creating new file");
      g_cfstreamDataLogger.open(lcDataLogFileName, ios::in|ios::out|ios::trunc);
      if(g_cfstream.fail())
      {
          return EXIT_FAILURE;
      }
      g_cfstreamDataLogger.clear();
   }
   g_cfstreamDataLogger.seekp(ios::end); //output
   g_cfstreamDataLogger.seekg(ios::end);

   return 0;
}
tagBufferData ConvertToNetworkBuffer(tagData& stData)
{
   tagBufferData lstBufferData = {0};
   lstBufferData.nCommand = stData.nCommand;
   lstBufferData.nGlobalIdentifier = stData.nGlobalIdentifier;
   strncpy(lstBufferData.cIdentifier ,stData.cIdentifier ,20);
   lstBufferData.nFrOrToServerFlg = stData.nFrOrToServerFlg;
   lstBufferData.nMessageCode = stData.nMessageCode;
   strncpy(lstBufferData.cBuffer, stData.cBuffer, MAXLINE);
   strncpy(lstBufferData.cTarget, stData.cTarget, 20 );
   strncpy(lstBufferData.cUniqueMessageIdentifier,stData.cUniqueMessageIdentifier ,30);
   lstBufferData.nSeqNo = stData.nSeqNo;
   lstBufferData.bFinalResponse = stData.bFinalResponse;
   lstBufferData.nLatestClntSeqNo = stData.nLatestClntSeqNo;
   lstBufferData.nSessionId = stData.nSessionId;
   return lstBufferData;
}


tagData ConvertToDataStruct(tagBufferData& stData)
{
   tagData lstBufferData = {0};
   lstBufferData.nCommand = stData.nCommand;
   lstBufferData.nGlobalIdentifier = stData.nGlobalIdentifier;
   strncpy(lstBufferData.cIdentifier ,stData.cIdentifier ,20);
   lstBufferData.nFrOrToServerFlg = stData.nFrOrToServerFlg;
   lstBufferData.nMessageCode = stData.nMessageCode;
   strncpy(lstBufferData.cBuffer, stData.cBuffer, MAXLINE);
   strncpy(lstBufferData.cTarget, stData.cTarget, 20 );
   strncpy(lstBufferData.cUniqueMessageIdentifier,stData.cUniqueMessageIdentifier ,30);
   lstBufferData.nSeqNo = stData.nSeqNo;
   lstBufferData.bFinalResponse = stData.bFinalResponse;
   lstBufferData.nLatestClntSeqNo = stData.nLatestClntSeqNo;
   lstBufferData.nSessionId = stData.nSessionId;
   return lstBufferData;
}



void* CheckResponse(void*)
{
   char lcUniqueIdentifierBuffer[30 + 1] = {0};
   int lnSleeptIme = 0;
   while(g_bProgramShouldWork == true)
   {

          pthread_mutex_lock(&g_ReSenderMutex);
          pthread_mutex_lock(&g_SenderMutex);
          if(!g_cEventResender.empty())
          {
            CEventResenderStoreIterator lcIter = g_cEventResender.begin();
            TESTLOG("iterator time %d \n", lcIter->first);
            TESTLOG("current time %d \n",time(NULL));
            if(lcIter->first <= time(NULL) )
            {


               if((long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY == lcIter->second.stData.nMessageCode)
               {
                      TESTLOG("Sending dummy message \n");
                      do
                      {
                        SetRand(lcUniqueIdentifierBuffer, 30);
                      }while(VerifyUniqueness(lcUniqueIdentifierBuffer) != 0);
                      strncpy(lcIter->second.stData.cUniqueMessageIdentifier, lcUniqueIdentifierBuffer,strlen(lcIter->second.stData.cUniqueMessageIdentifier)); 
               }
               pthread_mutex_lock(&g_GlobalSeqnoMutex);
               lcIter->second.stData.nLatestClntSeqNo = g_nLatestRecivedSequenceNo;
               pthread_mutex_unlock(&g_GlobalSeqnoMutex);
               
               TESTLOG("resending data \n");
               g_cSenderDataStore.push_front(lcIter->second.stData);
               if(lcIter->second.m_nCounter > 1)
               {
                      TESTLOG("Resending Data :%s %d %s %d %s %d %s %s %s %d","Message Code:", lcIter->second.stData.nMessageCode,"the seq no is ", lcIter->second.stData.nSeqNo , " the LatestRecieved Seq no is " , lcIter->second.stData.nLatestClntSeqNo , " from user id and name ", lcIter->second.stData.cIdentifier ," " , lcIter->second.stData.nGlobalIdentifier);
                      lcIter->second.m_nCounter--;
                      lcIter->second.m_nTime = time(NULL) + 3;
                      g_cEventResender.insert(pair<time_t,tagTimeData>(lcIter->second.m_nTime,lcIter->second));
               }
               lnSleeptIme = lcIter->first;
               g_cEventResender.erase(lcIter);
            }

          }

          pthread_mutex_unlock(&g_SenderMutex);
          pthread_mutex_unlock(&g_ReSenderMutex);
          //checks for resender events only once a second
          //sleep(1);
          //sleep(lnSleeptIme - time(NULL));
          this_thread::sleep_for(std::chrono::seconds(lnSleeptIme - time(NULL)));
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


int VerifyUniqueness(char* cUniqueMessage)
{
   int lnCompRetVal = 0;
   for(CEventResenderStoreIterator lcItertagTimeDataStore = g_cEventResender.begin() ;lcItertagTimeDataStore != g_cEventResender.end() ;lcItertagTimeDataStore++)
   {
          tagTimeData& lstagTimeData =(lcItertagTimeDataStore->second);
          lnCompRetVal = strncmp(lstagTimeData.stData.cUniqueMessageIdentifier, cUniqueMessage, strlen(cUniqueMessage));
          if(lnCompRetVal == 0)
          {
            return -1;
          }
   }
   return 0;
}


void SetUniqueRand(char* cBuf,int nSize)
{
   nSize--;
   int lnCharHalf = ceil(nSize/(13.33));
   for(int lnCounter = 0;lnCounter<nSize ;lnCounter++)
   {
          if(lnCounter < (nSize - lnCharHalf))
          {
            cBuf[lnCounter] = '0' + (rand() + (long long)&nSize) %10;
          }
          else
          {
            cBuf[lnCounter] = 'a' + (rand() + (long long)&nSize)%26;
          }
   }
}



///ExecuteResponse
///the endpoint ofa message
///this will be the last place where a message would end/cause effect
int ExecuteResponse(tagData& stData)
{
   long lnMessageCode = stData.nMessageCode;
   switch(lnMessageCode)
   {

          case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_RESPONSE):
            {
               TESTLOG("response recieved %d \n",stData.nGlobalIdentifier);
               TESTLOG("Identifier Registered %s \n",stData.cIdentifier);
               g_nGlobalIdentifier = stData.nGlobalIdentifier;
               g_bWaitForResponse = false;
            }
            break;

          case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET_RESPONSE):
            {
               g_nSessionId = stData.nSessionId;
               TESTLOG( "target fixed \n");
               g_bWaitForResponse = false;
            }
            break;

          case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MESSAGE):
            {
               //printf("%s : %s", stData.cIdentifier, stData.cBuffer );
               //pthread_mutex_lock(&g_ConsoleIOMutex);
                cout << stData.cIdentifier << " : "<< stData.cBuffer << endl;
                //TESTLOG("%s : %s" ,stData.cIdentifier , stData.cBuffer);
                TESTLOG("%d %s",stData.nSeqNo," : the current seq no ");
               //pthread_mutex_unlock(&g_ConsoleIOMutex);
               tagData lstSentDeliveryMessageData = {0};
               //g_cAllMessageStore.insert( stData);
               lstSentDeliveryMessageData.nGlobalIdentifier         = stData.nGlobalIdentifier;
               lstSentDeliveryMessageData.nCommand                  =(short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY;
               
               strncpy(lstSentDeliveryMessageData.cIdentifier, stData.cIdentifier, 20);
               
               lstSentDeliveryMessageData.nFrOrToServerFlg=(long)CEToFromServer::CEToFromServer_CLNT_TO_SERV;
               //informing the server that the message has been delivered 
               lstSentDeliveryMessageData.nMessageCode=(long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY;
               
               strncpy(lstSentDeliveryMessageData.cTarget, stData.cTarget, 20);//here target means the client/reciever
               strncpy(lstSentDeliveryMessageData.cUniqueMessageIdentifier, stData.cUniqueMessageIdentifier, 30);
               lstSentDeliveryMessageData.stNetWork.fd=g_nSockFd;
               //lstData.fd =g_nSockFd;
               lstSentDeliveryMessageData.stNetWork.n =sizeof(tagData);
               lstSentDeliveryMessageData.stNetWork.flags=MSG_WAITALL;
               lstSentDeliveryMessageData.stNetWork.addr =servaddr;
               lstSentDeliveryMessageData.stNetWork.restrict=sizeof(servaddr);
               //UDPChatServer 20-01-2019
               //UDPChatServer 20-01-2019
               lstSentDeliveryMessageData.nSeqNo=stData.nSeqNo;
               TESTLOG("sending delivery data message to sender threada \n");
               pthread_mutex_lock(&g_SenderMutex);
               //TESTLOG( "taking Sender lock : %d \n", __LINE__);
               g_cSenderDataStore.push_back(lstSentDeliveryMessageData);
               pthread_mutex_unlock(&g_SenderMutex);
               //TESTLOG("releasing Sender lock %d \n",__LINE__);

            }
            break;
          //final transcode indicating that the message has reached the client
          case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_SERV_TO_CLI:
            {
               g_nConnectedTest = 1;
               TESTLOG( "msg delivered \n");
               TESTLOG("\n %s : %s \n", stData.cIdentifier, stData.cBuffer);
               //cout << endl <<stData.cIdentifier << " : " <<stData.cBuffer << endl;
            }
            break;

          case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY_RES:
            {
               TESTLOG("delivery mesage reachd server \n");
               //cout << "Delivery Confirmation" << endl;
               TESTLOG(" %d %s",stData.nSeqNo, " : the current seq no Delivery Confirmation");
            }
            break;

          case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_RESPONSE):
            {
               TESTLOG("chat sent \n"); 
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


#ifndef WIN32

//The job of this function is to erase the sender request when the final response has arrived
int MakeReSender(tagData lstRecvData )
{

   if(lstRecvData.bFinalResponse == true)
   {
          pthread_mutex_lock(&g_ReSenderMutex);
          //for(list<tagTimeData>::iterator lcIter =g_cEventResender.begin();lcIter != g_cEventResender.end();)
          for(CEventResenderStoreIterator lcIter =g_cEventResender.begin();lcIter != g_cEventResender.end();)
          {

            //tagTimeData& lstData = *lcIter;
            tagTimeData& lstData = lcIter->second;
            if(strcmp(lstData.stData.cUniqueMessageIdentifier,lstRecvData.cUniqueMessageIdentifier) == 0)
            {
               TESTLOG( "response recieved now erasing from resender store\n");
               TESTLOG( "Details of the recieved message the details are nCommand %d nGlobalIdentifier         %ld cIdentifier               %s nFrOrToServerFlg          %d nMessageCode              %ld cBuffer                   %s cTarget                   %s cUniqueMessageIdentifier  %s nSeqNo                    %d  bFinalResponse           %d nLatestClntSeqNo          %d",lstRecvData.nCommand,lstRecvData.nGlobalIdentifier,lstRecvData.cIdentifier,lstRecvData.nFrOrToServerFlg,lstRecvData.nMessageCode,lstRecvData.cBuffer,lstRecvData.cTarget,lstRecvData.cUniqueMessageIdentifier,lstRecvData.nSeqNo,lstRecvData.bFinalResponse,lstRecvData.nLatestClntSeqNo);
               TESTLOG( "Details of the resender store message the details are nCommand %d nGlobalIdentifier         %ld cIdentifier               %s nFrOrToServerFlg          %d nMessageCode              %ld cBuffer                   %s cTarget                   %s cUniqueMessageIdentifier  %s nSeqNo                    %d  bFinalResponse           %d nLatestClntSeqNo          %d",lstData.stData.nCommand,lstData.stData.nGlobalIdentifier,lstData.stData.cIdentifier,lstData.stData.nFrOrToServerFlg,lstData.stData.nMessageCode,lstData.stData.cBuffer,lstData.stData.cTarget,lstData.stData.cUniqueMessageIdentifier,lstData.stData.nSeqNo,lstData.stData.bFinalResponse,lstData.stData.nLatestClntSeqNo);
               lcIter = g_cEventResender.erase( lcIter);
            }
            else
            {
               lcIter++;
            }
          }
          pthread_mutex_unlock(&g_ReSenderMutex);
   }
   return 0;
}

//this func is located for SenderThread if this message is unique then it is added to resender store
bool IsMessageUnique(tagData stData)
{
   int lnRetVal = 0;

   pthread_mutex_lock(&g_ReSenderMutex);
   bool lbUniqueMessageSend = true;

   //for(auto& lcTemp : g_cEventResender)
   for(CEventResenderStoreIterator lcIter =g_cEventResender.begin();lcIter != g_cEventResender.end();lcIter++)
   {
          //if(strcmp(lcTemp.stData.cUniqueMessageIdentifier, stData.cUniqueMessageIdentifier) == 0)
          if(strcmp(lcIter->second.stData.cUniqueMessageIdentifier, stData.cUniqueMessageIdentifier) == 0)   
          {
            //cout << "Resending Message as no response received yet" << endl;
            lbUniqueMessageSend = false;
            break;
          }
   }

   if(lbUniqueMessageSend == true)
   {
         lnRetVal = AddResendingMessageToEventStore(stData);
         if(lnRetVal != 0)
         {
            exit (lnRetVal);
         }
         // tagTimeData lstTimeData((time(NULL) + 10), stData);
         // g_cEventResender.insert(pair<time_t,tagTimeData>(lstTimeData.m_nTime,lstTimeData));
   }
   pthread_mutex_unlock(&g_ReSenderMutex);
   return lbUniqueMessageSend;
}



void* SenderThread(void* pVData)
{
   int lnDataRecievedLen = 0;
   tagData* lpstData = (tagData*)pVData;

   tagNetworkThread& lstThread = lpstData->stNetWork;

   int lnLen = 0;
   tagData lstToSendData = {0};

   int lnRetVal = 0;
   bool lbUniqueMessageSend = false;
   while(g_bProgramShouldWork == true)
   {
          lbUniqueMessageSend = true;

          memset(&lstToSendData, 0 ,sizeof(tagData)); 

          pthread_mutex_lock(&g_SenderMutex);
          if(!g_cSenderDataStore.empty())
          {
            lstToSendData = g_cSenderDataStore.front();
            g_cSenderDataStore.pop_front();
            pthread_mutex_unlock(&g_SenderMutex);
            TESTLOG ("Releasing sender lock %d \n",__LINE__);

            if(g_nFlagNoResendDupli == 0 && lstToSendData.nMessageCode != (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY)
            {
               bool lbUniqueMessageSend = IsMessageUnique(lstToSendData);
            }
            if(lstToSendData.nMessageCode == (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY)
            {
               TESTLOG( "sending data delivery; %d \n",lstThread.fd);
            }
            int lnDataStructSize = sizeof(tagBufferData);
            char lcBuffer[lnDataStructSize];
            memset(lcBuffer, 0, lnDataStructSize);
            tagBufferData lstBufferData = ConvertToNetworkBuffer(lstToSendData) ;
            lnRetVal = Encrypt(reinterpret_cast<char*>(&lstBufferData),sizeof(lstBufferData));
            if(lnRetVal != 0)
            {
               TESTLOG("%s","error");
               exit(1);
            }
            memcpy(&lcBuffer, (char*)&lstBufferData, lnDataStructSize);
            lnLen = sendto(lstThread.fd, (const char *)&lcBuffer, lnDataStructSize, MSG_CONFIRM, (const struct sockaddr *) &(lstThread.addr), sizeof((lstThread.addr))); 
            if(lnLen <= 0)
            {
               TESTLOG("%s","error");
               exit(1);
            }
            
          }

          else
          {
            pthread_mutex_unlock(&g_SenderMutex);
          }
   }
   return NULL;
}




void* RecieverThread(void* pVData)
{
   string lcKey ="";
   int lnDataRecievedLen = 0;
   tagData* lpstData = (tagData*)pVData;

   tagNetworkThread& lstThread = lpstData->stNetWork;

   tagData lstRecvData = {0};

   int lnRetVal = 0;
   bool lbToResend =false;
   bool lbDiscardPacket = false;
   //string lcKey = "";
   while(g_bProgramShouldWork == true)
   {
          lbToResend = true;
          memset(&lstRecvData, 0 ,sizeof(tagData)); 
          tagBufferData lstBufferData;
          lnDataRecievedLen= recvfrom(lstThread.fd, (char *)&lstBufferData, sizeof(tagBufferData), MSG_WAITALL, (struct sockaddr *) &(lstThread.addr),(socklen_t*)&(lstThread.restrict));
          if(0 >= lnDataRecievedLen)
          {
            cout << "data reception error" << endl;
            cout << strerror(errno) << endl;
            LOG_LOGGER("data reception error : [%d] %s ",errno,strerror(errno));
            exit(1);
          }
          lnRetVal = Decrypt(reinterpret_cast<char*>(&lstBufferData),sizeof(lstBufferData));
          if(lnRetVal != 0)
          {
              LOG_LOGGER("%s","Error while decryption");
              exit(1);
          }
          lstRecvData = ConvertToDataStruct(lstBufferData);
#ifdef LOGGING
          TESTLOG("%s \n", lstRecvData.cUniqueMessageIdentifier);
          TESTLOG("%s \n",lstRecvData.cIdentifier );
          TESTLOG("%s \n", lstRecvData.cTarget );
#endif

          pthread_mutex_lock(&g_GlobalSeqnoMutex);
          if(lstRecvData.bFinalResponse == true)
          {
            TESTLOG( "Getting into loop of order packet %d", lstRecvData.nSeqNo);
            TESTLOG( "Current latest seqno is %d", g_nLatestRecivedSequenceNo);
            TESTLOG( "the message code is %d", lstRecvData.nMessageCode);
            TESTLOG( "the Identifer is %s", lstRecvData.cIdentifier);
            TESTLOG( "the Identifer is %s", lstRecvData.cTarget);

            if(lstRecvData.nSeqNo == g_nExpectedSeqNo)
            {
               //g_nLastResRecSeq++;
               g_nExpectedSeqNo++;
               g_nLatestRecivedSequenceNo = lstRecvData.nSeqNo;
            }
            else if(lstRecvData.nSeqNo > g_nLatestRecivedSequenceNo)
            {
                     TESTLOG( "out of order packet %d", lstRecvData.nSeqNo);
                     TESTLOG( "Current latest seqno is %d", g_nLatestRecivedSequenceNo);
                     TESTLOG( "the message code is %d", lstRecvData.nMessageCode);
                     pthread_mutex_unlock(&g_GlobalSeqnoMutex);
                     continue;
                  /*
               //g_nLatestRecivedSequenceNo++;
	       if (g_bOutOfOrder == false)
	       {
                  g_bOutOfOrder = true;
	          std::set lcSet;
	          lcSet.insert(lstRecvData.nSeqNo);
		  long long lnDiff = lstRecvData.nSeqNo - g_nLatestRecivedSequenceNo;
		  while(lnDiff--)
		  {
		     lcSet.insert( lstRecvData.nSeqNo + lnDiff);
		  }
                  g_cSeqStoreForUnOrderedSeqNo.insert(pair<lstRecvData.nSeqNo,lcSet>);
		  g_cSeqNoUnOrderedPacketStore.insert( pair< int,tagData > ( lstRecvData.nSeqNo, lstRecvData) );
	       }
	       else
	       {
                  int lnNewAdditionFlag = 0;
                  CSequenceStorebyUnOrderedSeqNoIter lcIter = g_cSeqStoreForUnOrderedSeqNo.rbegin();
                  int lnNo = lcIter->first;
                  if (lnNo > lstRecvData.nSeqNo)
                  {
                         lnNewAdditionFlag  = 1;
                  }
                  //else
                  {
                     
                     
                  }
                  
//		  for(CSequenceStorebyUnOrderedSeqNoIter lcIter = g_cSeqStoreForUnOrderedSeqNo.begin();lcIter != g_cSeqStoreForUnOrderedSeqNo.end();lcIter++)
//		  {
//		     int lnNo = lcIter->first;
//                     if (lnNo > lstRecvData.nSeqNo)
//                     {
//                         lnNewAdditionFlag  = 1;
//                         break;
//                     }
//                     else if(lnNo == lstRecvData.nSeqNo)
//                     {
//                        lnNewAdditionFlag   = -1;
//                        break;
//                     }
//		  }
                  if(lnNewAdditionFlag > 0)
                  {
                     std::set lcSet;
	             lcSet.insert(lstRecvData.nSeqNo);
		     long long lnDiff = lstRecvData.nSeqNo - lnNo;
		     while(lnDiff--)
		     {
		        lcSet.insert( lstRecvData.nSeqNo + lnDiff);
		     }
                     g_cSeqStoreForUnOrderedSeqNo.insert(pair<lstRecvData.nSeqNo,lcSet>);
		     g_cSeqNoUnOrderedPacketStore.insert( pair< int,tagData > ( lstRecvData.nSeqNo, lstRecvData) );
                  }
                  else
                  {
                     
                  }
		 
	       }*/
            }
            else if(lstRecvData.nSeqNo < g_nLatestRecivedSequenceNo)
            {
                     TESTLOG( "out of order packet %d", lstRecvData.nSeqNo);
                     TESTLOG( "Current latest seqno is %d", g_nLatestRecivedSequenceNo);
                     TESTLOG( "the message code is %d", lstRecvData.nMessageCode);
                     pthread_mutex_unlock(&g_GlobalSeqnoMutex);
                     continue;
               //continue;
            }
          }
          pthread_mutex_unlock(&g_GlobalSeqnoMutex);

          if(g_nFlagNoResendDupli == 0)
          {
            pthread_mutex_lock( &g_cIdentifierMutex);
            //if(lpstData->nCommand != (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY)
            {
               for(auto& lstIdentifiers : g_cIdentifierStore)
               {
                      lcKey = SuffixAppropirateUniqueIdentifier(lstRecvData.cUniqueMessageIdentifier, lstRecvData.nCommand);
                      if(0 == strcmp(lstIdentifiers.c_str(), lcKey.c_str()))
                      {
                        TESTLOG("duplicate packet \n");
                        TESTLOG("%s %s %s %d %s %d",lstIdentifiers.c_str() ," duplicated ","rejected code ",lpstData->nMessageCode," command : " ,lpstData->nCommand);
                        lbDiscardPacket = true;
                        break;
                      }
               }
               if(true == lbDiscardPacket)
               {
                      //delete lpstData;
                      //lpstData = nullptr;
                      lbDiscardPacket = false;
                      pthread_mutex_unlock(&g_cIdentifierMutex);
                      continue;
               }
            }
            if(lstRecvData.bFinalResponse == true)
            {
               //pthread_mutex_lock();
               pthread_mutex_lock(&g_ReSenderMutex);
               //for(list<tagTimeData>::iterator lcIter = g_cEventResender.begin(); lcIter != g_cEventResender.end();)
               for(CEventResenderStoreIterator lcIter = g_cEventResender.begin(); lcIter != g_cEventResender.end();)
               {
                      //tagTimeData& lstData = *lcIter; 
                      tagTimeData& lstData = lcIter->second;
                      if(strcmp(lstData.stData.cUniqueMessageIdentifier, lstRecvData.cUniqueMessageIdentifier) == 0)
                      {
                        TESTLOG( "response recieved now erasing from resender store early\n");
                        TESTLOG( "data being removed for %s mesage code %d seqno %d current latestseqno %d \n",lstData.stData.cIdentifier,lstData.stData.nMessageCode,lstData.stData.nSeqNo,lstData.stData.nLatestClntSeqNo);
                        lcIter = g_cEventResender.erase(lcIter);
                      }
                      else
                      {
                        lcIter++;
                      }
               }
               pthread_mutex_unlock(&g_ReSenderMutex);
            }
            // else
            {
               lcKey = SuffixAppropirateUniqueIdentifier(lstRecvData.cUniqueMessageIdentifier,lstRecvData.nCommand);
               g_cIdentifierStore.push_back(lcKey);
            }
            //else
            {
               //cout << "delivery Operation recive" << endl;
            }
            pthread_mutex_unlock( &g_cIdentifierMutex);
          }

          lnRetVal = ExecuteResponse(lstRecvData);
          if(lnRetVal == -1)
          {
            TESTLOG("response handling error \n");
            LOG_LOGGER("response handling error for message code %d",lstRecvData.nMessageCode);
            exit(1);
          }

          if(MakeReSender(lstRecvData) != EXIT_SUCCESS)
          {
            LOG_LOGGER("")
               exit(1);
          }


          TESTLOG("Server : %d\n", lstRecvData.nMessageCode);
   }
   return NULL; 
}


//This functions does the job of which message code to be sent next
//UDpChatServer 21/12/2018 Aditya M.:START
int PreSender(tagData& stData)
{
  stData.nFrOrToServerFlg = 100001;
  stData.nSessionId = g_nSessionId;
  stData.nGlobalIdentifier = g_nGlobalIdentifier;
  TESTLOG ("lstData.nGlobalIdentifier is set as %d \n", stData.nGlobalIdentifier); 
   // g_ConditionalVarReciever
   switch(stData.nMessageCode)
   {
          case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER:
            {
               //pthread_mutex_lock(&g_ConsoleIOMutex);
               cout << "Enter your Identifier ID" << endl;
               //pthread_mutex_unlock(&g_ConsoleIOMutex);
               if(g_nTesting == 1)
               {
                      //strncpy( lstData.cTarget, "QWE", 3);
                      strncpy( stData.cIdentifier, g_pcParam[2], 4);
               }
               else
               {
                      //pthread_mutex_lock(&g_ConsoleIOMutex);
                      //while ((getchar()) != '\n');
                      cin >> stData.cIdentifier;
                      //pthread_mutex_unlock(&g_ConsoleIOMutex);
                      //strncpy( lstData.cIdentifier, "ABC", 3);
               }

               memcpy(g_cIdentifier, stData.cIdentifier, 20);
               //
               stData.nCommand = (short)CCOMMAND_TYPE::CCOMMAND_TYPE_REQUEST;
               stData.nFrOrToServerFlg = (long)CEToFromServer::CEToFromServer_CLNT_TO_SERV;
               //sleep(5);
            }
            break;
          case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET:
            {
               TESTLOG("Registering message sent.\n");

               memcpy(stData.cIdentifier, g_cIdentifier, 20);

               stData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET;
               //pthread_mutex_lock(&g_ConsoleIOMutex);

               cout << "Enter Target Identifier" << endl;
               //pthread_mutex_unlock(&g_ConsoleIOMutex);
               //cin >> stData.cTarget; 
               if(g_nTesting == 1)
               {
                      // strncpy( stData.cTarget, "QWE", 3);
                      strncpy( stData.cTarget, g_pcParam[3], 4);
               }
               else
               {
                      //while ((getchar()) != '\n');
                      //pthread_mutex_lock(&g_ConsoleIOMutex);
                      cin >> stData.cTarget;
                      //pthread_mutex_unlock(&g_ConsoleIOMutex);
               }
               stData.nCommand = (short)CCOMMAND_TYPE::CCOMMAND_TYPE_REQUEST;
               stData.nFrOrToServerFlg = (long)CEToFromServer::CEToFromServer_CLNT_TO_SERV;
               //sleep(2);
            }
            break;
          //To send chats
          case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT:
            {
               stData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT;
               strncpy(stData.cIdentifier, g_cIdentifier, 20);
               if(g_nConnectedTest == 0)
               {
                    g_nConnectedTest = -1;           
               }
               else 
               if(g_nConnectedTest == -1 && g_nTesting == 1)
               {
                        while(g_nConnectedTest == -1){}
               }

               if(g_nTesting == 1 && g_nConnectedTest == 1)
               {
                      usleep(100000);
                      static int x = 0;
                      //strncpy(stData.cBuffer, g_cIdentifier, 20);
                      snprintf(stData.cBuffer,50,"%s%d",g_cIdentifier,x++);
               }
               else if(g_nTesting == 1)
               {
                      //cout << "";
               }
               else
               {
                                                      //pthread_mutex_lock(&g_ConsoleIOMutex);
                               //pthread_mutex_unlock(&g_ConsoleIOMutex);
                               //cin>> stData.cBuffer; 
                               //while ((getchar()) != '\n');
#if 0
                               //pthread_mutex_unlock(&g_ConsoleIOMutex);
                               //string lcInput;
                               //                      char          name[MAXLINE + 1] = {0}; // in case of single character input
                               //                      fd_set       input_set;
                               //                      struct timeval  timeout;
                               //                      int          ready_for_reading = 0;
                               //                      int          read_bytes = 0;

                               //while(true)
                               {
                                       //pthread_mutex_lock(&g_ConsoleIOMutex);
                                       //while(true)
                                       {
                                               /* Empty the FD Set */
                                               //FD_ZERO(&input_set );
                                               /* Listen to the input descriptor */
                                               //FD_SET(STDIN_FILENO, &input_set);

                                               /* Waiting for some seconds */
                                               //timeout.tv_sec = 1; // WAIT seconds//no of seconds to wait
                                               //timeout.tv_usec = 0; // 0 milliseconds

                                               /* Invitation for the user to write something */
                                               //printf("Enter Username: (in %d seconds)\n", 10);
                                               //printf("Time start now!!!\n");

                                               /* Listening for input stream for any activity */
                                               //ready_for_reading = select(1, &input_set, NULL, NULL, &timeout);
                                               /* Here, first parameter is number of FDs in the set, 
                                                * second is our FD set for reading,
                                                * third is the FD set in which any write activity needs to updated,
                                                * which is not required in this case. 

                                                * Fourth is timeout
                                                */

                                               //if (ready_for_reading == -1) 
                                               {
                                                       /* Some error has occured in input */
                                                       //        printf("Unable to read your input\n");
                                                       //        return -1;
                                               }

                                               //if (ready_for_reading) 
                                               //{
                                               //while ((getchar()) != '\n');
                                               //        //g          getline(cin, lcInput);
                                               //        read_bytes = read(STDIN_FILENO, name, MAXLINE);
                                               //        if(name[read_bytes-1]=='\n'){
                                               //                --read_bytes;
                                               //                name[read_bytes]='\0';
                                               //        }
                                               //        if(read_bytes==0)
                                               //        {
                                               //printf("You just hit enter\n");
                                               //        } else
                                               //        {
                                               //printf("Read, %d bytes from input : %s \n", read_bytes, name);
                                               //               pthread_mutex_unlock(&g_ConsoleIOMutex);
                                               //                break;
                                               //        }
                                               //}
                                               // else 
                                               //{
                                               // printf(" %d Seconds are over - no data input \n", 10);
                                               //}
                                       }
                                       //pthread_mutex_unlock(&g_ConsoleIOMutex);
                               }
                               /*  while(!lcInput.empty() || lcInput.c_str()[0] == '\n')
                                   {
                                   getline(cin, lcInput);
                                   }
                                */
                               //strncpy(stData.cBuffer, name, MAXLINE); 
#endif
                       //cin.clear();
                       //cin.ignore(numeric_limits<streamsize>::max(), '\n');
                       cout << "Enter Chat Data" << endl;
                       do{
                               string lcInput = "";
                               getline(cin, lcInput);
                               if(lcInput.empty())
                               {
                                  //cout <<"empty" << endl;
                                  continue;
                               }
                               strncpy(stData.cBuffer, lcInput.c_str(), lcInput.length()); 
                               //if(*(stData.cBuffer) != '\n')
                               if(!lcInput.empty())
                               {
                                       //cout << "sent" << endl;
                                       //cin.clear();
                                       //cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                       break;
                               }
                       //}while(*(stData.cBuffer) == '\n' || *(stData.cBuffer) == '\0');
                       }while(true);
               }
               stData.nCommand = (short)CCOMMAND_TYPE::CCOMMAND_TYPE_REQUEST;
               stData.nFrOrToServerFlg = (long)CEToFromServer::CEToFromServer_CLNT_TO_SERV;
            }
            break;
   }
   return 0;
}
//Acions to perform after Sending
int PostSender(tagData& stData)
{
   switch(stData.nMessageCode)
   {
          case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER:
            {
               g_bWaitForResponse = true;
               stData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET;
               do{}
               while(g_bWaitForResponse == true);
            }
            break;
          case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET:
            {
               g_bWaitForResponse = true;
               stData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT;
               do{}
               while(g_bWaitForResponse == true);
               sleep(5);
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
void HandleSignal(int nSignal)
{
   const char *signal_name;
   signal(SIGINT,HandleSignal);
   sigset_t pending;
   int lnRetVal = 0;

   // Handling SIGINT
   switch (nSignal)
   {
          case SIGINT:
            {
               cout << "signalling thread" << pthread_self() << endl;
               printf("Caught SIGINT, exiting now\n");
               g_bProgramShouldWork = false;
               //CleanUp();
               //lnRetVal = pthread_cond_broadcast(&g_cCondVarForProcessThread);
               // if(lnRetVal != 0)
               //{
               //printf("%s, %d", strerror(errno), __LINE__);
               //perror("unable to join lnPThreadEventTime");
               //exit(EXIT_FAILURE);
               //}
               DeleteNewMap(pConfigObject);
               if(lnRetVal != 0 )
               {
                   printf("failure");
               }
               lnRetVal = pthread_kill(lnPThreadReciever, SIGKILL);
               if (lnRetVal != 0)
               {
                      printf("%s, %d", strerror(errno), __LINE__);
                      perror("unable to kill reciverthread Variable");
                      exit(EXIT_FAILURE);
               }
               //lnPThreadMain
               lnRetVal = pthread_join(lnPThreadReciever, NULL);
               if (lnRetVal != 0)
               {
                      printf("%s, %d", strerror(errno), __LINE__);
                      perror("unable to join reciverthread Variable");
                      exit(EXIT_FAILURE);
               }
               lnRetVal = pthread_join(lnPThreadSender,NULL);
               if (lnRetVal != 0)
               {
                      printf("%s, %d", strerror(errno), __LINE__);
                      perror("unable to destroy Conditional Variable");
                      exit(EXIT_FAILURE);
               }

               lnRetVal = pthread_join(lnPThreadCheckResponse,NULL);
               if (lnRetVal != 0)
               {
                      printf("%s, %d", strerror(errno), __LINE__);
                      perror("unable to destroy Conditional Variable");
                      exit(EXIT_FAILURE);
               }

               if(lpstThrdDataRcvr != NULL)
               {
                      delete lpstThrdDataRcvr;
                      lpstThrdDataRcvr = NULL;
               }

               if( lpstThrdSndr != NULL)
               {
                      delete lpstThrdSndr;
                      lpstThrdSndr = NULL;
               }
               //lnRetVal = pthread_kill(lnPThreadMain, SIGKILL);
               if (lnRetVal != 0)
               {
                      printf("%s, %d", strerror(errno), __LINE__);
                      perror("unable to kill reciverthread Variable");
                      exit(EXIT_FAILURE);
               }
               lnRetVal = pthread_join(lnPThreadMain,NULL);
               if (lnRetVal != 0)
               {
                      printf("%s, %d", strerror(errno), __LINE__);
                      perror("unable to destroy Conditional Variable");
                      exit(EXIT_FAILURE);
               }

               exit(EXIT_SUCCESS);
            }
            break;

          default:
            {
               fprintf(stderr, "Caught wrong signal: %d\n", nSignal);
               return;
            }
   }
}


int GetConfiguration(char* cServerIP,int nServerIPLen ,char* cPort,int nPortLen)
{
   int lnRetVal = 0;
//   pConfigObject = CreateNewMap();
   if(pConfigObject == NULL)
   {
          return -1;
   } 
   //lnRetVal =GetConfig(CNF_FILE_NAME,pConfigObject);
   //if(0 != lnRetVal)
   {
   //       return -1;
   }
   const char* lcPortOfServerOnHeap = ((GetValueForKey(CNF_PORT_TAG, CNF_FILE_NAME , pConfigObject)));
   if(lcPortOfServerOnHeap == NULL)
   {
          return -1;
   }
   int lnSize = strlen(lcPortOfServerOnHeap);
   char lcBufferPort[lnSize + 1];
   memset(lcBufferPort,0,lnSize + 1);
   {
          strncpy(cPort, lcPortOfServerOnHeap, nPortLen);
   }
   if(0 !=DeleteKeyVal((char*)lcPortOfServerOnHeap))
   {
          return -1;
   }

   const char* lcServerIPi=((GetValueForKey(CNF_SERVER_IP_TAG, CNF_FILE_NAME , pConfigObject)));
   if(lcServerIPi == NULL)
   {
          return -1;
   }
   lnSize = strlen(lcServerIPi);
   char lcBufferServerIP[lnSize + 1];
   memset(lcBufferServerIP,0,lnSize + 1);
   {
          strncpy(cServerIP, lcServerIPi, nServerIPLen);
   }
   if(0 !=DeleteKeyVal((char*)lcServerIPi))
   {
          return -1;
   }



}

void* PrintMessagesFromSet(void* cArg)
{
  sleep(30);
  while(g_bProgramShouldWork == true)
  {
    if(g_cAllMessageStore.empty() != true)
    {
       system("clear");
       for (CMessageStoreBySeqNoIter lcIter = g_cAllMessageStore.begin(); g_cAllMessageStore.end() != lcIter; lcIter++)
       {
             tagData lstData = *lcIter;
             if( lstData.nMessageCode == (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MESSAGE)
             {
                cout << lstData.cIdentifier << " : "<< lstData.cBuffer << endl;
             }
       }
    }
    sleep(2);
  }
}

#ifdef COREGEN
static void SetCoreUnlimited()
{
   struct rlimit sRLimit ;
   sRLimit.rlim_cur = RLIM_INFINITY;
   sRLimit.rlim_max = RLIM_INFINITY;
   setrlimit( RLIMIT_CORE, &sRLimit );
}

#endif
//UDpChatServer 21/12/2018 Aditya M.:END
int main(int argc,char* argv[])
{
   g_PID = getpid();
   lnPThreadMain = pthread_self();
   //SIgnal Handling
   signal(SIGINT, HandleSignal);
   struct sigaction lstSigAction = {0};
   lstSigAction.sa_flags = 0;
   lstSigAction.sa_handler= HandleSignal;
   sigaction(SIGINT, &lstSigAction, NULL);
   pConfigObject = CreateNewMap();
   if(pConfigObject == NULL)
   {
          return -1;
   }
   int lnRetVal = 0; 
   lnRetVal =GetConfig(CNF_FILE_NAME,pConfigObject);
   if(0 != lnRetVal)
   {
          return -1;
   }


   lnRetVal = InitiateLogging();
   if(lnRetVal != 0)
   {
      return -1;
   }
   g_nArgs = argc;
   g_nFlagNoResendDupli = 0;
   g_pcParam = argv;
//for random no generation
   srand ((time(NULL)));


   key_t key = ftok("shmfile",65); 

   //shared mem use 
   // shmget returns an identifier in shmid 
   int shmid = shmget(key, sizeof(tagSharedMem),0666|IPC_CREAT); 

   //shared mem handling 
   tagSharedMem* lpstSharedmemData = (tagSharedMem*) shmat(shmid,(void*)0,0);
   memset(lpstSharedmemData, 0,sizeof(tagSharedMem));
   lpstSharedmemData->nState = STATE_INITIALIZE;
   //shared mem use
   //int lnRetVal = 0;
   char lcBufferServerIP[100 + 1];
   char lcBufferPort[100 + 1];
   //char* cServerIP,int nServerIPLen ,char* cPort,int nPortLen
   lnRetVal  = GetConfiguration(lcBufferServerIP,100, lcBufferPort,100);
   if ( 0 > lnRetVal)
   {
      cout << "invalid config" << endl;
      exit(EXIT_FAILURE);
   }
   time_t     lnTime =         0;
              lnTime =         time(NULL);
   struct tm* psttm =          gmtime(&lnTime);
   char       lcBuffera[200] = {0};
   int        lnRandNo =       rand()%100;

   char lcSeqIdentifier[LENGHT_OF_BASE_SEQ_NO + 1] = {0};


   //if the user starts the program the following way
   // ./a.out TEST ABC QWE
   //where ABC will be the identififier and QWE be target
   if(argc == 4)
   {
          if( strncmp("TEST", g_pcParam[1], 4) == 0)
          {
            g_nTesting = 1;
          }
   }

   size_t lnLen = 0;
   if ( (g_nSockFd = CreateUDPSocketIP()) < 0 )
   {
          perror("socket creation failed");
          exit(EXIT_FAILURE);
   }

   FillSockAddrin(AF_INET, htons(atol(lcBufferPort)), (in_addr_t)(inet_addr(lcBufferServerIP)), &servaddr);

   tagData lstData = {0};

   lstData.stNetWork.fd       = g_nSockFd;
   lstData.stNetWork.n        = sizeof(tagData);
   lstData.stNetWork.flags    = MSG_WAITALL;
   lstData.stNetWork.addr     =  servaddr;
   lstData.stNetWork.restrict = sizeof(servaddr);

   lpstThrdDataRcvr = new tagData();
   lpstThrdSndr     = new tagData();
#ifdef LOGGING
   TESTLOG( "sockfd is %d\n", lpstThrdDataRcvr->stNetWork.fd );
#endif
   //sleep(1);
   //Creation Of threads
   memcpy(lpstThrdDataRcvr, &lstData, sizeof(tagData));
   memcpy(lpstThrdSndr, &lstData, sizeof(tagData));

   pthread_create(&lnPThreadReciever, NULL, RecieverThread, lpstThrdDataRcvr);
   pthread_create(&lnPThreadSender, NULL, SenderThread, lpstThrdSndr);

   if(g_nFlagNoResendDupli == 0)
   {
          pthread_create(&lnPThreadCheckResponse, NULL,CheckResponse,NULL);
   }
   //End of Creation of Threads

   lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER;
   {
          tagData lstDummyMessageData = {0};
          lstDummyMessageData.nMessageCode =(long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY;
          SetRand(lstDummyMessageData.cUniqueMessageIdentifier,30);
          lnRetVal = AddResendingMessageToEventStoreDummy(lstDummyMessageData);
          if (lnRetVal != 0)
          {
                  LOG_LOGGER("%s","Error in First Message");
                  return -1;
          }
          //tagTimeData lstTimeData(time(NULL)+3,lstDummyMessageData, INT_MAX) ;
          //g_cEventResender.insert(pair<time_t,tagTimeData>(lstTimeData.m_nTime, lstTimeData)); //to Uncomment 
   }
   while(g_bProgramShouldWork == true)
   {
          do
          {
            SetRand(lstData.cUniqueMessageIdentifier,30);
          }
          while(VerifyUniqueness(lstData.cUniqueMessageIdentifier) != 0);
          pthread_mutex_lock(&g_GlobalSeqnoMutex);
          lstData.nSeqNo = g_nSeqNo++;
         lstData.nLatestClntSeqNo = g_nLatestRecivedSequenceNo;
          pthread_mutex_unlock(&g_GlobalSeqnoMutex);


          lnRetVal = PreSender(lstData);
          if(0 != lnRetVal)
          {
            TESTLOG( "error occured at Presender \n");
            exit(1);
          }

          //Taking Lock so that the other thread doesnt corrupt g_cSenderDataStore
          pthread_mutex_lock(&g_SenderMutex); 
#ifdef LOGGING
          TESTLOG("Taking Sender lock %d" , __LINE__ );
#endif
          //Sending data to the Sender thread
          g_cSenderDataStore.push_back(lstData); 

          // unlocking the mutex
          pthread_mutex_unlock(&g_SenderMutex);
#ifdef LOGGING
          TESTLOG("Releasing Sender lock %d" ,__LINE__ );
#endif
          TESTLOG("%s \n", lstData.cUniqueMessageIdentifier);
          memset (lstData.cBuffer, 0,sizeof(lstData.cBuffer));
          memset (lstData.cUniqueMessageIdentifier, 0, sizeof(lstData.cUniqueMessageIdentifier));

          //shared memory usage
          /* pthread_mutex_lock(&(lpstSharedmemData->m_cSharedMemMutex));
          if(lpstSharedmemData->nState == STATE_INITIALIZE)
          {
          cout << "shared mem data " << lpstSharedmemData->cBuffer << endl;
          strncpy(lpstSharedmemData->cBuffer, "hi", 2);
          lpstSharedmemData->nState = STATE_WRITING_DONE;
          cout << "shared mem data " << lpstSharedmemData->cBuffer << endl;
          }
          pthread_mutex_unlock(&(lpstSharedmemData->m_cSharedMemMutex));*/
          //shared memory usage

          lnRetVal = PostSender(lstData);
          if(0 != lnRetVal)
          {
            TESTLOG("error occured at PostSender \n");
            exit(1);
          }
          //cout << "sent message" << endl;

   }
   DeleteNewMap(pConfigObject);
   if(lnRetVal != 0 )
   {
          printf("failure");
   }
   pthread_join(lnPThreadSender,NULL);
   pthread_join(lnPThreadReciever,NULL);
   delete lpstThrdDataRcvr;
   delete lpstThrdSndr;

   close(g_nSockFd);
   return 0;
}



string SuffixAppropirateUniqueIdentifier(string lcString,short nCommand)
{
   string lcKey = lcString;
   switch(nCommand)
   {
          case (short)CCOMMAND_TYPE::CCOMMAND_TYPE_REQUEST:
            {
               lcKey += lcKey + REQ_SUF_IDN;
            }
            break;

          case (short)CCOMMAND_TYPE::CCOMMAND_TYPE_RESPONSE:
            {
               lcKey += lcKey + RES_SUF_IDN;
            }
            break;

          case (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY:
            {
               lcKey += lcKey + DEQ_SUF_IDN;
            }
            break;

          case (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY_RES:
            {
               lcKey += lcKey + DES_SUF_IDN;
            }
            break;

          case (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY_CONF:
            { 
               lcKey += lcKey + DEC_SUF_IDN;
            }


          default:
            {

            }
            break;

   }
   return lcKey;
}

#endif
