#pragma once

void* CheckResponse(void*);

tagCSequenceNo GenerateSequenceNo(char* cSequenceBuf,char* cIdentiferBuf,int nSizeOfFirstBuff,int nSizeOfSecondBuff);

int ExecuteResponse(tagData& stData);

int CreateUDPSocketIP();

void FillSockAddrin(long sin_family, unsigned short int sin_port, long long sin_addr, sockaddr_in* sockaddrin);

int SendUDPData(int nSockFD, const void* cData, size_t nSize, const struct sockaddr_in* pstSockAddr, long nSockAddrLen);

int RecvUDPData(int nSockFD, void* cData, size_t nSize, sockaddr_in* pstSockAddr, long pnSockAddrLen);


void* SenderThread(void* pVData);


void* RecieverThread(void* pVData);

int PreSender(tagData& stData);

int PostSender(tagData& stData);
