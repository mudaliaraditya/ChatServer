#include<iostream>
#include "ConfigParser.h"
/*
  KEY_OF_CONFIG = VAL_OF_KEY

  PORT          = 100

  HOSTNAME      = localhost

*/


int main()
{
   int lnRetVal = 0;
   void* pData = CreateNewMap();
   lnRetVal = GetConfig("config.txt",pData);
   if(lnRetVal != 0)
   {
       return -1;
   }
   char* lcHi = NULL;
   lcHi = GetValueForKey("KEY_OF_CONFIG","config.txt" , pData);
   std::cout << lcHi << std::endl;
   lnRetVal = DeleteKeyVal(lcHi);
   if(lnRetVal != 0)
   {
       return -1;
   }
    lcHi = GetValueForKey("PORT","config.txt" , pData);
   std::cout << lcHi << std::endl;
   lnRetVal = DeleteKeyVal(lcHi);
   if(lnRetVal != 0)
   {
       return -1;
   }
   lcHi = GetValueForKey("HOSTNAME","config.txt" , pData);
   std::cout << lcHi << std::endl;
   lnRetVal = DeleteKeyVal(lcHi);
   if(lnRetVal != 0)
   {
       return -1;
   }




   DeleteNewMap(pData);

}
