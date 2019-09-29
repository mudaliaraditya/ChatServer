
#include "includes.h"


long g_OS = 0;
bool g_bProgramShouldWork = true;
std::set<std::string> g_cIdentifierStore;
struct sockaddr_in servaddr, cliaddr;

int g_nTesting = 1;

CIdentiferDataStore g_cPortIdentifier;
CDataStore          g_cResponseList;
CDataStore          g_cProcessList;


#ifndef WIN32

pthread_mutex_t g_cProcessMutex;

pthread_mutex_t g_cGlobalIdentifierMutex;

pthread_mutex_t g_cResponseMutex;

pthread_mutex_t g_cIdentifierMutex;

pthread_mutex_t g_cDataStoreMutex;

pthread_mutex_t g_cDataGlobalPortStoreMutex;

pthread_cond_t g_cCondVarForProcessThread;

pthread_t lnSenderPThread;

pthread_t lnProcessPThread[NO_OF_PROC_THREADS] = {0};

pthread_t lnPThreadEventTime;

pthread_t lnRecieverThread;

pthread_mutex_t         g_ReSenderMutex;

CEventResenderStore       g_cEventResender;

std::fstream g_cfstream;

std::fstream g_cDatafstream;
//int g_nFlagDupliResend = 0;
int g_nFlagDupliResend = 0;

std::map<std::string, std::string> g_cMap;

void* pConfigObject = NULL;
#endif

