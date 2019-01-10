#pragma once
#ifndef WIN32
#include <iostream>
#include <map>
#include <memory>
#include <math.h>
#include <deque>
#include <list>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include <pthread.h>
#include "structs.h"
#include "defines.h"
#include "enums.h"
//#include "Functions.h"

using namespace std;

#endif
#ifdef WIN32

#include <errno.h>
#include <stdio.h> 
#include <stdlib.h> 
//#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
//#include <sys/socket.h> 
//#include <arpa/inet.h> 
//#include <netinet/in.h> 
#include <iostream>  
#include <string.h>
#include <map>
#include <list>
#include <winsock.h>
//#include <WinSock2.h>
//#include <WS2tcpip.h>
#include <Windows.h>
#include <time.h>
#include "structs.h"
#include "defines.h"
#include "enums.h"

//#include <inaddr.h>
//#include <c:\Program Files (x86)\Windows Kits\8.1\Include\shared\inaddr.h>
#pragma comment (lib,"wsock32.lib")
#pragma comment (lib, "Ws2_32.lib")
#endif
