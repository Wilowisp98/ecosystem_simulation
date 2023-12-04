#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define FOX 'F'
#define RABBIT 'R'
#define STONE '*'
#define EMPTY ' '
#define VERBOSE 0
#define ALLGEN 0

// Struct representing an Entity (either Rock/Rabbit/Empty/Fox)
typedef struct {
    int gen_proc; // Current generation for procreation
    int gen_food; // Current generation for food
    int cur_gen; // Current Ecosystem generation (to evaluate if a move has already been made to a specific cell)
    char type; // <link>Entity</link> type: 'R' for rabbit, 'F' for fox
} Entity;

void get_valid_moves(Entity**, int, int, int, int, int**, Entity**);
int* get_next_move(Entity**, int, int, int, int, int, Entity**);
void copy_entity(Entity, Entity*);
void get_next_generation(Entity***, int, int, int, int, int, int, Entity***);
int chooseMove(int*);

int max(int a, int b){
    return a>b ? a: b;
}
int min(int a, int b){
    return a<b ? a: b;
}
void empty_entity(Entity* entity, int G) {
    (*entity).type = EMPTY;
    (*entity).gen_proc = 0;
    (*entity).gen_food = -1;
    (*entity).cur_gen  = G;
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
    printf("Generation %d\n", G);
    // printf(" TYPES    GENPROC GENFOOD     CURGEN\n");
    for (int i=0; i<rows+2; i++){
        printf("-");
    }
    printf("   ");
    for (int i=0; i<rows+2; i++){
        printf("-");
    }
    printf(" ");
    for (int i=0; i<rows+2; i++){
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < rows; ++i) {
        printf("|");
        for (int j = 0; j < cols; ++j) {
            printf("%c", ecosystem[i][j].type);
        }
        printf("|   |");
        for (int j = 0; j < cols; ++j) {
            if (ecosystem[i][j].type == RABBIT || ecosystem[i][j].type == FOX) 
                printf("%d", ecosystem[i][j].gen_proc);
            else
                printf("%c", ecosystem[i][j].type);
        }
        printf("| |");
        for (int j = 0; j < cols; ++j) {
            if (ecosystem[i][j].type == FOX) 
                printf("%d", ecosystem[i][j].gen_food);
            else
                printf("%c", ecosystem[i][j].type);
        }
        if (VERBOSE) {
            printf("|     |");
            for (int j = 0; j < cols; ++j) {
                printf("%d", ecosystem[i][j].cur_gen);
            }
        }
        printf("|\n");
    }
    for (int i=0; i<rows+2; i++){
        printf("-");
    }
    printf("   ");
    for (int i=0; i<rows+2; i++){
        printf("-");
    }
    printf(" ");
    for (int i=0; i<rows+2; i++){
        printf("-");
    }
    printf("\n\n");
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
void printMatrix_output_format(int GEN_PROC_RABBITS, int GEN_PROC_FOXES, int GEN_FOOD_FOXES, int rows, int cols, Entity** ecosystem) {
    printf("%d %d %d 0 %d %d ", GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, rows, cols);
    int N = 0;
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            if (ecosystem[i][j].type == EMPTY) continue;
            N++;
        }
    }
    printf("%d\n", N);

    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            if (ecosystem[i][j].type == EMPTY) continue;
            else if (ecosystem[i][j].type == STONE) printf("ROCK ");
            else if (ecosystem[i][j].type == FOX) printf("FOX ");
            else if (ecosystem[i][j].type == RABBIT) printf("RABBIT ");
            printf("%d %d\n", i, j);
        }
    }
}

