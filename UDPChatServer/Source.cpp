#include <vector>

#include "includes.h"

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
class CSession;
typedef list<CSession*>                     CSessionMangerStore;
typedef list<CSession*>::iterator           CSessionManagerStoreIterator;
typedef vector<string>                      CSessionStore;
typedef vector<string>::iterator            CSessionStoreIterator;
list<pair<string,tagData>> cDataStore;
int g_nClientIdentifier = 0;
CClientIdDataStore g_cClientIDStore;
int g_nMainSockFd = 0;

#pragma pack(1)
class CSession
{
   private:

   vector<string> cStore;
   int nSize;
   int nNoOfElements;
   bool m_bRestricted; //if truw then its a normal chat between two people
   int m_nSessionId;//sessionId generated for a chat
   
   public:

   CSession(int nA,bool bRestricted)
   {
       //cStore.
      cStore.reserve(nA);
      nSize = nA;
      nNoOfElements = cStore.size();
      m_bRestricted = bRestricted;
   }
   int SetNewRandomSessionId()
   {
      m_nSessionId = rand();
      return m_nSessionId;
   }
   

   int AddNextName(string cString)
   { 
      if((nSize >  cStore.size()) && (m_bRestricted == true))
      {
         cStore.push_back(cString);
         nNoOfElements = cStore.size();
         return 0;
      }
      else
      {
         cStore.push_back(cString);
         nNoOfElements = cStore.size();
         return 0;
      }
      return -1;
   }
   
   int UserExistsinSession(string cName)
   {
      for(CSessionStoreIterator lcIter =   cStore.begin();lcIter != cStore.end();lcIter++)
      {
         if(*lcIter == cName)
         {
            return 0;
         }
      }
   }
   
   
   int FreeUserExistsinSession(string cName)
   {
      if(nNoOfElements < nSize)
      {
         for(CSessionStoreIterator lcIter =   cStore.begin();lcIter != cStore.end();lcIter++)
         {
            if(*lcIter == cName)
            {
               return 0;
            }
         }
      }
   }
   
   int GetFreeSessionIdForUser(string cName)
   {
      if(nNoOfElements < nSize)
      {
         for(CSessionStoreIterator lcIter =   cStore.begin();lcIter != cStore.end();lcIter++)
         {
            if(*lcIter == cName)
            {
               return m_nSessionId;
            }
         }
      }
   }
   
   int GetSessionId()
   {
      return m_nSessionId;
   }
   
   void SetSessionId(int nSessionId)
   {
      m_nSessionId = nSessionId;
   }
};

class CSessionManager
{
   private:
   list<CSession*> m_cSessionStore;
   int IsSessionUnique(int nSessionId)
   {
  
      for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
      {
         if((*lcIter)->GetSessionId() == nSessionId)
         {
            //lnRetVal = (*lcIter)->AddNextName(cSecondName);
            //if(lnRetVal == 0)
            //{
            //   break;
            //}
            return -1;
         }
      }
      return 0;
   } 
   public:
      
   int CreateASession(string cFirstName,int nLength,bool bRestricted)
   {
       CSession* lcSession = new CSession(nLength,bRestricted);
       //lcSession->Initialize();
       int lnSessionNo = 0;
       do
       {
          lnSessionNo = lcSession->SetNewRandomSessionId();
       }while(IsSessionUnique(lnSessionNo) != 0);
       lcSession->AddNextName(cFirstName);
       m_cSessionStore.push_back(lcSession);
       return 0;
   }
   
   int AddSecondUserToSession(string cFirstName,string cSecondName)
   {
      int lnRetVal = 0;
      for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
      {
         if((*lcIter)->UserExistsinSession(cFirstName) == 0)
         {
            lnRetVal = (*lcIter)->AddNextName(cSecondName);
            if(lnRetVal == 0)
            {
               break;
            }
         }
      }
      return lnRetVal;
   }
   
   int AddMoreUserToSession(int nSessionId,string cSecondName)
   {
      int lnRetVal = 0;
      for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
      {
         if((*lcIter)->GetSessionId() == nSessionId)
         {
            lnRetVal = (*lcIter)->AddNextName(cSecondName);
            if(lnRetVal == 0)
            {
               break;
            }
         }
      }
      return lnRetVal;
   }
   

   int DoesSessionExistForUser(string cName)
   {
      for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
      {
         if((*lcIter)->UserExistsinSession(cName) == 0)
         {
            //cout << "yes the user exists" << endl;
            return 0;
         }
      }
   }
   
