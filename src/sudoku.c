#include "sudoku.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "hidden_singles.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <sudoku string>\n", argv[0]);
        return 1;
    }

    SudokuBoard *board = malloc(sizeof(SudokuBoard));
    init_sudoku(board);
    load_sudoku(board, argv[1]);

    Cell **p_solved_cells = board->solved_cells;
    int solved_counter = board->solved_counter;
    while (board->solved_counter < BOARD_SIZE * BOARD_SIZE)
    {
        solved_counter = check_solved_cells(board, &p_solved_cells);
        // printf("check_solved_cells %d\n", solved_counter);
        if (show_possible(board, p_solved_cells, solved_counter))
        {
            // printf("show_possible -> Yes\n");
            continue;
        }
        // solved_counter = hidden_singles(board);
        // if (solved_counter)
        // {
        //     printf("hidden_singles %d\n", solved_counter);
        //     continue;
        // }
    }
    print_solution(board);

    // clean up
    free_sudoku(board);
    free(board);
    return 0;
}