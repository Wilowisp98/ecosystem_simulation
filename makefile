# Makefile for ecosystem.c using gcc

# C compiler and compiler flags
COMP = gcc
CFLAGS = -fopenmp

# Source file and executable name
SRC = Ecosystem.c
EXECUTABLE = ecosystem

# Default target (executable)
$(EXECUTABLE): $(SRC)
	$(COMP) $(SRC) -o $(EXECUTABLE)  $(CFLAGS) 

# Phony target to clean up generated files
clean:
	rm -f $(EXECUTABLE)