#!/usr/bin/sh

function printout {
	echo "Results" $1 "Simulated Annealing" > 80.txt
	../main ../instance_generator/tsp80.dat 500 200 0.4 $2 $3 260.5 >> 80.txt
	printf "\n\n\n"
	../main ../instance_generator/tsp80.dat 500 200 0.4 $2 $3 260.5 >> 80.txt
	printf "\n\n\n"
	../main ../instance_generator/tsp80.dat 600 200 0.5 $2 $3 260.5 >> 80.txt
	printf "\n\n\n"
	../main ../instance_generator/tsp80.dat 1000 200 0.5 $2 $3 260.5 >> 80.txt	
	printf "\n\n\n"
	../main ../instance_generator/tsp80.dat 1000 500 0.5 $2 $3 260.5 >> 80.txt
}

#WITHOUT Simulated Annealing
echo "Results without Simulated Annealing" > 80.txt
../main ../instance_generator/tsp80.dat 500 200 0.4 0 0 260.5 >> 80.txt
printf "\n\n\n"
../main ../instance_generator/tsp80.dat 500 200 0.4 0 0 260.5 >> 80.txt
printf "\n\n\n"
../main ../instance_generator/tsp80.dat 600 200 0.5 0 0 260.5 >> 80.txt
printf "\n\n\n"
../main ../instance_generator/tsp80.dat 1000 200 0.5 0 0 260.5 >> 80.txt
printf "\n\n\n"
../main ../instance_generator/tsp80.dat 1000 500 0.5 0 0 260.5 >> 80.txt
