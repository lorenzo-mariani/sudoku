#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIM 9		// number of grid rows and columns
#define ATTEMPTS 30	// number of attempts to enter a correct value in the grid
#define EASY 32		// difficulty level: easy (32 default values already entered in the grid)
#define MEDIUM 24	// difficulty level: medium (24 default values already entered in the grid)
#define HARD 16		// difficulty level: hard (16 default values already entered in the grid)

void resetGrid(int grid[DIM][DIM]) {
	// reset the grid, i.e., all values are initialized to zero
	int row, col;
	
	for(row = 0; row < DIM; row++) {
		for(col = 0; col < DIM; col++) {
			grid[row][col]= 0;
		}
	}
	
	return;
}

void showGrid(int grid[DIM][DIM]) {
	// print the DIMxDIM grid
	int row, col;
	
	printf("\t    0 1 2   3 4 5   6 7 8\n");
	printf("\t  ÚÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄ¿\n");
	
	for(row = 0; row < DIM; row++) {
		printf("\t%d ", row);
		
		for(col = 0; col < DIM; col++) {
			if (col==0) {
				printf("³ ");
			}
			
			printf("%d ", grid[row][col]);
			
			if ((col + 1) % 3 == 0) {
				printf("³ ");
			}
		}
		
		if ((row + 1) % 3 == 0 && row != 8) {
			printf("\n\t  ÃÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄ´\n");
			continue;
		}
		printf("\n");
	}
	printf("\t  ÀÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÙ\n");
	return;
}

int checkRow(int row, int col, int grid[DIM][DIM]) {
	// check if the randomly generated number can be inserted in the row, i.e.,
	// if the row does not already contain values equal to the current number
	int k;
	
	for(k = 0; k < DIM; k++) {
		if (k != col) {
			if (grid[row][col] == grid[row][k]) {
				return 0;
			}
		}
	}
	
	return 1;
}

int checkCol(int row, int col, int grid[DIM][DIM]) {
	// check if the randomly generated number can be inserted in the column, i.e.,
	// if the column does not already contain values equal to the current number
	int k;
	
	for(k = 0; k < DIM; k++) {
		if (k != row) {
			if (grid[row][col] == grid[k][col]) {
				return 0;
			}
		}
	}
	
	return 1;
}

int checkSquare(int row, int col, int grid[DIM][DIM]) {
	// check if the randomly generated number can be inserted in the square, i.e.,
	// if the square does not already contain values equal to the current number
	int i, j;
	
	int row_start = row - row % 3;
	int row_end = 3 + row_start;
	
	int col_start = col - col % 3;
	int col_end = 3 + col_start;
	
	if (row >= row_start && row < row_end) {
		if (col >= col_start && col < col_end) {
			for(i = row_start; i < row_end; i++) {
				for(j = col_start; j < col_end; j++) {		
					if (grid[row][col] == grid[i][j] && (i != row && j != col)) {
						return 0;
					}
				}
			}
		}
	}
	
	return 1;
}

int checkNumber(int row, int col, int grid[DIM][DIM]) {
	// check if the randomly generated number is valid by calling three functions, i.e.:
	//	- checkRow(int row, int col, int grid[DIM][DIM])
	//	- checkCol(int row, int col, int grid[DIM][DIM])
	//	- checkSquare(int row, int col, int grid[DIM][DIM])
	int result = 0;
	
	if (checkRow(row, col, grid) == 1) {		
		if (checkCol(row, col, grid) == 1) {
			result = checkSquare(row, col, grid);
		}
	}
	
	return result;
}

void checkGrid(int grid[DIM][DIM]) {
	// check if the final grid is correct
	int row, col;
	
	system("cls");
	printf("Checking sudoku in progress...");
	
	for(row = 0; row < DIM; row++) {
		for(col = 0; col < DIM; col++) {
			if(checkNumber(row, col, grid) == 0) {
				printf("\nInvalid sudoku:\n\n");
				showGrid(grid);
				printf("\n\n");
				system("pause");
				return;
			}
		}
	}
	printf("\nCongratulations, it's correct'!\n\n");
	system("pause");
	return;
}

void loadDefaultValues(int difficulty, int grid[DIM][DIM], int default_values[difficulty]) {
	// depending on the level of difficulty, it loads EASY, MEDIUM, or HARD
	// default values to the game grid. It returns a vector containing the indexes
	// where the default values are located, calculated as row * DIM + col
	int i;
	int row, col;
	int is_valid;	// if 1, the randomly generated number is valid - if 0, the randomly generated number is not valid
	
	srand(time(NULL));	// used to generate random numbers
	
	for(i = 0; i < difficulty; i++) {
		row = (rand() % 9);	// random number between 0 and 8
		col = (rand() % 9);	// random number between 0 and 8
		
		if (grid[row][col] != 0) {
			i--;
			continue;
		}
		
		do {
			is_valid = 1;
			grid[row][col] = (rand() % 9) + 1;	// random number between 1 and 9
			
			if (checkNumber(row, col, grid) == 0) {
				is_valid = 0;
				grid[row][col] = 0;
			}		
		} while (is_valid == 0);
		
		default_values[i] = row*DIM + col;
	}
	return;
}

