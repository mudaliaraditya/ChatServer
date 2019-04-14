#include "includes.h"


#pragma pack(1)

#define LOG_LOGGER(cToBeLogged, ...)             \
{                                                 \
  {                                                \
   \
   time_t lnTime = time(NULL);\
   struct tm*  psttm =  localtime(&lnTime);\
   char lcBuffer[200] = {0};                       \
   snprintf(lcBuffer,200,"%d:%d:%d %d-%d-%d",psttm->tm_hour,psttm->tm_min,psttm->tm_sec,psttm->tm_mday,psttm->tm_mon,psttm->tm_year );\
    char lcBuffer111[500] = {0};                       \
    snprintf(lcBuffer111,500,cToBeLogged, ##__VA_ARGS__);         \
    (g_cfstream <<  lcBuffer << " | " << lcBuffer111 <<  " | "<< __func__ <<"() | " <<__FILE__<< ":"<<__LINE__ <<endl);           \
  }                                             \
}

#define DATA_LOGGER(cToBeLogged, ...)             \
{                                                 \
  {                                                \
   \
   time_t lnTime = time(NULL);\
   struct tm*  psttm =  localtime(&lnTime);\
   char lcBuffer[200] = {0};                       \
   snprintf(lcBuffer,200,"%d:%d:%d %d-%d-%d",psttm->tm_hour,psttm->tm_min,psttm->tm_sec,psttm->tm_mday,psttm->tm_mon,psttm->tm_year );\
    char lcBuffer111[500] = {0};                       \
    snprintf(lcBuffer111,500,cToBeLogged, ##__VA_ARGS__);         \
    (g_cfstreamDataLogger <<  lcBuffer << " | " << lcBuffer111 <<  " | "<< __func__ <<"() | " <<__FILE__<< ":"<<__LINE__ <<endl);           \
  }                                             \
}



int VerifyUniqueness(char* cUniqueMessage);
void SetRand(char* cBuf,int nSize);
string SuffixAppropirateUniqueIdentifier(string lcString,short nCommand);



#ifdef TESTLOGGING

#define TESTLOG(STRING, ...) \
{\
  /*printf(STRING, ##__VA_ARGS__);*/DATA_LOGGER(STRING, ##__VA_ARGS__)\
}
#endif

#ifndef TESTLOGGING

#define TESTLOG(STRING, ...) \
{                    \
                     \
}

#endif



struct tagSharedMem
{
   pthread_mutex_t m_cSharedMemMutex;
   int             nState;
   char            cBuffer[1024 + 1];
};
enum CState {
               STATE_INVALID        = 0,
               STATE_INITIALIZE     = 1,
               STATE_WRITING_READY  = 2,
               STATE_WRITING_DONE   = 3,
               STATE_READING_READY  = 4,
               STATE_READING_DONE   = 5,
            };
            

int g_nSeqNo = 0;

/// this function is the major event handler
/// here it majorly does the job of resending messages if there is no response yet            
/*void* CheckResponse(void*)
{
   while(true)
   {
      pthread_mutex_lock(&g_ReSenderMutex);
      char lcUniqueIdentifierBuffer[30 + 1];
      if(!g_cEventResender.empty())
      {
         CEventResenderStoreIterator lcIter = g_cEventResender.begin();
         if(lcIter->first <= time(NULL) )
         {
            pthread_mutex_lock(&g_SenderMutex);
            
           // if((long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY == lcIter->second.stData.nMessageCode)
           // {
           //    do
           //    {
          //        SetRand(lcUniqueIdentifierBuffer, strlen(lcIter->second.stData.cUniqueMessageIdentifier));
          //     }while(VerifyUniqueness(lcUniqueIdentifierBuffer) != 0);
            //   strncpy(lcIter->second.stData.cUniqueMessageIdentifier, lcUniqueIdentifierBuffer,strlen(lcIter->second.stData.cUniqueMessageIdentifier));
          //  }
            cout << "resending data" << endl;
            g_cSenderDataStore.push_front(lcIter->second.stData);
            pthread_mutex_unlock(&g_SenderMutex);
            if(lcIter->second.m_nCounter > 1)
            {
               lcIter->second.m_nCounter--;
               lcIter->second.m_nTime = time(NULL) + 10;
               g_cEventResender.insert(pair<time_t,tagTimeData>(lcIter->second.m_nTime,lcIter->second));
            }   
            g_cEventResender.erase(lcIter);
            //lcIter++;
         }
        // sleep(lcIter->first - time(NULL));
//         for(CEventResenderStoreIterator lcIter =  g_cEventResender.begin();lcIter !=  g_cEventResender.end();)
//         {
//            if(lcIter->first <= time(NULL))
//            {
//               //cout << "hello" << endl;
//               if(lcIter->second.m_nCounter <= 0)
//               {
//                  lcIter = g_cEventResender.erase(lcIter);
//               }
//               else
//               {
//                  pthread_mutex_lock(&g_SenderMutex);
//                  if((long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY == lcIter->stData.nMessageCode)
//                  {
//                      do
//                      {
//                         SetRand(lcIter->second.stData.cUniqueMessageIdentifier, strlen(lcIter->stData.cUniqueMessageIdentifier));
//                      }while(VerifyUniqueness(lcIter->stData.cUniqueMessageIdentifier) != 0);
//                  }    
//                      cout << "resending data" << endl;
//                      g_cSenderDataStore.push_front(lcIter->stData);
//                      pthread_mutex_unlock(&g_SenderMutex);
//                      lcIter->m_nCounter--;
//                      lcIter->m_nTime = time(NULL) + 10;
//                      lcIter++;
//                  
//               }
//            }
//            else
//            {
//               lcIter++;
//            }
//         }
      }
      pthread_mutex_unlock(&g_ReSenderMutex);
      //checks for resender events only once a second
      sleep(1);
   }
}*/

/*void* CheckResponse(void*)
{
   char lcIdentifier[31] = {0};
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
                  if((long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY == lcIter->stData.nMessageCode)
                  {
                      do
                      {
                         SetRand(lcIdentifier, 30);
                      }while(VerifyUniqueness(lcIdentifier) != 0);
                      strncpy(lcIter->stData.cUniqueMessageIdentifier, lcIdentifier,30);
                  }
                  cout << "resending data" << endl;
                  g_cSenderDataStore.push_front(lcIter->stData);
                  pthread_mutex_unlock(&g_SenderMutex);
	          lcIter->m_nCounter--;
                  lcIter->m_nTime = time(NULL) + 10;
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
      //checks for resender events only once a second
      sleep(1);
   }
}*/

tagBufferData ConvertToNetworkBuffer(tagData& stData)
{
    tagBufferData lstBufferData = {0};
    lstBufferData.nCommand = stData.nCommand;
    lstBufferData.nGlobalIdentifier = stData.nGlobalIdentifier;
    strncpy(lstBufferData.cIdentifier ,stData.cIdentifier ,20);
    lstBufferData.nFrOrToServerFlg = stData.nFrOrToServerFlg;
    lstBufferData.nMessageCode    = stData.nMessageCode;
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
    lstBufferData.nMessageCode    = stData.nMessageCode;
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
   char lcUniqueIdentifierBuffer[30 + 1];
   int lnSleeptIme = 0;
   //tagData lstData = {0};
   while(true)
   {
      
      pthread_mutex_lock(&g_ReSenderMutex);
	  #ifdef LOGGING
	  //cout << "taking resender lock" << endl;
	  #endif
	  pthread_mutex_lock(&g_SenderMutex);
	  #ifdef LOGGING
	  TESTLOG("taking sender lock \n");
	  #endif
      if(!g_cEventResender.empty())
      {
		 //pthread_mutex_unlock(&g_ReSenderMutex);
		 //pthread_mutex_lock(&g_ReSenderMutex);
         CEventResenderStoreIterator lcIter = g_cEventResender.begin();
		 //cout << "iterator time " << lcIter->first << endl;
         TESTLOG("iterator time %d \n", lcIter->first);
         TESTLOG("current time %d \n",time(NULL));
		 //cout << "current time " << time(NULL) << endl;
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
            lcIter->second.stData.nLatestClntSeqNo = g_nLatestRecivedSequenceNo;
            TESTLOG("resending data \n");
			//pthread_mutex_lock(&g_SenderMutex);
            g_cSenderDataStore.push_front(lcIter->second.stData);
            //pthread_mutex_unlock(&g_SenderMutex);
			//memcpy(&lstData, &(lcIter->second.stData), sizeof(tagData));
            if(lcIter->second.m_nCounter > 1)
            {
               TESTLOG("Resending Data :  %s %d %s %d %s %d %s %s %s %d","Message Code:", lcIter->second.stData.nMessageCode,"the seq no is ", lcIter->second.stData.nSeqNo , " the LatestRecieved Seq no is " , lcIter->second.stData.nLatestClntSeqNo , " from user id and name ", lcIter->second.stData.cIdentifier ," " , lcIter->second.stData.nGlobalIdentifier);
               lcIter->second.m_nCounter--;
               lcIter->second.m_nTime = time(NULL) + 3;
               g_cEventResender.insert(pair<time_t,tagTimeData>(lcIter->second.m_nTime,lcIter->second));
            }
	         lnSleeptIme = lcIter->first;
            g_cEventResender.erase(lcIter);
            //lcIter++;
         }
		 
		 //pthread_mutex_unlock(&g_ReSenderMutex);
         //g_cSenderDataStore.push_front(lstData);
         
		 //memset(&lstData, 0,sizeof(tagData));
         //sleep(lcIter->first - time(NULL));
		 //continue;
      }
	  
	  pthread_mutex_unlock(&g_SenderMutex);
	  #ifdef LOGGING
	  TESTLOG("releasing sender mutex \n");
	  #endif
	  pthread_mutex_unlock(&g_ReSenderMutex);
	  #ifdef LOGGING
	  TESTLOG("releasing resender mutex \n");
	  #endif
      //checks for resender events only once a second
      sleep(1);
	  //sleep(lnSleeptIme - time(NULL));
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
	//for(list<tagTimeData>::iterator lcItertagTimeDataStore = g_cEventResender.begin() ;lcItertagTimeDataStore != g_cEventResender.end() ;lcItertagTimeDataStore++)
    for(CEventResenderStoreIterator lcItertagTimeDataStore = g_cEventResender.begin() ;lcItertagTimeDataStore != g_cEventResender.end() ;lcItertagTimeDataStore++)
    {
       //tagTimeData& lstagTimeData =  *(lcItertagTimeDataStore);
       tagTimeData& lstagTimeData =  (lcItertagTimeDataStore->second);
       lnCompRetVal = strncmp(lstagTimeData.stData.cUniqueMessageIdentifier, cUniqueMessage, strlen(cUniqueMessage));
       if(lnCompRetVal == 0)
       {
		   //TESTLOG("verify uniqueness failed \n");
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
///the endpoint of  a message
///this will be the last place where a message would end/cause effect
int ExecuteResponse(tagData& stData)
{
    long lnMessageCode = stData.nMessageCode;
    switch(lnMessageCode)
    {

        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_RESPONSE):
        {
            TESTLOG("response recieved %d \n",  stData.nGlobalIdentifier);
            TESTLOG("Identifier Registered %s \n",  stData.cIdentifier);
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
			cout << stData.cIdentifier << " : " << stData.cBuffer << endl;
            tagData lstSentDeliveryMessageData = {0};
            
            lstSentDeliveryMessageData.nGlobalIdentifier                =  stData.nGlobalIdentifier;
            lstSentDeliveryMessageData.nCommand                =  (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY;
            strncpy(lstSentDeliveryMessageData.cIdentifier, stData.cIdentifier, 20);
            lstSentDeliveryMessageData.nFrOrToServerFlg        =  (long)CEToFromServer::CEToFromServer_CLNT_TO_SERV;  
            lstSentDeliveryMessageData.nMessageCode            =  (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY;
            strncpy(lstSentDeliveryMessageData.cTarget, stData.cTarget, 20);//here target means u
            strncpy(lstSentDeliveryMessageData.cUniqueMessageIdentifier, stData.cUniqueMessageIdentifier, 30);
            lstSentDeliveryMessageData.stNetWork.fd         =  g_nSockFd;
            //lstData.fd                   =  g_nSockFd;
            lstSentDeliveryMessageData.stNetWork.n          =  sizeof(tagData);
            lstSentDeliveryMessageData.stNetWork.flags      =  MSG_WAITALL;
            lstSentDeliveryMessageData.stNetWork.addr       =  servaddr;
            lstSentDeliveryMessageData.stNetWork.restrict   =  sizeof(servaddr);
            //UDPChatServer 20-01-2019
            //UDPChatServer 20-01-2019
            lstSentDeliveryMessageData.nSeqNo                  =  stData.nSeqNo;
            TESTLOG("sending delivery data message to sender threada \n");
            pthread_mutex_lock(&g_SenderMutex);
            TESTLOG( "taking Sender lock : %d \n", __LINE__);
            g_cSenderDataStore.push_back(lstSentDeliveryMessageData);
            pthread_mutex_unlock(&g_SenderMutex);
            TESTLOG("releasing Sender lock %d \n",__LINE__);
 
        }
        break;

        case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_SERV_TO_CLI:
        {
            TESTLOG( "msg delivered \n");
            TESTLOG("\n %s : %s \n", stData.cIdentifier, stData.cBuffer);
            //cout << endl <<stData.cIdentifier << " : " <<stData.cBuffer << endl;
        }
        break;
		
        case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY_RES:
        {
            TESTLOG("delivery mesage reachd server \n");
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

///CreateUDPSocketIP
///Creates a Simple UDP Socket
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

//The job of this function is to erase the sender request when the final response has arrived
int MakeReSender(tagData lstRecvData )
{
   
   if(lstRecvData.bFinalResponse == true)
   {
       pthread_mutex_lock(&g_ReSenderMutex);
	   #ifdef LOGGING
	   TESTLOG("taking resender lock %d\n", __LINE__);
	   #endif
       //for(list<tagTimeData>::iterator lcIter =  g_cEventResender.begin();lcIter != g_cEventResender.end();)
	   for(CEventResenderStoreIterator lcIter =  g_cEventResender.begin();lcIter != g_cEventResender.end();)
       {
		   
           //tagTimeData& lstData = *lcIter;
           tagTimeData& lstData = lcIter->second;
           if(strcmp(lstData.stData.cUniqueMessageIdentifier,lstRecvData.cUniqueMessageIdentifier) == 0)
           {
              TESTLOG( "response recieved now erasing from resender store\n");
              lcIter = g_cEventResender.erase( lcIter);
           }
           else
           {
              lcIter++;
           }
       }
       pthread_mutex_unlock(&g_ReSenderMutex);
	   #ifdef LOGGING
	    TESTLOG("releasing resender lock %d",  __LINE__ );
	   #endif
   }
   return 0;
}

//this func is located for SenderThread if this message is unique then it is added to resender store
bool IsMessageUnique(tagData stData)
{
	
   pthread_mutex_lock(&g_ReSenderMutex);
   #ifdef LOGGING
   TESTLOG( "Taking Resender lock %d \n", __LINE__ );
   #endif
   bool lbUniqueMessageSend = true;

   //for(auto& lcTemp : g_cEventResender)
   for(CEventResenderStoreIterator lcIter =  g_cEventResender.begin();lcIter != g_cEventResender.end();lcIter++)
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
      //cout << "this is a new unique message" << endl;
      tagTimeData lstTimeData((time(NULL) + 10), stData);
      //g_cEventResender.push_back(lstTimeData);
      g_cEventResender.insert(pair<time_t,tagTimeData>(lstTimeData.m_nTime,lstTimeData));
   }
   #ifdef LOGGING
   TESTLOG( "releasing resender lock %d \n", __LINE__);
   #endif
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
    while(true)
    {
      lbUniqueMessageSend = true;
	   
      memset(&lstToSendData, 0 ,sizeof(tagData));       
	   
      pthread_mutex_lock(&g_SenderMutex);
	  #ifdef LOGGING
      TESTLOG("taking sender lock %d\n",  __LINE__ ); 
	  #endif
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
          char lcBuffer[lnDataStructSize] = { 0 };
          tagBufferData lstBufferData = ConvertToNetworkBuffer(lstToSendData) ;
          memcpy(&lcBuffer, (char*)&lstBufferData, lnDataStructSize);
          lnLen = sendto(lstThread.fd, (const char *)&lcBuffer, lnDataStructSize, MSG_CONFIRM, (const struct sockaddr *) &(lstThread.addr), sizeof((lstThread.addr))); 
          if(lnLen <= 0)
          {
             TESTLOG("error");
             exit(1);
          }

       }
      
       else
       {
         pthread_mutex_unlock(&g_SenderMutex);
		 #ifdef LOGGING
		  TESTLOG("Releasing sender lock %d \n", __LINE__ );
		 #endif
       }
    }
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
   while(true)
   {
      lbToResend = true;
      memset(&lstRecvData, 0 ,sizeof(tagData));       
	   tagBufferData lstBufferData;
      lnDataRecievedLen  = recvfrom(lstThread.fd, (char *)&lstBufferData, sizeof(tagBufferData), MSG_WAITALL, (struct sockaddr *) &(lstThread.addr),(socklen_t*)&(lstThread.restrict));
      if(0 >= lnDataRecievedLen)
      {
          cout << "data reception error" << endl;
          cout << strerror(errno) << endl;
          LOG_LOGGER("data reception error : [%d] %s ",errno,strerror(errno));
          exit(1);
      }
	  lstRecvData = ConvertToDataStruct(lstBufferData);
#ifdef LOGGING
       TESTLOG("%s \n", lstRecvData.cUniqueMessageIdentifier);
       TESTLOG("%s \n",lstRecvData.cIdentifier );
       TESTLOG("%s \n", lstRecvData.cTarget );
#endif
       
       if(lstRecvData.bFinalResponse == true)
     {
         if(lstRecvData.nSeqNo == g_nLatestRecivedSequenceNo)
         {
             //g_nLastResRecSeq++;
             g_nLatestRecivedSequenceNo++;
         }
         if(lstRecvData.nSeqNo == g_nLatestRecivedSequenceNo + 1)
         {
           // g_nLatestRecivedSequenceNo++;
         }
     }

         if(g_nFlagNoResendDupli == 0)
         {
            pthread_mutex_lock( &g_cIdentifierMutex);
			#ifdef LOGGING
			TESTLOG( "Taking identifier lock %d \n" , __LINE__ );
			#endif
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
//               delete lpstData;
//              lpstData = nullptr;
                lbDiscardPacket = false;
                pthread_mutex_unlock(&g_cIdentifierMutex);
				#ifdef LOGGING
				 TESTLOG("reeleasing identifier lock %d\n", __LINE__);
				#endif
                continue;
             }
          }
#ifdef LOGGING
			  TESTLOG("%d %s \n", g_nLatestRecivedSequenceNo, "latest recieved sequene no");
          //cout << endl;
          //if(lpstData->nCommand != (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY)
#endif
          if(lstRecvData.bFinalResponse == true)
          {
              //pthread_mutex_lock();
              pthread_mutex_lock(&g_ReSenderMutex);
			  #ifdef LOGGING
			  TESTLOG("Taking resender lock %d\n", __LINE__ );
			  #endif
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
			  #ifdef LOGGING
			  TESTLOG( " releasing resender lock %d \n");
			  #endif
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
		  #ifdef LOGGING
		  TESTLOG("Realeasing identifier lock %d\n", __LINE__ );
		  #endif
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
//       int MakeReSender(tagData& lstRecvData )
//       {
//        pthread_mutex_lock(&g_ReSenderMutex);
//        for(list<tagTimeData>::iterator lcIter =  g_cEventResender.begin();lcIter != g_cEventResender.end();)
//        {
//             tagTimeData& lstData = *lcIter;
//            if(strcmp(lstData.stData.cUniqueMessageIdentifier,lstRecvData.cUniqueMessageIdentifier) == 0)
//            {
//               cout << "response recieved now erasing" << endl;
//               lcIter = g_cEventResender.erase( lcIter);
//            }
//            else
//            {
//               lcIter++;
//            }
//        }
//        pthread_mutex_unlock(&g_ReSenderMutex);
//    }
//        pthread_mutex_lock(&g_ReSenderMutex);
//        for(list<tagTimeData>::iterator lcIter =  g_cEventResender.begin();lcIter != g_cEventResender.end();)
//        {
//             tagTimeData& lstData = *lcIter;
//            if(strcmp(lstData.stData.cUniqueMessageIdentifier,lstRecvData.cUniqueMessageIdentifier) == 0)
//            {
//               cout << "response recieved now erasing" << endl;
//               lcIter = g_cEventResender.erase( lcIter);
//            }
//            else
//            {
//               lcIter++;
//            }
//        }
//        pthread_mutex_unlock(&g_ReSenderMutex);
        
       TESTLOG("Server : %d\n", lstRecvData.nMessageCode);
    }
    
}


//This functions does the job of which message code to be sent next
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
         stData.nCommand = (short)CCOMMAND_TYPE::CCOMMAND_TYPE_REQUEST;
         stData.nFrOrToServerFlg = (long)CEToFromServer::CEToFromServer_CLNT_TO_SERV;
         //sleep(2);
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
           //string lcInput(stData.cBuffer);
           //getline(cin, lcInput);
           //strncpy(stData.cBuffer, lcInput.c_str(), MAXLINE); 
         }
         stData.nCommand = (short)CCOMMAND_TYPE::CCOMMAND_TYPE_REQUEST;
         stData.nFrOrToServerFlg = (long)CEToFromServer::CEToFromServer_CLNT_TO_SERV;
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

//UDpChatServer 21/12/2018 Aditya M.:END
int main(int argc,char* argv[])
{
   g_nArgs = argc;
   g_nFlagNoResendDupli = 0;
   g_pcParam = argv;
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
    int lnRetVal = 0;
    
    
   time_t lnTime;
   lnTime = time(NULL);
   struct tm*  psttm =  gmtime(&lnTime);
   char lcBuffera[200] = {0};
   int lnRandNo =rand()%100;
   //snprintf(lcBuffera,200,"%d:%d:%d%d-%d-%d",psttm->tm_hour,psttm->tm_min,psttm->tm_sec,psttm->tm_mday,psttm->tm_mon,psttm->tm_year );
   snprintf(lcBuffera,200,"%s/%s-%s%d-%d-%d-%d_%d%d%d.%s","Logs","log","randno",lnRandNo,psttm->tm_mday,psttm->tm_mon +1  ,psttm->tm_year + 1900,psttm->tm_hour,psttm->tm_min,psttm->tm_sec,"log");
   
   TESTLOG(" log file name %s \n", lcBuffera );
   g_cfstream.open(lcBuffera,ios::in|ios::out | ios::app);
   snprintf(lcBuffera,200,"%s/%s-%s%d-%d-%d-%d_%d%d%d.%s","Logs","Data","randno",lnRandNo,psttm->tm_mday,psttm->tm_mon +1  ,psttm->tm_year + 1900,psttm->tm_hour,psttm->tm_min,psttm->tm_sec,"log");
   g_cfstreamDataLogger.open(lcBuffera,ios::in|ios::out | ios::app);
   
   pthread_t lnPThreadReciever;
   pthread_t lnPThreadSender;
   pthread_t lnPThreadCheckResponse;
   
   
   char lcSeqIdentifier[LENGHT_OF_BASE_SEQ_NO + 1];
   
   
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
    
    tagData* lpstThrdDataRcvr = NULL;
    tagData* lpstThrdSndr = NULL;
  

    size_t lnLen = 0;
    if ( (g_nSockFd = CreateUDPSocketIP()) < 0 )
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    //FillSockAddrin(AF_INET, htons(PORT), (in_addr_t)(inet_addr("127.0.0.1")), &servaddr);
    FillSockAddrin(AF_INET, htons(PORT), (in_addr_t)(inet_addr(SERVER_IP)), &servaddr);

    tagData lstData = {0};
    
    lstData.stNetWork.fd        = g_nSockFd;
    lstData.stNetWork.n         = sizeof(tagData);
    lstData.stNetWork.flags     = MSG_WAITALL;
    lstData.stNetWork.addr      = servaddr;
    lstData.stNetWork.restrict  = sizeof(servaddr);
   
   lpstThrdDataRcvr = new tagData();
   lpstThrdSndr = new tagData();
#ifdef LOGGING
   TESTLOG( "sockfd is %d\n", lpstThrdDataRcvr->stNetWork.fd );
#endif
   //sleep(1);
   memcpy(lpstThrdDataRcvr, &lstData, sizeof(tagData));
   memcpy(lpstThrdSndr, &lstData, sizeof(tagData));
   pthread_create(&lnPThreadReciever, NULL, RecieverThread, lpstThrdDataRcvr);
   pthread_create(&lnPThreadSender, NULL, SenderThread, lpstThrdSndr);
   if(g_nFlagNoResendDupli == 0)
   {
      pthread_create(&lnPThreadCheckResponse, NULL,  CheckResponse,NULL);
   }
   lstData.nMessageCode = (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER;
//   g_nLastResRecSeq;
   {
     tagData lstDummyMessageData = {0};
     lstDummyMessageData.nMessageCode =   (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY;
     SetRand(lstDummyMessageData.cUniqueMessageIdentifier,30);
	  //tagTimeData lstTimeData(time(NULL)+3,lstDummyMessageData, INT_MAX) ;
     //g_cEventResender.push_back(lstTimeData);
      tagTimeData lstTimeData(time(NULL)+3,lstDummyMessageData, INT_MAX) ;
      g_cEventResender.insert(pair<time_t,tagTimeData>(lstTimeData.m_nTime, lstTimeData)); //to Uncomment
//	 lstTimeData.stData = lstDummyMessageData;
	 
//	 g_cEventResender.push_back()
   } 
    while(true)
    {
      do
      {
          SetRand(lstData.cUniqueMessageIdentifier,30);
      }
      while(VerifyUniqueness(lstData.cUniqueMessageIdentifier) != 0);

      lstData.nSeqNo = g_nSeqNo++;
      lstData.nLatestClntSeqNo = g_nLatestRecivedSequenceNo;
      
      
      //lstData.stSenderSeqNo = lstSeqStruct;
      lstData.nFrOrToServerFlg = 100001;
      lstData.nSessionId = g_nSessionId;
      lstData.nGlobalIdentifier = g_nGlobalIdentifier;
      TESTLOG ("lstData.nGlobalIdentifier is set as %d \n", lstData.nGlobalIdentifier); 
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
