#include<stdio.h>
#include "ConfigParserFunctions.h"

int main()
{
   int lnRetVal = 0;
   ConfigParserHandle* pConfigParserHndl = CreateNewMap();
   lnRetVal = GetConfig("config.txt",pConfigParserHndl);
   if(lnRetVal != 0)
   {
       return -1;
   }
   char* lcHi = NULL;
   lcHi = GetValueForKey("KEY_OF_CONFIG","config.txt" , pConfigParserHndl);
   printf("%s\n",lcHi);
   lnRetVal = DeleteKeyVal(lcHi);
   if(lnRetVal != 0)
   {
       return -1;
   }
   lcHi = GetValueForKey("PORT","config.txt" , pConfigParserHndl);
   printf("%s\n",lcHi);
   lnRetVal = DeleteKeyVal(lcHi);
   if(lnRetVal != 0)
   {
       return -1;
   }
   lcHi = GetValueForKey("HOSTNAME","config.txt" , pConfigParserHndl);
   printf("%s\n",lcHi);
   //std::cout << lcHi << std::endl;
   lnRetVal = DeleteKeyVal(lcHi);
   if(lnRetVal != 0)
   {
       return -1;
   }
   DeleteNewMap(pConfigParserHndl);
   if(lnRetVal != 0)
   {
       return -1;
   }

}
