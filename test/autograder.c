#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#include "../src/sudoku.h"
#include "../src/hidden_singles.h"
#include "../src/hidden_pairs.h"
#include "../src/hidden_triples.h"
#include "../src/naked_pairs.h"
#include "../src/naked_triples.h"

// Verbosity of test runner. Overridden via compilation flag
#ifdef VERBOSE
#define VERBOSE 1
#endif
#ifndef VERBOSE
#define VERBOSE 0
#endif

char mapping[32] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};

int get_index(char *array, int len, char input)
{
    for (int i = 0; i < len; i++)
    {
        if (array[i] == input)
            return i;
    }
    return -1;
}

void toBinary(int n, int *bin_array)
{
    int id = 0;
    while (n)
    {
        bin_array[id++] = n % 2;
        n = n / 2;
    }
}

int toInteger(int *bin_array, int len)
{
    int num = 0;
    for (int i = 0; i < len; i++)
    {
        num += bin_array[i] * (1 << i);
    }
    return num;
}

void load_cell_candidates(Cell *p_cell, char *textData)
{
    int left_num = get_index(mapping, 32, textData[0]);
    int right_num = get_index(mapping, 32, textData[1]);

    int bin_candidates[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    toBinary(right_num, &(bin_candidates[0]));
    toBinary(left_num, &(bin_candidates[5]));

    int counter = 0;
    int candidates[BOARD_SIZE];

    for (int cand = 1; cand <= 9; cand++)
    {
        if (bin_candidates[cand])
            candidates[counter++] = cand;
    }

    set_candidates(p_cell, candidates, counter);
    if (bin_candidates[0])
        p_cell->fixed = true;
    else
        p_cell->fixed = false;
}

void load_sudoku_with_candidates(SudokuBoard *p_board, char *textData)
{
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        load_cell_candidates(&(p_board->data[i / BOARD_SIZE][i % BOARD_SIZE]), textData);
        textData += 2;
    }
}

void print_string_candidates(Cell *p_cell, char *textData)
{
    int *candidates = get_candidates(p_cell);
    int len = p_cell->num_candidates;

    int bin_candidates[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (p_cell->fixed)
        bin_candidates[0] = 1;

    for (int i = 0; i < len; i++)
    {
        bin_candidates[candidates[i]] = 1;
    }
    int left_index = toInteger(&(bin_candidates[5]), 5);
    int right_index = toInteger(&(bin_candidates[0]), 5);
    sprintf(textData, "%c%c", mapping[left_index], mapping[right_index]);
    free(candidates);
}

void print_sudoku_with_candidates(SudokuBoard *p_board, char *textData)
{
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        print_string_candidates(&(p_board->data[i / BOARD_SIZE][i % BOARD_SIZE]), textData);
        textData += 2;
    }
}

typedef int (*method)(SudokuBoard *p_board);

method get_method(char *method_name)
{
    if (strcmp(method_name, "hidden_singles") == 0)
        return hidden_singles;
    else if (strcmp(method_name, "naked_pairs") == 0)
        return naked_pairs;
    else if (strcmp(method_name, "hidden_pairs") == 0)
        return hidden_pairs;
    else if (strcmp(method_name, "naked_triples") == 0)
        return naked_triples;
    else if (strcmp(method_name, "hidden_triples") == 0)
        return hidden_triples;
    else
        return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Usage: %s <sudoku string> <method> <pipe>\n\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    SudokuBoard *board = malloc(sizeof(SudokuBoard));
    init_sudoku(board);
    load_sudoku_with_candidates(board, argv[1]);

    FILE *pipe = fdopen(atoi(argv[3]), "w");

    int num_detected = get_method(argv[2])(board);

    char *outText = malloc(BOARD_SIZE * BOARD_SIZE * 2 + 1);
    outText[0] = '\0';

    print_sudoku_with_candidates(board, outText);

    fprintf(pipe,
            "{\n"
            "    \"num_detects\": %d,\n"
            "    \"boards\": \"%s\"\n"
            "}\n",
            num_detected, outText);

    free_sudoku(board);
    free(board);
    free(outText);

    fclose(pipe);
    exit(EXIT_SUCCESS);
}