void load_ecosystem(Entity** ecosystem, int R, int C, int N) {
    for (int row=0; row<R; row++) {
        for (int col=0; col<C; col++) {
            Entity new_entity;
            empty_entity(&new_entity, 0);
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

void copy_ecosystem(Entity** original, Entity** copy, int rows, int cols) {
    for (int r=0; r<rows; r++) {
        for (int c=0; c<cols; c++) {
            Entity new_entity;
            copy_entity(original[r][c], &new_entity);
            copy[r][c] = new_entity;
        }
    }
}
void copy_entity(Entity original, Entity* copy) {
    (*copy).type     = original.type;
    (*copy).gen_proc = original.gen_proc;
    (*copy).gen_food = original.gen_food;
    (*copy).cur_gen  = original.cur_gen;
} 

int main(int argc, char *argv[]) {
    int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, rows, cols, N;

    double startTime, endTime;
    startTime = omp_get_wtime();

    read_inputs(&GEN_PROC_RABBITS, &GEN_PROC_FOXES, &GEN_FOOD_FOXES, &N_GEN, &rows, &cols, &N);

    Entity** ecosystem ;
    aloc_matrix(&ecosystem, rows, cols);
    // Load the ecosystem data into the matrix
    load_ecosystem(ecosystem, rows, cols, N);

    Entity** aux_ecosystem;
    aloc_matrix(&aux_ecosystem, rows, cols);
    copy_ecosystem(ecosystem, aux_ecosystem, rows, cols);
    for (int G=0; G<N_GEN; G++) {
        if (ALLGEN) printMatrix(ecosystem, rows, cols, G);

        // obtain next gen
        get_next_generation(&ecosystem, rows, cols, G, GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, &aux_ecosystem);
        
    }
    if (ALLGEN) printMatrix(ecosystem, rows, cols, N_GEN);


    if (!ALLGEN) printMatrix_output_format(GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, rows, cols, ecosystem);
    free(ecosystem);

    endTime = omp_get_wtime();
    printf("Execution time: %f seconds\n", endTime - startTime);

    return 0;
}


void get_next_generation(Entity*** ecosystem, int rows, int cols, int G, int GEN_PROC_RABBITS, int GEN_PROC_FOXES, int GEN_FOOD_FOXES, Entity*** aux_ecosystem) {
    // G -> Current Generation
    // MOVING RABBITS
    if (VERBOSE) printf("\n\nCHECKING MOVES FOR RABBITS\n\n");

    #pragma omp parallel for
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if ((*ecosystem)[row][col].type != RABBIT) continue;

            int* next_move = get_next_move((*ecosystem), rows, cols, row, col, G, (*aux_ecosystem));
            if (VERBOSE) printf("Rabbit from (%d, %d) -> (%d, %d) [which is a %c]\n", row, col, next_move[0], next_move[1], (*aux_ecosystem)[next_move[0]][next_move[1]].type);
            if (VERBOSE) printf("     rabbit (cur_gen, gen_prod)=(%d, %d)\n", (*ecosystem)[row][col].cur_gen, (*ecosystem)[row][col].gen_proc);

            // Check if rabbit is ready to procreate
            if ((*ecosystem)[row][col].gen_proc < GEN_PROC_RABBITS) {
                // If not, delete rabbit from its current position in the next generation (effectively moving)
                if (VERBOSE) printf("Removing rabbit from (%d, %d) in next gen\n", row, col);
                empty_entity(&((*aux_ecosystem)[row][col]), G+1);
            } 
            // If ready to procreate and rabbit moves to other pos, just reset gen_proc and move to the next position
            else if (row != next_move[0] || col != next_move[1]) {
                #pragma omp critical // Critical zone since multiple threads might attempt to update shared variables concurrently.
                {
                    (*ecosystem)[row][col].gen_proc = -1;
                    (*ecosystem)[row][col].cur_gen = G+1; 
                    (*aux_ecosystem)[row][col].gen_food = -1;
                    (*aux_ecosystem)[row][col].gen_proc = 0;
                    (*aux_ecosystem)[row][col].cur_gen = G+1;
                    (*aux_ecosystem)[row][col].type = RABBIT;
                }
                if (VERBOSE) printf("Rabbit from (%d, %d) procreating\n", row, col);
                if (VERBOSE) printf("Pos (%d, %d) in aux_eco - %c\n", row, col, (*aux_ecosystem)[row][col].type);
            }
            // If move goes to cell with something of next generation, then only a rabbit could've moved there
            if ((*aux_ecosystem)[next_move[0]][next_move[1]].cur_gen > G && (*aux_ecosystem)[next_move[0]][next_move[1]].type == RABBIT) {
                // if so, just remove current rabbit from its position and make the rabbit in the next pos have the highest gen_proc
                #pragma omp critical // Critical zone since multiple threads might attempt to update shared variables concurrently.
                {
                    (*aux_ecosystem)[next_move[0]][next_move[1]].gen_proc = max((*aux_ecosystem)[next_move[0]][next_move[1]].gen_proc, (*ecosystem)[row][col].gen_proc+1);
                }
                continue;
            }
            // If not rabbit in next_pos, then just move there and increase gen_proc and cur_gen
            #pragma omp critical // Critical zone since multiple threads might attempt to update shared variables concurrently.
            {
                (*ecosystem)[row][col].gen_proc ++;
                (*ecosystem)[row][col].cur_gen = G+1;
                copy_entity((*ecosystem)[row][col], &((*aux_ecosystem)[next_move[0]][next_move[1]]));
                if (VERBOSE) printf("Coppied entity '%c' from ecosystem[%d, %d] -> aux_ecosystem[%d, %d]", (*ecosystem)[row][col].type, row, col, next_move[0], next_move[1]);
            }
        }
    }
    
    // MOVING FOXES
    if (VERBOSE) printf("\n\nCHECKING MOVES FOR FOXES\n\n");
    #pragma omp parallel for
    for (int row=0; row<rows; row++) {
        for (int col=0; col<cols; col++) {
            // if in aux_ecosystem it has not been updated means it needs to be updated
            if ((*aux_ecosystem)[row][col].cur_gen != G+1) copy_entity((*ecosystem)[row][col], &(*aux_ecosystem)[row][col]); 
            if ((*ecosystem)[row][col].type != FOX) continue;
            int* next_move = get_next_move((*ecosystem), rows, cols, row, col, G, (*aux_ecosystem));
            if (VERBOSE) printf("Fox from (%d, %d) -> (%d, %d)\n", row, col, next_move[0], next_move[1]);

            // If fox hasn't eaten in "GEN_FOOD_FOXES" kill the fox
            if ((*ecosystem)[row][col].gen_food >= GEN_FOOD_FOXES) {
                empty_entity(&((*aux_ecosystem)[row][col]), G+1);
                if (VERBOSE) printf("Fox from (%d, %d) dies because of food\n", row, col);
                continue;
            }
            // Check if fox is ready to procriate
            if ((*ecosystem)[row][col].gen_proc < GEN_PROC_FOXES) {
                // If not, delete fox from it's current position in the next generation (effectively moving)
                empty_entity(&((*aux_ecosystem)[row][col]), G+1);
                if (VERBOSE) printf("Fox from (%d, %d) not ready to procriate\n", row, col);
            } 
             // If ready to procriate and fox moves to other pos, just reset gen_food and gen_proc and move to the next position
            else if (row != next_move[0] || col != next_move[1]) {    
                #pragma omp critical
                {
                (*aux_ecosystem)[row][col].gen_food = 0;
                (*aux_ecosystem)[row][col].gen_proc = 0;
                (*aux_ecosystem)[row][col].cur_gen = G+1;
                (*aux_ecosystem)[row][col].type = FOX;
                (*ecosystem)[row][col].gen_proc = -1;
                (*ecosystem)[row][col].cur_gen = G+1;
                if (VERBOSE) printf("Fox from (%d, %d) ready to procriate\n", row, col);
                }
            }

            // Get next position
            Entity ent_next_pos = (*aux_ecosystem)[next_move[0]][next_move[1]];

            // If next_pos is a rabbit, eat it and reset food (setting it as -1) to let other foxes know it just ate a rabbit
            if (ent_next_pos.type == RABBIT && ent_next_pos.cur_gen == G+1) {
                #pragma omp critical
                {
                    (*ecosystem)[row][col].gen_proc++;
                    (*ecosystem)[row][col].cur_gen = G + 1;
                    (*ecosystem)[row][col].gen_food = -1;
                    copy_entity((*ecosystem)[row][col], &((*aux_ecosystem)[next_move[0]][next_move[1]]));
                }
                if (VERBOSE) printf("Fox from (%d, %d) ate a rabbit\n", row, col);
                continue;
            }
            // If next_pos is a fox with next gen, compare "gen_proc" and "gen_food"
            if (ent_next_pos.type == FOX && ent_next_pos.cur_gen == G+1) {
                #pragma omp critical
                {
                    (*ecosystem)[row][col].gen_proc = max((*ecosystem)[row][col].gen_proc, ent_next_pos.gen_proc - 1);
                    (*ecosystem)[row][col].gen_food = min((*ecosystem)[row][col].gen_food, ent_next_pos.gen_food - 1);
                }
            }
            // Case where fox moves to an empty cell, need to increase everything and move to the new cell
            #pragma omp critical
            {
                (*ecosystem)[row][col].gen_proc++;
                (*ecosystem)[row][col].cur_gen = G + 1;
                (*ecosystem)[row][col].gen_food++;
            }
            // If fox hasn't eaten in "GEN_FOOD_FOXES" kill the fox
            if ((*ecosystem)[row][col].gen_food >= GEN_FOOD_FOXES) {
                empty_entity(&((*aux_ecosystem)[row][col]), G + 1);
                if (VERBOSE) printf("Fox from (%d, %d) dies because of food\n", row, col);
                continue;
            }
        
            copy_entity((*ecosystem)[row][col], &((*aux_ecosystem)[next_move[0]][next_move[1]]));
            
        }
    }

    
    // DELETING ALL PREVIOUS GENERATION THINGS
    for (int row=0; row<rows; row++) {
        for (int col=0; col<cols; col++) {
            if ((*aux_ecosystem)[row][col].type == STONE || (*aux_ecosystem)[row][col].type == EMPTY) {
                (*aux_ecosystem)[row][col].cur_gen = G+1;

            }
            if ((*aux_ecosystem)[row][col].cur_gen <= G) empty_entity(&(*aux_ecosystem)[row][col], G+1);
            if ((*aux_ecosystem)[row][col].type == FOX && (*aux_ecosystem)[row][col].gen_food == -1) (*aux_ecosystem)[row][col].gen_food = 0;
        }
    }
    
    // Swap ecosystem with "aux_ecosystem"
    Entity** tmp = (*aux_ecosystem);
    (*aux_ecosystem) = (*ecosystem);
    (*ecosystem) = tmp;
}

int* get_next_move(Entity** ecosystem, int rows, int cols, int row, int col, int G, Entity** aux_ecosystem) {
    int* validMoves = (int*)malloc(sizeof(int) * 4);
    for (int i=0; i<4; i++) {validMoves[i]=-1;}
    get_valid_moves(ecosystem, rows, cols, row, col, &validMoves, aux_ecosystem);
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

void get_valid_moves(Entity** ecosystem, int rows, int cols, int row, int col, int** validMoves, Entity** aux_ecosystem) {
    int count = 0;
    Entity currentEntity = ecosystem[row][col]; 
    if (VERBOSE)
        printf("\nAdjacent blocks for ecosystem[%d][%d] with entity '%c' ", row, col, currentEntity.type);
    if (VERBOSE) printMatrix(ecosystem, rows, cols, ecosystem[row][col].cur_gen);
    int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};


    if (VERBOSE) {
        printf("Neighbours of entity: (ecosystem | aux_ecosystem) \n");
        for (int i=-1; i<2; i++){
                for (int j=-1; j<2; j++) {
                if (row+i < 0 || row+i >= rows || col+j < 0 || col+j >= cols) {
                    printf("\\");
                    continue;
                }
                printf("%c", ecosystem[row+i][col+j].type);
            }
            printf("|");
            for (int j=-1; j<2; j++) {
                if (row+i < 0 || row+i >= rows || col+j < 0 || col+j >= cols) {
                    printf("\\");
                    continue;
                }
                printf("%c", aux_ecosystem[row+i][col+j].type);
            }
            printf("\n");
        }
    }
    Entity next_pos_entity;
    Entity next_pos_aux_entity;
    for (int dir=0; dir<4; dir++) {
        // If direction is outside ecosystem, try another direction
        if (row+directions[dir][0] < 0 || row+directions[dir][0] >= rows || col+directions[dir][1] < 0 || col+directions[dir][1] >= cols) continue;
        
        
        next_pos_entity = ecosystem[row+directions[dir][0]][col+directions[dir][1]];
        next_pos_aux_entity = aux_ecosystem[row+directions[dir][0]][col+directions[dir][1]];
        
        if (VERBOSE) printf("Checking for pos [%d, %d] = '%c' | '%c' ", row+directions[dir][0], col+directions[dir][1], next_pos_entity.type, next_pos_aux_entity.type);
        if (next_pos_entity.type == STONE) continue;

        // Checking moves for rabbits
        if (currentEntity.type == RABBIT) {
            if (next_pos_entity.type == FOX || next_pos_entity.type==RABBIT) continue;
            (*validMoves)[dir] = 1;
        } 
        else if (currentEntity.type == FOX) {
            if (next_pos_entity.type == FOX) continue;
            if (next_pos_aux_entity.type == FOX) {
                // Case when the fox on the next position ate a rabbit (current fox could also eat a rabbit) 
                if (next_pos_aux_entity.gen_food == -1 && next_pos_aux_entity.cur_gen == currentEntity.cur_gen+1 ) {
                    (*validMoves)[dir] = 2;
                }
                // Otherwise, she can't eat a rabbit
                else {
                    (*validMoves)[dir] = 1;
                }
            } else {
                if (next_pos_aux_entity.type == EMPTY) {
                    (*validMoves)[dir] = 1;
                }
                if ((next_pos_aux_entity.type == RABBIT && next_pos_aux_entity.cur_gen > currentEntity.cur_gen)) {
                    (*validMoves)[dir] = 2; // The higher the number, the higher the priority 
                } else if (next_pos_aux_entity.type == RABBIT) {
                    (*validMoves)[dir] = 1;
                }
            }
        }
        
        if (VERBOSE) printf(" VALIDITY = %d \n", (*validMoves)[dir]);

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
