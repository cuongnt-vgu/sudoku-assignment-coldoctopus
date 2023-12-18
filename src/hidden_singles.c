#include "hidden_singles.h"
// Function to find hidden single cells in a board and apply them
int hidden_singles(SudokuBoard *p_board)
{
    int counter = 0;        //counter is a counter for the number of candidate eliminations performed during the hidden pairs strategy.
                            //helps to quantify the effectiveness of the strategy in reducing the number of possible candidates in the Sudoku puzzle.
    // Check rows, cols, boxes
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        check_hidden_singles(p_board->p_rows[i]);
        check_hidden_singles(p_board->p_cols[i]);
        check_hidden_singles(p_board->p_boxes[i]);
    }

    return counter;
}

// Function to find hidden single cells in a row, in a column, or in a box and apply them to the board
void check_hidden_singles(Cell **p_cells)
{
    int hidden_single_values[BOARD_SIZE];
    int no_hidden_single_values = find_hidden_single_values(p_cells, hidden_single_values);

    for (int i = 0; i < no_hidden_single_values; i++)   //iterate through hidden single value from 0-8
    {
        for (int j = 0; j < BOARD_SIZE; j++)        //iterate through cell
        {
            if (is_candidate(p_cells[j], hidden_single_values[i]))      //if is a candidate, then change the value (is the hidden single)
            {
                // Apply the hidden single directly to the cell
                eliminate_candidates(p_cells[j], hidden_single_values[i]);
                break;
            }
        }
    }
}

// Function to apply hidden single to a cell
void eliminate_candidates(Cell *p_cell, int value)
{
    p_cell->value = value;
    p_cell->num_candidates = 0;
    free(p_cell->candidates);
    for (int i = 0; i < BOARD_SIZE; i++) {
        p_cell->candidates[i] = 0;  // Set each element to 0 or any default value
    }
}

// Function to find hidden single values (the value that is unique among all the candidates for a row, a column, or a box)
int find_hidden_single_values(Cell **p_cells, int *hidden_single_values)
{
    int counter = 0;
    int count_candidates[BOARD_SIZE +1] = {0}; // Initialize to 0

    // Count the occurrences of each candidate in the group
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int *candidates = get_candidates(p_cells[i]);       //find candidate of each cells
        for (int j = 0; j < p_cells[i]->num_candidates; j++)        //count candidate (like a histogram of candidate 0-1, 1-2, 2-0,...)
        {
            count_candidates[candidates[j]]++;
        }
        free(candidates);
    }

    // Find the hidden single values
    for (int i = 1; i <= BOARD_SIZE; i++)
    {
        if (count_candidates[i] == 1)
        {
            hidden_single_values[counter++] = i;
        }
    }

    return counter;
}




