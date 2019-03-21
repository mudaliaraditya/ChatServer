#pragma once

#ifndef WIN32


//#include "defines.h"
#include "includes.h"
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

struct tagCSequenceNo
{
   char cFinalSeqNo[30 + 1];
};

struct tagData
{
    short nCommand;
    long long nGlobalIdentifier;
    char cIdentifier[20 + 1];
    int nFrOrToServerFlg;    
    long nMessageCode;
    char cBuffer[MAXLINE + 1];
    char cTarget[20 + 1];
    char cUniqueMessageIdentifier[30 + 1];
    tagNetworkThread stNetWork;
    //UDPChatServer 20-01-2019
//    tagCSequenceNo stSenderSeqNo;
//    tagCSequenceNo stRecieverSeqNo;
    //UDPChatServer 20-01-2019
    int nSeqNo;
	 bool bFinalResponse;
    int nLatestClntSeqNo;
    int nSessionId;
};


struct tagTimeData
{
   public:
   tagData stData;
   time_t m_nTime;
   long m_nCounter;
   
   tagTimeData(const time_t& nTime,const tagData& stDatis)
   {
      m_nCounter = 2;
      m_nTime    = nTime;
      stData =     stDatis;
   }
   
   bool  operator<(const tagTimeData& lstB) const
   {
      return m_nTime < lstB.m_nTime;
   }
};

#endif
#ifdef WIN32


#include "defines.h"
#include "Includes.h"
//#pragma pack(pop)

struct tagData;

struct tagNetworkThread;


struct tagNetworkThread
{
   int     fd;
   size_t  n;
   int     flags;
   sockaddr_in addr;
   size_t restrict;
};

struct tagData
{
    short nCommand;
    char cIdentifier[20 + 1];
    long nMessageCode;
    char cBuffer[MAXLINE + 1];
    char cTarget[20 + 1];
    char cUniqueMessageIdentifier[30 + 1];
    tagNetworkThread stNetWork;
};
#endif
  
