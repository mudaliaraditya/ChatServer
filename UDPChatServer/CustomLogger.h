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

#define DEBUGLOG(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      LOG(DEBUG,g_cDatafstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}


#define TESTINGOUT(cToBeLogged, ...)             \
{                                                 \
   {                                                \
      LOG(TEST,std::cout,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}


#define TESTINGLOG(cToBeLogged, ...)             \
{                                                 \
   {                                                \
     LOG(TEST,g_cDatafstream,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}




#define TESTOUT(cToBeLogged, ...)             \
{                                                 \
   {                                                \
     LOG(OUT,std::cout,cToBeLogged,##__VA_ARGS__)\
   }                                             \
}



#endif
