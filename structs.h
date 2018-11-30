#pragma once

#include "defines.h"
struct tagData;


struct tagNetworkThread;
struct tagNetworkThread
{
      int     fd;
      //struct tagData buf;
      size_t  n; 
      int     flags;
      struct sockaddr_in addr;
      socklen_t restrict;
};

struct tagData
{
    char cIdentifier[20 + 1];
    long nMessageCode;
    char cBuffer[MAXLINE + 1];
    char cTarget[20 + 1];
    long lnSockFD;
    //long lnSockFD1;
    tagNetworkThread stNetWork;
};
