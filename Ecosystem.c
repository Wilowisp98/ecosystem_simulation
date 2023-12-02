#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOX 'F'
#define RABBIT 'R'
#define STONE '*'
#define EMPTY '.'
#define VERBOSE 1

// Struct representing an Entity (either Rock/Rabbit/Empty/Fox)
typedef struct {
    int gen_proc; // Current generation for procreation
    int gen_food; // Current generation for food
    int cur_gen; // Current Ecosystem generation (to evaluate if a move has already been made to a specific cell)
    char type; // <link>Entity</link> type: 'R' for rabbit, 'F' for fox
} Entity;

void get_valid_moves(Entity**, int, int, int, int, int**);
int* get_next_move(Entity** ecosystem, int rows, int cols, int row, int col, int G);
void get_next_generation(Entity**, int, int, int*);
int chooseMove(int*);

Entity empty_entity() {
    Entity entity;
    entity.type = EMPTY;
    entity.gen_proc = 0;
    entity.gen_food = -1;
    entity.cur_gen = 0;
    return entity;
}
void aloc_matrix(Entity*** ecosystem, int rows, int cols) {
    Entity** tmp = (Entity**)malloc(rows * sizeof(Entity*));        // Allocate memory for the rows (Pointer to contiguous memory)
    Entity* data = (Entity*)malloc(rows * cols * sizeof(Entity));   // Contiguous memory position which will actually store the data
    for (int i=0; i<rows; i++) {
        tmp[i] = &data[i*cols];     // Assign the starting address of each row in the contiguous memory block
    }

    *ecosystem = tmp;   // Set the pointer to the allocated matrix
}

