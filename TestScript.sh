#!/bin/bash

PIDS=0
PIDC=0


cleanup()
{
   if [ $NOERRORHANDLE -eq 0 ]
   then
   {
      return;
   }
   fi
   kill -SIGINT -$$ 
   exit 0
}

trap cleanup SIGINT SIGKILL 


##########
#        #
#  MAIN  #
#        #
##########
NOERRORHANDLE=0
PWD=`pwd`
cd UDPChatServer
./TestScriptForServer.sh &
PIDS=($!)
cd ..
sleep 30
cd UDPChatClient
./TestRunScriptForClient.sh &
PIDC=($!)
sleep 15
NOERRORHANDLE=1;
while true; do
echo -n ""
done

