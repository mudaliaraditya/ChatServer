#include "includes.h"

using namespace std;

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

int CreateSocket(int nType)
{
   int lnSockFD;
   switch(nType)
   {
      case UDP ://UDP
      {
         lnSockFD = CreateUDPSocketIP();
      }
      break;

      case TCP ://TCP
      {
         lnSockFD = CreateTCPSocketIP();
      }
      break;
   }
   return lnSockFD;
}

void FillSockAddrin(long sin_family, unsigned short int sin_port, long long sin_addr, sockaddr_in* sockaddrin)
{

   sockaddrin->sin_family = sin_family;
   sockaddrin->sin_port = sin_port;
   sockaddrin->sin_addr.s_addr = sin_addr;
}


int NetWorkInitialize(int& nSockfd)
{
   if(pConfigObject == NULL)
   {
      return -1;
   }

   char* lcPorti  =  ((GetValueForKey(CNF_PORT_TAG, CNF_FILE_NAME , pConfigObject)));
   if(lcPorti == NULL)
   {
      return -1;
   }
   string lcPort = lcPorti;
   if(0 !=  DeleteKeyVal(lcPorti))
   {
      return -1; 
   }
   if ((nSockfd = CreateUDPSocketIP()) < 0)//socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
   {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
   }
   long lnPort = atol(lcPort.c_str());
   cout << "PortVal is "<< lcPort << endl;
   memset(&servaddr, 0, sizeof(servaddr));
   memset(&cliaddr, 0, sizeof(cliaddr));

   FillSockAddrin(AF_INET, htons(lnPort), INADDR_ANY, &servaddr);

   if (bind(nSockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
   {
      perror("bind failed");
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
int NetWorkInitialize(int& nSockfd,int nType)
{
   if(pConfigObject == NULL)
   {
      return -1;
   }

   char* lcPorti  =  ((GetValueForKey(CNF_PORT_TAG, CNF_FILE_NAME , pConfigObject)));
   if(lcPorti == NULL)
   {
      return -1;
   }
   string lcPort = lcPorti;
   if(0 !=  DeleteKeyVal(lcPorti))
   {
      return -1; 
   }
   if ((nSockfd = CreateSocket(nType)) < 0)//socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
   {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
   }
   long lnPort = atol(lcPort.c_str());
   cout << "PortVal is "<< lcPort << endl;
   memset(&servaddr, 0, sizeof(servaddr));
   memset(&cliaddr, 0, sizeof(cliaddr));

   FillSockAddrin(AF_INET, htons(lnPort), INADDR_ANY, &servaddr);

   if (bind(nSockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
   {
      perror("bind failed");
      exit(EXIT_FAILURE);
   }
   if(nType == TCP)
   {
           if ((listen( nSockfd, 5)) != 0) 
           { 
                   printf("Listen failed...\n"); 
                   exit(0); 
           } 
           else
           {
                   printf("Server listening..\n"); 
           }
   }

   return EXIT_SUCCESS;
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
