#!/bin/bash

# Functions
function time_func()
{
    local TIME=$( { time out/$1 $2 $3; } 2>&1 )
    local TVAL=$(echo $TIME | sed 's/real 0m\([0-9.]*\)s.*/\1/')
    echo "$1: $TVAL"
}
function test_case()
{
    time_func sequential $1 $2
    time_func parralel $1 $2
}


# Code to run
echo "Testing.."
#   ----   #
#  Case 1  #
#   ----   #
echo "Case 1"
test_case 12 resources/case_1.txt
#   ----   #
#  Case 2  #
#   ----   #
echo "Case 2"
test_case 12 resources/case_2.txt
#   ----   #
#  Case 3  #
#   ----   #
echo "Case 3"
test_case 1000000000 resources/case_3.txt
