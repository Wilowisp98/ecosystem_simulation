@echo off
setlocal enabledelayedexpansion

REM Define the input folder
set "inputs_folder=C:\Users\P_PINTOL\OneDrive - Intelsat US LLC\unistuff\ecosystem_simulation\input_examples"

REM Define the OMP_NUM_THREADS values
set "omp_threads=1 2 4 8 16 32"



REM Compile the program
gcc Ecosystem.c -o a.out -fopenmp


REM Loop through each OMP_NUM_THREADS value
for %%t in (%omp_threads%) do (
    echo Iterating with %%t threads
    REM Loop through all the files in the input folder
    for %%f in ("%inputs_folder%\*") do (
        if exist "%%f" (
            echo Trying with file %%f
            REM Define the output file name based on the input file and the number of threads
            set "input_file=%%~nf"
            set "output_file=outputs/output_!input_file!_threads%%t.txt"

            REM Set the OMP_NUM_THREADS environment variable
            set OMP_NUM_THREADS=%%t
            REM Run the program with the specific input file and OMP_NUM_THREADS value
            a.out < "%%f" > "!output_file!"
        )
    )
)