   int DoesFreeSessionExistForUser(string cName)
   {
      for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
      {
         if((*lcIter)->FreeUserExistsinSession(cName) == 0)
         {
            //cout << "yes the user exists" << endl;
            return 0;
         }
      }
   }
   
   
   int GetFreeUserSessionIdForUser(string cName)
   {
      for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
      {
         if((*lcIter)->FreeUserExistsinSession(cName) == 0)
         {
            //cout << "yes the user exists" << endl;
            return 0;
         }
      }
   }

};






CSessionManager g_cSessionManager;

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


////////////////////FINIALIZINGyyyy FUNCTIONS/////////////////////////
int CleanUp()
{
   close(g_nMainSockFd);
   g_bProgramShouldWork = false;
   for(CIteratotrIdentiferDataStore lcIter = g_cPortIdentifier.begin(); lcIter != g_cPortIdentifier.end();lcIter++)
   {
      tagData* lpstData = lcIter->second;
      if(lpstData != nullptr)
      {
         delete (lpstData);
         lpstData = nullptr;
      }
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

////////////////////FINIALIZINGyyyy1 FUNCTIONS/////////////////////////
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
          
            printf("Caught SIGINT, exiting now\n");
            g_bProgramShouldWork = false;
            CleanUp();
            //lnRetVal = pthread_join(lnSenderPThread,NULL);
            if (lnRetVal != 0)
            {
               printf("%s, %d", strerror(errno), __LINE__);
               perror("unable to destroy Conditional Variable");
               exit(EXIT_FAILURE);
            }

            for (int lnCounter = 0; lnCounter< NO_OF_PROC_THREADS; lnCounter++)
            {
               lnRetVal = pthread_join(lnProcessPThread[lnCounter],NULL);
               if (lnRetVal != 0)
               {
                  printf("%s, %d", strerror(errno), __LINE__);
                  perror("unable to destroy Conditional Variable");
                  exit(EXIT_FAILURE);
               }
            }

            lnRetVal = pthread_cond_destroy(&g_cCondVarForProcessThread);
            if (lnRetVal != 0)
            {
               printf("%s, %d", strerror(errno), __LINE__);
               perror("unable to destroy Conditional Variable");
               exit(EXIT_FAILURE);
            }


            lnRetVal = pthread_mutex_destroy(&g_cProcessMutex);
            if (lnRetVal != 0)
            {
               printf("%s, %d", strerror(errno), __LINE__);
               perror("unable to destroy mnutex");
               exit(EXIT_FAILURE);
            }

            lnRetVal = pthread_mutex_destroy(&g_cResponseMutex);
            if (lnRetVal != 0)
            {
               printf("%s, %d", strerror(errno), __LINE__);
               perror("unable to destroy mnutex");
               exit(EXIT_FAILURE);
            }


            lnRetVal = pthread_mutex_destroy(&g_cIdentifierMutex);
            if (lnRetVal != 0)
            {
               printf("%s, %d", strerror(errno), __LINE__);
               perror("unable to destroy mnutex");
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

int ExecuteFunction(tagData& stData)
{
   int lnRetVal = 0;
   long lnMessageCode = stData.nMessageCode;
   tagData* lpstData = nullptr;
   int lnReturnVal = 0;
   switch (lnMessageCode)
   {
      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER) :
      {
          pthread_mutex_lock(&g_cGlobalIdentifierMutex);
         stData.nGlobalIdentifier = g_nClientIdentifier++;
          pthread_mutex_unlock(&g_cGlobalIdentifierMutex);
         lpstData = new tagData(stData);
         //g_cClientIDStore.insert()
         bool lbVal1 = (g_cClientIDStore.insert(pair<int , tagData*>(stData.nGlobalIdentifier, lpstData))).second;
         if (lbVal1 == false)
         {
            delete lpstData;
            lpstData = nullptr;
            printf("duplicate identifier insert to identifier(%s) store failed ", stData.cIdentifier);
            printf("%s %d", strerror(errno), __LINE__);
            exit(EXIT_FAILURE);
         }
        // bool lbVal = (g_cPortIdentifier.insert(pair<string, tagData*>(stData.cIdentifier, lpstData))).second;
        // if (lbVal == false)
        // {
        //    delete lpstData;
        //    lpstData = nullptr;
        //    printf("duplicate identifier insert to identifier(%s) store failed ", stData.cIdentifier);
        //    printf("%s %d", strerror(errno), __LINE__);
        //    exit(EXIT_FAILURE);
        // }
         
      }
      break;

      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET) :
      {
         map<int , tagData*>::iterator lcIteratoor = g_cClientIDStore.find(stData.nGlobalIdentifier);
         if (lcIteratoor == g_cClientIDStore.end())
         {
            printf("%d not found", stData.cIdentifier);
            printf("%s %d", strerror(errno), __LINE__);
            exit(EXIT_FAILURE);
         }

         //map<string, tagData*>::iterator lcIterator = g_cPortIdentifier.find(stData.cIdentifier);
         //if (lcIterator == g_cPortIdentifier.end())
         //{
         //   printf("%d not found", stData.cIdentifier);
         //   printf("%s %d", strerror(errno), __LINE__);
         //   exit(EXIT_FAILURE);
         //}
         if(lcIteratoor->second == nullptr)
         {
            cout << "invalid ptr" << endl;
            exit(EXIT_FAILURE);
         }
         tagData& lcData = *(lcIteratoor->second);
         strncpy(lcData.cTarget, stData.cTarget, 20);
         
         int lnSessionExists = g_cSessionManager.DoesFreeSessionExistForUser(stData.cTarget);
         if(lnSessionExists != 0)
         {
            lnRetVal = g_cSessionManager.CreateASession(stData.cIdentifier,2,true);
            if(lnRetVal != 0)
            {
               LOG_LOGGER("Unable to create a session for %s",stData.cIdentifier);
               return -1;
            }
            int lnSessionId =   g_cSessionManager.GetFreeUserSessionIdForUser(stData.cIdentifier);
           // stData.nSessionId = lnSessionId;
         }
         else
         {
            int lnSessionId =   g_cSessionManager.GetFreeUserSessionIdForUser(stData.cTarget);
            lnRetVal = g_cSessionManager.AddMoreUserToSession(lnSessionId,stData.cIdentifier);
            if(lnRetVal != 0)
            {
               LOG_LOGGER("Unable to add user %s to session %d", stData.cIdentifier, lnSessionId);
               return -1;
            }
            //stData.nSessionId = lnSessionId;
         }
      }
      break;

      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT) :
      {
         char lcBuffer[sizeof(tagData)] = { 0 };

         tagData lstData = stData;
         lstData.nMessageCode = (long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MESSAGE;
         
         map<int , tagData*>::iterator lcIter = g_cClientIDStore.find(stData.nGlobalIdentifier);
         if (lcIter== g_cClientIDStore.end())
         {
            printf("%d not found", stData.cIdentifier);
            printf("%s %d", strerror(errno), __LINE__);
            exit(EXIT_FAILURE);
         }	 
         //map<string, tagData*>::iterator lcIter = g_cPortIdentifier.find(stData.cIdentifier);
         //if (lcIter == g_cPortIdentifier.end())
         //{
         //   cout << stData.cIdentifier << "not found";
         //   printf("%s %d", strerror(errno), __LINE__);
           // exit(EXIT_FAILURE);
         //}

         map<int , tagData*>::iterator lcIter1;// = //g_cClientIDStore.find(lcIter->second->cTarget);
         for(map<int,tagData*>::iterator lcIterClientIdentityStore = g_cClientIDStore.begin(); lcIterClientIdentityStore != g_cClientIDStore.end() ; lcIterClientIdentityStore++)
         {
              if(strcmp(lcIterClientIdentityStore->second->cIdentifier, lcIter->second->cTarget) == 0)
              {
                  lcIter1 = lcIterClientIdentityStore;
              }
         }
         if (lcIter1 == g_cClientIDStore.end())
         {
            cout << "Target not found" << lcIter->second->cTarget <<  "By Identifier " << stData.cIdentifier <<endl;
            printf("%s %d", strerror(errno), __LINE__);
            exit(EXIT_FAILURE);
         }
         
         tagData* lpNewData = new tagData();
         memcpy(lpNewData, lcIter1->second, sizeof(tagData));
         lpNewData->nCommand = (short)CCOMMAND_TYPE::CCOMMAND_TYPE_REQUEST_CLI;
         strncpy(lpNewData->cIdentifier, lstData.cIdentifier, 20);
         lpNewData->nMessageCode = lstData.nMessageCode;
         strncpy(lpNewData->cBuffer, lstData.cBuffer, MAXLINE);
         strncpy(lpNewData->cTarget, lstData.cTarget, 20);
         strncpy(lpNewData->cUniqueMessageIdentifier, lstData.cUniqueMessageIdentifier, 30);
         
         pthread_mutex_lock(&g_cResponseMutex);
         
         g_cResponseList.push_back((lpNewData));
         pthread_mutex_unlock(&g_cResponseMutex);
      }
      break;
     case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY):
     {
        
         //char lcBuffer[sizeof(tagData)] = { 0 };

         tagData lstData = stData;
         lstData.nMessageCode = (long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_SERV_TO_CLI;
		 //lstData.nCommand = (short)CCOMMAND_TYPE::CCOMMAND_TYPE_REQUEST_CLI;
         //memcpy(&lcBuffer, (char*)&lstData, sizeof(tagData));
         //map<string, tagData*>::iterator lcIter = g_cPortIdentifier.find(stData.cIdentifier);
         map<int, tagData*>::iterator lcIter = g_cClientIDStore.find(stData.nGlobalIdentifier);
         if (lcIter == g_cClientIDStore.end())
         {
            cout << stData.cIdentifier << "not found";
            printf("%s %d", strerror(errno), __LINE__);
           // exit(EXIT_FAILURE);
         }
         cout << "Delivery Handling " << endl;
         tagData* lpNewData = new tagData();
         memcpy(lpNewData, lcIter->second, sizeof(tagData));
         lpNewData->nCommand = (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY_CONF;
         lpNewData->bFinalResponse = true;
         strncpy(lpNewData->cIdentifier, lstData.cIdentifier, 20);
         strncpy(lpNewData->cTarget, lstData.cTarget, 20);
         lpNewData->nMessageCode = lstData.nMessageCode;// lcIter1->second->nMessageCode;
         strncpy(lpNewData->cBuffer, lstData.cBuffer, MAXLINE);
         strncpy(lpNewData->cTarget, lstData.cTarget, 20);
         strncpy(lpNewData->cUniqueMessageIdentifier, lstData.cUniqueMessageIdentifier, 30);
         
         pthread_mutex_lock(&g_cResponseMutex);
         
         g_cResponseList.push_back((lpNewData));
         
         pthread_mutex_unlock(&g_cResponseMutex);
     }
    
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
        stData.nCommand = (short)CCOMMAND_TYPE::CCOMMAND_TYPE_RESPONSE;
        stData.bFinalResponse = true;
		return 0;
      }
      break;

      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET) :
      {

         stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET_RESPONSE);
         stData.nCommand =  (short)CCOMMAND_TYPE::CCOMMAND_TYPE_RESPONSE;
         stData.bFinalResponse = true;
		 return 0;
      }
      break;
      
      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT) :
      {

         stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_RESPONSE);
         stData.nCommand =  (short)CCOMMAND_TYPE::CCOMMAND_TYPE_RESPONSE;
		 return 0;
      }
      break;
      
      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY) :
      {

         stData.nMessageCode = (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY_RES);
         stData.nCommand =  (short)CCOMMAND_TYPE::CCOMMAND_TYPE_RESPONSE;
		 return 0;
      }
      break;

      case ((long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY)) :
      {
    
         return 0;
      }
      
      default :
      {
         perror("invalid case in GetResponseForFunc");
      }
   }

   return -1;
}



