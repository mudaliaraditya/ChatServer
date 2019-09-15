#pragma once
#include "structs.h"
#include <string>
#include <deque>
#include <vector>
#include <map>
#include <set>
#include <list>
struct tagData;
class CSession;


typedef std::map<std::string, tagData*>                    CIdentiferDataStore;
typedef std::map<std::string, tagData*>::iterator          CIteratotrIdentiferDataStore;

typedef std::map<int, tagData*>                            CClientIdDataStore;
typedef std::map<std::string, tagData*>::iterator          CClientIdDataStoreIter;


typedef  std::deque<tagData*>                              CDataStore;
typedef  std::deque<tagData*>::iterator                    CIteratorDataStore;


typedef std::set<std::string>                              CIdentifierStringStore;
typedef CIdentifierStringStore::iterator                   CIterIdentifierStringStore;
typedef std::pair<CIterIdentifierStringStore, bool>        CRetValInsIterIdentifierStringStore;
typedef std::list<CSession*>                               CSessionMangerStore;
typedef std::list<CSession*>::iterator                     CSessionManagerStoreIterator;
typedef std::vector<tagSessionIdentifierData>              CSessionStore;
typedef std::vector<tagSessionIdentifierData>::iterator    CSessionStoreIterator;

