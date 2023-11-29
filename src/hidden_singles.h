#pragma once

#include "sudoku.h"

typedef struct HiddenSingle_impl
{
    Cell *p_cell;
    int value;
} HiddenSingle;

int find_hidden_single_values(Cell **p_cells, int *hidden_single_values);
void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles,
                        int *p_counter);
int hidden_singles(SudokuBoard *p_board, Cell ***p_solved_cells);