#pragma once
#ifndef TYPEDEF_H
#define TYPEDEF_H
#include<map>
#include<set>


   typedef std::multimap<time_t,tagTimeData>                            CEventResenderStore;
   typedef std::multimap<time_t,tagTimeData>::iterator                  CEventResenderStoreIterator;
   typedef std::set<tagData,CtagDataSeqNoComp>                          CMessageStoreBySeqNo;
   typedef std::set<tagData,CtagDataSeqNoComp>::iterator                CMessageStoreBySeqNoIter;
   typedef std::map< int, std::set<int> >                        CSequenceStorebyUnOrderedSeqNo;
   typedef std::map< int, std::set<int> >::iterator              CSequenceStorebyUnOrderedSeqNoIter;
   typedef std::map<int,tagData>                                        CSequenceUnOrderedPacketsStore;
   typedef std::map<int,tagData>::iterator                              CSequenceUnOrderedPacketsStoreIter;

#endif
