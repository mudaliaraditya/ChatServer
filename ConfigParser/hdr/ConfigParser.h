#pragma once
#include <errno.h>
#include<malloc.h>

#include<string>
#include<cstring>
#include<map>
#include<fstream>
#include <thread>
#include <mutex>


struct ConfigParserHandle
{
   void* pMap;
};




extern std::mutex g_i_ConfigParser_mutex;


