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




#define LOG_LOGGER(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      \
       LOGGINGI(g_cfstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
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

#define TESTLOG(cToBeLogged, ...)             \
{                                                 \
   {                                                \
     LOGGINGI(g_cDatafstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}



#define TESTOUT(cToBeLogged, ...)             \
{                                                 \
   {                                                \
     LOGGINGI(std::cout,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}




long getMicrotime();


#endif
