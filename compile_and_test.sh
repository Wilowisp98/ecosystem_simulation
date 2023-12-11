#!/bin/bash
# Compile and run the program with the specific input file and OMP_NUM_THREADS value
gcc Ecosystem.c -o a.out -fopenmp && OMP_NUM_THREADS=1 ./a.out < "./input_examples/input100x100_unbal02"