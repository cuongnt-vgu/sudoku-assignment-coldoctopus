#include "naked_pairs.h"
#include <string.h>
#include <stdlib.h>

//extra funcs from utils.c
void unset_candidate(Cell *cell, int value);
int *get_candidates(Cell *cell);
bool apply_constraint(Cell **p_cells, int value);
bool is_candidate(Cell *cell, int value);

/*
int eliminate_candidates_except_npairs(Cell *cell, int *candidates, int size) {        //array as a pointer = no need for []
    int counter = 0;

    // Eliminate candidates from the cell except those in the given array
    for (int i = 1; i <= BOARD_SIZE; i++) {
        if (!is_candidate(cell, i)) {
            continue;               //check if it's a candidate. false=> continue searching
        }

        int found = 0;
        for (int j = 0; j < size; j++) {
            if (i == candidates[j]) {
                found = 1;         //check if the current value i is in the array
                break;
            }
        }

        if (!found) {                          // the loop goes throughh all possible value. If value is a candidate but not in array
            unset_candidate(cell, i);          //=>value not allow in the cell => unset them (cuz they're no longer avaiable)
            counter++;
        }
    }

    return counter;
}
*/
/*
int check_naked_pairs(Cell **p_cells, int *naked_pairs_values)
{
    int counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        // Skip cells with more than two candidates
        if (p_cells[i]->num_candidates != 2)
        {
            continue;
        }

        for (int j = i + 1; j < BOARD_SIZE; j++)
        {
            // Skip cells with more than two candidates
            if (p_cells[j]->num_candidates != 2)
            {
                continue;
            }

            // Check if the candidates are the same in both cells
            
            if (p_cells[i]->candidates[0] == p_cells[j]->candidates[0] &&
                p_cells[i]->candidates[1] == p_cells[j]->candidates[1])
            {
                // Naked pair found, add candidates to the naked_pairs_values array
                naked_pairs_values[counter++] = p_cells[i]->candidates[0];
                naked_pairs_values[counter++] = p_cells[i]->candidates[1];
            }
            if(p_cells[i]->num_candidates == 2 && p_cells[i]->num_candidates == 2 && memcmp(p_cells[i]->candidates, p_cells[j]->candidates, BOARD_SIZE)){
                naked_pairs_values[counter++] = p_cells[i]->candidates[0];
                naked_pairs_values[counter++] = p_cells[i]->candidates[1];
            }
        }
    }

    return counter;
}
*/

/*
int are_naked_pairs(Cell *cell1, Cell *cell2) {
    // Check if two cells have the same two candidates
    if (cell1->num_candidates == 2 && cell2->num_candidates == 2 && memcmp(cell1->candidates, cell2->candidates, BOARD_SIZE)){
        return 1;                       //must have 2 num_candidate (a pair) and the array is identical (memcmp)
    }
    return 0;
}

int check_naked_pairs(Cell **p_cells) {
    int counter = 0;
    //int naked_pairs_values[BOARD_SIZE];
    //int candidates[2] = {0};

    //int num_naked_pairs = check_naked_pairs(p_cells, naked_pairs_values);
    // Iterate through all pairs of cells
    for (int i = 0; i < BOARD_SIZE - 1; i++) {

        for (int j = i + 1; j < BOARD_SIZE; j++) {
            if(p_cells[j]->num_candidates>1 || p_cells[j]->num_candidates>1){
                if (are_naked_pairs(p_cells[i], p_cells[j])) {
                // Eliminate hidden pair candidates from other cells in the group
                
                
                //initiate a temp array candidates to store
                int *temp = malloc(2*sizeof(int));
                int *cands = get_candidates(p_cells[i]);
                temp[0] = cands[0];     //pair value no1
                temp[1] = cands[1];     //pair value no2
                free(cands);
                //set_candidates(p_cells[i],temp,2);
                //set_candidates(p_cells[j],temp,2);
                
                for (int k = 0; k < BOARD_SIZE; k++){
                    if(k!= i && k!= j && is_candidate(p_cells[k],temp[0])){
                        unset_candidate(p_cells[k],temp[0]);
                    }
                    if(k!= i && k!= j && is_candidate(p_cells[k],temp[1])){
                        unset_candidate(p_cells[k],temp[1]);
                    }
                }
                p_cells[i]->fixed=true;
                p_cells[j]->fixed=true;
                free(temp);
                //eliminate_candidates_npairs(p_cells, i, j);
                
                //implement the new function here!
                counter++;
            }
            
            }
        }
    }

    return counter;
}

int discard_overlapped_pairs(NakedPair *naked_pairs, int counter) {
    int overlap = 0;

    for (int i = 0; i < counter; i++) {
        int is_overlap = 0;

        for (int j = i + 1; j < counter; j++) {
            if (memcmp(naked_pairs[i].candidates, naked_pairs[j].candidates, 2 * sizeof(int)) == 0) {
                // Overlapping pair found
                is_overlap = 1;
                break;
            }
        }

        if (!is_overlap) {
            // No overlap, keep the pair
            naked_pairs[overlap] = naked_pairs[i];
            overlap++;
        }
    }

    return counter - overlap;
}


int naked_pairs(SudokuBoard *p_board) {
    int counter = 0;

    // Check hidden pairs in rows, columns, and boxes
    for (int i = 0; i < BOARD_SIZE; i++) {
        counter += check_naked_pairs(p_board->p_rows[i]);
        counter += check_naked_pairs(p_board->p_cols[i]);
        counter += check_naked_pairs(p_board->p_boxes[i]);
    }

    //call if u want here
    return counter;
}
*/

