#pragma once
#include "includes.h"
using namespace std;
/*
class CSession;
typedef list<CSession*>                     CSessionMangerStore;
typedef list<CSession*>::iterator           CSessionManagerStoreIterator;
typedef vector<tagSessionIdentifierData>                      CSessionStore;
typedef vector<tagSessionIdentifierData>::iterator            CSessionStoreIterator;



int AddTokenToStore(string cString);

int  GetConfig();



class CSession
{
   private:

   vector<tagSessionIdentifierData> cStore;
   int nSize;
   int nNoOfElements;
   bool m_bRestricted; //if truw then its a normal chat between two people
   int m_nSessionId;//sessionId generated for a chat
   
   public:

   CSession(int nA,bool bRestricted);
   

   int AddNextName(tagSessionIdentifierData stSessionIdentifierData);
   
   int UserExistsinSession(string cName);
   
   
   int FreeUserExistsinSession(string cName);
   
   int GetFreeSessionIdForUser(string cName);
   
   int GetGlobalClientIdentifierBySessionIdAndName(int nSessionId,string cName);
   
   
   int GetSessionId();
   void SetSessionId(int nSessionId);
};

class CSessionManager
{
   private:
   pthread_mutex_t m_cMutex;
   list<CSession*> m_cSessionStore;
   int IsSessionUnique(int nSessionId);

   public:
   int TakeLock();
   int ReleaseLock();
   
   int CreateASession(tagSessionIdentifierData stData,int nLength,bool bRestricted);
   
   
   int AddMoreUserToSession(int nSessionId,tagSessionIdentifierData stData);
   

   int DoesSessionExistForUser(string cName);
   
   int DoesFreeSessionExistForUser(string cName);
   
   
   int GetFreeUserSessionIdForUser(string cName);
   
   CSession* GetSessionBySessionId(int nSessionId);
   
   
   int GetGlobalClientIdentifierBySessionIdAndName(int nSessionId,string cName);

};
*/

string SuffixAppropirateUniqueIdentifier(string lcString,short nCommand);

tagBufferData ConvertToNetworkBuffer(tagData& stData);


tagData ConvertToDataStruct(tagBufferData& stData);

////////////////////FINIALIZINGyyyy FUNCTIONS/////////////////////////
int CleanUp();

////////////////////FINIALIZINGyyyy1 FUNCTIONS/////////////////////////
void HandleSignal(int nSignal); 

int ExecuteFunction(tagData& stData);

int GetResponseForFunction(tagData& stData);




#ifndef WIN32
void* RecieverThread(void*);

void* EventThread(void*);


int DeleteMsgFromResenderStoreByUniqueIdentifier(const tagData lstRecvData );

bool IsMessageUniqueSoAddToResenderStore(tagData stData);

void* EventHandling(void* pEventHandlingArg);


int RejectDummyMsgCode(long long nMessageCode);



void* ProcessThread(void* pArg);



void* SenderThread(void* pArg);



//#define LOG_LOGGER(cToBeLogged, ...)             \
{                                                \
  char lcBuffer[200] = {0};                       \
  snprintf(lcBuffer,200,cToBeLogged, ##__VA_ARGS__);         \
  (g_cfstream <<  lcBuffer << endl);           \
}


string GetValueForKey(string cKey);
int AddTokenToStore(string cString);
int  GetConfig(string cFileName);



#endif

#ifndef WIN32
int main();

#endif



