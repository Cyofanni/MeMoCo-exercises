#!/usr/bin/sh
#WITHOUT Simulated Annealing
echo "Results without Simulated Annealing" >> 80.txt
echo "500 200 0.4" >> 80.txt
../main ../instance_generator/tsp80.dat 500 200 0.4 0 0 >> 80.txt
echo "500 200 0.4" >> 30.txt
../main ../instance_generator/tsp80.dat 500 200 0.4 0 0 >> 80.txt
echo "600 200 0.5" >> 30.txt
../main ../instance_generator/tsp80.dat 600 200 0.5 0 0 >> 80. txt
echo "1000 200 0.5" >> 30.txt
../main ../instance_generator/tsp80.dat 1000 200 0.5 0 0 >> 80.txt
echo "1000 500 0.4" >> 30.txt
../main ../instance_generator/tsp80.dat 1000 500 0.5 0 0 >> 80.txt
