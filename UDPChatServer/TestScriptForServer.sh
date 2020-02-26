#!/bin/bash

PID=0

cleanup()
{
   echo "cleanup"
   kill -s SIGINT $PID
   RETVAL=`echo $?`
   if [ $RETVAL -ne 0 ]
   then
    {
          kill -9 $PID
    }
    fi
   exit 0
}

trap cleanup SIGINT SIGKILL 





#########
#       #
#  MAIN #
#       #
#########

./server.out &
PID=($!)

while true; do
echo -n ""

done

