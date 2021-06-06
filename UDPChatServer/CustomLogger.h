#ifndef CUSTOM_LOGGER_H
#define CUSTOM_LOGGER_H
#include "Logger.h"
#include "globals.h"

#define LOG_LOGGER(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      \
       LOG(LOGGER_ERROR,g_cfstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}
#define TESTLOG(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      LOG(LOGGER_INFO,g_cDatafstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}

#define DEBUGLOG(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      LOG(LOGGER_DEBUG,g_cDatafstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}


#define TESTINGOUT(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      LOG(LOGGER_TEST,std::cout,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}


#define TESTINGLOG(cToBeLogged, ...)             \
{                                                 \
   {                                                \
     LOG(LOGGER_TEST,g_cDatafstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}




#define TESTOUT(cToBeLogged, ...)             \
{                                                 \
   {                                                \
     LOG(LOGGER_OUT,std::cout,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}



#endif
