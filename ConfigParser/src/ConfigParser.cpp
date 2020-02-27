//#include<iostream>
#include <string>
#include <cstring>
#include <map>
#include <errno.h>
#include <fstream>
#include <malloc.h>
#include "ConfigParser.h"
using namespace std;
std::mutex g_i_ConfigParser_mutex;
#ifdef __cplusplus
extern "C" 
{
#endif

/*
*********************************************************************************





*********************************************************************************

*/


void*   CreateNewMap()
{
    const std::lock_guard<std::mutex> lock(g_i_ConfigParser_mutex);
    return new map< string,map < string,string > >;
}

void DeleteNewMap(void* pMap)
{
    const std::lock_guard<std::mutex> lock(g_i_ConfigParser_mutex);
    map< string,map < string,string > >* lpcMap = (map< string,map < string,string > >*)pMap;
    delete lpcMap;
    pMap = NULL;
}



char* GetValueForKey(char* cKey,char* cFileName , void* pcVMap)
{
   const std::lock_guard<std::mutex> lock(g_i_ConfigParser_mutex);
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
   const std::lock_guard<std::mutex> lock(g_i_ConfigParser_mutex);
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

int CheckForGivenSetofChars(char cChar,string sListOfRChars)
{
    for(unsigned int i = 0;i < sListOfRChars.length();i++)
    {
          if (cChar == sListOfRChars[i])
          {
             return 0;
          }
    }
    return -1;
}


int CheckForCommentSpceNewLine(char cVal)
{
    switch(cVal)
    {
       case  ' ':
       case '\r':
       case '\n':
       case '#':
       {
          return 0;
       }
       default:
       {
          return -1;
       }
    }
    return -1;
}
//
int CheckForSpceNewLine(char cVal)
{
    switch(cVal)
    {
       case  ' ':
       case '\r':
       case '\n':
       {
          return 0;
       }
       default:
       {
          return -1;
       }
    }
    return -1;
}

int CheckForNewLine(char cVal)
{
    switch(cVal)
    {
       case '\r':
       case '\n':
       {
          return 0;
       }
       default:
       {
          return -1;
       }
    }
    return -1;
}

int CheckForComment(char cVal)
{
    switch(cVal)
    {
       case '#':
       {
          return 0;
       }
       default:
       {
          return -1;
       }
    }
    return -1;
}

int CheckForSpce(char cVal)
{
    switch(cVal)
    {
       case '#':
       {
          return 0;
       }
       default:
       {
          return -1;
       }
    }
    return -1;
}



int AddTokenToStore(string cString,map<string,string>& cMap,map< string,map < string,string > >* pcBigMap)
{
   char lcBuffer[cString.length() + 1];
   char lcBufferForDQ[cString.length() + 1];//Temp Buffer for handling '"' strings
   memset(lcBuffer, 0 , cString.length() + 1);
   memset(lcBufferForDQ, 0 , cString.length() + 1);
   strncpy(lcBuffer, cString.c_str(), cString.length() );
   strncpy(lcBufferForDQ, cString.c_str(), cString.length() );
   int lnCounter = 0;
   string lcKey = "";
   string lcVal = "";
   //char lcEqualSign = '=';
   char* lpcDelimiter = strchr(lcBuffer, '=');
   char* lcToken = lcBuffer;
   char* lcBuffStart = lcBuffer;//for nullable strtok
   int lnSplitToken = 0;
   while(lcToken != NULL)
   {
         lcToken = strtok(lcBuffStart,"= ");
         switch(lnCounter)
         {
            /*setting the LHS of the config*/
            case 0:
            {
                lcBuffStart = NULL;//for strtok to work
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
                    // = value Occurance 
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

            /*setting the RHS of the config*/             
            case 1:
            {
                if(lcToken != NULL)
                {
                   if((lcToken - lpcDelimiter) <= 0)
                   {
                        return -1;
                   }
                   else if( lcToken[0] =='\"' )
                   { 
                        char* lpcR = strrchr(lcBufferForDQ,'\"');
                        char* lpcF = strchr(lcBufferForDQ,'\"');
                        char* lpcDelim = strchr(lcBufferForDQ,'=');//special validations for " " type files 
                        if (lpcF < lpcDelim )
                        {
                           return -1;
                        }
                        long lnLength = (lpcR )  - (lpcF + 1);
                        int lnlastIndex  = lpcR - lcBufferForDQ;//index of the ending "
                        for(unsigned int i = lnlastIndex + 1;i < cString.length();i++) 
                        {
                           switch(lcBufferForDQ[i])
                           {
                              case '\0':
                              case '\n':
                              case '\r':
                              case  ' ':
                              {
                              }
                              break;
                              default:
                              {
                                  return -1;
                              }
                           }
                        }
                        *lpcR = '\0';//making the second double quote as null terminated
                        lnSplitToken = 1;
                        lcVal = lpcF + 1;
                        break; 
                   }
                   else
                   {
                      lcVal = lcToken;
                   }
                }
                else
                {
                      return -1;
                } 
            }
            break;
            /*checking for any trash tokens*/            
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
                else if(lnSplitToken == 1)
                {
                    break;
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
    const std::lock_guard<std::mutex> lock(g_i_ConfigParser_mutex);
    if(cFileName == NULL || pMap == NULL)
    {
        return -1;
    }
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
