/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Globals.h
 * Author: DELL-PC
 *
 * Created on March 24, 2019, 10:32 AM
 */

#include "Globals.h"
#include <fstream>
#include <deque>
#include <pthread.h>
#include "defines.h"
#include "typedefs.h"
#include "ConfigParserFunctions.h"
#include <set>


/*
 Store keeps track of all identifiers

*/
std::deque<std::string>           g_cIdentifierStore;
CEventResenderStore               g_cEventResender;
std::deque<tagData>               g_cSenderDataStore;
CMessageStoreBySeqNo              g_cAllMessageStore;
CSequenceStorebyUnOrderedSeqNo    g_cSeqStoreForUnOrderedSeqNo;
CSequenceUnOrderedPacketsStore    g_cSeqNoUnOrderedPacketStore;

pthread_mutex_t              g_cIdentifierMutex;
pthread_mutex_t              g_SenderMutex;
pthread_mutex_t              g_ReSenderMutex;
pthread_mutex_t              g_ConsoleIOMutex;
pthread_mutex_t              g_GlobalSeqnoMutex;
pthread_mutex_t              g_CinVarMutex;

pthread_t g_nPThreadReciever;
pthread_t g_nPThreadSender;
pthread_t g_nPThreadCheckResponse;
pthread_t g_nPThreadMain;
pthread_t g_nPThreadPrintMessagesSet;

pthread_cond_t g_CinVarCondVar = PTHREAD_COND_INITIALIZER;
//std::list<tagTimeData>     g_cEventResender;
char                         g_cIdentifier[20 + 1] = {0};
//map <string,tagData>       g_cMessageHolder;//this will contain the messages for duplicate check
struct sockaddr_in           g_ServAddr = {0};
int  g_nSockFd = 0;
int  g_nLatestRecivedSequenceNo = 0;
int  g_nExpectedSeqNo = 0;
int  g_nTesting = 0;
int  g_nFlagNoResendDupli = 0;
int  g_nSessionId = 0;
int  g_nGlobalIdentifier = 0;
bool g_bWaitForResponse = false;
std::fstream g_cfstream;
std::fstream g_cfstreamDataLogger;
char**    g_pcParam = nullptr;
short     g_nArgs = 0;
bool      g_bProgramShouldWork = true;
tagData*  g_pstThrdDataRcvr = NULL;
tagData*  g_pstThrdSndr = NULL;
ConfigParserHandle*     pConfigObject;
int       g_nSeqNo = 0;
bool      g_bOutOfOrder = false;
long      g_PID=0;
__thread char  g_cThreadName[20 + 1];

