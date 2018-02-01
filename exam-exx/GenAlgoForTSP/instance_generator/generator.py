import numpy as np
from random import *
import sys

def symMatrixGen(dim):
    shape = (dim, dim)
    matr = np.zeros(shape)
    for i in range(dim):
	diag_val = 0.0
	matr[i, i] = diag_val
	for count in range(matr[i, i+1:].size):
	      nondiag_randVal = uniform(1, 100)
	      nondiag_randVal = round(nondiag_randVal, 1)
	      matr[i, i+1:][count] = nondiag_randVal
	      matr[i+1:, i][count] = nondiag_randVal
    return matr

def printInstance(inst):
    size = inst[0].size
    for i in range(size):
	for j in range(size):
	    print inst[i, j],
	    print "\t",
	print "\n",

    
if (len(sys.argv) < 2):
    print "usage: python generator.py instance_size"

else:
   instance_size = int(sys.argv[1])
   instance = symMatrixGen(instance_size)
   print instance_size
   printInstance(instance)

     
