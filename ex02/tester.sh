#!/bin/bash

NUMBERTESTS=10
SPAN=30
NUMBERRANGE=10000

echo -e "\033[32m=============TESTS=============\033[0m"
#echo "Generating $SPAN random numbers in range 0 to $NUMBERRANGE..."

for ((j = 0; j <= NUMBERTESTS; j++))
do
	echo -e "\033[32m-----TEST $j-----\033[0m"
	args=""
	for ((i = 0; i < SPAN; i++))
	do
		RAND_NUM=$(( RANDOM % NUMBERRANGE))
		args+="$RAND_NUM "
	done

	#echo -e "\033[34mRunning with arguments: $args\033[0m"
	echo ""
	./PmergeMe $args
	echo ""
done
