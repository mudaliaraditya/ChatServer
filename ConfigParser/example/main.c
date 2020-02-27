#include<stdio.h>
#include "ConfigParserFunctions.h"

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
   printf("%s\n",lcHi);
   lnRetVal = DeleteKeyVal(lcHi);
   if(lnRetVal != 0)
   {
       return -1;
   }
   lcHi = GetValueForKey("PORT","config.txt" , pData);
   printf("%s\n",lcHi);
   lnRetVal = DeleteKeyVal(lcHi);
   if(lnRetVal != 0)
   {
       return -1;
   }
   lcHi = GetValueForKey("HOSTNAME","config.txt" , pData);
   printf("%s\n",lcHi);
   //std::cout << lcHi << std::endl;
   lnRetVal = DeleteKeyVal(lcHi);
   if(lnRetVal != 0)
   {
       return -1;
   }
   DeleteNewMap(pData);
   if(lnRetVal != 0)
   {
       return -1;
   }

}
