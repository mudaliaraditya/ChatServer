#include "encrypt.h"


void* dofunc(void* pvData)
{
   long long lnSize = ((Data*)(pvData))->nSize;
   long long lnVal;
   char* lpcBuffer = ((Data*)(pvData))->cKey;
   long long lnKey = atol(lpcBuffer);
   for(lnVal = 0;lnVal < lnSize;lnVal++)
   {
         (((Data*)(pvData))->cBuffer)[lnVal] = (((Data*)(pvData))->cBuffer)[lnVal] ^ lnKey;
   }
   return 0;
}
