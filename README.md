# ecosystem_simulation

Implementation of the Ecosystem with Foxes and Rabbits using OpenMP thread computing-

Report Overleaf URL:
[https://www.overleaf.com/project/65740fe5480eb956a190598f](https://www.overleaf.com/project/65740fe5480eb956a190598f)

# How to run
## Using the .sh files
1. Open a shell and execute the bash scripts with `./compile_and_test.sh`

## Using the makefile
1. Navigate to the directory containing the git repository
1. Execute `make` in the command line
1. Execute the input file wanted with the desired number of threads with `OMP_NUM_THREADS=<num_threads> ./a.out < <input_file>`

## Other way
1. Compile the program "Ecosystem.c" with the following mpi command `gcc Ecosystem.c -o a.out -fopenmp`
2. Running the program:
    1. Execute the input file wanted with the desired number of threads with `OMP_NUM_THREADS=<num_threads> ./a.out < <input_file>`