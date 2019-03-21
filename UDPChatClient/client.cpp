#include "includes.h"
#include "Functions.h"

#pragma pack(1)


struct tagData;
char** g_pcParam = nullptr;
short  g_nArgs = 0;
pthread_mutex_t g_cIdentifierMutex;

tagCSequenceNo  g_cstSeqStruct;

int VerifyUniqueness(char* cUniqueMessage);
void SetRand(char* cBuf,int nSize);
string SuffixAppropirateUniqueIdentifier(string lcString,short nCommand);

std::deque<std::string> g_cIdentifierStore;//store all reciever packets and reject duplicate
list<tagTimeData>       g_cEventResender;

char                    g_cIdentifier[20 + 1] = {0};

map <string,tagData>    g_cMessageHolder;//this will contain the messages for duplicate check

deque<tagData>          g_cSenderDataStore;

deque<tagData>          g_cRecieverDataStore;



pthread_mutex_t         g_SenderMutex;

pthread_mutex_t         g_ReSenderMutex;

struct sockaddr_in      servaddr = {0};

int g_nSockFd = 0;

int g_nLastResRecSeq = 0;
int g_nLatestRecivedSequenceNo = 0;
int g_nTesting = 0;
int g_nFlagNoResendDupli = 0;
int g_nSessionId = 0;
int g_nGlobalIdentifier = 0;
bool g_bWaitForResponse = false;


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
                  if((long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY == lcIter->stData.nMessageCode)
                  {
                      do
                      {
                         SetRand(lcIter->stData.cUniqueMessageIdentifier, strlen(lcIter->stData.cUniqueMessageIdentifier));
                      }while(VerifyUniqueness(lcIter->stData.cUniqueMessageIdentifier) != 0);
                      cout << "resending data" << endl;
                      g_cSenderDataStore.push_front(lcIter->stData);
                      pthread_mutex_unlock(&g_SenderMutex);
				          lcIter->m_nCounter--;
                      lcIter->m_nTime = time(NULL) + 10;
                      lcIter++;
                    }
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
    for(list<tagTimeData>::iterator lcItertagTimeDataStore = g_cEventResender.begin() ;lcItertagTimeDataStore != g_cEventResender.end() ;lcItertagTimeDataStore++)
    {
       tagTimeData& lstagTimeData =  *(lcItertagTimeDataStore);
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
///the endpoint of  a message
///this will be the last place where a message would end/cause effect
int ExecuteResponse(tagData& stData)
{
    long lnMessageCode = stData.nMessageCode;
    switch(lnMessageCode)
    {

        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_RESPONSE):
        {
            cout << "response recieved" << stData.nGlobalIdentifier <<endl;
            g_nGlobalIdentifier = stData.nGlobalIdentifier;
            g_bWaitForResponse = false;
        }
        break;
        
        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET_RESPONSE):
        {
            g_nSessionId = stData.nSessionId;
            cout << "target fixed" << endl;
            g_bWaitForResponse = false;
        }
        break;
        
        case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MESSAGE):
        {
            cout << stData.cIdentifier << " : " <<stData.cBuffer << endl;
            tagData lstData = {0};
            
            lstData.nGlobalIdentifier                =  stData.nGlobalIdentifier;
            lstData.nCommand                =  (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY;
            strncpy(lstData.cIdentifier, stData.cIdentifier, 20);
            lstData.nFrOrToServerFlg        =  (long)CEToFromServer::CEToFromServer_CLNT_TO_SERV;  
            lstData.nMessageCode            =  (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY;
            strncpy(lstData.cTarget, stData.cTarget, 20);//here target means u
            strncpy(lstData.cUniqueMessageIdentifier, stData.cUniqueMessageIdentifier, 30);
            lstData.stNetWork.fd         =  g_nSockFd;
            //lstData.fd                   =  g_nSockFd;
            lstData.stNetWork.n          =  sizeof(tagData);
            lstData.stNetWork.flags      =  MSG_WAITALL;
            lstData.stNetWork.addr       =  servaddr;
            lstData.stNetWork.restrict   =  sizeof(servaddr);
            //UDPChatServer 20-01-2019
            //UDPChatServer 20-01-2019
            lstData.nSeqNo                  =  stData.nSeqNo;
            cout << "sending data to sender threada" << endl;
            pthread_mutex_lock(&g_SenderMutex);
            g_cSenderDataStore.push_back(lstData);
            pthread_mutex_unlock(&g_SenderMutex);
 
        }
        break;

        case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_SERV_TO_CLI:
        {
            cout << "msg delivered" << endl;
            cout << endl <<stData.cIdentifier << " : " <<stData.cBuffer << endl;
        }
        break;
		
        case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY_RES:
        {
            cout << "delivery mesage reachd server" << endl;
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
   string lcKey = "";
   lcKey = SuffixAppropirateUniqueIdentifier(lstRecvData.cUniqueMessageIdentifier, lstRecvData.nCommand);
   
   if(lstRecvData.bFinalResponse == true)
   {
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
   }
   return 0;
}

//this func is located for SenderThread if this message is unique then it is added to resender store
bool IsMessageUnique(tagData stData)
{
   pthread_mutex_lock(&g_ReSenderMutex);
   bool lbUniqueMessageSend = true;

   for(auto& lcTemp : g_cEventResender)
   {
       if(strcmp(lcTemp.stData.cUniqueMessageIdentifier, stData.cUniqueMessageIdentifier) == 0)
       {
          cout << "Resending Message as no response received yet" << endl;
          lbUniqueMessageSend = false;
       }
   }

   if(lbUniqueMessageSend == true)
   {
      cout << "this is a new unique message" << endl;
      tagTimeData lstTimeData((time(NULL) + 10), stData);
      g_cEventResender.push_back(lstTimeData);
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
    while(true)
    {
      lbUniqueMessageSend = true;
	   
      memset(&lstToSendData, 0 ,sizeof(tagData));       
	   
      pthread_mutex_lock(&g_SenderMutex);
       
      if(!g_cSenderDataStore.empty())
      {
         lstToSendData = g_cSenderDataStore.front();
         g_cSenderDataStore.pop_front();
         pthread_mutex_unlock(&g_SenderMutex);

          if(g_nFlagNoResendDupli == 0)
          {
              bool lbUniqueMessageSend = IsMessageUnique(lstToSendData);
          }
          if(lstToSendData.nMessageCode == (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY)
          {
              cout << "sending data delivery;" << lstThread.fd << endl;
          }

          lnLen = sendto(lstThread.fd, (const char *)&lstToSendData, sizeof(tagData), MSG_CONFIRM, (const struct sockaddr *) &(lstThread.addr), sizeof((lstThread.addr))); 
          if(lnLen <= 0)
          {
             printf("error");
             exit(1);
          }

       }
      
       else
       {
         pthread_mutex_unlock(&g_SenderMutex);
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
	   
       lnDataRecievedLen  = recvfrom(lstThread.fd, (char *)&lstRecvData, sizeof(tagData), MSG_WAITALL, (struct sockaddr *) &(lstThread.addr),(socklen_t*)&(lstThread.restrict));
       if(0 >= lnDataRecievedLen)
       {
           cout << "data reception error" << endl;
           cout << strerror(errno) << endl;
           //exit(1);
       }
        cout << lstRecvData.cUniqueMessageIdentifier << endl;
        cout << lstRecvData.cIdentifier << endl;
        cout << lstRecvData.cTarget << endl;
      if(lstRecvData.bFinalResponse == true)
      {
          if(lstRecvData.nLatestClntSeqNo == g_nLatestRecivedSequenceNo)
          {
              g_nLastResRecSeq++;
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
          
          //if(lpstData->nCommand != (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY)
          {
             for(auto& lstIdentifiers : g_cIdentifierStore)
             {
			   lcKey = SuffixAppropirateUniqueIdentifier(lstRecvData.cUniqueMessageIdentifier, lstRecvData.nCommand);
               if(0 == strcmp(lstIdentifiers.c_str(), lcKey.c_str()))
               {
                   cout << "duplicate packet" << endl;
                   cout << lstIdentifiers.c_str() << " duplicated " << "rejected code "<< lpstData->nMessageCode << " command : " <<lpstData->nCommand << endl;;
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
                continue;
             }
          }
          cout << endl << g_nLatestRecivedSequenceNo << "latest recieved sequene no" << endl;
          //cout << endl;
          //if(lpstData->nCommand != (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY)

          if(lstRecvData.bFinalResponse == true)
          {
              //pthread_mutex_lock();
              pthread_mutex_lock(&g_ReSenderMutex);
              for(list<tagTimeData>::iterator lcIter = g_cEventResender.begin(); lcIter != g_cEventResender.end();)
              {
                 tagTimeData& lstData = *lcIter; 
                 if(strcmp(lstData.stData.cUniqueMessageIdentifier, lstRecvData.cUniqueMessageIdentifier) == 0)
                 {
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
           cout << "response handling error " << endl;
          //exit(1);
       }
	   
       if(MakeReSender(lstRecvData) != EXIT_SUCCESS)
       {
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
        
       printf("Server : %d\n", lstRecvData.nMessageCode);
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
          sleep(5);
      }
      break;
      case (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET:
      {
         printf("Registering message sent.\n");
         
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
   g_nFlagNoResendDupli = 1;
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
    FillSockAddrin(AF_INET, htons(PORT), (in_addr_t)(inet_addr("172.25.1.8")), &servaddr);

    tagData lstData = {0};
    
    lstData.stNetWork.fd        = g_nSockFd;
    lstData.stNetWork.n         = sizeof(tagData);
    lstData.stNetWork.flags     = MSG_WAITALL;
    lstData.stNetWork.addr      = servaddr;
    lstData.stNetWork.restrict  = sizeof(servaddr);
   
   lpstThrdDataRcvr = new tagData();
   lpstThrdSndr = new tagData();
   
   cout << "sockfd is "<< lpstThrdDataRcvr->stNetWork.fd << endl;
   
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
	  tagTimeData lstTimeData(time(NULL)+3,lstDummyMessageData, INT_MAX) ;
     g_cEventResender.push_back(lstTimeData);
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
      cout << "lstData.nGlobalIdentifier is set as " << lstData.nGlobalIdentifier; 
      lnRetVal = PreSender(lstData);
      if(0 != lnRetVal)
      {
         cout << "error occured at Presender" << endl;
         exit(1);  
      }
      
      //Taking Lock so that the other thread doesnt corrupt g_cSenderDataStore
      pthread_mutex_lock(&g_SenderMutex); 
      
      //Sending data to the Sender thread
      g_cSenderDataStore.push_back(lstData); 
      
      // unlocking the mutex
      pthread_mutex_unlock(&g_SenderMutex);
      cout << lstData.cUniqueMessageIdentifier << endl;
      memset (lstData.cBuffer, 0,sizeof(lstData.cBuffer));
      memset (lstData.cUniqueMessageIdentifier, 0, sizeof(lstData.cUniqueMessageIdentifier));
     
      //shared memory usage
      pthread_mutex_lock(&(lpstSharedmemData->m_cSharedMemMutex));
      if(lpstSharedmemData->nState == STATE_INITIALIZE)
      {
         cout << "shared mem data " << lpstSharedmemData->cBuffer << endl;
         strncpy(lpstSharedmemData->cBuffer, "hi", 2);
         lpstSharedmemData->nState = STATE_WRITING_DONE;
         cout << "shared mem data " << lpstSharedmemData->cBuffer << endl;
      }
      pthread_mutex_unlock(&(lpstSharedmemData->m_cSharedMemMutex));
      //shared memory usage

      lnRetVal = PostSender(lstData);
      if(0 != lnRetVal)
      {
         cout << "error occured at PostSender" << endl;
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