int checkDefault(int difficulty, int index, int default_values[difficulty]) {
	// check if the current value is a default value
	int i;
	for (i = 0; i < difficulty; i++) {
		if (index == default_values[i]) {
			return 1;
		}
	}
	return 0;
}

void save(int difficulty, int grid[DIM][DIM], int default_values[difficulty]) {
	//save the game
	FILE *saved_game;
	int i;
	int row, col;
	saved_game = fopen("saved_game.txt", "w");
	
	fprintf(saved_game, "%d\n", difficulty);		// write difficulty level
	
	for(i = 0; i < difficulty; i++) {
		fprintf(saved_game, "%d ", default_values[i]);	// write default values
	}
	
	fprintf(saved_game, "\n");
	
	for(row = 0; row < DIM; row++) {
		for(col = 0; col < DIM; col++) {
			fprintf(saved_game, "%d ", grid[row][col]);		// write game grid
		}
		fprintf(saved_game, "\n");
	}
	
	fclose(saved_game);
	return;
}

void playGame(int difficulty, int grid[DIM][DIM], int default_values[difficulty]) {
	// play game
	int i;
	int row, col;
	float percentage;
	
	for(i = 0; i + difficulty < DIM*DIM; i++) {
		percentage = ((i + difficulty) * 100) / (DIM*DIM);
		system("cls");	
		printf("To return to the menu enter 9-9. The game will be saved automatically.\nSudoku 3x3 - completion %d/%d ~ %.2f%%:\nX: row - Y: column\n\n", (i + difficulty), DIM*DIM, percentage);
		showGrid(grid);
		printf("\n\n");
		
		do {
			printf("Position (X-Y): ");
			scanf("%d-%d", &row, &col);
		} while ((row < 0 || row > 9) || (col < 0 || col > 9));
		
		if (row == 9 && col == 9) {
			// back to menu
			save(difficulty, grid, default_values);
			printf("\nSaved game on 'saved_game.txt'.\n");
			system("pause");
			return;
		} else if (row == 9 || col == 9) {
			// invalid choice
			printf("\nInvalid choice. To return to the menu enter 9-9.\n");
			i--;
			system("pause");
		} else {
			// Insert a value in the grid
			int index = col*DIM + row;
			int is_default = checkDefault(difficulty, index, default_values);
			
			if (is_default == 1) {
				printf("\nYou can't replace a value given by the game!\n");
				system("pause");
				i--;
				continue;
			} else if (is_default == 0) {
				if (grid[col][row] != 0) {
					i--;
				}
				
				do {
					printf("Value: ");
					scanf("%d", &grid[col][row]);
				} while (grid[col][row] < 1 || grid[col][row] > 9);
				
				printf("\nValue added correctly in: %d-%d\n", row, col);
				system("pause");	
			}	
		}
	}
	
	checkGrid(grid);
	return;
}

void loadSavedGame(int grid[DIM][DIM]) {
	// load saved game
	FILE *saved_game;
	int row = 0;
	int col = 0;

	saved_game = fopen("saved_game.txt", "r");
	
	if(saved_game == NULL) {
		printf("\nThere are no saved games... You need to start a new one.\n\n");
		exit(1);
	} else {
		int i = 0;
		int n;
		int difficulty;
		
		fscanf(saved_game, " %d", &difficulty);		// read difficulty level
		
		int default_values[difficulty];
		
		while (fscanf(saved_game, " %d", &n) == 1) {
		    if (i >= 0 && i < difficulty) {
				default_values[i] = n;		// read default values
				i++;
			} else {
				if (col < DIM) {
					grid[row][col] = n;
					col++;
				} else {
					row++;
					col = 0;
					grid[row][col] = n;
					col++;
						
				}
			}
		}
		showGrid(grid);
		system("pause");
		fclose(saved_game);
		playGame(difficulty, grid, default_values);
	}
}

int main(void) {
	int grid[DIM][DIM];		// game grid
	
	int menu_choice;
	int level_choice;
	
	while(1){
		int difficulty;
		
		system("cls");
		printf("Choose an option:\n1. New game\n2. Load saved game\n3. Exit\n\n");
		scanf("%d", &menu_choice);
		system("cls");
		
		if (menu_choice == 1) {
			resetGrid(grid);
			do {
				system("cls");
				printf("Choose the difficulty level:\n1. Easy\n2. Medium\n3. Hard\n\n");
				scanf("%d", &level_choice);
			} while(level_choice < 1 && level_choice > 3);
			
			if (level_choice == 1) {
				difficulty = EASY;
			} else if (level_choice == 2) {
				difficulty = MEDIUM;
			} else if (level_choice == 3) {
				difficulty = HARD;
			}
			
			int default_values[difficulty];
			loadDefaultValues(difficulty, grid, default_values);
			showGrid(grid);
			playGame(difficulty, grid, default_values);
			
		} else if (menu_choice == 2) {
			loadSavedGame(grid);
		} else if (menu_choice == 3) {
			exit(0);
		} else {
			printf("Invalid choice.\n\n");
			system("pause");
		}
	}
}
