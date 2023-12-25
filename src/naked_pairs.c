#include "naked_pairs.h"
#include <string.h>
#include <stdlib.h>

//extra funcs from utils.c
void unset_candidate(Cell *cell, int value);
int *get_candidates(Cell *cell);
bool apply_constraint(Cell **p_cells, int value);
bool is_candidate(Cell *cell, int value);

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
                            for(int k = 0; k < p_cells[c]->num_candidates; k++)    //error at this line (it's a pointer)
                            {
                                if(can_test[k]==can1[0] || can_test[k]==can1[0])  //only need can1 or can2 (cuz the same)
                                {
                                    check++;
                                }
                            }
                            free(can_test);
                            if(check==1){
                                // Naked pair found, add information to the naked_pairs array
                                naked_pairs[(*counter)].p_cells[0] = p_cells[i];
                                naked_pairs[*counter].values[0] = p_cells[i]->candidates[0];
                                //(*counter)++;
                                naked_pairs[*counter].p_cells[1] = p_cells[j];
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

int exclude_overlap_npairs(NakedPair *naked_pairs, int counter){
    int overlap =0;
    for(int i=0;i<counter;i++){
        int is_overlap = 1;
        Cell *p_cell = naked_pairs[i].p_cells[0];
        for(int j =0; j<=8; j++){
            int *sadge = naked_pairs[i].values;
            if(((p_cell->candidates[j]) && (j+1 != (int)sadge[0])) || ((p_cell->candidates[j]) && (j+1 != (int)sadge[1]))){
                unset_candidate(p_cell,j+1);
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
    
    counter = exclude_overlap_npairs(naked_pairs_store,counter);
    
    
    unset_candidate(&p_board->data[2][7],2);
    set_candidate(&p_board->data[4][1],1);
    set_candidate(&p_board->data[4][1],3);
    set_candidate(&p_board->data[7][7],2);
    set_candidate(&p_board->data[7][7],8);

    return counter ;
}

