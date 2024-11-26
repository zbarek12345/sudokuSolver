#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

struct sudoku{
  uint8_t * _matrix;
  short *_vertical;short* _horizontal;
};

static void setLineAt(short* line, int index, int value);

static uint8_t getLineAt(short* line, int index);

static struct sudoku* init(uint8_t* ptr);

static uint8_t* readFile(char * filename);

static void printSudoku(struct sudoku* data);

static void freeSudoku(struct sudoku* data);

static void solve(struct sudoku* data);

static uint8_t fit(struct sudoku* data, int* index);

static void backtrack(struct sudoku* data, int* index);
