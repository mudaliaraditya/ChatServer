#pragma once

#include "defines.h"
#include "includes.h"
#ifndef WIN32
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
    //long lnSockFD;
    char cUniqueMessageIdentifier[30 + 1];
    tagNetworkThread stNetWork;
};
#endif
#ifdef WIN32
struct tagData;

struct tagNetworkThread;


struct tagNetworkThread
{
   int     fd;
   //tagData buf;
   size_t  n;
   int     flags;
   sockaddr_in addr;
   size_t restrict;
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
#endif
