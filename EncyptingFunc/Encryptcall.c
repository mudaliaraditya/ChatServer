#include "Encryptcall.h"


int Encrypt(char* cBuffer,long long nLen)
{
   void* pConfigObject = NULL;
   pConfigObject = CreateNewMap();
   if(pConfigObject == NULL)
   {
      return -1;
   }
   int lnRetVal =  GetConfig(ENCRYPT_CONFIG,pConfigObject);
   if(0 != lnRetVal)
   {
      return -1;
   }
   char* lcEncryptKey1  =  ((GetValueForKey(ENCRYPT_KEY1, ENCRYPT_CONFIG , pConfigObject)));
   if(lcEncryptKey1 == NULL)
   {  
      return -1;
   }
    char* lcEncryptKey2  =  ((GetValueForKey(ENCRYPT_KEY2, ENCRYPT_CONFIG , pConfigObject)));
   if(lcEncryptKey1 == NULL)
   {  
      return -1;
   }

    char* lcEncryptingFunc = ((GetValueForKey(ENCRYPT_METHOD, ENCRYPT_CONFIG , pConfigObject)));
   if(lcEncryptingFunc == NULL)
   {
      return -1;
   }

    char* lcDecryptingFunc = ((GetValueForKey(DECRYPT_METHOD, ENCRYPT_CONFIG , pConfigObject)));
   if(lcDecryptingFunc == NULL)
   {
      return -1;
   }

    void *handle;
    void* (*lpfEncryptDecrypt)(void*);
    char *error;

    handle = dlopen("libencrypt.so", RTLD_LAZY);
    if (!handle) 
    {
      fprintf(stderr, "%s\n", dlerror());
      exit(EXIT_FAILURE);
    }
    Data stData = {0};
    stData.nSize=nLen;    
    stData.cBuffer= cBuffer;
    strncpy(stData.cKey, lcEncryptKey1, ONE_KB);
    dlerror();    /* Clear any existing error */

    *(void **) (&lpfEncryptDecrypt) = dlsym(handle, lcEncryptingFunc);

    if ((error = dlerror()) != NULL)  
    {
      fprintf(stderr, "%s\n", error);
      exit(EXIT_FAILURE);
    }

    (*lpfEncryptDecrypt)((void*)&stData);
    dlclose(handle);
    if(0 !=  DeleteKeyVal(lcEncryptKey1))
    {  
       return -1;
    }

    if(0 !=  DeleteKeyVal(lcEncryptKey2))
    {  
       return -1;
    }
    if(0 !=  DeleteKeyVal(lcEncryptingFunc))
    {  
       return -1;
    }
    if(0 !=  DeleteKeyVal(lcDecryptingFunc))
    {  
       return -1;
    }
    DeleteNewMap(pConfigObject); 

    //exit(EXIT_SUCCESS);
    return 0;
}


int Decrypt(char* cBuffer,long long nLen)
{
   void* pConfigObject = NULL;
   pConfigObject = CreateNewMap();
   if(pConfigObject == NULL)
   {
      return -1;
   }
   int lnRetVal =  GetConfig(ENCRYPT_CONFIG,pConfigObject);
   if(0 != lnRetVal)
   {
      return -1;
   }
   char* lcEncryptKey1  =  ((GetValueForKey(ENCRYPT_KEY1, ENCRYPT_CONFIG , pConfigObject)));
   if(lcEncryptKey1 == NULL)
   {  
      return -1;
   }
    char* lcEncryptKey2  =  ((GetValueForKey(ENCRYPT_KEY2, ENCRYPT_CONFIG , pConfigObject)));
   if(lcEncryptKey1 == NULL)
   {  
      return -1;
   }

    char* lcEncryptingFunc = ((GetValueForKey(ENCRYPT_METHOD, ENCRYPT_CONFIG , pConfigObject)));
   if(lcEncryptingFunc == NULL)
   {
      return -1;
   }

    char* lcDecryptingFunc = ((GetValueForKey(DECRYPT_METHOD, ENCRYPT_CONFIG , pConfigObject)));
   if(lcDecryptingFunc == NULL)
   {
      return -1;
   }

    void *handle;
    void* (*lpfEncryptDecrypt)(void*);
    char *error;

    handle = dlopen("libencrypt.so", RTLD_LAZY);
    if (!handle) 
    {
      fprintf(stderr, "%s\n", dlerror());
      exit(EXIT_FAILURE);
    }
    Data stData = {0};
    stData.nSize=nLen;    
    stData.cBuffer= cBuffer;
    strncpy(stData.cKey, lcEncryptKey1, ONE_KB);
    dlerror();    /* Clear any existing error */

    *(void **) (&lpfEncryptDecrypt) = dlsym(handle, lcEncryptingFunc);

    if ((error = dlerror()) != NULL)  
    {
      fprintf(stderr, "%s\n", error);
      exit(EXIT_FAILURE);
    }

    (*lpfEncryptDecrypt)((void*)&stData);
    dlclose(handle);
    if(0 !=  DeleteKeyVal(lcEncryptKey1))
    {  
       return -1;
    }

    if(0 !=  DeleteKeyVal(lcEncryptKey2))
    {  
       return -1;
    }
    if(0 !=  DeleteKeyVal(lcEncryptingFunc))
    {  
       return -1;
    }
    if(0 !=  DeleteKeyVal(lcDecryptingFunc))
    {  
       return -1;
    }
    DeleteNewMap(pConfigObject); 
    //exit(EXIT_SUCCESS);
    return 0;
}

