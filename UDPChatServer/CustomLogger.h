#ifndef CUSTOM_LOGGER_H
#define CUSTOM_LOGGER_H
#include "Logger.h"

#define LOG_LOGGER(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      \
       LOGGINGI(g_cfstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
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



#endif
