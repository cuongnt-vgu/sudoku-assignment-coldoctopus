#include "hidden_singles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//extra funcs from utils.c
void unset_candidate(Cell *cell, int value);
int *get_candidates(Cell *cell);
bool apply_constraint(Cell **p_cells, int value);
bool is_candidate(Cell *cell, int value);

// Function to find hidden single values (the value that is unique among all the candidates for a row, a column, or a box)
int find_hidden_single_values(Cell **p_cells, int *hidden_single_values)
{
    int counter = 0;
    int counts_candidate[BOARD_SIZE] = {0};
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (p_cells[i]->num_candidates > 1)
        {
            int *candidates = get_candidates(p_cells[i]);
            for (int j = 0; j < p_cells[i]->num_candidates; j++)
            {
                counts_candidate[candidates[j]-1]+=1;
            }
            free(candidates);
        }
    }
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (counts_candidate[i] == 1)
        {
            hidden_single_values[counter++] = i + 1;
        }
    }        
    return counter;
}


// Function to apply hidden single to a cell
void eliminate_candidates_hsingles(Cell *p_cell, int value)
{
    p_cell->value = value;
    //p_cell->num_candidates = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if(i!=value){
            p_cell->candidates[i] = 0;  // Set each element to 0 or any default value
        }
        
    }
    
}

// Function to find hidden single cells in a row, in a column, or in a box and apply them to the board
int check_hidden_singles(Cell **p_cells)
{
    int counter=0;
    int hidden_single_values[BOARD_SIZE];
    int no_hidden_single_values = find_hidden_single_values(p_cells, hidden_single_values);
    for (int i = 0; i < no_hidden_single_values; i++)   //iterate through hidden single value from 0-8
    {
        /*
        if(apply_constraint(p_cells,hidden_single_values[i])){
            counter++;
        }
        */
        for (int j = 0; j < BOARD_SIZE; j++)        //iterate through cell
        {
            // Skip cells that already have a number assigned
            if (p_cells[j]->value != 0) {
                continue;
            }

            if (is_candidate(p_cells[j], hidden_single_values[i]))      //if is a candidate, then change the value (is the hidden single)
            {
                // Apply the hidden single directly to the cell
                eliminate_candidates_hsingles(p_cells[j], hidden_single_values[i]);
                counter++;

                break;
            }
        }
    }
    return counter;
}

// Function to find hidden single cells in a board and apply them
int hidden_singles(SudokuBoard *p_board)
{
    int counter = 0;        //counter is a counter for the number of candidate eliminations performed during the hidden pairs strategy.
                            //helps to quantify the effectiveness of the strategy in reducing the number of possible candidates in the Sudoku puzzle.
    // Check rows, cols, boxes
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        counter+=check_hidden_singles(p_board->p_rows[i]);
        counter+=check_hidden_singles(p_board->p_cols[i]);
        counter+=check_hidden_singles(p_board->p_boxes[i]);
    }

    return counter;
}

