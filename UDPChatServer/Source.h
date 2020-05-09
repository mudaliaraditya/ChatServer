#ifndef SOURCE_H
#define SOURCE_H

#include <vector>


class CSession;
typedef std::list<CSession*>                                       CSessionMangerStore;
typedef std::list<CSession*>::iterator                             CSessionManagerStoreIterator;
typedef std::vector<tagSessionIdentifierData>                      CSessionStore;
typedef std::vector<tagSessionIdentifierData>::iterator            CSessionStoreIterator;

#pragma pack(1)







template<typename T,typename G>
int ClearStore(T& cStore);




template<typename T,typename G>
int ClearPairStore(T& cStore);



std::string SuffixAppropirateUniqueIdentifier(std::string lcString, short nCommand);





////////////////////FINIALIZINGyyyy FUNCTIONS/////////////////////////
int CleanUp();


////////////////////FINIALIZINGyyyy1 FUNCTIONS/////////////////////////
void HandleSignal(int nSignal);


int ExecuteFunction(tagData& stData);


int GetResponseForFunction(tagData& stData);




   #ifndef WIN32
void* RecieverThread(void* pData);



void* EventThread(void*);


int DeleteMsgFromResenderStoreByUniqueIdentifier(const tagData lstRecvData );


bool IsMessageUniqueSoAddToResenderStore(tagData stData);

int JoinAllThreads();


int DestroyMutexCondVar();

void* EventHandling(void* pEventHandlingArg);


int RejectDummyMsgCode(long long nMessageCode);


/*
Handles Conversion from Request to Response
Process Data
*/
int HandleRequest(tagData* pstData);





void* ProcessThread(void* pArg);



void* SenderThread(void* pArg);



//#define LOG_LOGGER(cToBeLogged, ...)             \
{                                                \
  char lcBuffer[200] = {0};                       \
  snprintf(lcBuffer,200,cToBeLogged, ##__VA_ARGS__);         \
  (g_cfstream <<  lcBuffer << endl);           \
}



int AddDataToStore(std::list<tagData> cList, pthread_mutex_t cMutex);

int InitializeAllMutex();
   #endif

   #ifndef WIN32

   #endif


int InitiateLogging();


int CreateAllThreads();




void Initialize();



int JoinAllThreads();



int DestroyMutexCondVar();


int main();

#endif
