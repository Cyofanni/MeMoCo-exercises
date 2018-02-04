#!/bin/bash

function printout {
	echo "Results" $1 "Simulated Annealing" > 30.txt
	../main ../instance_generator/tsp30.dat 500 200 0.4 0 0 203.5 >> 30.txt
	printf "\n\n\n"
	../main ../instance_generator/tsp30.dat 500 200 0.4 0 0 203.5 >> 30.txt
	printf "\n\n\n"
	../main ../instance_generator/tsp30.dat 600 200 0.5 0 0 203.5 >> 30.txt
	printf "\n\n\n"
	../main ../instance_generator/tsp30.dat 1000 200 0.5 0 0 203.5 >> 30.txt
	printf "\n\n\n"
	../main ../instance_generator/tsp30.dat 1000 500 0.5 0 0 203.5 >> 30.txt
	printf "\n\n\n"
	../main ../instance_generator/tsp30.dat 1000 500 0.7 0 0 203.5 >> 30.txt
}

#WITHOUT Simulated Annealing
echo "Results without Simulated Annealing" > 30.txt
../main ../instance_generator/tsp30.dat 500 200 0.4 0 0 203.5 >> 30.txt
printf "\n\n\n"
../main ../instance_generator/tsp30.dat 500 200 0.4 0 0 203.5 >> 30.txt
printf "\n\n\n"
../main ../instance_generator/tsp30.dat 600 200 0.5 0 0 203.5 >> 30.txt
printf "\n\n\n"
../main ../instance_generator/tsp30.dat 1000 200 0.5 0 0 203.5 >> 30.txt
printf "\n\n\n"
../main ../instance_generator/tsp30.dat 1000 500 0.5 0 0 203.5 >> 30.txt
printf "\n\n\n"
../main ../instance_generator/tsp30.dat 1000 500 0.7 0 0 203.5 >> 30.txt





