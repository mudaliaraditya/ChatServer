#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void* CheckResponse(void*);

tagCSequenceNo GenerateSequenceNo(char* cSequenceBuf,char* cIdentiferBuf,int nSizeOfFirstBuff,int nSizeOfSecondBuff);

int ExecuteResponse(tagData& stData);


void* SenderThread(void* pVData);


void* RecieverThread(void* pVData);

int PreSender(tagData& stData);

int PostSender(tagData& stData);

#endif
