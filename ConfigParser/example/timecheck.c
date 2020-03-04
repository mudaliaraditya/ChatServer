#include<stdio.h>
#include "ConfigParserFunctions.h"
#include <time.h> 
#include<sys/time.h>

long long timeInMilliseconds(void) 
{
    struct timeval tv;

    gettimeofday(&tv,NULL);
    //return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
    return  (tv.tv_sec*1000000) + (tv.tv_usec);
}

int main()
{
   int lnRetVal = 0;
   long long lnTime = 0;
   lnTime = timeInMilliseconds();
   ConfigParserHandle* pConfigParserHndl = CreateNewMap();
   lnRetVal = GetConfig("config.txt",pConfigParserHndl);
   if(lnRetVal != 0)
   {
       return -1;
   }
   printf("\n time taken : %ld (ref time : %ld current time : %ld)\n", timeInMilliseconds() - lnTime,lnTime,timeInMilliseconds());lnTime = timeInMilliseconds();
   char* lcHi = NULL;
   lcHi = GetValueForKey("KEY_OF_CONFIG","config.txt" , pConfigParserHndl);
   printf("%s\n",lcHi);
   lnRetVal = DeleteKeyVal(lcHi);
   if(lnRetVal != 0)
   {
       return -1;
   }
   printf("\n time taken : %ld (ref time : %ld current time : %ld)\n", timeInMilliseconds() - lnTime,lnTime,timeInMilliseconds());lnTime = timeInMilliseconds();
   lcHi = GetValueForKey("PORT","config.txt" , pConfigParserHndl);
   printf("%s\n",lcHi);
   lnRetVal = DeleteKeyVal(lcHi);
   if(lnRetVal != 0)
   {
       return -1;
   }
   printf("\n time taken : %ld (ref time : %ld current time : %ld)\n", timeInMilliseconds() - lnTime,lnTime,timeInMilliseconds());lnTime = timeInMilliseconds();
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