void printPosition(int row, int col) {
    printf("[%d, %d]\n", row, col);
}
void printMatrix(Entity** ecosystem, int rows, int cols, int G) {
    printf("Ecosystem for Generation=%d:\n", G);
    printf("TYPES | GEN_PROC | GEN_FOOD | CUR_GEN\n");
    for (int i = 0; i < rows; ++i) {
        printf("| ");
        for (int j = 0; j < cols; ++j) {
            printf("%c ", ecosystem[i][j].type);
        }
        printf("|     | ");
        for (int j = 0; j < cols; ++j) {
            printf("%d ", ecosystem[i][j].gen_proc);
        }
        printf("|     | ");
        for (int j = 0; j < cols; ++j) {
            printf("%d ", ecosystem[i][j].gen_food);
        }
        printf("|     | ");
        for (int j = 0; j < cols; ++j) {
            printf("%d ", ecosystem[i][j].cur_gen);
        }
        printf("|\n");
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

void load_ecosystem(Entity** ecosystem, int R, int C, int N) {
    for (int row=0; row<R; row++) {
        for (int col=0; col<C; col++) {
            Entity new_entity = empty_entity();
            ecosystem[row][col] = new_entity;
        }
    } 
    char obj[10]; // Assuming the maximum length of the object name is 10 characters
    for (int i = 0; i < N; i++) {
        int row, col;
        scanf("%s %d %d", obj, &row, &col);
        
        // Map object representation to character
        if (strcmp(obj, "ROCK") == 0) {
            ecosystem[row][col].type = STONE;
        } else if (strcmp(obj, "RABBIT") == 0) {
            ecosystem[row][col].type = RABBIT;
        } else if (strcmp(obj, "FOX") == 0) {
            ecosystem[row][col].type = FOX;
            ecosystem[row][col].gen_food = 0;
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
    int G=0;

    // Print the matrix array after allocating it into a contiguous array
    printMatrix(ecosystem, rows, cols, G);


    // obtain next gen
    get_next_generation(ecosystem, rows, cols, &G);
    
    // Print the matrix array after allocating it into a contiguous array
    printMatrix(ecosystem, rows, cols, G);
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


void get_next_generation(Entity** ecosystem, int rows, int cols, int* G) {
    // G -> Current Generation

    // Finding rabbits moves first
    printf("\n\nCHECKING MOVES FOR RABBITS\n\n");
    for (int row=0; row<rows; row++) {
        for (int col=0; col<cols; col++) {
            if (ecosystem[row][col].type != RABBIT || ecosystem[row][col].cur_gen > *G) continue;
            int* next_move = get_next_move(ecosystem, rows, cols, row, col, *G);
            printf("Rabbit from (%d, %d) -> (%d, %d)\n", row, col, next_move[0], next_move[1]);

            // Moving Rabbit
            // Checking if a rabbit already moved there with less gen_proc, if not move rabbit
            Entity* ent_next_pos = &ecosystem[next_move[0]][next_move[1]];
            if ((*ent_next_pos).type != RABBIT) {
                (*ent_next_pos).gen_proc = ecosystem[row][col].gen_proc + 1;
                (*ent_next_pos).cur_gen = *G + 1;
                (*ent_next_pos).gen_food = -1;
                (*ent_next_pos).type = RABBIT;
            } else if ((*ent_next_pos).gen_proc < ecosystem[row][col].gen_proc) {
                (*ent_next_pos).gen_proc = ecosystem[row][col].gen_proc + 1;
                (*ent_next_pos).cur_gen = *G + 1;
            }
        }
    }
    // Finding Foxes moves second
    printf("\n\nCHECKING MOVES FOR FOXES\n\n");
    for (int row=0; row<rows; row++) {
        for (int col=0; col<cols; col++) {
            if (ecosystem[row][col].type != FOX || ecosystem[row][col].cur_gen > *G) continue;
            int* next_move = get_next_move(ecosystem, rows, cols, row, col, *G);
            printf("Fox from (%d, %d) -> (%d, %d)\n", row, col, next_move[0], next_move[1]);
            // Moving Fox
            // Checking if a rabbit already moved there with less gen_proc, if not move rabbit
            Entity* ent_next_pos = &ecosystem[next_move[0]][next_move[1]];
            if ((*ent_next_pos).type == RABBIT) {
                (*ent_next_pos).gen_proc = ecosystem[row][col].gen_proc + 1;
                (*ent_next_pos).cur_gen = *G + 1;
                (*ent_next_pos).gen_food = 0;
                (*ent_next_pos).type = FOX;
            } else if ((*ent_next_pos).gen_proc < ecosystem[row][col].gen_proc) {
                (*ent_next_pos).gen_proc = ecosystem[row][col].gen_proc + 1;
                (*ent_next_pos).gen_food = ecosystem[row][col].gen_food + 1;
                (*ent_next_pos).cur_gen = *G + 1;
                (*ent_next_pos).type = FOX;
            } else {
                (*ent_next_pos).gen_proc = ecosystem[row][col].gen_proc + 1;
                (*ent_next_pos).gen_food = ecosystem[row][col].gen_food + 1;
                (*ent_next_pos).cur_gen = *G + 1;
                (*ent_next_pos).type = FOX;
            }
        }
    }
    // After moving all the rabits, deleting old generation rabbits and Foxes
    for (int row=0; row<rows; row++) {
        for (int col=0; col<cols; col++) {
            if (ecosystem[row][col].type == STONE) {
                ecosystem[row][col].cur_gen = *G+1;
            }
            if (ecosystem[row][col].cur_gen == *G)  ecosystem[row][col] = empty_entity();
        }
    }

    // Increase generation
    (*G)++;
}

int* get_next_move(Entity** ecosystem, int rows, int cols, int row, int col, int G) {
    int* validMoves = (int*)malloc(sizeof(int) * 4);
    for (int i=0; i<4; i++) {validMoves[i]=-1;}
    get_valid_moves(ecosystem, rows, cols, row, col, &validMoves);
    int* output = (int*)malloc(sizeof(int)*2);
    output[0] = row;
    output[1] = col;
    int max_val = 1;
    int p = 0;
    for (int i=0; i<4; i++) {
        if (max_val < validMoves[i]) {
            max_val = validMoves[i];
            p = 0;
        } 
        if (max_val == validMoves[i]) {
            p++;
        }
    }

    if (VERBOSE) {printf("validMoves = {%d, %d, %d, %d}\n", validMoves[0], validMoves[1], validMoves[2], validMoves[3]);}
    // return output;
    // Finding the first greatest valid move != -1 
    int cur_count = -1;
    for (int i=0; i<4; i++) {
        // Whenever a move != -1 is found, return the respective resulting coordinates (clockwise order, so)
        // 0 -> UP    (-1, 0)
        // 1 -> LEFT  (0, -1)
        // 2 -> DOWN  (1, 0)
        // 3 -> RIGHT (0, 1)
        if (validMoves[i] == max_val) {
            cur_count++;  
            // Choosing only the G + X + Y mod p move
            if ((G + row + col) % p != cur_count) continue;
            if (i==0) {         // 0 -> UP    (-1, 0)
                output[0] = -1 + row;
                output[1] = col;
            } else if (i==1) { // 1 -> RIGHT (0, 1)
                output[0] = row;
                output[1] = 1 + col;
            } else if (i==2) { // 2 -> DOWN  (1, 0)
                output[0] = 1 + row;
                output[1] = col;
            } else if (i==3) {  // 3 -> LEFT  (0, -1)
                output[0] = row;
                output[1] = -1 + col;
            }          
            return output;
        }
    } 
    return output;
}

void get_valid_moves(Entity** ecosystem, int rows, int cols, int row, int col, int** validMoves) {
    int count = 0;
    Entity currentEntity = ecosystem[row][col]; 
    if (VERBOSE)
        printf("\nAdjacent blocks for ecosystem[%d][%d] with entity '%c' ", row, col, currentEntity.type);

    int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    Entity next_pos_entity;
    for (int dir=0; dir<4; dir++) {
        // If direction is outside ecosystem, try another direction
        if (row+directions[dir][0] < 0 || row+directions[dir][0] >= rows || col+directions[dir][1] < 0 || col+directions[dir][1] >= cols) continue;

        next_pos_entity = ecosystem[row+directions[dir][0]][col+directions[dir][1]];
        if (next_pos_entity.type == STONE) continue;

        // Checking moves for rabbits
        if (currentEntity.type == RABBIT) {
            if (next_pos_entity.type == FOX || (next_pos_entity.type==RABBIT && currentEntity.cur_gen == next_pos_entity.cur_gen)) continue;
            (*validMoves)[dir] = 1;
        } 
        else if (currentEntity.type == FOX) {
            if (next_pos_entity.type == RABBIT && next_pos_entity.cur_gen > currentEntity.cur_gen) {
                (*validMoves)[dir] = 2; // The higher the number, the higher the priority 
            }
            if (next_pos_entity.type == EMPTY) {
                (*validMoves)[dir] = 1;
            }
        }
    }
    if (VERBOSE) printf("\n");
}



int chooseMove(int* validMoves) {
    for (int i=0; i<4; i++) {
        if (validMoves[i] > -1)
            return i;
    }
    return -1;
}
