#pragma once


#ifndef NETWORK_SOURCES_H
#define NETWORK_SOURCES_H

//#include "includes.h"
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>




///CreateUDPSocketIP
///Creates a Simple UDP Socket
int CreateUDPSocketIP();
   

void FillSockAddrin(long sin_family, unsigned short int sin_port, long long sin_addr, sockaddr_in* sockaddrin);


int SendUDPData(int nSockFD, const void* cData, size_t nSize, const struct sockaddr_in* pstSockAddr, long nSockAddrLen);
int RecvUDPData(int nSockFD, void* cData, size_t nSize, sockaddr_in* pstSockAddr, long pnSockAddrLen);


#endif
