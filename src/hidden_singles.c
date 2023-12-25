#include "hidden_singles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//extra funcs from utils.c
void unset_candidate(Cell *cell, int value);
int *get_candidates(Cell *cell);
bool apply_constraint(Cell **p_cells, int value);
bool is_candidate(Cell *cell, int value);
void set_candidates(Cell *cell, int *candidates, int size);


// Function to find hidden single values (the value that is unique among all the candidates for a row, a column, or a box)
int find_hidden_single_values(Cell **p_cells, int *hidden_single_values)
{
    int hidden_single_count = 0;
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

            hidden_single_values[hidden_single_count++] = i + 1;
            
        }
    }        
    return hidden_single_count;
}

// Function to find hidden single cells in a row, in a column, or in a box and apply them to the board
void check_hidden_singles(Cell **p_cells, HiddenSingle *hidden_singles, int *counter)
{
    int hidden_single_values[BOARD_SIZE];
    int no_hidden_single_values = find_hidden_single_values(p_cells, hidden_single_values);
    for (int i = 0; i < BOARD_SIZE; i++)   //iterate through hidden single value from 0-8
    {
        for (int j = 0; j < no_hidden_single_values; j++)        //iterate through cell
        {
            
            if (is_candidate(p_cells[i], hidden_single_values[j]))      //if is a candidate, then change the value (is the hidden single)
            {
                // Apply the hidden single directly to the cell
                hidden_singles[*counter].p_cell = p_cells[i];
                hidden_singles[*counter].value = hidden_single_values[j];
                (*counter)++;
                break;
            }
        }
    }
    //counter +=no_hidden_single_values; 
}

int exclude_overlap(HiddenSingle *hidden_singles, int counter){
    int overlap =0;
    for(int i=0;i<counter;i++){
        int is_overlap = 1;
        Cell *p_cell = hidden_singles[i].p_cell;
        for(int j =0; j<=8; j++){
            if((p_cell->candidates[j]) && (j+1 != hidden_singles[i].value)){
                unset_candidate(p_cell,j+1);
                is_overlap=0;
            }
        }
        overlap+=is_overlap;
    }
    return counter - overlap;
}


// Function to find hidden single cells in a board and apply them
int hidden_singles(SudokuBoard *p_board)
{   
    HiddenSingle hidden_singles_store [BOARD_SIZE * BOARD_SIZE];

    
    int counter = 0;        //counter is a counter for the number of candidate eliminations performed during the hidden pairs strategy.
                            //helps to quantify the effectiveness of the strategy in reducing the number of possible candidates in the Sudoku puzzle.
    // Check rows, cols, boxes
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        check_hidden_singles(p_board->p_rows[i], hidden_singles_store, &counter);
        check_hidden_singles(p_board->p_cols[i], hidden_singles_store, &counter);
        check_hidden_singles(p_board->p_boxes[i], hidden_singles_store, &counter);
    }

    counter = exclude_overlap(hidden_singles_store,counter);
    
    return counter;
}

