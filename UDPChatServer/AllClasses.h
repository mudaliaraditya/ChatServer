#pragma once
#include "includes.h"



class   CNetwork
{
   private:
      int m_nSocket;
      int m_nOtherSocket;
      
   public:
      CNetwork();
      int CreateSocket();
};


class CTCPConnection : public CNetwork
{
   private:
      
   public:
      
      
};


class CUDPConnection : public CNetwork
{
   private:
      
   public:
};