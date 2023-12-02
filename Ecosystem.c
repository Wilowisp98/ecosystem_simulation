#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOX 'F'
#define RABBIT '*'
#define STONE 'S'
#define EMPTY '.'
#define VERBOSE 0

// Struct representing an Entity (either Rock/Rabbit/Empty/Fox)
typedef struct {
    int gen_proc; // Current generation
    int gen_food; // Current generation
    char type; // <link>Entity</link> type: 'R' for rabbit, 'F' for fox
} Entity;

void get_valid_moves(Entity**, int, int, int, int, int**);
void next_generation(Entity**, int, int, int);
int chooseMove(int*);

void aloc_matrix(Entity*** matrix, int rows, int cols) {
    Entity** tmp = (Entity**)malloc(rows * sizeof(Entity*));        // Allocate memory for the rows (Pointer to contiguous memory)
    Entity* data = (Entity*)malloc(rows * cols * sizeof(Entity));   // Contiguous memory position which will actually store the data
    for (int i=0; i<rows; i++) {
        tmp[i] = &data[i*cols];     // Assign the starting address of each row in the contiguous memory block
    }

    *matrix = tmp;   // Set the pointer to the allocated matrix
}

void printPosition(int row, int col) {
    printf("[%d, %d]\n", row, col);
}
void printMatrix(Entity** matrix, int rows, int cols) {
    printf("Matrix array after allocating into a contiguous array:\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%c ", matrix[i][j].type);
        }
        printf("\n");
    }
    printf("\n");
}
void read_inputs(int* GEN_PROC_RABBITS, int* GEN_PROC_FOXES, int* GEN_FOOD_FOXES, int* N_GEN, int* R, int* C, int* N) {
    scanf("%d", GEN_PROC_RABBITS);
    scanf("%d", GEN_PROC_FOXES);
    scanf("%d", GEN_FOOD_FOXES);
    scanf("%d", N_GEN);
    scanf("%d", R);
    scanf("%d", C);
    scanf("%d", N);
}

void load_ecosystem(Entity** matrix, int R, int C, int N) {
    for (int row=0; row<R; row++) {
        for (int col=0; col<C; col++) {
            Entity new_entity;
            new_entity.type = EMPTY;
            new_entity.gen_proc = 0;
            new_entity.gen_food = -1;
            matrix[row][col] = new_entity;
        }
    } 
    char obj[10]; // Assuming the maximum length of the object name is 10 characters
    for (int i = 0; i < N; i++) {
        int row, col;
        scanf("%s %d %d", obj, &row, &col);
        
        // Map object representation to character
        if (strcmp(obj, "ROCK") == 0) {
            matrix[row][col].type = STONE;
        } else if (strcmp(obj, "RABBIT") == 0) {
            matrix[row][col].type = RABBIT;
        } else if (strcmp(obj, "FOX") == 0) {
            matrix[row][col].type = FOX;
            matrix[row][col].gen_food = 0;
        } else {
            printf("Unknown object type: %s\n", obj);
            exit(1); // Terminate program due to unknown object type
        }
    }
}
int main(int argc, char *argv[]) {
    int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, rows, cols, N;
    // long startTime = time(NULL); 

    read_inputs(&GEN_PROC_RABBITS, &GEN_PROC_FOXES, &GEN_FOOD_FOXES, &N_GEN, &rows, &cols, &N);

    Entity** ecosystem ;
    aloc_matrix(&ecosystem, rows, cols);
    // Load the ecosystem data into the matrix
    load_ecosystem(ecosystem, rows, cols, N);

    // Print the matrix array after allocating it into a contiguous array
    printMatrix(ecosystem, rows, cols);

    // obtain next gen
    next_generation(ecosystem, rows, cols, 0);
    // for (int i = 0; i < rows; ++i) {
    //     for (int j = 0; j < cols; ++j) {
    //         printf("Processing matrix[%d][%d] with entity '%c'\n", i, j, ecosystem[i][j].type);

    //         int validMoves[4] = {-1, -1, -1, -1};
    //         printAdjacentBlocks(ecosystem, rows, cols, i, j, ecosystem[i][j].type, validMoves);
    //         int chosenMove = chooseMove(validMoves);
    //         if (chosenMove != -1) {
    //             printf("Chosen move position: ");
    //             printPosition(chosenMove / cols, chosenMove % cols);
    //         } else {
    //             printf("No valid move for matrix[%d][%d].\n", i, j);
    //         }
    //     }
    // }

    free(ecosystem);

    return 0;
}



void next_generation(Entity** ecosystem, int rows, int cols, int G) {
    // G -> Current Generation
    for (int row=0; row<rows; row++) {
        for (int col=0; col<cols; col++) {
            int* valid_moves;
            get_valid_moves(ecosystem, rows, cols, row, col, &valid_moves);
        }
    }
}
void get_valid_moves(Entity** matrix, int rows, int cols, int row, int col, int** validMoves) {
    int count = 0;
    char currentEntity = matrix[row][col].type; 

    if (VERBOSE)
        printf("Adjacent blocks for matrix[%d][%d] with entity '%c': ", row, col, currentEntity);

    // Right
    if (col + 1 < cols && currentEntity == FOX && matrix[row][col + 1].type == RABBIT) {
        *(validMoves)[count++] = (row * cols) + (col + 1);
        if (VERBOSE) printf("[%d, %d] ", row, col + 1);
    } else if (col + 1 < cols && currentEntity == RABBIT && matrix[row][col + 1].type == EMPTY) {
        *(validMoves)[count++] = (row * cols) + (col + 1);
        if (VERBOSE) printf("[%d, %d] ", row, col + 1);
    }

    // Left
    if (col - 1 >= 0 && currentEntity == FOX && matrix[row][col - 1].type == RABBIT) {
        *(validMoves)[count++] = (row * cols) + (col - 1);
        if (VERBOSE) printf("[%d, %d] ", row, col - 1);
    } else if (col - 1 >= 0 && currentEntity == RABBIT && matrix[row][col - 1].type == EMPTY) {
        *(validMoves)[count++] = (row * cols) + (col - 1);
        if (VERBOSE) printf("[%d, %d] ", row, col - 1);
    }

    // Up
    if (row - 1 >= 0 && currentEntity == FOX && matrix[(row - 1)][col].type == RABBIT) {
        *(validMoves)[count++] = ((row - 1) * cols) + col;
        if (VERBOSE) printf("[%d, %d] ", row - 1, col);
    } else if (row - 1 >= 0 && currentEntity == RABBIT && matrix[(row - 1)][col].type == EMPTY) {
        *(validMoves)[count++] = ((row - 1) * cols) + col;
        if (VERBOSE) printf("[%d, %d] ", row - 1, col);
    }

    // Down
    if (row + 1 < rows && currentEntity == FOX && matrix[(row + 1)][col].type == RABBIT) {
        *(validMoves)[count++] = ((row + 1) * cols) + col;
        if (VERBOSE) printf("[%d, %d] ", row + 1, col);
    } else if (row + 1 < rows && currentEntity == RABBIT && matrix[(row + 1)][col].type == EMPTY) {
        *(validMoves)[count++] = ((row + 1) * cols) + col;
        if (VERBOSE) printf("[%d, %d] ", row + 1, col);
    }

    if (VERBOSE) printf("\n");
}



int chooseMove(int* validMoves) {
    return validMoves[0];
}
