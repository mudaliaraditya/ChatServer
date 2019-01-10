#pragma once

#ifndef GLOBAL
#define GLOBAL
#include "includes.h"
extern long g_OS;
extern bool g_bProgramShouldWork;
extern std::deque<std::string> g_cIdentifierStore;
extern struct sockaddr_in servaddr, cliaddr;

extern int g_nTesting;

extern CIdentiferDataStore g_cPortIdentifier;
extern CDataStore          g_cResponseList;
extern CDataStore          g_cProcessList;


#ifndef WIN32

extern pthread_mutex_t g_cProcessMutex;

extern pthread_mutex_t g_cResponseMutex;

extern pthread_mutex_t g_cIdentifierMutex;

extern pthread_mutex_t g_cDataStoreMutex;

extern pthread_cond_t g_cCondVarForProcessThread;

#endif




#endif

/*
 * #pragma once

#ifndef GLOBAL
#define GLOBAL
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

pthread_cond_t g_cConditionalVar;

#endif




#endif


 * 
 */