#!/bin/bash
inputs_folder="./input_examples"

omp_threads=(1 2 4 8 12)


# Loop through each OMP_NUM_THREADS value
for threads in "${omp_threads[@]}"; do
    echo "Iterating with <$threads> threads"
    # Loop through all the files in the input folder
    for input_file in "$inputs_folder"/*; do
        if [ -f "$input_file" ]; then
            echo "Trying with file $input_file"
            # Define the output file name based on the input file and the number of threads
            output_file=output_$(basename $input_file .txt)_threads${threads}.txt

            # Compile and run the program with the specific input file and OMP_NUM_THREADS value
            gcc Ecosystem.c -o a.out -fopenmp && OMP_NUM_THREADS=$threads ./a.out < "$input_file" > "outputs/$output_file"
        fi
    done
done