#!/usr/bin/sh

function printout {
     echo "Results" $1 "Simulated Annealing" > 100.txt
     ../main ../instance_generator/tsp100.dat 500 200 0.4 $2 $3 287.7 >> 100.txt
     printf "\n\n\n"
     ../main ../instance_generator/tsp100.dat 500 200 0.4 $2 $3 287.7 >> 100.txt
     printf "\n\n\n"
     ../main ../instance_generator/tsp100.dat 600 200 0.5 $2 $3 287.7 >> 100.txt
     printf "\n\n\n"
     ../main ../instance_generator/tsp100.dat 1000 200 0.5 $2 $3 287.7 >> 100.txt
     printf "\n\n\n"
     ../main ../instance_generator/tsp100.dat 1000 500 0.5 $2 $ 287.7 >> 100.txt
     printf "\n\n\n"
    ../main ../instance_generator/tsp100.dat 1500 500 0.7 $2 $3 287.7 >> 100.txt
}

#WITHOUT Simulated Annealing
echo "Results without Simulated Annealing" > 100.txt
../main ../instance_generator/tsp100.dat 500 200 0.4 0 0 287.7 >> 100.txt
printf "\n\n\n"
../main ../instance_generator/tsp100.dat 500 200 0.4 0 0 287.7 >> 100.txt
printf "\n\n\n"
../main ../instance_generator/tsp100.dat 600 200 0.5 0 0 287.7 >> 100.txt
printf "\n\n\n"
../main ../instance_generator/tsp100.dat 1000 200 0.5 0 0 287.7 >> 100.txt
printf "\n\n\n"
../main ../instance_generator/tsp100.dat 1000 500 0.5 0 0 287.7 >> 100.txt
printf "\n\n\n"
../main ../instance_generator/tsp100.dat 1500 500 0.7 0 0 287.7 >> 100.txt

