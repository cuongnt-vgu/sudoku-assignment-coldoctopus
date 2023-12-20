#include "naked_pairs.h"
#include <string.h>

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

void find_naked_pairs_values(Cell **p_cells, int *naked_pairs_values)
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
        }
    }

    //return counter;
}
/*
void eliminate_candidates_npairs(Cell **p_cells, int index1, int index2) {
    //int counter = 0;

    // Get the naked pair candidates
    int candidates[2];      //initiate a temp array candidates to store
    candidates[0] = get_candidates(p_cells[index1])[0];
    candidates[1] = get_candidates(p_cells[index1])[1];

    // Eliminate the naked pair candidates from other cells in the group
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (i != index1 && i != index2) {       //also eliminate in other cells in group
            unset_candidate(p_cells,candidates[0]);
            unset_candidate(p_cells,candidates[1]);
        }
    }

    //return counter;
}
*/

int are_naked_pairs(Cell *cell1, Cell *cell2) {
    // Check if two cells have the same two candidates
    if (cell1->num_candidates == 2 && cell2->num_candidates == 2 && memcmp(cell1->candidates, cell2->candidates, BOARD_SIZE)){
        return 1;                       //must have 2 num_candidate (a pair) and the array is identical (memcmp)
    }
    return 0;
}

int check_naked_pairs(Cell **p_cells) {
    int counter = 0;
    int naked_pairs_values[BOARD_SIZE];
    find_naked_pairs_values(p_cells, naked_pairs_values);
    // Iterate through all pairs of cells
    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        for (int j = i + 1; j < BOARD_SIZE; j++) {
            if (are_naked_pairs(p_cells[i], p_cells[j])) {
                // Eliminate hidden pair candidates from other cells in the group
                for(int k=0;k<BOARD_SIZE;k++){
                    if(k!=i && k!=j){
                        unset_candidate(p_cells[k],naked_pairs_values[0]);
                        unset_candidate(p_cells[k],naked_pairs_values[1]);
                    }
                }
                //counter += eliminate_candidates_npairs(p_cells, i, j);

                counter++;
            }
        }
    }

    return counter;
}



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

/*
void eliminate_candidates_npairs(Cell **p_cells, int *naked_pairs_values, int num_naked_pairs) {
    for (int i = 0; i < num_naked_pairs; i++) {
        int value1 = naked_pairs_values[2 * i];
        int value2 = naked_pairs_values[2 * i + 1];

        for (int j = 0; j < BOARD_SIZE; j++) {
            // Skip cells that are part of the Naked Pairs
            if (is_candidate(p_cells[j], value1) || is_candidate(p_cells[j], value2)) {
                continue;
            }

            // Eliminate candidates in other cells in the unit (row, column, or box)
            unset_candidate(p_cells[j], value1);
            unset_candidate(p_cells[j], value2);
        }
    }
}

// Function to find naked pairs in a row, in a column, or in a box
int find_naked_pairs(Cell **p_cells, int *naked_pairs_values)
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
        }
    }

    return counter;
}

// Function to eliminate naked pairs from a row, in a column, or in a box
int check_naked_pairs(Cell **p_cells, int *naked_pairs_values)
{
    int counter = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        
        // Skip cells with more than two candidates
        if (p_cells[i]->num_candidates > 2)
        {
            continue;
        }
        
        // Eliminate candidates in the cell if they match the naked pair
        for (int k = 0; k < 2; k++)
        {
            if (is_candidate(p_cells[i], naked_pairs_values[k]))
            {
                // Pass the individual cell to eliminate_candidates_npairs
                //eliminate_candidates_npairs(p_cells[i], naked_pairs_values, 1);
                unset_candidate(p_cells[i], naked_pairs_values[k]);
                counter++;
            }
        }
    }

    return counter;
}

// Function to check naked pairs in a board and apply them
int naked_pairs(SudokuBoard *p_board)
{
    int counter = 0; // Counter for the number of candidate eliminations

    
    // Check rows, cols, boxes
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int naked_pairs_values[9];
        int num_naked_pairs = find_naked_pairs(p_board->p_rows[i], naked_pairs_values);

        if (num_naked_pairs > 0)
        {
            // Eliminate naked pairs from the row
            counter += check_naked_pairs(p_board->p_rows[i], naked_pairs_values);
        }

        num_naked_pairs = find_naked_pairs(p_board->p_cols[i], naked_pairs_values);

        if (num_naked_pairs > 0)
        {
            // Eliminate naked pairs from the column
            counter += check_naked_pairs(p_board->p_cols[i], naked_pairs_values);
        }

        num_naked_pairs = find_naked_pairs(p_board->p_boxes[i], naked_pairs_values);

        if (num_naked_pairs > 0)
        {
            // Eliminate naked pairs from the box
            counter += check_naked_pairs(p_board->p_boxes[i], naked_pairs_values);
        }
    }

    return counter;
}
*/

