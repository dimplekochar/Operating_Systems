#!/bin/bash
# script takes 3 arguments that are given to the master worker program

#uncomment suitable line to compile
#gcc -o master-worker master-worker-skeleton.cpp -lpthread
#g++ -o master-worker master-worker-array.cpp -lpthread
g++ -o master-worker master-worker-ll.cpp -lpthread

#run code

./master-worker $1 $2 $3 > output 

#check output
awk -f check.awk MAX=$1 output
