//#include<iostream>
#include<string>
#include<cstring>
#include<map>
#include <errno.h>
#include<fstream>
#include<malloc.h>
#include "ConfigParser.h"
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

void*   CreateNewMap()
{
    return new map< string,map < string,string > >;
}

void DeleteNewMap(void* pMap)
{
    map< string,map < string,string > >* lpcMap = (map< string,map < string,string > >*)pMap;
    delete lpcMap;
    pMap = NULL;
}



char* GetValueForKey(char* cKey,char* cFileName , void* pcVMap)
{
   map<string, map<string,string> >* pcMap = (map<string, map<string,string> >*)pcVMap;    
   map<string, map<string,string> >::iterator lcIterator =  pcMap->find(cFileName);
   if(lcIterator == pcMap->end())
   { 
        return NULL;
   }
   map<string,string> lcNao = lcIterator->second;
   map<string,string>::iterator lcIteratori =  lcNao.find(cKey);
   if(lcIteratori == lcNao.end())
   {    
        return NULL;
   }
   char* lpcBuffer =(char*) calloc(1,lcIteratori->second.length() + 1);;
   strncpy(lpcBuffer,lcIteratori->second.c_str(), lcIteratori->second.length());
   return lpcBuffer;
}

int DeleteKeyVal(char* pVal)
{
   errno = 0;
   free(pVal);
   pVal = NULL;
   if(errno == 0)
   {
      return 0;
   }
   else
   {
      return -1; 
   }
}



int AddTokenToStore(string cString,map<string,string>& cMap,map< string,map < string,string > >* pcBigMap)
{
   char lcBuffer[cString.length() + 1];
   memset(lcBuffer, 0 , cString.length() + 1);
   strncpy(lcBuffer, cString.c_str(), cString.length() );
   int lnCounter = 0;
   string lcKey = "";
   string lcVal = "";
   char lcEqualSign = '=';
   char* lpcDelimiter = strchr(lcBuffer, lcEqualSign);
   char* lcToken = lcBuffer;
   char* lcBuffStart = lcBuffer;
   while(lcToken != NULL)
   {
         lcToken = strtok(lcBuffStart,"= ");
         switch(lnCounter)
         {
            case 0:
            {
                lcBuffStart = NULL;
                if(lcToken != NULL)
                {
                   //Commented line
                   if(*lcToken == '#')
                   {
                      return 0;
                   }
                   //empty line
                   if(*lcToken == '\r' || *lcToken == '\n')
                   {
                      return 0;
                   }
                   if((lcToken - lpcDelimiter) >= 0)
                   {
                        return -1;
                   }
                   lcKey = lcToken;
                }
                else
                {
                   return 0;
                }
            }
            break;
             
            case 1:
            {
                if(lcToken != NULL)
                {
                   if((lcToken - lpcDelimiter) <= 0)
                   {
                        return -1;
                   }
                   lcVal = lcToken;
                }
                else
                {
                      return -1;
                } 
            }
            break;
            
            default:
            {
                if(lcToken == NULL)
                {
                   break;
                }
                else if(*lcToken == '\r' || *lcToken == '\n' || *lcToken == ' ')
                {
                    continue;
                }
                else
                {
                   return -1;
                }
            }
            break;

         }
         lnCounter++;
   }
   pair<map<std::string,std::string>::iterator,bool> lcIter =  cMap.insert(pair<string,string>(lcKey,lcVal));
   if(lcIter.second == false )
   {
       //cout << "duplicate" << endl;
       return -1;
   }
   return 0;
}

int  GetConfig(char* cFileName,void* pMap)
{
    map< string,map < string,string > >* lpcBigMap = (map< string,map < string,string > >*)pMap;
    map<string,string> lcMap;
    int lnRetVal = 0;
    std::fstream file(cFileName);
    std::string str;
    while (std::getline(file, str))
    {
      if(-1 == AddTokenToStore(str,lcMap,lpcBigMap))
      {
         return -1;
      }
    }
    pair<map< string,map < string,string > >::iterator,bool> lcInsertIter = lpcBigMap->insert(pair<string, map<string,string> >(cFileName,lcMap));
    if(lcInsertIter.second == false)
    {
      return -1;
    }
    return 0;
}
#ifdef __cplusplus
}
#endif
