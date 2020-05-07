#pragma once
#ifndef WIN32
#include "defines.h"
//#include "includes.h"

#pragma pack(1)

struct tagData;

struct tagCSequenceNo;

struct tagNetworkThread;
struct tagBufferData;

struct tagNetworkThread
{
   int                nFD;
   size_t             nMessageLen; 
   int                nFlags;
   struct sockaddr_in stSockAddr;
   socklen_t          nSockLen;
};

struct tagCSequenceNo
{
   char cFinalSeqNo[30 + 1];
};

struct tagData
{

	short   nCommand;
	int64_t nGlobalIdentifier;
	char    cIdentifier[20 + 1];
	int32_t nFrOrToServerFlg;
	int32_t nMessageCode;
	char    cBuffer[MAXLINE + 1];
	char    cTarget[20 + 1];
	char    cUniqueMessageIdentifier[30 + 1];
	int32_t nSeqNo;
	bool    bFinalResponse;
	int32_t nLatestClntSeqNo;
	int32_t nSessionId;
    tagNetworkThread stNetWork;
};

class CtagDataSeqNoComp
{
   public:
   bool operator()(const tagData& lstData1,const tagData& lstData2) const
   {
         return lstData1.nSeqNo < lstData2.nSeqNo;
   }

};


struct tagBufferData
{
	short   nCommand;
	int64_t nGlobalIdentifier;
	char    cIdentifier[20 + 1];
	int32_t nFrOrToServerFlg;
	int32_t nMessageCode;
	char    cBuffer[MAXLINE + 1];
	char    cTarget[20 + 1];
	char    cUniqueMessageIdentifier[30 + 1];
	int32_t nSeqNo;
	bool    bFinalResponse;
	int32_t nLatestClntSeqNo;
	int32_t nSessionId;
};


struct tagTimeData
{
   public:
   tagData  stData;
   time_t   m_nTime;
   long     m_nCounter;
   
   tagTimeData(const time_t& nTime,const tagData& stDatis)
   {
      m_nCounter = 2;
      m_nTime    = nTime;
      stData =     stDatis;
   }
   
   tagTimeData(const time_t& nTime,const tagData& stDatis,long long nCounter)
   {
      m_nCounter = nCounter;
      m_nTime    = nTime;
      stData =     stDatis;
   } 
   bool  operator<(const tagTimeData& lstB) const
   {
      return m_nTime < lstB.m_nTime;
   }
};



struct tagEventData
{
  public:
  tagEventData(const time_t& nTime,const tagData& stDatis) : stTimeData(nTime,stDatis)
  {
  }
  tagTimeData stTimeData;

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
