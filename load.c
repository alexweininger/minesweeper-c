#include "load.h"
#include "minesweep.h"
#include "cell.h"

game* file_load(char* filename) {
	game *board = (game*) malloc(sizeof(game));

	FILE * fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Error: could not open file: %s for reading.\n", filename);
		return NULL;
	}

	// read numRows/numCols from file
	int numRows, numCols;
	fscanf(fp, "%d%d", &numRows, &numCols);
	
	printf("From file:\nnumRows: %d, numCols: %d\n", numRows, numCols);

	// ternery ops to make sure numRows/numCols are at least 3
	numRows = (numRows > 2)? numRows:3;
	numCols = (numCols > 2)? numCols:3;

	board->row_max = numRows;
	board->col_max = numCols;

	board->cells = (cell**) calloc(numRows, sizeof(cell*));

	int i;
	for(i = 0; i < numRows; i++) {
		board->cells[i] = (cell *) calloc(numCols, sizeof(cell));
		board->cells[i]->color = gray;
	}
}