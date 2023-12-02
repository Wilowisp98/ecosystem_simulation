#include <stdio.h>
#include <stdlib.h>

#define FOX 'F'
#define RABBIT 'R'
#define STONE 'S'
#define EMPTY '.'
#define VERBOSE 0

// Struct representing an <link>animal</link>
typedef struct {
    int current_gen; // Current generation
    char type; // <link>Animal</link> type: 'R' for rabbit, 'F' for fox
} Animal;

void printMatrix(char* matrix, int rows, int cols);
void printAdjacentBlocks(char* matrix, int rows, int cols, int row, int col, char currentEntity, int* validMoves);
void printPosition(int row, int col);

int chooseMove(int* validMoves);

void read_inputs(int* GEN_PROC_RABBITS, int* GEN_PROC_FOXES, int* GEN_FOOD_FOXES, int* N_GEN, int* R, int* C, int* N) {
    scanf("%d", GEN_PROC_RABBITS);
    scanf("%d", GEN_PROC_FOXES);
    scanf("%d", GEN_FOOD_FOXES);
    scanf("%d", N_GEN);
    scanf("%d", R);
    scanf("%d", C);
    scanf("%d", N);
}

void load_ecosystem(Animal** matrix, int R, int C, int N) {
    for (int i = 0; i < N; i++) {
        char obj[10]; // Assuming the maximum length of the object name is 10 characters
        int row, col;
        scanf("%s %d %d", obj, &row, &col);
        
        // Map object representation to character
        char representation;
        if (strcmp(obj, "ROCK") == 0) {
            representation = STONE;
        } else if (strcmp(obj, "RABBIT") == 0) {
            representation = RABBIT;
        } else if (strcmp(obj, "FOX") == 0) {
            representation = FOX;
        } else {
            printf("Unknown object type: %s\n", obj);
            exit(1); // Terminate program due to unknown object type
        }
        
        // Store the mapped representation in the matrix
        matrix[row][col] = representation;
    }
}
int main(int argc, char *argv[]) {
    int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, rows, cols, N;
    char* ecosystem;
    long startTime = time(NULL); 

    read_inputs(&GEN_PROC_RABBITS, &GEN_PROC_FOXES, &GEN_FOOD_FOXES, &N_GEN, &R, &C, &N);
    char* ecosystem = (char*)malloc(R * C * sizeof(char));
    // Load the ecosystem data into the matrix
    load_ecosystem(ecosystem, rows, cols, N);

    // Print the matrix array after allocating it into a contiguous array
    printMatrix(matrix, rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("Processing matrix[%d][%d] with entity '%c'\n", i, j, matrix[i * cols + j]);

            int validMoves[4] = {-1, -1, -1, -1};
            printAdjacentBlocks(matrix, rows, cols, i, j, matrix[i * cols + j], validMoves);
            int chosenMove = chooseMove(validMoves);
            if (chosenMove != -1) {
                printf("Chosen move position: ");
                printPosition(chosenMove / cols, chosenMove % cols);
            } else {
                printf("No valid move for matrix[%d][%d].\n", i, j);
            }
        }
    }

    free(matrix);

    return 0;
}

char* readMatrix(FILE* file, int rows, int cols) {
    char* matrix = (char*)malloc(rows * cols * sizeof(char));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (fscanf(file, " %c", &matrix[i * cols + j]) != 1) {
                fprintf(stderr, "Error reading matrix[%d][%d] from file.\n", i, j);
                exit(1);
            }
        }
    }
    return matrix;
}

void printMatrix(char* matrix, int rows, int cols) {
    printf("Matrix array after allocating into a contiguous array:\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%c ", matrix[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printAdjacentBlocks(char* matrix, int rows, int cols, int row, int col, char currentEntity, int* validMoves) {
    int count = 0;

    printf("Adjacent blocks for matrix[%d][%d] with entity '%c': ", row, col, currentEntity);

    // Right
    if (col + 1 < cols && currentEntity == FOX && matrix[row * cols + col + 1] == RABBIT) {
        validMoves[count++] = (row * cols) + (col + 1);
        printf("[%d, %d] ", row, col + 1);
    } else if (col + 1 < cols && currentEntity == RABBIT && matrix[row * cols + col + 1] == EMPTY) {
        validMoves[count++] = (row * cols) + (col + 1);
        printf("[%d, %d] ", row, col + 1);
    }

    // Left
    if (col - 1 >= 0 && currentEntity == FOX && matrix[row * cols + col - 1] == RABBIT) {
        validMoves[count++] = (row * cols) + (col - 1);
        printf("[%d, %d] ", row, col - 1);
    } else if (col - 1 >= 0 && currentEntity == RABBIT && matrix[row * cols + col - 1] == EMPTY) {
        validMoves[count++] = (row * cols) + (col - 1);
        printf("[%d, %d] ", row, col - 1);
    }

    // Up
    if (row - 1 >= 0 && currentEntity == FOX && matrix[(row - 1) * cols + col] == RABBIT) {
        validMoves[count++] = ((row - 1) * cols) + col;
        printf("[%d, %d] ", row - 1, col);
    } else if (row - 1 >= 0 && currentEntity == RABBIT && matrix[(row - 1) * cols + col] == EMPTY) {
        validMoves[count++] = ((row - 1) * cols) + col;
        printf("[%d, %d] ", row - 1, col);
    }

    // Down
    if (row + 1 < rows && currentEntity == FOX && matrix[(row + 1) * cols + col] == RABBIT) {
        validMoves[count++] = ((row + 1) * cols) + col;
        printf("[%d, %d] ", row + 1, col);
    } else if (row + 1 < rows && currentEntity == RABBIT && matrix[(row + 1) * cols + col] == EMPTY) {
        validMoves[count++] = ((row + 1) * cols) + col;
        printf("[%d, %d] ", row + 1, col);
    }

    printf("\n");
}

void printPosition(int row, int col) {
    printf("[%d, %d]\n", row, col);
}

int chooseMove(int* validMoves) {
    return validMoves[0];
}
