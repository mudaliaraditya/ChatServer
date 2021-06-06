#ifndef CUSTOM_LOGGER_H
#define CUSTOM_LOGGER_H
#pragma once
#include "Logger.h"

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
      LOG(LOGGER_INFO,g_cfstreamDataLogger,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}

#define DEBUGLOG(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      LOG(LOGGER_DEBUG,g_cfstreamDataLogger,cToBeLogged,##__VA_ARGS__)\
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
     LOG(LOGGER_TEST,g_cfstreamDataLogger,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}




#define TESTOUT(cToBeLogged, ...)             \
{                                                 \
   {                                                \
     LOG(LOGGER_OUT,std::cout,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}



#endif
