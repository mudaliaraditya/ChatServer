#pragma once
#include "includes.h"

struct tagData;

typedef std::map<std::string, tagData*>           CIdentiferDataStore;
typedef std::map<std::string, tagData*>::iterator CIteratotrIdentiferDataStore;

typedef std::map<int, tagData*>           CClientIdDataStore;
typedef std::map<std::string, tagData*>::iterator CClientIdDataStoreIter;


typedef  std::deque<tagData*>           CDataStore;
typedef  std::deque<tagData*>::iterator CIteratorDataStore;

