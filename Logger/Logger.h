#ifndef LOGGER_H
#define LOGGER_H
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
//#include <sys/time.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
//#include "globals.h"
//#include "Logger.h"
//#include "CustomLogger.h"

//extern unsigned short g_nLogLevel;

#ifdef WIN32
#include<chrono>
#include <windows.h>
#include<processthreadsapi.h>
#endif

//unsigned short g_nLogLevel = 8;
int GetLoggingLevel(char* pcLoggingLvl);
int InitiateLoggingFor(std::fstream& cFileStream,const char* cPath,const char* cName,const char* cExt);
long getMicrotime();
#ifndef WIN32
#define THREAD_ID pthread_self()
#endif
#ifdef WIN32
#define THREAD_ID GetCurrentThreadId()
#endif

inline unsigned short*  GetLogLevel()
{
    static unsigned short g_nLogLevel = 0;
    return &g_nLogLevel;
}

#define LOGGINGI(cFileStream,cToBeLogged, ...)\
{\
    \
   {                                                \
      \
      char lcBufferMessage[700];                       \
		memset(lcBufferMessage,0,700);\
      std::string lcTimeLogBuffer = "";\
      /*memset(lcTimeLogBuffer,0,700);*/\
      GETTIMEBUFFER(lcTimeLogBuffer);\
      snprintf(lcBufferMessage,500,cToBeLogged, ##__VA_ARGS__);         \
      (cFileStream <<  lcTimeLogBuffer << " | " << lcBufferMessage <<  " | "<< __func__ <<"() | " << "thread id :" <<  THREAD_ID << " | "<<__FILE__<< ":"<<__LINE__ << std::endl);           \
   }               \
}




#define GETTIMEBUFFER(cTimeBuffer)\
{\
    {\
      time_t g_nTime = 0;\
      g_nTime = time(NULL);\
      long long lnMicro = getMicrotime();\
      struct tm*  lpsttm =  localtime(&g_nTime);\
      char lcBufferTime[200] = {0};                       \
      snprintf(lcBufferTime,200,"%02d:%02d:%02d.%06lld %02d-%02d-%04d",lpsttm->tm_hour, lpsttm->tm_min, lpsttm->tm_sec, lnMicro,lpsttm->tm_mday,lpsttm->tm_mon,(int)(lpsttm->tm_year + 1900) );\
      cTimeBuffer = lcBufferTime;\
   }\
}


#define DO_THING(cToBeLogged, ...)    switch(lnEnum){ case 0: ;goto loggingout;break; default:  ACTUALLOG(lnEnum & lnBitCheck,cToBeLogged, ##__VA_ARGS__) ;lnBitCheck=lnBitCheck<<1;break; }
#define DO_THING_2(cToBeLogged, ...)  DO_THING(cToBeLogged, ##__VA_ARGS__); ;DO_THING(cToBeLogged, ##__VA_ARGS__)
#define DO_THING_4(cToBeLogged, ...)  DO_THING_2(cToBeLogged, ##__VA_ARGS__); ;DO_THING_2(cToBeLogged, ##__VA_ARGS__)
#define DO_THING_8(cToBeLogged, ...) DO_THING_4(cToBeLogged, ##__VA_ARGS__); ;DO_THING_4(cToBeLogged, ##__VA_ARGS__)
#define DO_THING_16(cToBeLogged, ...) DO_THING_8(cToBeLogged, ##__VA_ARGS__); DO_THING_8(cToBeLogged, ##__VA_ARGS__)


enum LOG_LEVEL { LOGGER_INFO = 32, LOGGER_DEBUG = 16, LOGGER_WARN = 64, LOGGER_ERROR = 128, LOGGER_FATAL = 256, LOGGER_OUT = 8, LOGGER_TEST = 4 };

#define ACTUALLOG(nEnum,cToBeLogged,cFileStream, ...)\
{\
{\
    \
    char lcVar[1500];               \
    switch(nEnum )                   \
    {                               \
        case LOGGER_INFO:                  \
        snprintf(lcVar,1500,"%s | %s  ","INFO",cToBeLogged);\
        LOGGINGI(cFileStream ,lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - LOGGER_INFO;               \
        break;                      \
        case LOGGER_DEBUG:                 \
        snprintf(lcVar,1500,"%s | %s  ","DEBUG",cToBeLogged);\
        LOGGINGI(cFileStream ,lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - LOGGER_DEBUG;              \
        break;                      \
        case LOGGER_WARN:                  \
        snprintf(lcVar,1500,"%s | %s  ","WARN",cToBeLogged);\
        LOGGINGI(cFileStream ,lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - LOGGER_WARN;               \
        break;                      \
        case LOGGER_OUT:                   \
        snprintf(lcVar,1500,"%s | %s  ","OUT",cToBeLogged);\
        LOGGINGI(cFileStream ,lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - LOGGER_OUT;                \
        break;                      \
        case LOGGER_ERROR:                 \
        snprintf(lcVar,1500,"%s | %s  ","ERROR",cToBeLogged);\
        LOGGINGI(cFileStream ,lcVar, ##__VA_ARGS__)\
        lnEnum = lnEnum - LOGGER_ERROR;              \
        break;                      \
        case LOGGER_FATAL:                 \
        snprintf(lcVar,1500,"%s | %s ","FATAL",cToBeLogged);\
        LOGGINGI(cFileStream , lcVar, ##__VA_ARGS__)\
        lnEnum=lnEnum - LOGGER_FATAL;              \
        break;                      \
    }                               \
    \
                                    \
    \
}\
\
}

#define LOG(nEnum,cFileStream,cToBeLogged, ...) \
{\
   {\
    __label__ loggingout; \
    int lnBitCheck = 1;\
    int lnEnum = nEnum;\
    if(*GetLogLevel() > lnEnum){ goto loggingout;}\
    ACTUALLOG(lnEnum,cToBeLogged,cFileStream, ##__VA_ARGS__);\
 loggingout:int x;  \
\
    \
 }\
}                            



//long getMicrotime();


// cFileStream -- On which file to write
// cPath       -- Directory Of the file
// cName       -- Name Of the file
// cExt        -- extension of the file
//int InitiateLoggingFor(std::fstream& cFileStream,char* cPath,char* cName,char* cExt);



int GetLoggingLevel(char* pcLoggingLvl);


inline long  getMicrotime()
{
    #ifndef WIN32
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_usec;
    #endif
    #ifdef WIN32
    //std::chrono::seconds sec(1);
    //long long lnMicroSecond = std::chrono::microseconds(sec).count();

    long long lnMicroSecond = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();//std::chrono::microseconds(sec).count();
    std::string lcMicroSecond = std::to_string(lnMicroSecond);
    //std::cout << lcMicroSecond << std:l
    for (int i = 0; i < lcMicroSecond.length(); i++)
    {
        lcMicroSecond[i] = '0';
        if (i == 3)
            break;
    }
    lnMicroSecond = std::atol(lcMicroSecond.c_str());
    return lnMicroSecond;
   #endif
}


int inline InitiateLoggingFor(std::fstream& cFileStream,const char* cPath,const char* cName,const char* cExt)
{
    //LOG File Handling START
    time_t lnTime;
    struct tm* lpsttm = NULL;
    char lcErrorLogFileName[200];
    memset(lcErrorLogFileName, 0, 200);
    lnTime = time(NULL);
    lpsttm = gmtime(&lnTime);
    snprintf(lcErrorLogFileName, 200, "%s/%s-%02d-%02d-%04d_%02d%02d%02d.%s", cPath, cName, lpsttm->tm_mday, lpsttm->tm_mon + 1, lpsttm->tm_year + 1900, lpsttm->tm_hour, lpsttm->tm_min, lpsttm->tm_sec, cExt);
    cFileStream.open(lcErrorLogFileName, std::ios::in | std::ios::out | std::ios::app);
    if (cFileStream.fail())
    {
        cFileStream.open(lcErrorLogFileName, std::ios::in | std::ios::out | std::ios::trunc);
        if (cFileStream.fail())
        {
            return -1;
        }
        cFileStream.clear();
    }
    cFileStream.seekp(std::ios::end); //output
    cFileStream.seekg(std::ios::end);
    return 0;
}

int inline GetLoggingLevel(char* pcLoggingLvl)
{
   std::map<std::string,int> m_cLogLvl;
   m_cLogLvl["INFO"]  = LOGGER_INFO;
   m_cLogLvl["DEBUG"] = LOGGER_DEBUG;
   m_cLogLvl["WARN"]  = LOGGER_WARN;
   m_cLogLvl["ERROR"] = LOGGER_ERROR;
   m_cLogLvl["FATAL"] = LOGGER_FATAL;
   m_cLogLvl["OUT"]   = LOGGER_OUT;
   m_cLogLvl["TEST"]  = LOGGER_TEST;
   auto lcIter = m_cLogLvl.find(pcLoggingLvl);
   if(lcIter ==  m_cLogLvl.end())
   {
      return -1;
   }
   return lcIter->second;
}

#define LOG_OUT(cToBeLogged, ...)             \
{                                                 \
   {                                                \
     LOG(LOGGER_OUT,std::cout,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}




#endif
