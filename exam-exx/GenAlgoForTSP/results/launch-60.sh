#!/usr/bin/sh

function printout {
   echo "Results" $1 "Simulated Annealing" > 60.txt
   ../main ../instance_generator/tsp60.dat 500 200 0.4 0 0 233.3 >> 60.txt
   printf "\n\n\n"
   ../main ../instance_generator/tsp60.dat 500 200 0.4 0 0 233.3 >> 60.txt
   printf "\n\n\n"
   ../main ../instance_generator/tsp60.dat 600 200 0.5 0 0 233.3 >> 60.txt
   printf "\n\n\n"
   ../main ../instance_generator/tsp60.dat 1000 200 0.5 0 0 233.3 >> 60.txt
   printf "\n\n\n" 
   ../main ../instance_generator/tsp60.dat 1000 500 0.5 0 0 233.3 >> 60.txt 
}

#WITHOUT Simulated Annealing
echo "Results without Simulated Annealing" > 60.txt
../main ../instance_generator/tsp60.dat 500 200 0.4 0 0 233.3 >> 60.txt
printf "\n\n\n"
../main ../instance_generator/tsp60.dat 500 200 0.4 0 0 233.3 >> 60.txt
printf "\n\n\n"
../main ../instance_generator/tsp60.dat 600 200 0.5 0 0 233.3 >> 60.txt
printf "\n\n\n"
../main ../instance_generator/tsp60.dat 1000 200 0.5 0 0 233.3 >> 60.txt
printf "\n\n\n"
../main ../instance_generator/tsp60.dat 1000 500 0.5 0 0 233.3 >> 60.txt
