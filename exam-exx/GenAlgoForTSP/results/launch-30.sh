#!/usr/bin/sh
#WITHOUT Simulated Annealing
echo "Results without Simulated Annealing" >> 30.txt
echo "500 200 0.4" >> 30.txt
../main ../instance_generator/tsp30.dat 500 200 0.4 0 0 >> 30.txt
echo "500 200 0.4" >> 30.txt
../main ../instance_generator/tsp30.dat 500 200 0.4 0 0 >> 30.txt
echo "600 200 0.5" >> 30.txt
../main ../instance_generator/tsp30.dat 600 200 0.5 0 0 >> 30. txt
echo "1000 200 0.5" >> 30.txt
../main ../instance_generator/tsp30.dat 1000 200 0.5 0 0 >> 30.txt
echo "1000 500 0.4" >> 30.txt
../main ../instance_generator/tsp30.dat 1000 500 0.5 0 0 >> 30.txt





