#ifndef CUSTOM_LOGGER_H
#define CUSTOM_LOGGER_H
#include "Logger.h"

#define LOG_LOGGER(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      \
       LOG(ERROR,g_cfstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}
#define TESTLOG(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      LOG(INFO,g_cDatafstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}



#define TESTOUT(cToBeLogged, ...)             \
{                                                 \
   {                                                \
     LOG(INFO,std::cout,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}



#endif
