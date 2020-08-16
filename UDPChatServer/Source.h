#ifndef SOURCE_H
#define SOURCE_H

#include <vector>


class CSession;
typedef std::list<CSession*>                                       CSessionMangerStore;
typedef std::list<CSession*>::iterator                             CSessionManagerStoreIterator;
typedef std::vector<tagSessionIdentifierData>                      CSessionStore;
typedef std::vector<tagSessionIdentifierData>::iterator            CSessionStoreIterator;

#pragma pack(1)









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






void* EventHandling(void* pEventHandlingArg);


int RejectDummyMsgCode(long long nMessageCode);



void* ProcessThread(void* pArg);



void* SenderThread(void* pArg);




int AddDataToStore(std::list<tagData> cList, pthread_mutex_t cMutex);

int InitializeAllMutex();
   #endif

   #ifndef WIN32

   #endif


#endif
