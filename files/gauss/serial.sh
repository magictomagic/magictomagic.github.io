#!/bin/bash
val=200
while(( val <= 8000 ))
do
./serial.o $val
val=$[val+200]
#echo $val
done
