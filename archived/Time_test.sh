#!/bin/bash

# Number of executions
num_executions=100

# Initialize total execution time
total_time=0

# Loop for the specified number of executions
for ((i=1; i<=$num_executions; i++)); do
    # Record the start time
    start_time=$(date +%s%N)

    # Execute your C code (replace ./your_c_executable with your actual executable)
    ./compile_and_test.sh

    # Record the end time
    end_time=$(date +%s%N)

    # Calculate the execution time in milliseconds
    execution_time=$((($end_time - $start_time) / 1000000))

    # Add the execution time to the total
    total_time=$((total_time + execution_time))

    # Print the execution time for each iteration if needed
    # echo "Execution $i: $execution_time ms"
done

# Calculate the average execution time
average_time=$((total_time / num_executions))

echo "Average Execution Time: $average_time ms"