#ifndef WIN32
void* CheckResponse(void*)
{
  while(true == g_bProgramShouldWork)
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
                 pthread_mutex_lock(&g_cResponseMutex);
                 cout << "resending data" << endl;
                 g_cResponseList.push_front(new tagData(lcIter->stData));
                 pthread_mutex_unlock(&g_cResponseMutex);
                 
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
     sleep(1);
   }
}

int MakeReSender(const tagData lstRecvData )
{
   int lnRetval = 0;
   string lcKey = "";
   lcKey = SuffixAppropirateUniqueIdentifier(lstRecvData.cUniqueMessageIdentifier, lstRecvData.nCommand);
   lnRetval = pthread_mutex_lock(&g_ReSenderMutex);
   if(lnRetval != 0)
   {
	   perror("mutex lock error");
	   exit(1);
   }
   
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
   
   lnRetval = pthread_mutex_unlock(&g_ReSenderMutex);
   if(lnRetval != 0)
   {
	   perror("mutex lock error");
	   exit(1);
   
   }
   
   return 0;
}

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





void* EventHandling(void* pEventHandlingArg)
{
   while(true == g_bProgramShouldWork)
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

int CheckIfNotDummyMessageCode(long long nMessageCode)
{
   if(nMessageCode == (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY)
   {
       return -1;
   }
   return 0;
}


void* ProcessThread(void* pArg)
{
   int lnReturnVal = 0;
   
   tagData* lstData = nullptr;
   while (true == g_bProgramShouldWork)
   {

      lnReturnVal = pthread_mutex_lock(&g_cProcessMutex);
      if (lnReturnVal != 0)
      {
            printf("%s, %d ", strerror(errno), __LINE__);
            perror("unable to take lock");
            exit(EXIT_FAILURE);
      }
#ifdef LOGGING
         printf("In funtion \nthread id = %d\n", pthread_self());
#endif

      
      if(!g_cProcessList.empty())
      {

#ifdef LOGGING
         cout << "process thread process mutex acquired" << endl;
#endif
         lstData = g_cProcessList.front();
         cout << "Process fired" << endl;
         g_cProcessList.pop_front();         
      }
      else
      {
         pthread_cond_wait(&g_cCondVarForProcessThread, &g_cProcessMutex);
         lnReturnVal = pthread_mutex_unlock(&g_cProcessMutex);
         if (lnReturnVal != 0)
         {
            printf("%s, %d", strerror(errno), __LINE__);
            perror("unable to unlock");      
            exit(EXIT_FAILURE);
         }
         continue;
      }
      cout << "the thread no is"<< pthread_self() << lstData->cBuffer << endl;
      
      lnReturnVal = pthread_mutex_unlock(&g_cProcessMutex);
      if (lnReturnVal != 0)
      {
         printf("%s, %d", strerror(errno), __LINE__);
         perror("unable to unlock");
         exit(EXIT_FAILURE);
      }
#ifdef LOGGING           
         cout << "process thread process mutex acquired" << endl;
#endif
      
      //sleep(3);
      int lnVal = ExecuteFunction(*lstData);
      if (lnVal != 0)
      {
         printf("%s, %d", strerror(errno), __LINE__);
         perror("Failure in Execute Function");
         exit(EXIT_FAILURE);
      }
      lnVal =  CheckIfNotDummyMessageCode(lstData->nMessageCode);
      if(lnVal != 0)
      {
           delete lstData;
           continue;
      }

      lnReturnVal = GetResponseForFunction(*lstData);
      if (0 > lnReturnVal)
      {
            cout << "wrong response code" << lstData->nMessageCode << endl;
            printf("%s", strerror(errno));
            exit(EXIT_FAILURE);
      }

      lnReturnVal = pthread_mutex_lock(&g_cResponseMutex);
      if (lnReturnVal != 0)
      {
         printf("%s, %d", strerror(errno), __LINE__);
         perror("unable to take lock");
         exit(EXIT_FAILURE);
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
         exit(EXIT_FAILURE);
      }
#ifdef LOGGING
         cout << "processthread mutex lock released" << endl;
#endif
   }
   //}
   return NULL;
}


void* SenderThread(void* pArg)
{
   tagData* lpstData = nullptr;
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
      
      lpstData = g_cResponseList.front();
      if(nullptr == lpstData)
      {
         cout << "error nullptr" << endl;
         printf("%s, %d", strerror(errno), __LINE__);
         exit(EXIT_FAILURE);
      }
      
      g_cResponseList.pop_front();
      lnReturnVal = pthread_mutex_unlock(&g_cResponseMutex);
      if (lnReturnVal != 0)
      {
         printf("%s, %d", strerror(errno), __LINE__);
         perror("unable to take lock");
         exit(EXIT_FAILURE);
      }
      if(g_nFlagDupliResend == 0)
      {
         bool lbUniqueMesg = IsMessageUnique(*lpstData);
      }
      int lnDataStructSize = sizeof(tagData);
      char lcBuffer[lnDataStructSize] = { 0 };
   
      memcpy(&lcBuffer, (char*)lpstData, lnDataStructSize);
      //sendto(lcIter1->second->stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData), 0, (const struct sockaddr *) &(lcIter1->second->stNetWork.addr), lcIter1->second->stNetWork.restrict);
      //cout << "network arams";
      lnReturnVal = sendto(lpstData->stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData), MSG_CONFIRM, (const struct sockaddr *) &(lpstData->stNetWork.addr), lpstData->stNetWork.restrict);
      if (0 > lnReturnVal)
      {
         printf("%s, %d", strerror(errno), __LINE__);
         exit(EXIT_FAILURE);
      }
      delete (lpstData);
      lpstData = nullptr;
      //#define LOGGING
#ifdef LOGGING        
      printf("%s", strerror(errno));
      printf("Hello message sent.\n");
#endif
   }
   return NULL;
}


