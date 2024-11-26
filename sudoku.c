#include "sudoku.h"

uint8_t* readFile(char* filePath){
	FILE* f = fopen(filePath,"r");

	uint8_t* ptr = malloc(sizeof(uint8_t)*81);
	char c;
	for(int i=0;i<81;i++)
	{
		fscanf(f,"%c", &c);

		if(c == '\n'){
			i--;
			continue;
		}

		ptr[i] = c == '.' ? 0 : c-'0';
	}
	return ptr;
}

void setLineAt(short* ptr, int index, int value){
	
	if(value == 0)
	{
		*ptr = *ptr & ~((uint8_t)1 << index);
	}
	else if (value == 1){
		*ptr = *ptr | ((uint8_t)1 << index);
	}
	else{
		fprintf(stderr,"%s\n", "Wrong byte value given");
	}
}

uint8_t getLineAt(short* line, int value)
{
	return (*line >> value) & (uint8_t)1 == 1 ? 1: 0;
}

struct sudoku* init(uint8_t* ptr){
	struct sudoku* s = malloc(sizeof(struct sudoku));
	s->_matrix = ptr;
	s->_vertical = malloc(sizeof(short)*9);
	s->_horizontal = malloc(sizeof(short)*9);
	for(int i =0;i<81;i++){
		if(ptr[i]>0)
		{
			setLineAt(s->_vertical+(i%9),ptr[i],1);
			setLineAt(s->_horizontal+(i/9),ptr[i],1);
			ptr[i] = ptr[i]|0x010;
		}
	}
	return s;
}

void printSudoku(struct sudoku* data)
{
	for(int i =0;i<81;i++){
		if(i/9>=1 && i%9 == 0)
			printf("\n");
		printf("%hhd ", data->_matrix[i]>16?data->_matrix[i]-16:data->_matrix[i]);
	}
	printf("\n");
}

void freeSudoku(struct sudoku* data){
	free(data->_vertical);
	free(data->_horizontal);
	return;
}

void solve(struct sudoku* s);

int main(){
	char buffer[16];
	sprintf(buffer, "test.txt");

	uint8_t* arr = readFile(buffer);

	struct sudoku* sudo = init(arr);


	printSudoku(sudo);
	solve(sudo);
	printSudoku(sudo);
	freeSudoku(sudo);
}

static void solve(struct sudoku* data) {
    int index = 0;
    while (index < 81 && index >= 0) {
        uint8_t fit_var = fit(data, &index);
        printf("%d ", index);
        if (fit_var != 1) {
            backtrack(data, &index);
        }
    }
    if (index == 0) {
        printf("Sudoku was not solvable\n");
    } else {
        printf("Sudoku solved\n");
    }
}

static uint8_t fit(struct sudoku* data, int* index) {
    // Skip predefined values
    if (data->_matrix[*index] && 0x10) {
        (*index)++;
        return 1;
    }

    for (int i = 1; i <= 9; i++) {
        // Check if the number can be placed in the current position
        if (getLineAt(data->_vertical + *index % 9, i) == 0 && 
            getLineAt(data->_horizontal + (*index / 9), i) == 0) {
            // Set the number in the Sudoku matrix
            setLineAt(data->_horizontal + (*index / 9), i, 1);
            setLineAt(data->_vertical + (*index % 9), i, 1);
            data->_matrix[*index] = i; // Place the number
            (*index)++;
            return 1;
        }
    }
    return 0; // No fitting number found
}

static void backtrack(struct sudoku* data, int* index) {
    if (*index <= 0) {
        return; // No more cells to backtrack to
    }

    //(*index)--; // Move back to the previous cell

    if (*index < 0) {
        return; // Prevent going out of bounds
    }

    // Clear the current cell
    if (data->_matrix[*index] && 0x10) {
        // If the cell was predefined, we skip it
        (*index)--; // Move back again to skip the predefined cell
        return;
    }

    // Reset the current cell
    setLineAt(data->_horizontal + (*index / 9), data->_matrix[*index], 0);
    setLineAt(data->_vertical + (*index % 9), data->_matrix[*index], 0);
    data->_matrix[*index] = 0; // Reset the cell value
    (*index--);
}