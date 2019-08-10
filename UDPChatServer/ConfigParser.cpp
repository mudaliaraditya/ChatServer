#include<iostream>
#include<string>
#include<cstring>
#include<map>
#include <errno.h>
#include<fstream>
#include<malloc.h>
#include "ConfigParser.h"
using namespace std;
//map<string,string> g_cMap;
//map< string,map < string,string > > g_cBigMap;

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
   char* lpcBuffer =(char*) calloc(1,lcIteratori->second.length() + 1);//new[lcIteratori.length() + 1];
   strncpy(lpcBuffer,lcIteratori->second.c_str(), lcIteratori->second.length());
   return lpcBuffer;
}

int DeleteKeyVal(char* pVal)
{
   free(pVal);
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
   //cout << "splitting tokens" << endl;
   char lcBuffer[cString.length() + 1] = {0};
   strncpy(lcBuffer, cString.c_str(), cString.length() );
   int lnCounter = 0;
   string lcKey = "";
   string lcVal = "";
   char lcEqualSign = '=';
   char* lpcDelimiter = strchr(lcBuffer, lcEqualSign);
   char* lcToken = lcBuffer;
   char* lcBuffStart = lcBuffer;
   int lnPosition1 = 0; 
   int lnPosition2 = 0; 
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
                   if(*lcToken == '#')
                   {
                      cout << "line commented" << endl;
                      return 0;
                   }
                }
                if(lcToken != NULL)
                {
                   if((lcToken - lpcDelimiter) >= 0)
                   {
                        return -1;
                   }
                   //cout << "the key is " << lcKey << endl;
                }
                if(lcToken != NULL)
                {
                   lcKey = lcToken;
                   cout << "the key is " << lcKey << endl;
                }
                else
                {
                   return -1;
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
                   //cout << "the key is " << lcKey << endl;
                }
                if(lcToken != NULL)
                {
                    if(lcToken != NULL)
                    {
                      lcVal = lcToken;
                      cout << "the value is " << lcVal << endl;
                    }
                    else
                   {
                      return -1;
                   }
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
       cout << "duplicate" << endl;
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
        // Process str
      cout << str.c_str() << endl;
      if(-1 == AddTokenToStore(str,lcMap,lpcBigMap))
      {
         cout << "failure" << endl;
        // break;
         return -1;
      }
    }
   // pair< map< <string,map<string,string> >::iterator, bool> lcInsertIter = g_cBigMap.insert(pair<string, map<string,string> >(cFileName,lcMap));
   // map<int,int> lcMap;
    //pair<>= lcMap.insert(pair<int,int>(1,1))
    pair<map< string,map < string,string > >::iterator,bool> lcInsertIter = lpcBigMap->insert(pair<string, map<string,string> >(cFileName,lcMap));
    if(lcInsertIter.second == false)
    {
      cout << "failure";
      return -1;
    }
    return 0;
}
/*
int main(int nArgc,char* cArgv[])
{
   if(-1 ==  GetConfig("config.txt"))
   {
      cout << "FAILURE" << endl;
      return -1; 
   }
   cout << GetValueForKey("hello","config.txt") << endl;

}*/
#ifdef __cplusplus
}
#endif