//#define LOG_LOGGER(cToBeLogged, ...)             \
{                                                \
  char lcBuffer[200] = {0};                       \
  snprintf(lcBuffer,200,cToBeLogged, ##__VA_ARGS__);         \
  (g_cfstream <<  lcBuffer << endl);           \
}






#endif

#ifndef WIN32
int main()
{
   g_nFlagDupliResend = 0;
   
   //SIgnal Handling
   signal(SIGINT,HandleSignal);
   struct sigaction lstSigAction;
   lstSigAction.sa_flags = 0;
   lstSigAction.sa_handler  = HandleSignal;
   sigaction(SIGINT, &lstSigAction, NULL);
   
   //LOG File Handling START
   time_t lnTime;
   lnTime = time(NULL);
   struct tm*  psttm =  gmtime(&lnTime);
   char lcBuffera[200] = {0};
   //snprintf(lcBuffera,200,"%d:%d:%d%d-%d-%d",psttm->tm_hour,psttm->tm_min,psttm->tm_sec,psttm->tm_mday,psttm->tm_mon,psttm->tm_year );
   snprintf(lcBuffera,200,"%s-%d-%d-%d_%d%d%d.%s","log",psttm->tm_mday,psttm->tm_mon +1  ,psttm->tm_year + 1900,psttm->tm_hour,psttm->tm_min,psttm->tm_sec,"log");
   cout << " log file name "<< lcBuffera << endl;
   g_cfstream.open(lcBuffera,ios::in|ios::out | ios::app);
   if(g_cfstream.fail())
   {
      cout << "g_cfstream failed truncating or creating new file" << endl; 
      LOG_LOGGER( "g_cfstream failed truncating or creating new file");
      g_cfstream.open(lcBuffera,ios::in|ios::out|ios::trunc);
      g_cfstream.clear();
   }
   g_cfstream.seekp(ios::end); //output
   g_cfstream.seekg(ios::end);
   //LOG File Handling END
   //LOG_LOGGER("%d",1);
   
   g_OS = 1;

   int lnRetVal = 0;


//initializing all - conditional variables and creating threads
   if (pthread_cond_init(&g_cCondVarForProcessThread,NULL) != 0)
   {
      //printf("%s %d", strerror(errno), __LINE__);
      LOG_LOGGER("%s : conditional var init has failed", strerror(errno));
      //printf("\n conditional var init has failed\n");
      return 1;
   }
   
   if (pthread_mutex_init(&g_cProcessMutex, NULL) != 0)
   {
     // printf("%s %d", strerror(errno), __LINE__);
      ///printf("\n mutex init has failed\n");
      LOG_LOGGER("%s : mutex init has failed", strerror(errno));
      return 1;
   }
   
   if (pthread_mutex_init(&g_cResponseMutex, NULL) != 0)
   {
      //printf("%s %d", strerror(errno), __LINE__);
      //printf("\n mutex init has failed\n");
      LOG_LOGGER("%s : mutex init has failed", strerror(errno));
      return 1;
   }
       
   
   if (pthread_mutex_init(&g_cIdentifierMutex, NULL) != 0)
   {
      //printf("%s %d", strerror(errno), __LINE__);
      //printf("\n mutex init has failed\n");
      LOG_LOGGER("%s : mutex init has failed", strerror(errno));
      return 1;
   }

   if (pthread_mutex_init(&g_cGlobalIdentifierMutex, NULL) != 0)
   {
      //printf("%s %d", strerror(errno), __LINE__);
      //printf("\n mutex init has failed\n");
      LOG_LOGGER("%s : mutex init has failed", strerror(errno));
      return 1;
   }
   
   lnRetVal = pthread_create(&lnSenderPThread, NULL, SenderThread, NULL);
   if(0 > lnRetVal)
   {
      //printf("%s, %d", strerror(errno), __LINE__);
      //perror("pthread create thread");
      LOG_LOGGER("%s : thread creation failed", strerror(errno));
      exit(EXIT_FAILURE);
   }

   if(  g_nFlagDupliResend == 0) 
   {
      lnRetVal = pthread_create(&lnPThreadEventTime, NULL, CheckResponse,NULL);
      if(0 > lnRetVal)
      {
         //printf("%s, %d", strerror(errno), __LINE__);
        // perror("pthread create thread");
         LOG_LOGGER("%s : thread creation failed", strerror(errno));
         exit(EXIT_FAILURE);
      }
   }

   for (int lnCounter = 0; lnCounter< NO_OF_PROC_THREADS; lnCounter++)
   {
      lnRetVal = pthread_create(((pthread_t*)&(lnProcessPThread[lnCounter])), NULL, ProcessThread, NULL);
      if(0 > lnRetVal)
      {
         //printf("%s, %d", strerror(errno), __LINE__);
         //perror("pthread create thread");
         LOG_LOGGER("%s : thread creation failed for thread no %d", strerror(errno), lnCounter);
         exit(EXIT_FAILURE);
      }
   }
   
   lnRetVal = NetWorkInitialize(g_nMainSockFd);
   if (lnRetVal != EXIT_SUCCESS)
   {
      //printf("%s, %d", strerror(errno), __LINE__);
      //perror("Network Initialize failed");
      LOG_LOGGER("%s : NetWorkInitialize failed", strerror(errno));
      exit(EXIT_FAILURE);
   }

   long long lnSockAddrlen = 0, lnNoOfBytes = 0;
   tagData* lpstData = nullptr;
   bool lbDiscardPacket = false;

//Start of reciever program

   while (true == g_bProgramShouldWork)
   {
      lpstData = new tagData();
      lnSockAddrlen = sizeof(cliaddr);
      lnNoOfBytes = RecvUDPData(g_nMainSockFd, (char *)lpstData, sizeof(tagData), &cliaddr, lnSockAddrlen);
      if (lnNoOfBytes <= 0)
      {
         cout << "" << endl;
         delete lpstData;
         lpstData = nullptr;
         //printf("%s, %d", strerror(errno), __LINE__);
         LOG_LOGGER("%s : RecvUDPData failed", strerror(errno));
         exit(EXIT_FAILURE);
      }
      cout << lpstData->cUniqueMessageIdentifier << endl;
      cout << " packet from " << lpstData->cIdentifier << endl;
      if(g_nFlagDupliResend == 0)
      {
         lnRetVal = pthread_mutex_lock( &g_cIdentifierMutex);
         if (lnRetVal != EXIT_SUCCESS)
         {
            //printf("%s, %d", strerror(errno), __LINE__);
            //perror("pthread mutex lock error");
            LOG_LOGGER("%s : mutex lock failed", strerror(errno));
            exit(EXIT_FAILURE);
         }
        
//!        Rejecting duplicates any extra packets are discarded
        if(lpstData->nLatestClntSeqNo >lpstData->nSeqNo)
        {
           pthread_mutex_lock(&g_ReSenderMutex);
           if(!g_cEventResender.empty())
           {
               for(list<tagTimeData>::iterator lcIter =  g_cEventResender.begin();lcIter !=  g_cEventResender.end();)
               {
                      tagTimeData& lstData = *lcIter;
                      if( lpstData->nLatestClntSeqNo > lstData.stData.nSeqNo)
                      {
                           if(strcmp(lstData.stData.cIdentifier,lpstData->cIdentifier) == 0)
                           {
                               lcIter =g_cEventResender.erase(lcIter);
                           }
                      }
                      else
                      {
                           lcIter++;
                      }
               }
          } 
        }
         string lcKey = SuffixAppropirateUniqueIdentifier(lpstData->cUniqueMessageIdentifier, lpstData->nCommand);
         //if(lpstData->nCommand != (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY)
         {
             for(auto& lstIdentifiers : g_cIdentifierStore)
             {
                // cout << lstIdentifiers.c_str() << " ";
                if(0 == strcmp(lstIdentifiers.c_str(),lcKey.c_str()))
                {
                   LOG_LOGGER("%s : Duplicate Packet with identifier %s", strerror(errno), lstIdentifiers.c_str());
                   cout << "duplicate packet" << endl;
                   cout << lstIdentifiers.c_str() << " duplicated" << endl;;
                   lbDiscardPacket = true;
                   break;
                }
             }
             if(true == lbDiscardPacket)
            {
                delete lpstData;
                lpstData = nullptr;
                lbDiscardPacket = false;
                pthread_mutex_unlock(&g_cIdentifierMutex);
                continue;
            }
            g_cIdentifierStore.push_back(lcKey);
      }
	  
      pthread_mutex_unlock( &g_cIdentifierMutex);
	  
     // if(lpstData->nCommand != (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY)
      {
         if(MakeReSender(*lpstData) != EXIT_SUCCESS)
         {
             exit(1);
         }
	  }
    }
      if (lpstData->nMessageCode == (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER)
      {
         lpstData->stNetWork.fd = g_nMainSockFd;
      }
      memcpy(&(lpstData->stNetWork.addr), &cliaddr, sizeof(sockaddr_in));
      lpstData->stNetWork.fd = g_nMainSockFd;
      lpstData->stNetWork.restrict = sizeof(sockaddr_in);
      lpstData->stNetWork.flags = MSG_CONFIRM;
//##ifdef LOGGING        
      cout << "chat data " << lpstData->cBuffer << endl;
      cout << "identifier " << lpstData->cIdentifier << endl;
      cout << "target " << lpstData->cTarget << endl;
//##endif        
      lnRetVal = pthread_mutex_lock(&g_cProcessMutex);
      if (lnRetVal != 0)
      {
         delete lpstData;
         lpstData = nullptr;
         printf("%s %d", strerror(errno), __LINE__);
         exit(EXIT_FAILURE);
      }

#ifdef LOGGING
      cout << "mtex acquired process main" << endl;
#endif
      g_cProcessList.push_back(lpstData);
      
     
      lnRetVal = pthread_mutex_unlock(&g_cProcessMutex);
      if (lnRetVal != 0)
      {
         printf("%s %d", strerror(errno), __LINE__);
         exit(EXIT_FAILURE);
      }
      
      pthread_cond_signal(&g_cCondVarForProcessThread);
#ifdef LOGGING
      cout << "mtex Released process main" << endl;
#endif

   }
   
   lnRetVal = pthread_join(lnPThreadEventTime,NULL);
   if (lnRetVal != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to join lnPThreadEventTime");
      exit(EXIT_FAILURE);
   }
   
   lnRetVal = pthread_join(lnSenderPThread,NULL);
   if (lnRetVal != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to join lnSenderPThread");
      exit(EXIT_FAILURE);
   }
   
   for (int lnCounter = 0; lnCounter< NO_OF_PROC_THREADS; lnCounter++)
   {
      lnRetVal = pthread_join(lnProcessPThread[lnCounter],NULL);
      if (lnRetVal != 0)
      {
         printf("%s, %d", strerror(errno), __LINE__);
         perror("unable to join processthread");
         exit(EXIT_FAILURE);
      }
   }
   
   //close(g_nMainSockFd);
   if(CleanUp() != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("cleanup dailed");
      exit(EXIT_FAILURE);
   }
   
   lnRetVal = pthread_cond_destroy(&g_cCondVarForProcessThread);
   if (lnRetVal != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to destroy Conditional Variable");
      exit(EXIT_FAILURE);
   }
   
   
   lnRetVal = pthread_mutex_destroy(&g_cProcessMutex);
   if (lnRetVal != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to destroy mnutex");
      exit(EXIT_FAILURE);
   }

   lnRetVal = pthread_mutex_destroy(&g_cResponseMutex);
   if (lnRetVal != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to destroy mnutex");
      exit(EXIT_FAILURE);
   }
   
   
   lnRetVal = pthread_mutex_destroy(&g_cIdentifierMutex);
   if (lnRetVal != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to destroy mnutex");
      exit(EXIT_FAILURE);
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
            exit(EXIT_FAILURE);
         }

         lnReturnVal = GetResponseForFunction(lstData);
         if (0 > lnReturnVal)
         {
            cout << "wrong response code" << endl;
            //printf("%s", strerror(errno));
            // exit(EXIT_FAILURE);
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
         // exit(EXIT_FAILURE);
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
         exit(EXIT_FAILURE);
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
//            exit(EXIT_FAILURE);
//         }
//
//         lnReturnVal = GetResponseForFunction(lstData);
//         if (0 > lnReturnVal)
//         {
//            cout << "wrong response code" << endl;
//            //printf("%s", strerror(errno));
//            // exit(EXIT_FAILURE);
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
//               // exit(EXIT_FAILURE);
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
//               exit(EXIT_FAILURE);
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
//            exit(EXIT_FAILURE);
//         }
//         map<string, tagData>::iterator lcIter1 = g_cPortIdentifier.find(lcIter->second.cTarget);
//         if (lcIter1 == g_cPortIdentifier.end())
//         {
//            cout << "Target not found";
//            exit(EXIT_FAILURE);
//         }
//         int len, n;
//         n = sendto(lcIter1->second.stNetWork.fd, (const char *)&lcBuffer, sizeof(tagData), 0, (const struct sockaddr *) &(lcIter1->second.stNetWork.addr), lcIter1->second.stNetWork.restrict);
//         if (n <= 0)
//         {
//            printf("%s", strerror(errno));
//            exit(EXIT_FAILURE);
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
         exit(EXIT_FAILURE);

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
         exit(EXIT_FAILURE);
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
         exit(EXIT_FAILURE);
      }

      if (g_cLockerInt == 1)
      {
         g_cLockerInt = 0;
      }
      LeaveCriticalSection(&g_cProcessMutex);
      if (lnRetVal != 0)
      {
         printf("%s %d", strerror(errno), __LINE__);
         exit(EXIT_FAILURE);
      }
#ifdef LOGGING
      cout << "mtex Released process main" << endl;
#endif
#endif
#ifdef singlethread
      int lnVal = ExecuteFunction(lstData);
      if (lnVal != 0)
      {
         exit(EXIT_FAILURE);
      }
      printf("%s", strerror(errno));
      int lnReturnVal = GetResponseForFunction(lstData);
      if (lnReturnVal != 0)
      {
         exit(EXIT_FAILURE);
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
