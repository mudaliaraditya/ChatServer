#ifndef CSESSION_H
#define CSESSION_H

class CSession;





class CSession
{
   private:

      std::vector<tagSessionIdentifierData> cStore;
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
         nNoOfElements = 0;
         m_bRestricted = bRestricted;
      }

      CSession(const CSession& cSession)
      {
         *this = cSession;
      }

      CSession operator=(CSession cSession)
      {
         cStore.reserve(cSession.nSize);
         nSize = cSession.nSize;
         nNoOfElements = 0;
         m_bRestricted = cSession.m_bRestricted;
         return *this;
      }

     int SetNewRandomSessionId()
      {
         m_nSessionId = rand();
         return m_nSessionId;
      }


      int AddNextName(tagSessionIdentifierData stSessionIdentifierData)
      {
         if((nSize >  cStore.size()) && (m_bRestricted == true))
         {
            cStore.push_back(stSessionIdentifierData);
            nNoOfElements += 1;
            return 0;
         }
         else
         {
            cStore.push_back(stSessionIdentifierData);
            nNoOfElements += 1;
            return 0;
         }
         return -1;
      }

      int UserExistsinSession(std::string cName)
      {
         for(CSessionStoreIterator lcIter =   cStore.begin();lcIter != cStore.end();lcIter++)
         {
            if((*lcIter).sName == cName)
            {
               return 0;
            }
         }
         return -1;
      }


      int FreeUserExistsinSession(std::string cName)
      {
         if(nNoOfElements < nSize)
         {
            for(CSessionStoreIterator lcIter =   cStore.begin();lcIter != cStore.end();lcIter++)
            {
               if((*lcIter).sName == cName)
               {
                  return 0;
               }
            }
         }
      }

      int GetFreeSessionIdForUser(std::string cName)
      {
         if(nNoOfElements < nSize)
         {
            for(CSessionStoreIterator lcIter =   cStore.begin();lcIter != cStore.end();lcIter++)
            {
               if((*lcIter).sName == cName)
               {
                  return m_nSessionId;
               }
            }
         }
         return -1;
      }

      int GetGlobalClientIdentifierBySessionIdAndName(int nSessionId,std::string cName)
      {
         //if(nNoOfElements < nSize)
         {
            for(CSessionStoreIterator lcIter =   cStore.begin();lcIter != cStore.end();lcIter++)
            {
               if(((*lcIter).sName == cName) && m_nSessionId == nSessionId)
               {
                  return (*lcIter).nGlobalIdentifier;
               }
            }
         }
         return -1;
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
      pthread_mutex_t m_cMutex;
      std::list<CSession*> m_cSessionStore;
      int IsSessionUnique(int nSessionId)
      {

         for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
         {
            if((*lcIter)->GetSessionId() == nSessionId)
            {
               return -1;
            }
         }
         return 0;
      }
   public:
      int TakeLock()
      {
         return pthread_mutex_lock(&m_cMutex);
      }


      int ReleaseLock()
      {
         return pthread_mutex_unlock(&m_cMutex);
      }


      int CreateASession(tagSessionIdentifierData stData,int nLength,bool bRestricted)
      {
         CSession* lcSession = new CSession(nLength,bRestricted);
         // CSession* lcSession = NULL;//new CSession(nLength,bRestricted);
         //lcSession->Initialize();
         int lnSessionNo = 0;
         do
         {
            lnSessionNo = lcSession->SetNewRandomSessionId();
         }while(IsSessionUnique(lnSessionNo) != 0);
         lcSession->AddNextName(stData);
         lcSession->SetSessionId(lnSessionNo);
         m_cSessionStore.push_back(lcSession);
         return 0;
      }



      int AddMoreUserToSession(int nSessionId,tagSessionIdentifierData stData)
      {
         int lnRetVal = 0;
         for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
         {
            if((*lcIter)->GetSessionId() == nSessionId)
            {
               lnRetVal = (*lcIter)->AddNextName(stData);
               if(lnRetVal == 0)
               {
                  break;
               }
            }
         }
         return lnRetVal;
      }
             

      int DoesSessionExistForUser(std::string cName)
      {
         for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
         {
            if((*lcIter)->UserExistsinSession(cName) == 0)
            {
               //cout << "yes the user exists" << endl;
               return 0;
            }
         }
         return -1;
      }

      int DoesFreeSessionExistForUser(std::string cName)
      {
         for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
         {
            if((*lcIter)->FreeUserExistsinSession(cName) == 0)
            {
               //cout << "yes the user exists" << endl;
               return 0;
            }
         }
         return -1;
      }


      int GetFreeUserSessionIdForUser(std::string cName)
      {
         for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
         {
            int lnSessionId = (*lcIter)->GetFreeSessionIdForUser(cName);
            if( lnSessionId != -1)
            {
               //cout << "yes the user exists" << endl;
               return lnSessionId;
            }
         }
         return -1;
      }

      CSession* GetSessionBySessionId(int nSessionId)
      {
         for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
         {
            if((*lcIter)->GetSessionId() == nSessionId)
            {
               //cout << "yes the user exists" << endl;
               return (*lcIter);
            }
         }
         return NULL;
      }



      int GetGlobalClientIdentifierBySessionIdAndName(int nSessionId,std::string cName)
      {
         for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();lcIter++)
         {
            if((*lcIter)->GetSessionId() == nSessionId)
            {
               //cout << "yes the user exists" << endl;
               //return (*lcIter);
               return (*lcIter)->GetGlobalClientIdentifierBySessionIdAndName(nSessionId,cName);
            }
         }
         return -1;
      }

      ~CSessionManager()
      {

         for(CSessionManagerStoreIterator lcIter = m_cSessionStore.begin(); lcIter != m_cSessionStore.end();)
         {
            CSession* lpcSession = *lcIter;
            delete(lpcSession);
            lpcSession = NULL;
            lcIter = m_cSessionStore.erase(lcIter);
         }
      }

};






















#endif
