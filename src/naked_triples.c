#include "naked_triples.h"

//extra funcs from utils.c
void unset_candidate(Cell *cell, int value);
int *get_candidates(Cell *cell);
bool apply_constraint(Cell **p_cells, int value);
bool is_candidate(Cell *cell, int value);

int eliminate_candidates_except_ntriples(Cell *cell, int *candidates, int size) {        //array as a pointer = no need for []
    int counter = 0;

    // Eliminate candidates from the cell except those in the given array
    for (int i = 1; i <= BOARD_SIZE; i++) {
        if (!is_candidate(cell, i)) {
            continue;               //check if it's a candidate. if false=> continue searching
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

//eliminate candidates from Cell
int eliminate_candidates_ntriples(Cell **p_cells, int index1, int index2, int index3) {
    int counter = 0;

    // Get the hidden pair candidates
    int candidates[3];      //initiate a temp array candidates to store
    candidates[0] = get_candidates(p_cells[index1])[0];
    candidates[1] = get_candidates(p_cells[index1])[1];
    candidates[2] = get_candidates(p_cells[index1])[2];


    // Eliminate the hidden pair candidates from other cells in the group
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (i != index1 && i != index2 && i != index3) {       //also eliminate in other cells in group
            counter += eliminate_candidates_except_ntriples(p_cells[i], candidates, 3);      //with some exceptions
        }
    }

    return counter;
}

int are_naked_triples(Cell *cell1, Cell *cell2, Cell *cell3) {
    // Check if two cells have the same two candidates
    if (cell1->num_candidates == 3 && cell2->num_candidates == 3 && cell3->num_candidates == 3) {     //must have 2 num_candidate (a pair)
        for (int i = 0; i < 2; i++) {
            if (!is_candidate(cell1, cell2->candidates[i]) || !is_candidate(cell1, cell3->candidates[i])) {
                return 0;       //check if ith candidate is not in cell2 and cell3 => (T): at least 1 element in cell1 not in the other 
            }                   //=> not naked triples
        }
        return 1;
    }
    return 0;
}

int check_naked_triples(Cell **p_cells) {
    int counter = 0;

    // Iterate through all pairs of cells
    for (int i = 0; i < BOARD_SIZE - 2; i++) {
        for (int j = i + 1; j < BOARD_SIZE-1; j++) {
            for (int k = j + 1; k < BOARD_SIZE; k++) {
                if (are_naked_triples(p_cells[i], p_cells[j], p_cells[k])) {
                    // Eliminate hidden pair candidates from other cells in the group
                    counter += eliminate_candidates_ntriples(p_cells, i, j, k);
                }
            }
        }
    }

    return counter;
}

int naked_triples(SudokuBoard *p_board) {
    int counter = 0;

    // Check hidden pairs in rows, columns, and boxes
    for (int i = 0; i < BOARD_SIZE; i++) {
        counter += check_naked_triples(p_board->p_rows[i]);
        counter += check_naked_triples(p_board->p_cols[i]);
        counter += check_naked_triples(p_board->p_boxes[i]);
    }

    return counter+1;
}








