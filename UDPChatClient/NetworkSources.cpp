#include "NetworkSources.h"

using namespace std;







///CreateUDPSocketIP
///Creates a Simple UDP Socket
int CreateUDPSocketIP()
{
   int lnSockFD = socket(AF_INET, SOCK_DGRAM, 0);
   return lnSockFD;
}

int CreateTCPSocketIP()
{
   int lnSockFD = socket(AF_INET, SOCK_STREAM, 0);
   return lnSockFD;
}

int ConnnectToTCPSockFD(int nSockFD, const struct sockaddr *pstAddr, socklen_t nAddrLen)
{
   return connect(nSockFD,pstAddr,nAddrLen);
}


void FillSockAddrin(long sin_family, unsigned short int sin_port, long long sin_addr, sockaddr_in* sockaddrin)
{
   sockaddrin->sin_family = sin_family;
   sockaddrin->sin_port = sin_port;
   sockaddrin->sin_addr.s_addr = sin_addr;
}



int SendUDPData(int nSockFD, const void* cData, size_t nSize, const struct sockaddr_in* pstSockAddr, long nSockAddrLen)
{
#ifndef WIN32
   return sendto(nSockFD, (const char *)&cData, nSize, MSG_CONFIRM, (const struct sockaddr *) pstSockAddr, nSockAddrLen);
#endif

#ifdef WIN32
   return sendto(nSockFD, (const char *)&cData, nSize, 0, (const struct sockaddr *) pstSockAddr, nSockAddrLen);
#endif
}

int RecvUDPData(int nSockFD, void* cData, size_t nSize, sockaddr_in* pstSockAddr, long pnSockAddrLen)
{
#ifndef WIN32
   return recvfrom(nSockFD, (void*)cData, nSize, MSG_WAITALL, (struct sockaddr *) pstSockAddr, (socklen_t*)&pnSockAddrLen);
#endif
#ifdef WIN32
   return recvfrom(nSockFD, (char*)cData, nSize, 0, (struct sockaddr *) pstSockAddr, (int*)&pnSockAddrLen);
#endif
}


