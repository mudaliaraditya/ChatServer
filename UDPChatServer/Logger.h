#ifndef LOGGER_H
#define LOGGER_H
#include "includes.h"


#define LOGGINGI(cFileStream,cToBeLogged, ...)\
{\
    \
   {                                                \
      \
      char lcBufferMessage[700] = {0};                       \
      char lcTimeLogBuffer[700] = {0};\
      GETTIMEBUFFER(lcTimeLogBuffer);\
      snprintf(lcBufferMessage,500,cToBeLogged, ##__VA_ARGS__);         \
      (cFileStream <<  lcTimeLogBuffer << " | " << lcBufferMessage <<  " | "<< __func__ <<"() | " << "thread id :" << pthread_self() << " | "<<__FILE__<< ":"<<__LINE__ <<endl);           \
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


enum LOG_LEVEL{INFO = 1 ,DEBUG = 2,WARN = 4,ERROR = 8,FATAL = 16,OUT = 32};

#define ACTUALLOG(nEnum,cToBeLogged, ...)\
{\
{\
    \
    char lcVar[1500];               \
    switch(nEnum )                   \
    {                               \
        case INFO:                  \
        snprintf(lcVar,1500," | %s | %s","INFO",cToBeLogged);\
        TESTLOG( lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - INFO;               \
        break;                      \
        case DEBUG:                 \
        snprintf(lcVar,1500," | %s | %s","DEBUG",cToBeLogged);\
        TESTLOG( lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - DEBUG;              \
        break;                      \
        case WARN:                  \
        snprintf(lcVar,1500," | %s | %s","WARN",cToBeLogged);\
        TESTLOG( lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - WARN;               \
        break;                      \
        case OUT:                   \
        snprintf(lcVar,1500," | %s | %s","WARN",cToBeLogged);\
        TESTOUT( lcVar, ##__VA_ARGS__);  \
        lnEnum=lnEnum - OUT;                \
        break;                      \
        case ERROR:                 \
        snprintf(lcVar,1500," | %s | %s","ERROR",cToBeLogged);\
        LOG_LOGGER( lcVar, ##__VA_ARGS__)\
        lnEnum = lnEnum - ERROR;              \
        break;                      \
        case FATAL:                 \
        snprintf(lcVar,1500," | %s | %s","FATAL",cToBeLogged);\
        LOG_LOGGER( lcVar, ##__VA_ARGS__)\
        lnEnum=lnEnum - FATAL;              \
        break;                      \
    }                               \
    \
                                    \
    \
}\
\
}

#define LOG(nEnum,cToBeLogged, ...) \
{\
                   \
    int lnBitCheck = 1;\
    int lnEnum = nEnum;\
    ACTUALLOG(lnEnum,cToBeLogged, ##__VA_ARGS__);\
    switch(lnEnum)\
    {   case 0:\
        break;\
        default:\
        DO_THING_8(cToBeLogged, ##__VA_ARGS__);   \
        break;                           \
    }                                \
loggingout:    \
    int n;\
    \
}                            



long getMicrotime();
int InitiateLoggingFor(std::fstream& cFileStream,char* cPath,char* cName,char* cExt);



#endif
