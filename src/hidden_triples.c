#include "hidden_triples.h"

int hidden_pairs(SudokuBoard *p_board) {
    int counter = 0;

    // Check hidden pairs in rows, columns, and boxes
    for (int i = 0; i < BOARD_SIZE; i++) {
        counter += check_hidden_pairs(p_board->p_rows[i]);
        counter += check_hidden_pairs(p_board->p_cols[i]);
        counter += check_hidden_pairs(p_board->p_boxes[i]);
    }

    return counter;
}

int check_hidden_triples(Cell **p_cells) {
    int counter = 0;

    // Iterate through all pairs of cells
    for (int i = 0; i < BOARD_SIZE - 2; i++) {
        for (int j = i + 1; j < BOARD_SIZE-1; j++) {
            for (int k = j + 1; k < BOARD_SIZE; k++) {
                if (are_hidden_triples(p_cells[i], p_cells[j], p_cells[k])) {
                    // Eliminate hidden pair candidates from other cells in the group
                    counter += eliminate_candidates(p_cells, i, j, k);
                }
            }
        }
    }

    return counter;
}

int are_hidden_triples(Cell *cell1, Cell *cell2, Cell *cell3) {
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

//eliminate candidates from Cell
int eliminate_candidates(Cell **p_cells, int index1, int index2, int index3) {
    int counter = 0;

    // Get the hidden pair candidates
    int candidates[3];      //initiate a temp array candidates to store
    candidates[0] = get_candidates(p_cells[index1])[0];
    candidates[1] = get_candidates(p_cells[index1])[1];
    candidates[2] = get_candidates(p_cells[index1])[2];


    // Eliminate the hidden pair candidates from other cells in the group
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (i != index1 && i != index2 && i != index3) {       //also eliminate in other cells in group
            counter += eliminate_candidates_except(p_cells[i], candidates, 3);      //with some exceptions
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









/*#include "hidden_triples.h"

int hidden_triples(SudokuBoard *p_board)
{
    return 0;
}
////////////////////////////////////////////////////// new version
#include "hidden_triples.h"

int hidden_triples(SudokuBoard *p_board) {
    int counter = 0;

    // Iterate through each box, row, and column
    for (int index = 0; index < BOARD_SIZE; index++) {
        counter += hidden_triples_in_group(p_board->p_cols[index]);
        counter += hidden_triples_in_group(p_board->p_rows[index]);
        counter += hidden_triples_in_group(p_board->p_boxes[index]);
    }

    return counter;
}

int hidden_triples_in_group(Cell **group) {
    int counter = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        // Get candidates in the current cell
        int *candidates_i = get_candidates(group[i]);

        for (int j = i + 1; j < BOARD_SIZE; j++) {
            // Get candidates in the next cell
            int *candidates_j = get_candidates(group[j]);

            for (int k = j + 1; k < BOARD_SIZE; k++) {
                // Get candidates in the third cell
                int *candidates_k = get_candidates(group[k]);

                // Check if there are exactly 3 candidates in each cell
                if (count_candidates(candidates_i) == 3 && count_candidates(candidates_j) == 3 &&
                    count_candidates(candidates_k) == 3) {
                    // Check if the candidates are the same in all three cells
                    if (are_same_candidates(candidates_i, candidates_j, candidates_k)) {
                        // Remove these candidates from other cells in the group
                        counter += eliminate_hidden_triples(group, i, j, k, candidates_i);
                    }
                }

                // Free memory
                free(candidates_k);
            }

            free(candidates_j);
        }

        free(candidates_i);
    }

    return counter;
}

int eliminate_hidden_triples(Cell **group, int i, int j, int k, int *candidates) {
    int counter = 0;

    for (int l = 0; l < BOARD_SIZE; l++) {
        if (l != i && l != j && l != k) {
            // Get candidates in other cells in the group
            int *other_candidates = get_candidates(group[l]);

            // Check if there are any common candidates to eliminate
            if (has_common_candidates(other_candidates, candidates)) {
                // Eliminate candidates from the other cell
                counter += eliminate_candidates_from_cell(group[l], candidates);

                // Print the elimination step (for debugging)
                // printf("Eliminated candidates %d%d%d from (%d, %d)\n",
                //        candidates[0], candidates[1], candidates[2],
                //        group[l]->row_index, group[l]->col_index);
            }

            // Free memory
            free(other_candidates);
        }
    }

    return counter;
}

int eliminate_candidates_from_cell(Cell *cell, int *candidates) {
    int counter = 0;

    for (int i = 0; i < count_candidates(candidates); i++) {
        // Check if the candidate is present before elimination
        if (is_candidate(cell, candidates[i])) {
            // Eliminate the candidate from the cell
            unset_candidate(cell, candidates[i]);
            counter++;

            // If there's only one candidate left, set it as the value
            if (cell->num_candidates == 1) {
                cell->value = get_candidates(cell)[0];
                counter++;
            }
        }
    }

    return counter;
}

int has_common_candidates(int *candidates1, int *candidates2) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (candidates1[i] && candidates2[i]) {
            return 1; // True, common candidates found
        }
    }
    return 0; // False, no common candidates
}

int are_same_candidates(int *candidates1, int *candidates2, int *candidates3) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (candidates1[i] != (candidates2[i] && candidates3[i])) {
            return 0; // False, candidates are not the same
        }
    }
    return 1; // True, candidates are the same
}

int count_candidates(int *candidates) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        count += candidates[i];
    }
    return count;
}

////////////////////////////////////////////////////// new version
#include "hidden_triples.h"

int hidden_triples(SudokuBoard *p_board) {
    int counter = 0;

    // Iterate through each box, row, and column
    for (int index = 0; index < BOARD_SIZE; index++) {
        counter += hidden_triples_in_group(p_board, p_board->p_cols, index);
        counter += hidden_triples_in_group(p_board, p_board->p_rows, index);
        counter += hidden_triples_in_group(p_board, p_board->p_boxes, index);
    }

    return counter;
}

int hidden_triples_in_group(SudokuBoard *p_board, Cell ***groups, int group_index) {
    int counter = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        // Get candidates in the current cell
        int *candidates_i = get_candidates(groups[group_index][i]);

        for (int j = i + 1; j < BOARD_SIZE; j++) {
            // Get candidates in the next cell
            int *candidates_j = get_candidates(groups[group_index][j]);

            for (int k = j + 1; k < BOARD_SIZE; k++) {
                // Get candidates in the third cell
                int *candidates_k = get_candidates(groups[group_index][k]);

                // Check if there are exactly 3 candidates in each cell
                if (count_candidates(candidates_i) == 3 && count_candidates(candidates_j) == 3 &&
                    count_candidates(candidates_k) == 3) {
                    // Check if the candidates are the same in all three cells
                    if (are_same_candidates(candidates_i, candidates_j, candidates_k)) {
                        // Remove these candidates from other cells in the group
                        counter += eliminate_hidden_triples(p_board, groups, group_index, i, j, k, candidates_i);
                    }
                }

                // Free memory
                free(candidates_k);
            }

            free(candidates_j);
        }

        free(candidates_i);
    }

    return counter;
}

int eliminate_hidden_triples(SudokuBoard *p_board, Cell ***groups, int group_index, int i, int j, int k, int *candidates) {
    int counter = 0;

    for (int l = 0; l < BOARD_SIZE; l++) {
        if (l != i && l != j && l != k) {
            // Get candidates in other cells in the group
            int *other_candidates = get_candidates(groups[group_index][l]);

            // Check if there are any common candidates to eliminate
            if (has_common_candidates(other_candidates, candidates)) {
                // Eliminate candidates from the other cell
                counter += eliminate_candidates_from_cell(p_board, groups[group_index][l], candidates);

                // Print the elimination step (for debugging)
                // printf("Eliminated candidates %d%d%d from (%d, %d)\n",
                //        candidates[0], candidates[1], candidates[2],
                //        groups[group_index][l]->row_index, groups[group_index][l]->col_index);
            }

            // Free memory
            free(other_candidates);
        }
    }

    return counter;
}

int eliminate_candidates_from_cell(SudokuBoard *p_board, Cell *cell, int *candidates) {
    int counter = 0;

    for (int i = 0; i < count_candidates(candidates); i++) {
        // Check if the candidate is present before elimination
        if (is_candidate(cell, candidates[i])) {
            // Eliminate the candidate from the cell
            unset_candidate(cell, candidates[i]);
            counter++;

            // If there's only one candidate left, set it as the value
            if (cell->num_candidates == 1) {
                cell->value = get_candidates(cell)[0];
                counter++;
            }
        }
    }

    return counter;
}

// The rest of the functions remain unchanged
// ...

////////////////////////////////////////////////////// new version


#include "hidden_triples.h"
#include "utils.h"

int hidden_triples(SudokuBoard *p_board)
{
    int num_detects = 0;

    for (int group = 0; group < BOARD_SIZE; group++)
    {
        for (int candidate = 1; candidate <= BOARD_SIZE; candidate++)
        {
            // Find cells with the candidate in the group
            Cell **candidate_cells = find_candidate_cells(p_board->p_rows[group], BOARD_SIZE, candidate);

            // If there are at least three cells, check for hidden triples
            if (count_candidates(candidate_cells) >= 3)
            {
                // Iterate through all combinations of three cells
                for (int i = 0; i < BOARD_SIZE - 2; i++)
                {
                    for (int j = i + 1; j < BOARD_SIZE - 1; j++)
                    {
                        for (int k = j + 1; k < BOARD_SIZE; k++)
                        {
                            Cell *cell1 = candidate_cells[i];
                            Cell *cell2 = candidate_cells[j];
                            Cell *cell3 = candidate_cells[k];

                            // Check if the three cells form a hidden triple
                            if (are_hidden_triples(cell1, cell2, cell3))
                            {
                                // Remove the candidates from other cells in the group
                                num_detects += eliminate_candidates_hidden_triples(p_board->p_rows[group], cell1, cell2, cell3, candidate);
                                num_detects += eliminate_candidates_hidden_triples(p_board->p_cols[group], cell1, cell2, cell3, candidate);
                                num_detects += eliminate_candidates_hidden_triples(p_board->p_boxes[group], cell1, cell2, cell3, candidate);
                            }
                        }
                    }
                }
            }

            // Free allocated memory for candidate_cells
            free(candidate_cells);
        }
    }

    return num_detects;
}*/