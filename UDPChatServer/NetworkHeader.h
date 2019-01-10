/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NetworkHeader.h
 * Author: root
 *
 * Created on 06 January 2019, 03:23
 */

#pragma once
#include "includes.h"

int CreateUDPSocketIP();
void FillSockAddrin(long sin_family, unsigned short int sin_port, long long sin_addr, sockaddr_in* sockaddrin);
int NetWorkInitialize(int& nSockfd);
int SendUDPData(int nSockFD, const void* cData, size_t nSize, const struct sockaddr_in* pstSockAddr, long nSockAddrLen);
int RecvUDPData(int nSockFD, void* cData, size_t nSize, sockaddr_in* pstSockAddr, long pnSockAddrLen);

/* NETWORKHEADER_H */

