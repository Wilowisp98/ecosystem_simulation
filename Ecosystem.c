#include <stdio.h>
#include <stdlib.h>

#define FOX 'F'
#define RABBIT 'R'
#define STONE 'S'
#define EMPTY '.'

void printMatrix(char* matrix, int rows, int cols);
void printAdjacentBlocks(char* matrix, int rows, int cols, int row, int col, char currentEntity, int* validMoves);
void printPosition(int row, int col);

int chooseMove(int* validMoves);
char* readMatrix(FILE* file, int rows, int cols);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <filename> <rows> <cols>\n", argv[0]);
        return 1;
    }

    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s.\n", argv[1]);
        return 1;
    }

    char* matrix = readMatrix(file, rows, cols);

    fclose(file);

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
