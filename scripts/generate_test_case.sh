#!/bin/bash

TOT=$(( 0 ))
COUNTER=0
VAL=$(( ( RANDOM % 10 )  + 1 ))
WGT=$(( ( RANDOM % 1000 )  + 1 ))
TOT=$(( TOT  + WGT ))
echo "$VAL $WGT" > $1
while [  $COUNTER -lt 10 ]; do
    VAL=$(( ( RANDOM % 10 )  + 1 ))
    WGT=$(( ( RANDOM % 1000 )  + 1 ))
    TOT=$(( TOT  + WGT ))
    echo "$VAL $WGT" >> $1
    let COUNTER=COUNTER+1 
done
KNAPSACK=$(( ( RANDOM % TOT )  + 1 ))
echo "$KNAPSACK"