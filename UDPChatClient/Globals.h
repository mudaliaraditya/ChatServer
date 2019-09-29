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
#include"includes.h"
#ifndef GLOBALS_H
#define GLOBALS_H





std::deque<std::string> g_cIdentifierStore;
CEventResenderStore      g_cEventResender;
std::deque<tagData>          g_cSenderDataStore;

pthread_mutex_t              g_cIdentifierMutex;
pthread_mutex_t              g_SenderMutex;
pthread_mutex_t              g_ReSenderMutex;

pthread_t lnPThreadReciever;
pthread_t lnPThreadSender;
pthread_t lnPThreadCheckResponse;
pthread_t lnPThreadMain;
//std::list<tagTimeData>     g_cEventResender;
char                         g_cIdentifier[20 + 1] = {0};
//map <string,tagData>       g_cMessageHolder;//this will contain the messages for duplicate check
struct sockaddr_in           servaddr = {0};
int  g_nSockFd = 0;
int  g_nLatestRecivedSequenceNo = 0;
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
tagData*  lpstThrdDataRcvr = NULL;
tagData*  lpstThrdSndr = NULL;
void*     pConfigObject;
int       g_nSeqNo = 0;
#endif /* GLOBALS_H */

