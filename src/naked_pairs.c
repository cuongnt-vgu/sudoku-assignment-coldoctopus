#include "naked_pairs.h"
#include <string.h>
#include <stdlib.h>


//extra funcs from utils.c
void unset_candidate(Cell *cell, int value);
int *get_candidates(Cell *cell);
bool apply_constraint(Cell **p_cells, int value);
bool is_candidate(Cell *cell, int value);

int naked_pairs_exists(NakedPairs *pairs, int counter, int value_1, int value_2) {
    for (int i = 0; i < counter; i++) {
        if ((pairs[i].value_1 == value_1 && pairs[i].value_2 == value_2) || (pairs[i].value_1 == value_2 && pairs[i].value_2 == value_1)) {
            return 1;
        }
    }
    return 0;
}

int naked_pairs(SudokuBoard *p_board) {
    NakedPairs pairs[BOARD_SIZE * BOARD_SIZE];
    int counter = 0;

    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            Cell *cell = &(p_board->data[i][j]);
            if (cell->num_candidates != 2)
                continue;

            int *candidates = get_candidates(cell);


            // Check each rows, columns, and boxes
            for (int iter_type = 0; iter_type < 3; iter_type++) 
            {
                Cell **unit;
                switch (iter_type) {
                    case 0:
                        unit = p_board->p_rows[i];
                        break;
                    case 1:
                        unit = p_board->p_cols[j];
                        break;
                    case 2:
                        unit = p_board->p_boxes[(i / 3) * 3 + j / 3];
                        break;
                }

                for (int k = 0; k < BOARD_SIZE; k++) 
                {
                    if (unit[k] == cell)
                        continue;

                    Cell *other = unit[k];
                    if (other->num_candidates != 2)
                        continue;

                    int *other_candidates = get_candidates(other);
                    if (candidates[0] == other_candidates[0] && candidates[1] == other_candidates[1]) 
                    {
                        if (!naked_pairs_exists(pairs, counter, candidates[0], candidates[1])) 
                        {
                            pairs[counter].value_1 = candidates[0];
                            pairs[counter].value_2 = candidates[1];
                            counter++;
                        }
                        for (int l = 0; l < BOARD_SIZE; l++) 
                        {
                            if (unit[l] == cell || unit[l] == other)
                                continue;

                            Cell *remove = unit[l];
                            if (is_candidate(remove, candidates[0])) 
                            {
                                unset_candidate(remove, candidates[0]);
                            }
                            if (is_candidate(remove, candidates[1])) 
                            {
                                unset_candidate(remove, candidates[1]);
                            }
                        }
                    }
                    free(other_candidates);
                }
            }
            free(candidates);
        }
    }
    return counter;
}
