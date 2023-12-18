/*#include "naked_pairs.h"

int naked_pairs(SudokuBoard *p_board)
{
    return 0;
}
*/
// naked_pairs.c

#include "naked_pairs.h"

int naked_pairs(SudokuBoard *p_board) {
    int counter = 0;

    // Check hidden pairs in rows, columns, and boxes
    for (int i = 0; i < BOARD_SIZE; i++) {
        counter += check_naked_pairs(p_board->p_rows[i]);
        counter += check_naked_pairs(p_board->p_cols[i]);
        counter += check_naked_pairs(p_board->p_boxes[i]);
    }

    return counter;
}

int check_naked_pairs(Cell **p_cells) {
    int counter = 0;

    // Iterate through all pairs of cells
    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        for (int j = i + 1; j < BOARD_SIZE; j++) {
            if (are_naked_pairs(p_cells[i], p_cells[j])) {
                // Eliminate hidden pair candidates from other cells in the group
                counter += eliminate_candidates(p_cells, i, j);
            }
        }
    }

    return counter;
}

int are_naked_pairs(Cell *cell1, Cell *cell2) {
    // Check if two cells have the same two candidates
    if (cell1->num_candidates == 2 && cell2->num_candidates == 2 && memcmp(cell1->candidates, cell2->candidates, BOARD_SIZE)){
        return 1;                       //must have 2 num_candidate (a pair) and the array is identical (memcmp)
    }
    return 0;
}

int eliminate_candidates(Cell **p_cells, int index1, int index2) {
    int counter = 0;

    // Get the hidden pair candidates
    int candidates[2];      //initiate a temp array candidates to store
    candidates[0] = get_candidates(p_cells[index1])[0];
    candidates[1] = get_candidates(p_cells[index1])[1];

    // Eliminate the hidden pair candidates from other cells in the group
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (i != index1 && i != index2) {       //also eliminate in other cells in group
            counter += eliminate_candidates_except(p_cells[i], candidates, 2);      //with some exceptions
        }
    }

    return counter;
}

int eliminate_candidates_except(Cell *cell, int *candidates, int size) {        //array as a pointer = no need for []
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