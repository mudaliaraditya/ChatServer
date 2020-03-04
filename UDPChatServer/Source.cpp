#include "includes.h"
using namespace std;

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
         break;


      default:
         {

         }
         break;

   }
   return lcKey;
}



tagBufferData ConvertToNetworkBuffer(tagData& stData)
{
   tagBufferData lstNetworkData = {0};
   lstNetworkData.nCommand = stData.nCommand;
   lstNetworkData.nGlobalIdentifier = stData.nGlobalIdentifier;
   strncpy(lstNetworkData.cIdentifier ,stData.cIdentifier ,20);
   lstNetworkData.nFrOrToServerFlg = stData.nFrOrToServerFlg;
   lstNetworkData.nMessageCode    = stData.nMessageCode;
   strncpy(lstNetworkData.cBuffer, stData.cBuffer, MAXLINE);
   strncpy(lstNetworkData.cTarget, stData.cTarget, 20 );
   strncpy(lstNetworkData.cUniqueMessageIdentifier,stData.cUniqueMessageIdentifier ,30);
   lstNetworkData.nSeqNo = stData.nSeqNo;
   lstNetworkData.bFinalResponse = stData.bFinalResponse;
   lstNetworkData.nLatestClntSeqNo = stData.nLatestClntSeqNo;
   lstNetworkData.nSessionId = stData.nSessionId;
   return lstNetworkData;
}


tagData ConvertToDataStruct(tagBufferData& stData)
{
   tagData lstlocalData = {0};
   lstlocalData.nCommand = stData.nCommand;
   lstlocalData.nGlobalIdentifier = stData.nGlobalIdentifier;
   strncpy(lstlocalData.cIdentifier ,stData.cIdentifier ,20);
   lstlocalData.nFrOrToServerFlg = stData.nFrOrToServerFlg;
   lstlocalData.nMessageCode    = stData.nMessageCode;
   strncpy(lstlocalData.cBuffer, stData.cBuffer, MAXLINE);
   strncpy(lstlocalData.cTarget, stData.cTarget, 20 );
   strncpy(lstlocalData.cUniqueMessageIdentifier,stData.cUniqueMessageIdentifier ,30);
   lstlocalData.nSeqNo = stData.nSeqNo;
   lstlocalData.bFinalResponse = stData.bFinalResponse;
   lstlocalData.nLatestClntSeqNo = stData.nLatestClntSeqNo;
   lstlocalData.nSessionId = stData.nSessionId;
   return lstlocalData;
}


