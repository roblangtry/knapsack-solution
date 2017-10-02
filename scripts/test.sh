#!/bin/bash
echo "Testing.."
#   ----   #
#  Case 1  #
#   ----   #
echo "Case 1"
time out/sequential 12 resources/case_1.txt
#   ----   #
#  Case 2  #
#   ----   #
echo "Case 2"
time out/sequential 12 resources/case_2.txt
#   ----   #
#  Case 3  #
#   ----   #
echo "Case 3"
time out/sequential 1000000000 resources/case_3.txt