#pragma once

#include "sudoku.h"

typedef struct {
    Cell *p_cells[2];
    //int values[2];
    int value_1;
    int value_2;
} NakedPairs;

int naked_pairs(SudokuBoard *p_board);