int are_naked_pairs(Cell *cell1, Cell *cell2) {
    // Check if two cells have the same two candidates
    if (cell1->num_candidates == 2 && cell2->num_candidates == 2 &&
        memcmp(cell1->candidates, cell2->candidates, 2 * sizeof(int)) == 0) {
        return 1;  // The cells form a naked pair
    }
    return 0;  // The cells do not form a naked pair
}


void check_naked_pairs(Cell **p_cells, NakedPair *naked_pairs, int *counter) {
    for (int i = 0; i < BOARD_SIZE - 1; i++) {
            for (int j = i + 1; j < BOARD_SIZE; j++) {
                if (p_cells[i]->num_candidates == 2 && p_cells[j]->num_candidates == 2) {
                    
                    int *can1 = get_candidates(p_cells[i]);
                    int *can2 = get_candidates(p_cells[j]);

                    if (can1[0] == can2[0] &&can1[1] == can2[1]) 
                    {
                        int check=0;
                        for(int c=0;c<BOARD_SIZE;c++){
                            int *can_test = get_candidates(p_cells[c]);
                            for(int k=0;k<can_test;k++)
                            {
                                if(can_test[k]==can1[0] || can_test[k]==can1[0])  //only need can1 or can2 (cuz the same)
                                {
                                    check++;
                                }
                            }
                            if(check==1){
                                // Naked pair found, add information to the naked_pairs array
                                naked_pairs[*counter].p_cells[0] = p_cells[i];
                                naked_pairs[*counter].p_cells[1] = p_cells[j];

                                naked_pairs[*counter].values[0] = p_cells[i]->candidates[0];
                                naked_pairs[*counter].values[1] = p_cells[i]->candidates[1];
                                (*counter)++;
                            }
                        }
                    }
                    free(can1);
                    free(can2);
                    
                }
            }
    }
}

int exclude_overlap_npairs(NakedPair *naked_pairs, int counter) {
    int overlap =0;
    for(int i=0;i<counter;i++){
        int is_overlap = 1;
        Cell *p_cell1 = naked_pairs[i].p_cells[0];
        Cell *p_cell2 = naked_pairs[i].p_cells[1];
        for(int j =0; j< BOARD_SIZE; j++){
            if((p_cell1->candidates[j]) && (j+1 != naked_pairs[i].values[0]))
                {
                unset_candidate(p_cell1,j+1);
                
                is_overlap=0;
            }
            if((p_cell2->candidates[j]) && (j+1 != naked_pairs[i].values[0]))
                {
                unset_candidate(p_cell2,j+1);
                is_overlap=0;
            }
        }
        overlap+=is_overlap;
    }
    return counter - overlap;
}



int naked_pairs(SudokuBoard *p_board) {
    NakedPair naked_pairs_store[BOARD_SIZE * BOARD_SIZE];
    int counter = 0;

    // Check rows, cols, boxes
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        check_naked_pairs(p_board->p_rows[i], naked_pairs_store, &counter);
        check_naked_pairs(p_board->p_cols[i], naked_pairs_store, &counter);
        check_naked_pairs(p_board->p_boxes[i], naked_pairs_store, &counter);
    }

    //counter = exclude_overlap_npairs(naked_pairs_store,counter);
    return counter;
}
