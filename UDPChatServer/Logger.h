#ifndef LOGGER_H
#define LOGGER_H
#include <errno.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <string>
#include <sys/time.h>  
#include <fstream>
#include <sstream> 
#include "globals.h"
#include "Logger.h"
#include "CustomLogger.h"

extern unsigned short g_nLogLevel;
#define LOGGINGI(cFileStream,cToBeLogged, ...)\
{\
    \
   {                                                \
      \
      char lcBufferMessage[700] = {0};                       \
      char lcTimeLogBuffer[700] = {0};\
      GETTIMEBUFFER(lcTimeLogBuffer);\
      snprintf(lcBufferMessage,500,cToBeLogged, ##__VA_ARGS__);         \
      (cFileStream <<  lcTimeLogBuffer << " | " << lcBufferMessage <<  " | "<< __func__ <<"() | " << "thread id :" << pthread_self() << " | "<<__FILE__<< ":"<<__LINE__ << std::endl);           \
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
      snprintf(lcBufferTime,200,"%d:%d:%d.%d %d-%d-%d",lpsttm->tm_hour, lpsttm->tm_min, lpsttm->tm_sec, lnMicro,lpsttm->tm_mday,lpsttm->tm_mon,lpsttm->tm_year );\
      snprintf(cTimeBuffer,500,"%s",lcBufferTime);\
   }\
}


#define DO_THING(cToBeLogged, ...)    switch(lnEnum){ case 0: goto loggingout;break; default:  ACTUALLOG(lnEnum & lnBitCheck,cToBeLogged, ##__VA_ARGS__) ;lnBitCheck=lnBitCheck<<1;break; }
#define DO_THING_2(cToBeLogged, ...)  DO_THING(cToBeLogged, ##__VA_ARGS__); ;DO_THING(cToBeLogged, ##__VA_ARGS__)
#define DO_THING_4(cToBeLogged, ...)  DO_THING_2(cToBeLogged, ##__VA_ARGS__); ;DO_THING_2(cToBeLogged, ##__VA_ARGS__)
#define DO_THING_8(cToBeLogged, ...) DO_THING_4(cToBeLogged, ##__VA_ARGS__); ;DO_THING_4(cToBeLogged, ##__VA_ARGS__)
#define DO_THING_16(cToBeLogged, ...) DO_THING_8(cToBeLogged, ##__VA_ARGS__); DO_THING_8(cToBeLogged, ##__VA_ARGS__)


enum LOG_LEVEL{INFO = 32 ,DEBUG = 16,WARN = 64,ERROR = 128,FATAL = 256,OUT = 8,TEST = 4};

#define ACTUALLOG(nEnum,cToBeLogged,cFileStream, ...)\
{\
{\
    \
    char lcVar[1500];               \
    switch(nEnum )                   \
    {                               \
        case INFO:                  \
        snprintf(lcVar,1500,"%s | %s  ","INFO",cToBeLogged);\
        LOGGINGI(cFileStream ,lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - INFO;               \
        break;                      \
        case DEBUG:                 \
        snprintf(lcVar,1500,"%s | %s  ","DEBUG",cToBeLogged);\
        LOGGINGI(cFileStream ,lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - DEBUG;              \
        break;                      \
        case WARN:                  \
        snprintf(lcVar,1500,"%s | %s  ","WARN",cToBeLogged);\
        LOGGINGI(cFileStream ,lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - WARN;               \
        break;                      \
        case OUT:                   \
        snprintf(lcVar,1500,"%s | %s  ","WARN",cToBeLogged);\
        LOGGINGI(cFileStream ,lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - OUT;                \
        break;                      \
        case ERROR:                 \
        snprintf(lcVar,1500,"%s | %s  ","ERROR",cToBeLogged);\
        LOGGINGI(cFileStream ,lcVar, ##__VA_ARGS__)\
        lnEnum = lnEnum - ERROR;              \
        break;                      \
        case FATAL:                 \
        snprintf(lcVar,1500,"%s | %s ","FATAL",cToBeLogged);\
        LOGGINGI(cFileStream , lcVar, ##__VA_ARGS__)\
        lnEnum=lnEnum - FATAL;              \
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
    if(g_nLogLevel > lnEnum){ goto loggingout;}\
    ACTUALLOG(lnEnum,cToBeLogged,cFileStream, ##__VA_ARGS__);\
   goto loggingout; \
/*    switch(lnEnum)\
    {   case 0:\
        break;\
        default:\
        DO_THING_8(cToBeLogged, ##__VA_ARGS__);   \
        break;                           \
    }       \
*/                         \
;loggingout:    \
    int n;\
    \
 }\
}                            



long getMicrotime();


// cFileStream -- On which file to write
// cPath       -- Directory Of the file
// cName       -- Name Of the file
// cExt        -- extension of the file
int InitiateLoggingFor(std::fstream& cFileStream,char* cPath,char* cName,char* cExt);



int GetLoggingLevel(char* pcLoggingLvl);



#endif
