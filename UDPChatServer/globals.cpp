
#include "includes.h"


long g_OS = 0;
bool g_bProgramShouldWork = true;
std::deque<std::string> g_cIdentifierStore;
struct sockaddr_in servaddr, cliaddr;

int g_nTesting = 1;

CIdentiferDataStore g_cPortIdentifier;
CDataStore          g_cResponseList;
CDataStore          g_cProcessList;


#ifndef WIN32

pthread_mutex_t g_cProcessMutex;

pthread_mutex_t g_cResponseMutex;

pthread_mutex_t g_cIdentifierMutex;

pthread_mutex_t g_cDataStoreMutex;

pthread_cond_t g_cCondVarForProcessThread;

#endif

