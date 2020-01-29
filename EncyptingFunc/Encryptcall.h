#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include "encrypt.h"
#include "ConfigParser.h"


#define ENCRYPT_CONFIG "encrypt-config.txt"
#define ENCRYPT_KEY1   "ENCRYPT_KEY1"
#define ENCRYPT_KEY2   "ENCRYPT_KEY2"
#define ENCRYPT_METHOD "ENCRYPT_METHOD"
#define DECRYPT_METHOD "DECRYPT_METHOD"


int Encrypt(char* cBuffer,long long nLen);



int Decrypt(char* cBuffer,long long nLen);

