#!/bin/bash
A=0
clean_up()
{
   pkill client.out
   echo "done"
   exit 0
}


for ((k=1 ;k<=2 ;k++ ))
do
   A=`expr $A + 1`
   ./client.out TEST C$A B$A &
   sleep 4
   ./client.out TEST B$A C$A &
   sleep 4
done


trap clean_up SIGHUP SIGINT SIGTERM




while true; do
echo ""

done

