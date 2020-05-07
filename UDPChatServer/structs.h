#pragma once

#ifndef WIN32


#include "defines.h"
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#pragma pack(1)

struct tagData;


struct tagNetworkThread;

struct tagNetworkThread
{
      int     nSockFD;
      //struct tagData buf;
      size_t  nSizeBufferSendRecv; 
      int     nFlags;
      struct sockaddr_in stAddr;
      socklen_t nSizeofStrtSockAddr;
};

struct tagCSequenceNo
{
   char cFinalSeqNo[30 + 1];
};
/*
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
    int nSeqNo;
    bool bFinalResponse;
    int nLatestClntSeqNo;
    int nSessionId;
    tagNetworkThread stNetWork;
};
*/

/*
   nGlobalIdentifier == unique Identifier so that each client can be uniquely identified
   cIdentifier       == client declared identifer,a name which the client uses 
   nMessageCode      == the action which the target server/client has to perform
   cBuffer           == the data container
   cTarget           == the target client Identifier,the target where the message has to be delivered to
   cUniqueMessageIdentifier == each message send by the client is unique so this is used for identifing uniqueness
   nSeqNo            == the seqno of the messages of the corresponding client
   bFinalResponse    == last packet of a transaction/action which was triggered by a initial transcode
   nLatestClntSeqNo  == the latest client seqno
   nSessionId        == the session Id for a particular message session
*/
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




struct tagData : public tagBufferData
{
    tagNetworkThread stNetWork;
};


struct tagError
{
   int32_t nErrorCode;
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


struct tagSessionIdentifierData
{
   int nGlobalIdentifier;
   std::string sName;
   tagSessionIdentifierData()
   {
      nGlobalIdentifier = 0;
      sName             = "";
   }
};


struct tagThreadData
{
    char cThreadId[20];
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
  
