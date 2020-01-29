#!/bin/bash
A=0
PID=0
ARR=()


clean_up()
{

for i in ${ARR[@]};
    do 
      kill -s SIGINT $i
done


   #pkill client.out
   echo "done"
   exit 0
}


####################################################
###                 MAIN                          ##
####################################################

trap clean_up SIGHUP SIGINT SIGTERM

for ((k=1 ;k<=1 ;k++ ))
do
   A=`expr $A + 1`
   ./client.out TEST B$A C$A > Logs/BC$A_$$.txt &
   PID=($!)
   ARR+=$PID
   ARR+=" "
   sleep 4
   ./client.out TEST C$A B$A > Logs/CB$A_$$.txt   &
   PID=($!)
   ARR+=$PID 
   ARR+=" "
   sleep 4
done




#pkill client.out
while true; do
echo -n ""

done

