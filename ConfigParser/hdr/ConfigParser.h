#pragma once
#include <errno.h>
#include<malloc.h>

#include<string>
#include<cstring>
#include<map>
#include<fstream>
#include <thread>
#include <mutex>
#include <unordered_map>

struct ConfigParserHandle
{
   void* pMap;
};




extern std::mutex g_i_ConfigParser_mutex;
typedef std::unordered_map<std::string, std::string> ConfigKeyVal;
typedef std::unordered_map<std::string, ConfigKeyVal>  ConfigParserConfigHolder;
