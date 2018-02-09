#!/bin/bash

DIMPOP=500
NUMGEN=200

function printout {
	echo "***RESULTS" $1 "Simulated Annealing" >> $5
	printf "\n\n" >> $5
	COUNTER=0
	while [  $COUNTER -lt 7 ]; do
             ../main ../instance_generator/tsp26.dat $DIMPOP $NUMGEN 0.2 $2 $3 $4 >> $5
	     printf "\n\n\n" >> $5
             let COUNTER=COUNTER+1 
        done
}

#WITHOUT Simulated Annealing
#firstarg: "with" or "without"
#second and third: simulated annealing parameters
#fourth: taken from command line, it's the known optimum
#fifth: output file
echo "BEGIN OF FILE" > $2

RUNCOUNTER=0
while [  $RUNCOUNTER -lt 7 ]; do
    echo "RESULTS FROM RUN $RUNCOUNTER" >> $2
    printf "\n" >> $2
    printout "without" 0 0 $1 $2
    echo "#####################################################" >> $2
    printf "\n" >> $2
    
    let RUNCOUNTER=RUNCOUNTER+1 
done
