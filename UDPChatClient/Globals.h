#pragma once
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
#ifndef GLOBALS_H
#define GLOBALS_H
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
extern std::deque<std::string>           g_cIdentifierStore;
extern CEventResenderStore               g_cEventResender;
extern std::deque<tagData>               g_cSenderDataStore;
extern CMessageStoreBySeqNo              g_cAllMessageStore;
extern CSequenceStorebyUnOrderedSeqNo    g_cSeqStoreForUnOrderedSeqNo;
extern CSequenceUnOrderedPacketsStore    g_cSeqNoUnOrderedPacketStore;

extern pthread_mutex_t              g_cIdentifierMutex;
extern pthread_mutex_t              g_SenderMutex;
extern pthread_mutex_t              g_ReSenderMutex;
extern pthread_mutex_t              g_ConsoleIOMutex;
extern pthread_mutex_t              g_GlobalSeqnoMutex;
extern pthread_mutex_t              g_CinVarMutex;

extern pthread_t g_nPThreadReciever;
extern pthread_t g_nPThreadSender;
extern pthread_t g_nPThreadCheckResponse;
extern pthread_t g_nPThreadMain;
extern pthread_t g_nPThreadPrintMessagesSet;

extern pthread_cond_t g_CinVarCondVar;
//std::list<tagTimeData>     g_cEventResender;
extern char                         g_cIdentifier[20 + 1];
//map <string,tagData>       g_cMessageHolder;//this will contain the messages for duplicate check
extern struct sockaddr_in           g_ServAddr;
extern int  g_nSockFd;
extern int  g_nLatestRecivedSequenceNo;
extern int  g_nExpectedSeqNo;
extern int  g_nTesting;
extern int  g_nFlagNoResendDupli;
extern int  g_nSessionId;
extern int  g_nGlobalIdentifier;
extern bool g_bWaitForResponse;
extern std::fstream g_cfstream;
extern std::fstream g_cfstreamDataLogger;
extern char**    g_pcParam;
extern short     g_nArgs;
extern bool      g_bProgramShouldWork;
extern tagData*  g_pstThrdDataRcvr;
extern tagData*  g_pstThrdSndr;
extern ConfigParserHandle*     pConfigObject;
extern int       g_nSeqNo;
extern bool      g_bOutOfOrder;
extern long      g_PID;
extern __thread char  g_cThreadName[20 + 1];
#endif /* GLOBALS_H */

