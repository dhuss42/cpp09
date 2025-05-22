#!/bin/bash

SPAN=24
NUMBERRANGE=100

echo -e "\033[32m=============TEST=============\033[0m"
echo "Generating $SPAN random numbers in range 0 to $NUMBERRANGE..."

args=""
for ((i = 0; i < SPAN; i++))
do
	RAND_NUM=$(( RANDOM % NUMBERRANGE))
	args+="$RAND_NUM "
done

echo -e "\033[34mRunning with arguments: $args\033[0m"
echo ""
./PmergeMe $args