////////////////////FINIALIZINGyyyy FUNCTIONS/////////////////////////
int CleanUp()
{
   TESTLOG("%s","Doing Cleanup");
   g_bProgramShouldWork = false;
   int lnRetVal = 0;
   tagBufferData lstBufferData = {0}; 
   int lnSockAddrlen = sizeof(servaddr);
   int lnDataStructSize = sizeof(tagBufferData);
   //int lnNoOfBytes = SendUDPData(g_nMainSockFd, (char *)&lstBufferData, sizeof(tagBufferData), &servaddr, lnSockAddrlen);
   //lnRetVal = sendto(g_nMainSockFd, (const char *)&lstBufferData, lnDataStructSize, MSG_CONFIRM, (const struct sockaddr *) &(servaddr), lnSockAddrlen);
   if (0 > lnRetVal)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      exit(EXIT_FAILURE);
   }
   lnRetVal = close(g_nMainSockFd);
   if(lnRetVal != 0)
   {
      cout << "issue in closing socket" << endl;
      LOG_LOGGER("unable to close reciver socket");
      exit(1);
   }
   shutdown(g_nMainSockFd , SHUT_RDWR);
   //Cleaning g_cPortIdentifier store
   for(CIteratotrIdentiferDataStore lcIter = g_cPortIdentifier.begin(); lcIter != g_cPortIdentifier.end();lcIter++)
   {
      tagData* lpstData = lcIter->second;
      if(lpstData != nullptr)
      {
         delete (lpstData);
         lpstData = nullptr;
      }
   }

   //Cleaning g_cResponseList store 
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
   for(CClientIdDataStore::iterator lcIter = g_cClientIDStore.begin();lcIter != g_cClientIDStore.end();)
   {
      tagData* lpstData = lcIter->second;
      if(lpstData != NULL)
      {
         delete lpstData;
         lpstData = NULL;
      }
      lcIter = g_cClientIDStore.erase(lcIter);
   }

   for( CEventResenderStoreIterator lcIter = g_cEventResender.begin(); g_cEventResender.end() != lcIter; lcIter++)
   {
      tagTimeData lstData = (lcIter->second);
      TESTLOG("message code :  %d, Identifier :  %d, data : %s \n",lstData.stData.nMessageCode,lstData.stData.nGlobalIdentifier,lstData.stData.cBuffer);
      //if(lpstData != nullptr)
      //{
      //   delete (lpstData);
      //   lpstData = nullptr;
      //}
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
            if(pConfigObject != NULL)
            {
               DeleteNewMap(pConfigObject);
               pConfigObject = NULL;
            }
            lnRetVal = pthread_cond_broadcast(&g_cCondVarForProcessThread);
            if(lnRetVal != 0)
            {
               printf("%s, %d", strerror(errno), __LINE__);
               perror("unable to join Process Threads");
               exit(EXIT_FAILURE);
            }

            lnRetVal = pthread_join(lnRecieverThread, NULL);
            if (lnRetVal != 0)
            {
               printf("%s, %d", strerror(errno), __LINE__);
               perror("unable to join Reciver Thread ");
               exit(EXIT_FAILURE);
            }
            lnRetVal = pthread_join(lnSenderPThread,NULL);
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
            //exit(EXIT_SUCCESS);
           //cin.putback('S');
           g_ExceptionRaised = 1;
           streambuf *backup;
           string S = "S";
           stringstream oss(S);
           backup = cin.rdbuf();
           cin.rdbuf(oss.rdbuf());
           ready_for_reading = 0;
           return;
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
            //incrementint the sequenceNo                    
            if(((g_nClientIdentifier + 1) == INT_MAX))       
            {                                                
               //g_nClientIdentifier = 0;                   
               LOG_LOGGER("Max clients reached");           
               exit(EXIT_FAILURE);                          
            }                                                
            stData.nGlobalIdentifier = g_nClientIdentifier++;
            pthread_mutex_unlock(&g_cGlobalIdentifierMutex);
            tagData* lpstNewUserData = NULL;
            lpstNewUserData = new tagData(stData);
            if(lpstNewUserData == NULL)
            {
               LOG_LOGGER("new error");
               exit(1);
            }

            if ( 0 != pthread_mutex_lock(&g_cDataGlobalPortStoreMutex))
            {
               LOG_LOGGER("unable to take Lock on g_cDataGlobalPortStoreMutex");
               exit(1);
            }
            bool lbVal1 = (g_cClientIDStore.insert(pair<int , tagData*>(stData.nGlobalIdentifier, lpstNewUserData))).second;
            if (lbVal1 == false)
            {
               delete lpstNewUserData;
               lpstNewUserData = nullptr;
               printf("duplicate identifier insert to identifier(%s) store failed ", stData.cIdentifier);
               printf("%s %d", strerror(errno), __LINE__);
               exit(EXIT_FAILURE);
            }
            if ( 0 != pthread_mutex_unlock(&g_cDataGlobalPortStoreMutex))
            { 
               LOG_LOGGER("unable to unLock on g_cDataGlobalPortStoreMutex");
               exit(1);
            }
            // bool lbVal = (g_cPortIdentifier.insert(pair<string, tagData*>(stData.cIdentifier, lpstNewUserData))).second;
            // if (lbVal == false)
            // {
            //    delete lpstNewUserData;
            //    lpstNewUserData = nullptr;
            //    printf("duplicate identifier insert to identifier(%s) store failed ", stData.cIdentifier);
            //    printf("%s %d", strerror(errno), __LINE__);
            //    exit(EXIT_FAILURE);
            // }

         }
         break;

      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_REGISTER_TARGET) :
         {
            if ( 0 != pthread_mutex_lock(&g_cDataGlobalPortStoreMutex))
            {
               LOG_LOGGER("unable to take Lock on g_cDataGlobalPortStoreMutex");
               exit(1);
            }
            map<int , tagData*>::iterator lcSelfIter = g_cClientIDStore.find(stData.nGlobalIdentifier);
            if (lcSelfIter == g_cClientIDStore.end())
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
            if(lcSelfIter->second == nullptr)
            {
               LOG_LOGGER("invalid ptr");
               exit(EXIT_FAILURE);
            }
            tagData& lstSelfStoreData = *(lcSelfIter->second);
            strncpy(lstSelfStoreData.cTarget, stData.cTarget, 20);
            tagSessionIdentifierData lstSessionIdentifier;
            lstSessionIdentifier.sName = stData.cIdentifier;
            lstSessionIdentifier.nGlobalIdentifier = stData.nGlobalIdentifier;
            g_cSessionManager.TakeLock();
            int lnSessionExists = g_cSessionManager.DoesFreeSessionExistForUser(stData.cTarget);
            if(lnSessionExists != 0)
            {
               lnRetVal = g_cSessionManager.CreateASession(lstSessionIdentifier,2,true);
               if(lnRetVal != 0)
               {
                  LOG_LOGGER("Unable to create a session for %s",stData.cIdentifier);
                  g_cSessionManager.ReleaseLock();
                  return -1;
               }
               int lnSessionId =   g_cSessionManager.GetFreeUserSessionIdForUser(stData.cIdentifier);
               stData.nSessionId = lnSessionId;
               lstSelfStoreData.nSessionId = lnSessionId;
            }
            else
            {
               int lnSessionId =   g_cSessionManager.GetFreeUserSessionIdForUser(stData.cTarget);
               lnRetVal = g_cSessionManager.AddMoreUserToSession(lnSessionId,lstSessionIdentifier);
               if(lnRetVal != 0)
               {
                  LOG_LOGGER("Unable to add user %s to session %d", stData.cIdentifier, lnSessionId);
                  return -1;
               }
               stData.nSessionId = lnSessionId;
               lstSelfStoreData.nSessionId = lnSessionId;
            }
            if ( 0 != pthread_mutex_unlock(&g_cDataGlobalPortStoreMutex))
            { 
               LOG_LOGGER("unable to unLock on g_cDataGlobalPortStoreMutex");
               exit(1);
            }
            g_cSessionManager.ReleaseLock();
         }
         break;

      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT) :
         {
            char lcBuffer[sizeof(tagData)] = { 0 };

            //tagData lstData = stData;
            //lstData.nMessageCode = (long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MESSAGE;
            if ( 0 != pthread_mutex_lock(&g_cDataGlobalPortStoreMutex))
            { 
               LOG_LOGGER("unable to unLock on %s","g_cDataGlobalPortStoreMutex");
               exit(1);
            }
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

            int lnGlobalTargetIdentifier = g_cSessionManager.GetGlobalClientIdentifierBySessionIdAndName(stData.nSessionId, stData.cTarget);
            if(lnGlobalTargetIdentifier < 0)
            {
               LOG_LOGGER("ERROR");
               exit(1);
            }

            map<int , tagData*>::iterator lcIter1;// = //g_cClientIDStore.find(lcIter->second->cTarget);
            for(map<int,tagData*>::iterator lcIterClientIdentityStore = g_cClientIDStore.begin(); lcIterClientIdentityStore != g_cClientIDStore.end() ; lcIterClientIdentityStore++)
            {
               if(lcIterClientIdentityStore->second->nGlobalIdentifier == lnGlobalTargetIdentifier)
               {
                  lcIter1 = lcIterClientIdentityStore;
                  break;
               }
            }
            if (lcIter1 == g_cClientIDStore.end())
            {
               LOG_LOGGER("%s %s %s %s","Target not found" ,lcIter->second->cTarget ,  "By Identifier " , stData.cIdentifier );
               printf("%s %d", strerror(errno), __LINE__);
               exit(EXIT_FAILURE);
            }
//Code for forwarding data to the other client
            tagData* lpstForwardMessageData = new tagData();
            if(NULL == lpstForwardMessageData)
            {
               LOG_LOGGER("memory error");
               exit(EXIT_FAILURE);
            }
            memcpy(lpstForwardMessageData, lcIter1->second, sizeof(tagData));
            if ( 0 != pthread_mutex_unlock(&g_cDataGlobalPortStoreMutex))
            {
               LOG_LOGGER("unable to unLock on %s","g_cDataGlobalPortStoreMutex");
               exit(EXIT_FAILURE);
            }
            lpstForwardMessageData->nCommand = (short)CCOMMAND_TYPE::CCOMMAND_TYPE_REQUEST_CLI;
            strncpy(lpstForwardMessageData->cIdentifier, stData.cIdentifier, 20);
            lpstForwardMessageData->nMessageCode = (long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MESSAGE;
            lpstForwardMessageData->nSeqNo = stData.nSeqNo;
            strncpy(lpstForwardMessageData->cBuffer, stData.cBuffer, MAXLINE);
            strncpy(lpstForwardMessageData->cTarget, stData.cTarget, 20);
            strncpy(lpstForwardMessageData->cUniqueMessageIdentifier, stData.cUniqueMessageIdentifier, 30);

            pthread_mutex_lock(&g_cResponseMutex);

            g_cResponseList.push_back((lpstForwardMessageData));
            pthread_mutex_unlock(&g_cResponseMutex);
//End of Forwarding code
         }
         break;
      case (long long)(CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_DELIVRY):
         {

            tagData lstData = stData;
            if ( 0 != pthread_mutex_lock(&g_cDataGlobalPortStoreMutex))
            {
               LOG_LOGGER("%s ", "error in mutex");
               printf("%s %d", strerror(errno), __LINE__);
               exit(EXIT_FAILURE);
            }
            map<int, tagData*>::iterator lcIter = g_cClientIDStore.find(stData.nGlobalIdentifier);
            if (lcIter == g_cClientIDStore.end())
            {
               LOG_LOGGER("%s %s", stData.cIdentifier , " not found");
               printf("%s %d", strerror(errno), __LINE__);
               exit(EXIT_FAILURE);
            }
//Code for handling forwarding
            lstData.nMessageCode = (long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_CHAT_MSG_SERV_TO_CLI;
            TESTLOG("Delivery Handling ");
            tagData* lpNewData = new tagData();
            if(NULL == lpNewData )
            {
               LOG_LOGGER("memory error");
               exit(EXIT_FAILURE);
            }
            memcpy(lpNewData, lcIter->second, sizeof(tagData));
            if ( 0 != pthread_mutex_unlock(&g_cDataGlobalPortStoreMutex))
            {
               LOG_LOGGER("%s %s", stData.cIdentifier , "not found");
               printf("%s %d", strerror(errno), __LINE__);
               exit (EXIT_FAILURE); 
            }
            cout << "the seq no is "     << lstData.nSeqNo << endl;
            cout << "the Identifier is " << lstData.cIdentifier << endl;
            cout << "the Target is " << lstData.cTarget << endl;
            cout << lstData.nMessageCode << endl;// lcIter1->second->nMessageCode;
            lpNewData->nCommand = (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY_CONF;
            lpNewData->nSeqNo = lstData.nSeqNo;
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
//End of Forward Handling

         }
         break;
      default :
         {
            TESTLOG("invalid or dummy message code" );
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
            TESTLOG("%s","Invalid case");
            //perror("invalid case in GetResponseForFunc");
         }
   }

   return -1;
}



#ifndef WIN32
void* RecieverThread(void* pData)
{
   TESTLOG("%s","Thread : Reciever Thread");
   tagData* lpstData = NULL;
   fd_set          input_set;
   struct timeval  timeout;
   int lnReady_for_readingr = 0;
   int lnRetVal = 0;
   int lnNoOfBytes = 0;
   int lnSockAddrlen = 0;
   bool lbDiscardPacket = false;
   while (true == g_bProgramShouldWork)
   {
      lpstData = new tagData();
      if(NULL == lpstData)
      {
          LOG_LOGGER("error in new");
          exit(-1);
      }
      tagBufferData lstBufferData;
      lnSockAddrlen = sizeof(cliaddr);
#ifdef LOGGING
      cout << "Recieving.." << endl;
#endif
      FD_ZERO(&input_set);
      FD_SET(g_nMainSockFd, &input_set);

      timeout.tv_sec = 5;
      timeout.tv_usec = 0;


      lnReady_for_readingr = select(g_nMainSockFd + 1, &input_set, NULL, NULL, &timeout);
      //cout << "value of Slect ret "<< lnReady_for_readingr << endl;
      if (lnReady_for_readingr) {
      lnNoOfBytes = RecvUDPData(g_nMainSockFd, (char *)&lstBufferData, sizeof(tagBufferData), &cliaddr, lnSockAddrlen);
      if (lnNoOfBytes <= 0)
      {
         cout << "" << endl;
         if(lpstData != NULL)
         {
            delete lpstData;
            lpstData = nullptr;
         }
         //printf("%s, %d", strerror(errno), __LINE__);
         LOG_LOGGER("%s : RecvUDPData failed", strerror(errno));
         exit(EXIT_FAILURE);
      }
     }
     else
     {
         if(lpstData != NULL)
         {
            delete lpstData;
            lpstData = nullptr;
         }
         continue;
     }
     cout << "data recieved" << endl;
      lnRetVal =  Decrypt(reinterpret_cast<char*>(&lstBufferData),sizeof(lstBufferData));
      if( lnRetVal != 0)
      {
        LOG_LOGGER("%s : Decryption failed", strerror(errno));
        exit(1);
      } 
      *lpstData = ConvertToDataStruct(lstBufferData);
#ifdef LOGGING
      TESTLOG("Recieved");
#endif
      TESTLOG("%s %s", " packet with message identifier : ",lpstData->cUniqueMessageIdentifier);
      TESTLOG("%s %s" ," packet with data from " , lpstData->cIdentifier );
      TESTLOG("%s %d", " packet with data from ", lpstData->nMessageCode );
      TESTLOG("%s %d", " packet with current seqno ", lpstData->nSeqNo );
      TESTLOG("%s %d", " packet with last recieved seqno ", lpstData->nLatestClntSeqNo );
      if(g_nFlagDupliResend == 0)
      {
         lnRetVal = pthread_mutex_lock( &g_cIdentifierMutex);
         if (lnRetVal != EXIT_SUCCESS)
         {
            if(lpstData != NULL)
            {
               delete lpstData;
               lpstData = nullptr;
            }
            LOG_LOGGER("%s : mutex lock failed", strerror(errno));
            exit(EXIT_FAILURE);
         }

#ifdef LOGGING
         cout << "Taking Identifier Mutex" << __LINE__ <<endl;
#endif
         //!        Rejecting duplicates any extra packets are discarded
#ifdef LOGGING
#endif
         TESTLOG("duplicate packet Rejction started and lesser seq no rejection ");
         if(lpstData->nLatestClntSeqNo > lpstData->nSeqNo)
         {
            pthread_mutex_lock(&g_ReSenderMutex);
#ifdef LOGGING
#endif
            if(!g_cEventResender.empty())
            {
               for(CEventResenderStoreIterator lcIter =  g_cEventResender.begin();lcIter !=  g_cEventResender.end();)
               {
                  tagTimeData& lstData = (lcIter->second);
                  if( lpstData->nLatestClntSeqNo > lstData.stData.nSeqNo)
                  {
                     if((strcmp(lstData.stData.cIdentifier,lpstData->cIdentifier) == 0) && ( lstData.stData.nGlobalIdentifier == lpstData->nGlobalIdentifier))
                     {
                        TESTLOG("%s with latest seq cntr %d message clnt seq cntr %d","weird",lpstData->nLatestClntSeqNo,lpstData->nSeqNo);

                        lcIter =g_cEventResender.erase(lcIter);
                     }
                     else
                     {
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
#ifdef LOGGING
            cout << "Releasing Resender Mutex" << __LINE__ <<endl;
#endif
         }
         string lcKey = SuffixAppropirateUniqueIdentifier(lpstData->cUniqueMessageIdentifier, (short)lpstData->nCommand);
         //if(lpstData->nCommand != (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY)
         {
            // for(auto& lstIdentifiers : g_cIdentifierStore)
            //{
            // cout << lstIdentifiers.c_str() << " ";
            //     if(0 == strcmp(lstIdentifiers.c_str(),lcKey.c_str()))
            //     {
            //        LOG_LOGGER("%s : Duplicate Packet with identifier %s", strerror(errno), lstIdentifiers.c_str());
            //        cout << "duplicate packet" << endl;
            //        cout << lstIdentifiers.c_str() << " duplicated" << endl;;
            //        lbDiscardPacket = true;
            //        break;
            //     }
            CIterIdentifierStringStore lcIterIdentifierStringStore  = g_cIdentifierStore.find(lcKey);
            if( lcIterIdentifierStringStore != g_cIdentifierStore.end())
            {
               TESTLOG("%s : Duplicate Packet with identifier %s", strerror(errno), lcKey.c_str());
               #ifdef LOGGING
                  cout << "duplicate packet" << endl;
                  cout << lpstData->cUniqueMessageIdentifier << " duplicated" << endl;;
               #endif
               lbDiscardPacket = true;
            }
            //}
            if(true == lbDiscardPacket)
            {
               if(lpstData != NULL)
               {
                  delete lpstData;
                  lpstData = nullptr;
               }
               //delete lpstData;
               //lpstData = nullptr;
               lbDiscardPacket = false;
               pthread_mutex_unlock(&g_cIdentifierMutex);
               continue;
            }
            CRetValInsIterIdentifierStringStore lcRetValIterIdentifierStringStore = g_cIdentifierStore.insert(lcKey);
            if( lcRetValIterIdentifierStringStore.second == false)
            {
               if(lpstData != NULL)
               {
                  delete lpstData;
                  lpstData = nullptr;
               }

               LOG_LOGGER("%s : Duplicate Packet with identifier %s after checking for duplicate packets\n", strerror(errno), lcKey.c_str());
               exit(-1);     
            } 
         }

#ifdef LOGGING
         cout << "Done with Duplicate Packet Rejection" << endl;
#endif
         pthread_mutex_unlock( &g_cIdentifierMutex);
#ifdef LOGGING
         cout << "Releasing Identifier Mutex" << __LINE__ <<endl;
#endif

         // if(lpstData->nCommand != (short)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY)
         {
            cout << "deleting all old data" << endl;
            TESTLOG("DeleteMsgFromResenderStoreByUniqueIdentifier started")
            if(DeleteMsgFromResenderStoreByUniqueIdentifier(*lpstData) != EXIT_SUCCESS)
            {
               if(lpstData != NULL)
               {
                  delete lpstData;
                  lpstData = nullptr;
               } 
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
      #ifdef LOGGING        
      cout << "chat data " << lpstData->cBuffer << endl;
      cout << "identifier " << lpstData->cIdentifier << endl;
      cout << "target " << lpstData->cTarget << endl;
      #endif        
      lnRetVal = pthread_mutex_lock(&g_cProcessMutex);
      if (lnRetVal != 0)
      {
         if(lpstData != NULL)
         {
            delete lpstData;
            lpstData = nullptr;
         }
         //printf("%s %d", strerror(errno), __LINE__);
         LOG_LOGGER( "unable to take mutex lock %s",strerror(errno));
         exit(EXIT_FAILURE);
      }

#ifdef LOGGING
      TESTLOG("mtex acquired process reciever");
#endif
      g_cProcessList.push_back(lpstData);
      lpstData = nullptr; 

      

      if ( 0  != pthread_cond_signal(&g_cCondVarForProcessThread))
      {
         LOG_LOGGER( "unable to signal process thread %s",strerror(errno));
         exit(EXIT_FAILURE);
      }
      lnRetVal = pthread_mutex_unlock(&g_cProcessMutex);
      if (lnRetVal != 0)
      {
         printf("%s %d", strerror(errno), __LINE__);
         LOG_LOGGER( "unable to take mutex lock %s",strerror(errno));
         exit(EXIT_FAILURE);
      }
#ifdef LOGGING
      TESTLOG("mtex Released process reciever" );
#endif

   } 

   //cout << "deleting newly created struct" << endl;
   if(lpstData != nullptr)
   {
      delete lpstData;
      lpstData = nullptr;
   }
   pthread_exit(NULL);
   return NULL;
   //pthread_exit(NULL);
}


//========================================================================




void* EventThread(void*)
{
   char lcUniqueIdentifierBuffer[30 + 1] = {0};
   int lnSleeptIme = 0;
   tagData* lpstData = NULL;
   while(g_bProgramShouldWork == true)
   {

          pthread_mutex_lock(&g_ReSenderMutex);
#ifdef LOGGING
#endif
#ifdef LOGGING
          TESTLOG("taking sender lock \n");
#endif
          if(!g_cEventResender.empty())
          {
            CEventResenderStoreIterator lcIter = g_cEventResender.begin();
            TESTLOG("iterator time %d \n", lcIter->first);
            TESTLOG("current time %d \n",time(NULL));
            if(lcIter->first <= time(NULL) )
            {


               if((long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY == lcIter->second.stData.nMessageCode)
               {
                     // TESTLOG("Sending dummy message \n");
                     // do
                     // {
                     //   SetRand(lcUniqueIdentifierBuffer, 30);
                     // }while(VerifyUniqueness(lcUniqueIdentifierBuffer) != 0);
                     // strncpy(lcIter->second.stData.cUniqueMessageIdentifier, lcUniqueIdentifierBuffer,strlen(lcIter->second.stData.cUniqueMessageIdentifier)); 
               }
               //lcIter->second.stData.nLatestClntSeqNo = g_nLatestRecivedSequenceNo;
               TESTLOG("resending data \n");
               lpstData = new tagData();
               if(NULL == lpstData)
               {
                  LOG_LOGGER("new error");
                  exit(-1);
               }
               memcpy(lpstData,&(lcIter->second.stData),sizeof(tagData));
               pthread_mutex_lock(&g_cResponseMutex);
               g_cResponseList.push_front(lpstData);
               if(lcIter->second.m_nCounter > 1)
               {
                      TESTLOG("Resending Data :%s %d %s %d %s %d %s %s %s %d","Message Code:", lcIter->second.stData.nMessageCode,"the seq no is ", lcIter->second.stData.nSeqNo , " the LatestRecieved Seq no is " , lcIter->second.stData.nLatestClntSeqNo , " from user id and name ", lcIter->second.stData.cIdentifier ," " , lcIter->second.stData.nGlobalIdentifier);
                      lcIter->second.m_nCounter--;
                      lcIter->second.m_nTime = time(NULL) + 3;
                      g_cEventResender.insert(pair<time_t,tagTimeData>(lcIter->second.m_nTime,lcIter->second));
               }
               lnSleeptIme = lcIter->first;
               g_cEventResender.erase(lcIter);
               pthread_mutex_unlock(&g_cResponseMutex);
            }

          }

#ifdef LOGGING
          TESTLOG("releasing sender mutex \n");
#endif
          pthread_mutex_unlock(&g_ReSenderMutex);
#ifdef LOGGING
          TESTLOG("releasing resender mutex \n");
#endif
          //checks for resender events only once a second
          //sleep(1);
          if(lnSleeptIme != 0)
          {
             sleep(lnSleeptIme - time(NULL));
          }
   }
   pthread_exit(NULL);
   return NULL;
}

//===========================================================================
/*
void* aEventThread(void*)
{
   while(true == g_bProgramShouldWork)
   {
      pthread_mutex_lock(&g_ReSenderMutex);
#ifdef LOGGING
      //out << "Taking Resender Mutex" << __LINE__ <<endl;
#endif
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
                  TESTLOG( "resending data","");
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
#ifdef LOGGING
      // cout << "Releasing Resender Mutex" << __LINE__ <<endl;
#endif
      sleep(1);
   }
   return NULL;
}
*/
int DeleteMsgFromResenderStoreByUniqueIdentifier(const tagData lstRecvData )
{
   TESTLOG("started deleting messages");
   int lnRetval = 0;
   string lcKey = "";
   lcKey = SuffixAppropirateUniqueIdentifier(lstRecvData.cUniqueMessageIdentifier, (short)lstRecvData.nCommand);
   lnRetval = pthread_mutex_lock(&g_ReSenderMutex);
   if(lnRetval != 0)
   {
      perror("mutex lock error");
      exit(1);
   }
#ifdef LOGGING
   TESTLOG("%s %d", "Taking Resender Mutex" ,__LINE__ );
#endif
   TESTLOG("%s %d %s %d %s %s %s %d", "the seq no is ", lstRecvData.nSeqNo , " the LatestRecieved Seq no is " , lstRecvData.nLatestClntSeqNo , " from user id and name ", lstRecvData.cIdentifier ," " , lstRecvData.nGlobalIdentifier);
   for(CEventResenderStoreIterator lcIter =  g_cEventResender.begin();lcIter != g_cEventResender.end();)
   {
      tagTimeData& lstData = lcIter->second;
      TESTLOG("%s", lstData.stData.cUniqueMessageIdentifier );
      //message instore whose sequenceno is less than the latest clnt seqno that is recieved for the same user with same session id
      /*if(( (lstData.stData.nSessionId == lstRecvData.nSessionId) && (strcmp(lstData.stData.cIdentifier,lstRecvData.cIdentifier) == 0) &&
       (
            (long long)CCOMMAND_TYPE::CCOMMAND_TYPE_RESPONSE      == lstRecvData.nCommand ||
            (long long)CCOMMAND_TYPE::CCOMMAND_TYPE_DELIVERY_RES == lstRecvData.nCommand
       )

))
      {
         TESTLOG( "deleting all lower seqno for user id %d" , lstRecvData.nGlobalIdentifier );
         lcIter = g_cEventResender.erase( lcIter);
         //continue;
      }
      else if((strcmp(lstData.stData.cUniqueMessageIdentifier,lstRecvData.cUniqueMessageIdentifier) == 0) && (lstData.stData.nSessionId == lstRecvData.nSessionId))
      {
         //TESTLOG( "response recieved now erasing","" );
         //lcIter = g_cEventResender.erase( lcIter);
         //continue;
         lcIter++;
      }
      else
      {
         lcIter++;
      } */
      if((lstData.stData.nSeqNo < lstRecvData.nLatestClntSeqNo) && (lstData.stData.nSessionId == lstRecvData.nSessionId) && (strcmp(lstData.stData.cIdentifier,lstRecvData.cIdentifier) == 0))
      {
         TESTLOG( "deleting all lower seqno for user id %d" , lstRecvData.nGlobalIdentifier );
         lcIter = g_cEventResender.erase( lcIter);
         //continue;
      }
      else if((strcmp(lstData.stData.cUniqueMessageIdentifier,lstRecvData.cUniqueMessageIdentifier) == 0) && (lstData.stData.nSessionId == lstRecvData.nSessionId))
      {
         TESTLOG( "response recieved now erasing" );
         lcIter = g_cEventResender.erase( lcIter);
         //continue;
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
#ifdef LOGGING
   cout << "Releasing Resender Mutex" << __LINE__ <<endl;
#endif
   TESTLOG("%s","finished deleting messages");

   return 0;
}

bool IsMessageUniqueSoAddToResenderStore(tagData stData)
{
   pthread_mutex_lock(&g_ReSenderMutex);
   bool lbUniqueMessageSend = true;
   for(auto& lcTemp : g_cEventResender)
   {
      if(strcmp(lcTemp.second.stData.cUniqueMessageIdentifier, stData.cUniqueMessageIdentifier) == 0)
      {
         TESTLOG("Resending Message as no response received yet" );
         lbUniqueMessageSend = false;
      }
   }

   if(lbUniqueMessageSend == true)
   {
      TESTLOG("this is a new unique message");
      tagTimeData lstTimeData((time(NULL) + 10), stData);
      g_cEventResender.insert(pair<long long,tagTimeData>(lstTimeData.m_nTime,lstTimeData));
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
         //g_cIdentifierStore.pop_front();
         pthread_mutex_unlock(&g_cIdentifierMutex);
      }
      else
      {
         pthread_mutex_unlock(&g_cIdentifierMutex);
      }
   }
   return NULL;
}

int RejectDummyMsgCode(long long nMessageCode)
{
   if(nMessageCode == (long long)CMESSAGE_CODE_ACTIONS::MESSAGE_CODE_ACTIONS_DUMMY)
   {
      return -1;
   }
   return 0;
}


void* ProcessThread(void* pArg)
{
   TESTLOG("%s","Thread : Process Thread");
   int lnReturnVal = 0;

   tagData* lstData = nullptr;
   while (true == g_bProgramShouldWork)
   {

      TESTLOG("%s ","Thread getting inside loop");
      lnReturnVal = pthread_mutex_lock(&g_cProcessMutex);
      if (lnReturnVal != 0)
      {
         printf("%s, %d ", strerror(errno), __LINE__);
         perror("unable to take lock");
         exit(EXIT_FAILURE);
      }
#ifdef LOGGING
      TESTLOG("%s %d","Taking Process Mutex in Process Thread" , __LINE__);
#endif
#ifdef LOGGING
      TESTLOG("In function ProcessThread thread id = %d\n", pthread_self());
#endif


      while(g_cProcessList.empty())
      {
         TESTLOG("%s ","taking conditional lock");
         pthread_cond_wait(&g_cCondVarForProcessThread, &g_cProcessMutex);
         TESTLOG("%s ","unlocking mutex after conditional lock");
         //lnReturnVal = pthread_mutex_unlock(&g_cProcessMutex);
         if (lnReturnVal != 0)
         {
            printf("%s, %d", strerror(errno), __LINE__);
            perror("unable to unlock");      
            exit(EXIT_FAILURE);
         }
#ifdef LOGGING
         TESTLOG("%s %d", "Releasing Process Mutex in cond var" , __LINE__ );
#endif
         if((false == g_bProgramShouldWork) && g_cProcessList.empty())
         {
            //lnReturnVal = pthread_mutex_unlock(&g_cProcessMutex);
            if (lnReturnVal != 0)
            {
               printf("%s, %d", strerror(errno), __LINE__);
               perror("unable to unlock");
               exit(EXIT_FAILURE);
            }
            break;
            //return NULL;
         }
         //continue;
      }
      if(!g_cProcessList.empty())
      {

#ifdef LOGGING
         TESTLOG("%s", "process thread process mutex acquired" );
#endif
         lstData = g_cProcessList.front();
         TESTLOG( "Process fired" );
         g_cProcessList.pop_front();         
      }
      else
      {
          //if(false == g_bProgramShouldWork)
           {
             lnReturnVal = pthread_mutex_unlock(&g_cProcessMutex);
             if (lnReturnVal != 0)
             {  
               printf("%s, %d", strerror(errno), __LINE__);
               perror("unable to unlock");
               exit(EXIT_FAILURE);
             }
             continue; 
           }
      }

      TESTLOG("%s %d %s ","the thread no is",pthread_self(), lstData->cBuffer );

      lnReturnVal = pthread_mutex_unlock(&g_cProcessMutex);
      if (lnReturnVal != 0)
      {
         printf("%s, %d", strerror(errno), __LINE__);
         perror("unable to unlock");
         exit(EXIT_FAILURE);
      }
#ifdef LOGGING
      TESTLOG( "Releasing Process Mutex %d", __LINE__ );
#endif
#ifdef LOGGING           
      TESTLOG("%s", "process thread process mutex acquired");
#endif
      if(NULL == lstData)
      {
         LOG_LOGGER("NULL lstData");
         perror("Failure  NULL check in ProcessThread");
         exit(EXIT_FAILURE);
      }
      //sleep(3);
      int lnVal = ExecuteFunction(*lstData);
      if (lnVal != 0)
      {
         LOG_LOGGER("%s, %d", strerror(errno), __LINE__);
         perror("Failure in Execute Function");
         exit(EXIT_FAILURE);
      }
      lnVal =  RejectDummyMsgCode(lstData->nMessageCode);
      if(lnVal != 0)
      {
         TESTLOG( "dummy message","" );
         delete lstData;
         lstData = NULL;
         continue;
      }
      TESTLOG( "not a dummy message","");
      lnReturnVal = GetResponseForFunction(*lstData);
      if (0 > lnReturnVal)
      {
         TESTLOG("%s %d", "wrong response code" , lstData->nMessageCode );
         printf("%s", strerror(errno));
         exit(EXIT_FAILURE);
      }

      lnReturnVal = pthread_mutex_lock(&g_cResponseMutex);
      if (lnReturnVal != 0)
      {
         LOG_LOGGER("%s, %d", strerror(errno), __LINE__);
         perror("unable to take lock");
         exit(EXIT_FAILURE);
      }
#ifdef LOGGING
      TESTLOG ("%s","processthread mutex lock acquired");
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
      TESTLOG("%s","processthread mutex lock released");
#endif
   }
   pthread_exit(NULL);   
   return NULL;
}

/*
   This Thread Is Responsible for Sending data out
 */
void* SenderThread(void* pArg)
{
   TESTLOG("%s","Thread : Sender Thread");
   tagData* lpstData = nullptr;
   int lnReturnVal = 0;
   // Initislization
   int lnDataStructSize = sizeof(tagBufferData);
   char lcBuffer[sizeof(tagBufferData)] = { 0 };
   tagBufferData lstBufferData;
   bool lbUniqueMesg = false;
   //Initializatoin end
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
         LOG_LOGGER("error nullptr");
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
         lbUniqueMesg = IsMessageUniqueSoAddToResenderStore(*lpstData);
      }
      lstBufferData = ConvertToNetworkBuffer(*lpstData) ;
      lnReturnVal = Encrypt(reinterpret_cast<char*>(&lstBufferData),sizeof(lstBufferData));
      if(lnReturnVal != 0)
      {
        TESTLOG("%s","Thread : Sender Thread");
        exit(EXIT_FAILURE); 
      }
      memcpy(&lcBuffer, (char*)&lstBufferData, lnDataStructSize);
      lnReturnVal = sendto(lpstData->stNetWork.fd, (const char *)&lcBuffer, lnDataStructSize, MSG_CONFIRM, (const struct sockaddr *) &(lpstData->stNetWork.addr), lpstData->stNetWork.restrict);
      if (0 > lnReturnVal)
      {
         printf("%s, %d", strerror(errno), __LINE__);
         exit(EXIT_FAILURE);
      }
      memset(&lcBuffer, 0, lnDataStructSize);
      delete (lpstData);
      lpstData = nullptr;
      //#define LOGGING
#ifdef LOGGING        
      printf("%s", strerror(errno));
      printf("Hello message sent.\n");
#endif
   }
   pthread_exit(NULL);
   return NULL;
}


//#define LOG_LOGGER(cToBeLogged, ...)             \
{                                                \
   char lcBuffer[200] = {0};                       \
      snprintf(lcBuffer,200,cToBeLogged, ##__VA_ARGS__);         \
      (g_cfstream <<  lcBuffer << endl);           \
}


int InitializeAllMutex()
{ 
   if (pthread_mutex_init(&g_cProcessMutex, NULL) != 0)
   {
      LOG_LOGGER("%s : process mutex init has failed", strerror(errno));
      return -1;
   }

   if (pthread_mutex_init(&g_cResponseMutex, NULL) != 0)
   {
      LOG_LOGGER("%s : respponse mutex init has failed", strerror(errno));
      return -1;
   }


   if (pthread_mutex_init(&g_cIdentifierMutex, NULL) != 0)
   {
      LOG_LOGGER("%s : identifier mutex init has failed", strerror(errno));
      return -1;
   }

   if (pthread_mutex_init(&g_cGlobalIdentifierMutex, NULL) != 0)
   {
      LOG_LOGGER("%s : GlobalIdentifier mutex init has failed", strerror(errno));
      return -1;
   }

   if (pthread_mutex_init(&g_cDataGlobalPortStoreMutex, NULL) != 0)
   {
      LOG_LOGGER("%s : mutex init has failed", strerror(errno));
      return -1;
   }

   return 0;
}




#endif

#ifndef WIN32



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
   g_cDatafstream.open(lcDataLogFileName,ios::in|ios::out | ios::app);
   if(g_cDatafstream.fail())
   {
      LOG_LOGGER("%s","g_cDatafstream failed truncating or creating new file"); 
      LOG_LOGGER( "g_cDatafstream failed truncating or creating new file");
      g_cDatafstream.open(lcDataLogFileName, ios::in|ios::out|ios::trunc);
      if(g_cfstream.fail())
      {
          return EXIT_FAILURE;
      }
      g_cDatafstream.clear();
   }
   g_cDatafstream.seekp(ios::end); //output
   g_cDatafstream.seekg(ios::end);

   return 0;
}


int main()
{
   int lnRetVal =0;
   g_nFlagDupliResend = 0;

   //SIgnal Handling
   signal(SIGINT, HandleSignal);
   struct sigaction lstSigAction = {0};
   lstSigAction.sa_flags = 0;
   lstSigAction.sa_handler  = HandleSignal;
   sigaction(SIGINT, &lstSigAction, NULL);
   fd_set          input_set = {0};
   struct timeval  timeout = {0};
   timeout.tv_sec = 1;
   timeout.tv_usec = 0;

  


   pConfigObject = CreateNewMap();
   if(pConfigObject == NULL)
   {
      return -1;
   } 
   lnRetVal =  GetConfig(CNF_FILE_NAME,pConfigObject); 
   if(0 != lnRetVal)
   {
      return -1;
   }
   //LOG File Handling START
   if (0 !=InitiateLogging())
   {
      TESTOUT("Error in intializing Log files exiting");
      return -1;
   }
   //LOG File Handling END
   //LOG_LOGGER("%d",1);

   g_OS = 1;

   lnRetVal = 0;

   lnRetVal = NetWorkInitialize(g_nMainSockFd);
   if (lnRetVal != EXIT_SUCCESS)
   {
      //printf("%s, %d", strerror(errno), __LINE__);
      //perror("Network Initialize failed");
      LOG_LOGGER("%s : NetWorkInitialize failed", strerror(errno));
      exit(EXIT_FAILURE);
   }

   //initializing all - conditional variables and creating threads
   if (pthread_cond_init(&g_cCondVarForProcessThread,NULL) != 0)
   {
      //printf("%s %d", strerror(errno), __LINE__);
      LOG_LOGGER("%s : conditional var init has failed", strerror(errno));
      //printf("\n conditional var init has failed\n");
      exit (EXIT_FAILURE);
   }

   lnRetVal = InitializeAllMutex();
   if(0 > lnRetVal)
   {
      LOG_LOGGER("%s : Mutex initialization failed", strerror(errno));
      exit(EXIT_FAILURE);
   }
   lnRetVal = pthread_create( &lnRecieverThread, NULL, RecieverThread, NULL);
   if(0 > lnRetVal)
   {
      //printf("%s, %d", strerror(errno), __LINE__);
      //perror("pthread create thread");
      LOG_LOGGER("%s : thread creation failed", strerror(errno));
      exit(EXIT_FAILURE);
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
      lnRetVal = pthread_create(&lnPThreadEventTime, NULL, EventThread,NULL);
      if(0 > lnRetVal)
      {
         LOG_LOGGER("%s : thread creation failed", strerror(errno));
         exit(EXIT_FAILURE);
      }
   }

   for (int lnCounter = 0; lnCounter< NO_OF_PROC_THREADS; lnCounter++)
   {
      lnRetVal = pthread_create(((pthread_t*)&(lnProcessPThread[lnCounter])), NULL, ProcessThread, NULL);
      if(0 > lnRetVal)
      {
         LOG_LOGGER("%s : thread creation failed for thread no %d", strerror(errno), lnCounter);
         exit(EXIT_FAILURE);
      }
   }


   long long lnSockAddrlen = 0, lnNoOfBytes = 0;
   tagData* lpstData = nullptr;
   bool lbDiscardPacket = false;

   //Start of reciever program

   TESTLOG("%s","Thread : main Thread");
   cout << "type S to Stop" << endl;
   while (true == g_bProgramShouldWork)
   {

      string lcVal = "";
      FD_ZERO(&input_set);
      FD_SET(0, &input_set);

      ready_for_reading = select( 1, &input_set, NULL, NULL, &timeout);
      if (ready_for_reading) 
      {

         cin >> lcVal;

         if(strncmp(lcVal.c_str(),"S",lcVal.length())== 0)
         {
            g_bProgramShouldWork = false;
         }
         else
         {
            cout << "type S to Stop" << endl;
         }
      }
   }
   if(g_ExceptionRaised == 0)
  {
   if(CleanUp() != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("cleanup dailed");
      exit(EXIT_FAILURE);
   }

   if(pConfigObject != NULL)
   {
      DeleteNewMap(pConfigObject);
      pConfigObject = NULL;
   }

   lnRetVal = pthread_cond_broadcast(&g_cCondVarForProcessThread);
   if(lnNoOfBytes > 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to join lnPThreadEventTime");
      exit(EXIT_FAILURE);
   }

   lnRetVal = pthread_join(lnRecieverThread,NULL);
   if (lnRetVal != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to join lnPThreadEventTime");
      exit(EXIT_FAILURE);
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
      perror("unable to destroy mutex");
      exit(EXIT_FAILURE);
   }


   lnRetVal = pthread_mutex_destroy(&g_cResponseMutex);
   if (lnRetVal != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to destroy mutex");
      exit(EXIT_FAILURE);
   }


   lnRetVal = pthread_mutex_destroy(&g_cIdentifierMutex);
   if (lnRetVal != 0)
   {
      printf("%s, %d", strerror(errno), __LINE__);
      perror("unable to destroy mutex");
      exit(EXIT_FAILURE);
   }

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
