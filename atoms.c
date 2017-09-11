#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "atoms.h"

 // load and save don't work
bool gameRunning = 0;
grid_t** board;
player_t* players;
game_t game;
int constWidth;
int constHeight;
int constPlayNum;
char colours[] = {'R', 'G', 'P', 'B', 'Y', 'W'};
player_t* currTurn;
move_t *prevMove;
int turnCount;
char* memArr[6];
bool gameWon = 0;

void freeMem(move_t *root) {
	if (root->extra == NULL) {
		free(root);
		return;
	}
	if (root->extra != NULL) {
		freeMem(root->extra);
	}
	free(root);
}
void quit() {
	if (gameWon == 0) {
		printf("Bye!\n");
	}
	for (int i = 0; i < 6; i++) {
		free(memArr[i]);
	}
	if (gameRunning == 1) {
		freeMem(game.moves);
	}

	exit(0);
}

void expansion(int x, int y) {
	if (gameRunning == 0) {
		return;
	}
	if(turnCount >= constPlayNum) {
		int zeroCount;
		for (int i = 0; i < constPlayNum; i++) {
			for (int j = 0; j < constPlayNum; j++) {
				if (i == j) {
					continue;
				}
				if (players[j].grids_owned == 0) {
					zeroCount++;
				}
			}
			if (zeroCount == constPlayNum - 1) {
				switch (players[i].colour[0]) {
					case 'R':
						printf("Red Wins!\n");
						gameRunning = 0;
						gameWon = 1;
						quit();
						break;
					case 'G':
						printf("Green Wine!\n");
						gameRunning = 0;
						gameWon = 1;
						quit();
						break;
					case 'P':
						printf("Purple Wins!\n");
						gameRunning = 0;
						gameWon = 1;
						quit();
						break;
					case 'B':
						printf("Blue Wins!\n");
						gameRunning = 0;
						gameWon = 1;
						quit();
						break;
					case 'Y':
						printf("Yellow Wins!");
						gameRunning = 0;
						gameWon = 1;
						quit();
						break;
					case 'W':
						printf("White Wins!");
						gameRunning = 0;
						gameWon = 1;
						quit();
						break;
					default:
						break;
				}
			} else {
				zeroCount = 0;
			}
		}
	}
	if (constHeight == 2 && constWidth == 2 && turnCount > 1) {
		if (players[0].grids_owned == 0) {
			gameRunning = 0;
			printf("Green Wins!\n");
			return;
		} else if (players[1].grids_owned == 0) {
			gameRunning = 0;
			printf("Red Win's\n");
			return;
		}
	}
	if(x == 0 && y == 0 && board[y][x].atom_count > 1) {
		if(board[y+1][x].owner == NULL && board[y][x+1].owner == NULL) {
			board[y+1][x].owner = currTurn;
			board[y+1][x].atom_count++;
			currTurn->grids_owned++;
			board[y][x+1].owner = currTurn;
			board[y][x+1].atom_count++;
			board[y][x].owner = NULL;
			board[y][x].atom_count = 0;
			return;
		} else {
			if (board[y+1][x].owner != NULL && board[y][x+1].owner == NULL){
				board[y+1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
			} else if (board[y+1][x].owner == NULL && board[y][x+1].owner != NULL) {
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
			} else if (board[y+1][x].owner != NULL && board[y][x+1].owner != NULL) {
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
				expansion(x, y+1);
			}
		}
	} else if (x == constWidth - 1 && y == constHeight - 1 && board[y][x].atom_count > 1) {
		if(board[y-1][x].owner == NULL && board[y][x-1].owner == NULL) {
			board[y-1][x].owner = currTurn;
			board[y-1][x].atom_count++;
			currTurn->grids_owned++;
			board[y][x-1].owner = currTurn;
			board[y][x-1].atom_count++;
			board[y][x].owner = NULL;
			board[y][x].atom_count = 0;
			return;
		} else {
			if (board[y-1][x].owner != NULL && board[y][x-1].owner == NULL){
				board[y-1][x].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
			} else if (board[y-1][x].owner == NULL && board[y][x-1].owner != NULL) {
				board[y][x-1].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x-1, y);
			} else if (board[y-1][x].owner != NULL && board[y][x-1].owner != NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x-1, y);
				expansion(x, y-1);
			}
		}
	} else if (x == 0 && y == constHeight - 1 && board[y][x].atom_count > 1) {
		if(board[y-1][x].owner == NULL && board[y][x+1].owner == NULL) {
			board[y-1][x].owner = currTurn;
			board[y-1][x].atom_count++;
			currTurn->grids_owned++;
			board[y][x+1].owner = currTurn;
			board[y][x+1].atom_count++;
			board[y][x].owner = NULL;
			board[y][x].atom_count = 0;
			return;
		} else {
			if (board[y-1][x].owner != NULL && board[y][x+1].owner == NULL){
				board[y-1][x].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
			} else if (board[y-1][x].owner == NULL && board[y][x+1].owner != NULL) {
				board[y][x+1].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
			} else if (board[y-1][x].owner != NULL && board[y][x+1].owner != NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
				expansion(x, y-1);
			}
		}
	} else if (x == constWidth - 1 && y == 0 && board[y][x].atom_count > 1) {
		if(board[y+1][x].owner == NULL && board[y][x-1].owner == NULL) {
			board[y+1][x].owner = currTurn;
			board[y+1][x].atom_count++;
			currTurn->grids_owned++;
			board[y][x-1].owner = currTurn;
			board[y][x-1].atom_count++;
			board[y][x].owner = NULL;
			board[y][x].atom_count = 0;
			return;
		} else {
			if (board[y+1][x].owner != NULL && board[y][x-1].owner == NULL){
				board[y+1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
			} else if (board[y+1][x].owner == NULL && board[y][x-1].owner != NULL) {
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x-1, y);
			} else if (board[y+1][x].owner != NULL && board[y][x-1].owner != NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x-1, y);
				expansion(x, y+1);
			}
		}
	} else if (x == 0 && 0 < y < constHeight-1 && board[y][x].atom_count > 2) {
		if(board[y+1][x].owner == NULL && board[y-1][x].owner == NULL && board[y][x+1].owner == NULL) {
			board[y+1][x].owner = currTurn;
			board[y+1][x].atom_count++;
			currTurn->grids_owned++;
			board[y-1][x].owner = currTurn;
			board[y-1][x].atom_count++;
			board[y][x+1].owner = currTurn;
			board[y][x+1].atom_count++;
			currTurn->grids_owned++;
			board[y][x].owner = NULL;
			board[y][x].atom_count = 0;
			return;
		} else {
			if(board[y+1][x].owner != NULL && board[y-1][x].owner == NULL && board[y][x+1].owner == NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
			} else if(board[y+1][x].owner == NULL && board[y-1][x].owner != NULL && board[y][x+1].owner == NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
			} else if(board[y+1][x].owner == NULL && board[y-1][x].owner == NULL && board[y][x+1].owner != NULL) {
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
			} else if(board[y+1][x].owner != NULL && board[y-1][x].owner == NULL && board[y][x+1].owner != NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
				expansion(x, y+1);
			} else if(board[y+1][x].owner == NULL && board[y-1][x].owner != NULL && board[y][x+1].owner != NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
				expansion(x, y-1);
			} else if(board[y+1][x].owner != NULL && board[y-1][x].owner != NULL && board[y][x+1].owner == NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y-1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x, y-1);
			} else {
				board[y+1][x].owner->grids_owned--;
				board[y-1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x, y-1);
				expansion(x+1, y);
			}
		}
	} else if (x == constWidth - 1 && 0 < y < constHeight-1 && board[y][x].atom_count > 2) {
		if(board[y+1][x].owner == NULL && board[y-1][x].owner == NULL && board[y][x-1].owner == NULL) {
			board[y+1][x].owner = currTurn;
			board[y+1][x].atom_count = 1;
			currTurn->grids_owned++;
			board[y-1][x].owner = currTurn;
			board[y-1][x].atom_count = 1;
			board[y][x-1].owner = currTurn;
			board[y][x-1].atom_count++;
			currTurn->grids_owned++;
			board[y][x].owner = NULL;
			board[y][x].atom_count = 0;
			return;
		} else {
			if(board[y+1][x].owner != NULL && board[y-1][x].owner == NULL && board[y][x-1].owner == NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
			} else if(board[y+1][x].owner == NULL && board[y-1][x].owner != NULL && board[y][x-1].owner == NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
			} else if(board[y+1][x].owner == NULL && board[y-1][x].owner == NULL && board[y][x-1].owner != NULL) {
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x-1, y);
			} else if(board[y+1][x].owner != NULL && board[y-1][x].owner == NULL && board[y][x-1].owner != NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x-1, y);
				expansion(x, y+1);
			} else if(board[y+1][x].owner == NULL && board[y-1][x].owner != NULL && board[y][x-1].owner != NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x-1, y);
				expansion(x, y-1);
			} else if(board[y+1][x].owner != NULL && board[y-1][x].owner != NULL && board[y][x-1].owner == NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y-1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x, y-1);
			} else {
				board[y+1][x].owner->grids_owned--;
				board[y-1][x].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x, y-1);
				expansion(x-1, y);
			}
		}
	} else if ( 0 < x < constWidth - 1 && y == 0 && board[y][x].atom_count > 2) {
		if(board[y][x+1].owner == NULL && board[y][x-1].owner == NULL && board[y+1][x].owner == NULL) {
			board[y+1][x].owner = currTurn;
			board[y+1][x].atom_count = 1;
			currTurn->grids_owned++;
			board[y][x+1].owner = currTurn;
			board[y][x+1].atom_count = 1;
			board[y][x-1].owner = currTurn;
			board[y][x-1].atom_count++;
			currTurn->grids_owned++;
			board[y][x].owner = NULL;
			board[y][x].atom_count = 0;
			return;
		} else {
			if(board[y+1][x].owner != NULL && board[y][x-1].owner == NULL && board[y][x+1].owner == NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
			} else if(board[y+1][x].owner == NULL && board[y][x-1].owner != NULL && board[y][x+1].owner == NULL) {
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x-1, y);
			} else if(board[y+1][x].owner == NULL && board[y][x-1].owner == NULL && board[y][x+1].owner != NULL) {
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
			} else if(board[y+1][x].owner != NULL && board[y][x-1].owner == NULL && board[y][x+1].owner != NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
				expansion(x, y+1);
			} else if(board[y+1][x].owner == NULL && board[y][x-1].owner != NULL && board[y][x+1].owner != NULL) {
				board[y][x-1].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
				expansion(x-1, y);
			} else if(board[y+1][x].owner != NULL && board[y][x-1].owner != NULL && board[y][x+1].owner == NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x-1, y);
			} else {
				board[y+1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x-1, y);
				expansion(x+1, y);
			}
		}
	} else if ( 0 < x < constWidth - 1 && y == constHeight - 1 && board[y][x].atom_count > 2) {
		if(board[y][x+1].owner == NULL && board[y][x-1].owner == NULL && board[y-1][x].owner == NULL) {
			board[y-1][x].owner = currTurn;
			board[y-1][x].atom_count = 1;
			currTurn->grids_owned++;
			board[y][x+1].owner = currTurn;
			board[y][x+1].atom_count = 1;
			board[y][x-1].owner = currTurn;
			board[y][x-1].atom_count++;
			currTurn->grids_owned++;
			board[y][x].owner = NULL;
			board[y][x].atom_count = 0;
			return;
		} else {
			if(board[y-1][x].owner != NULL && board[y][x-1].owner == NULL && board[y][x+1].owner == NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
			} else if(board[y-1][x].owner == NULL && board[y][x-1].owner != NULL && board[y][x+1].owner == NULL) {
				board[y][x-1].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
			} else if(board[y-1][x].owner == NULL && board[y][x-1].owner == NULL && board[y][x+1].owner != NULL) {
				board[y][x+1].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x-1, y);
			} else if(board[y-1][x].owner != NULL && board[y][x-1].owner == NULL && board[y][x+1].owner != NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
				expansion(x, y-1);
			} else if(board[y-1][x].owner == NULL && board[y][x-1].owner != NULL && board[y][x+1].owner != NULL) {
				board[y][x-1].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
				expansion(x-1, y);
			} else if(board[y-1][x].owner != NULL && board[y][x-1].owner != NULL && board[y][x+1].owner == NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
				expansion(x-1, y);
			} else if(board[y-1][x].owner != NULL && board[y][x-1].owner != NULL && board[y][x+1].owner != NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
				expansion(x-1, y);
				expansion(x+1, y);
			}
		}
	} else if ( 0 < x < constWidth - 1 && 0 < y < constHeight -1 && board[y][x].atom_count > 3) {
		if(board[y][x+1].owner == NULL && board[y][x-1].owner == NULL && board[y+1][x].owner == NULL && board[y-1][x].owner == NULL) {
			board[y+1][x].owner = currTurn;
			board[y+1][x].atom_count = 1;
			board[y-1][x].owner = currTurn;
			board[y-1][x].atom_count = 1;
			currTurn->grids_owned++;
			board[y][x+1].owner = currTurn;
			board[y][x+1].atom_count = 1;
			currTurn->grids_owned++;
			board[y][x-1].owner = currTurn;
			board[y][x-1].atom_count++;
			currTurn->grids_owned++;
			board[y][x].owner = NULL;
			board[y][x].atom_count = 0;
			return;
		} else {
			if(board[y][x+1].owner != NULL && board[y][x-1].owner == NULL && board[y+1][x].owner == NULL && board[y-1][x].owner == NULL) {
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count = 1;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count++;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
			} else if(board[y][x+1].owner == NULL && board[y][x-1].owner != NULL && board[y+1][x].owner == NULL && board[y-1][x].owner == NULL) {
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x-1, y);
			}  else if(board[y][x+1].owner == NULL && board[y][x-1].owner == NULL && board[y+1][x].owner != NULL && board[y-1][x].owner == NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
			}  else if(board[y][x+1].owner == NULL && board[y][x-1].owner == NULL && board[y+1][x].owner == NULL && board[y-1][x].owner != NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
			}  else if(board[y][x+1].owner != NULL && board[y][x-1].owner != NULL && board[y+1][x].owner == NULL && board[y-1][x].owner == NULL) {
				board[y][x+1].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x+1, y);
				expansion(x-1, y);
			}  else if(board[y][x+1].owner == NULL && board[y][x-1].owner != NULL && board[y+1][x].owner != NULL && board[y-1][x].owner == NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x-1, y);
			}  else if(board[y][x+1].owner == NULL && board[y][x-1].owner == NULL && board[y+1][x].owner != NULL && board[y-1][x].owner != NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y-1][x].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x, y-1);
			}  else if(board[y][x+1].owner != NULL && board[y][x-1].owner == NULL && board[y+1][x].owner != NULL && board[y-1][x].owner == NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x+1, y);
			}  else if(board[y][x+1].owner == NULL && board[y][x-1].owner != NULL && board[y+1][x].owner == NULL && board[y-1][x].owner != NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
				expansion(x-1, y);
			}  else if(board[y][x+1].owner != NULL && board[y][x-1].owner == NULL && board[y+1][x].owner == NULL && board[y-1][x].owner != NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
				expansion(x+1, y);
			}  else if(board[y][x+1].owner != NULL && board[y][x-1].owner != NULL && board[y+1][x].owner == NULL && board[y-1][x].owner != NULL) {
				board[y-1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y-1);
				expansion(x+1, y);
				expansion(x-1, y);
			} else if(board[y][x+1].owner != NULL && board[y][x-1].owner != NULL && board[y+1][x].owner != NULL && board[y-1][x].owner == NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x+1, y);
				expansion(x-1, y);
			} else if(board[y][x+1].owner == NULL && board[y][x-1].owner != NULL && board[y+1][x].owner != NULL && board[y-1][x].owner != NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y-1][x].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x, y-1);
				expansion(x-1, y);
			} else if(board[y][x+1].owner != NULL && board[y][x-1].owner == NULL && board[y+1][x].owner != NULL && board[y-1][x].owner != NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y-1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x, y-1);
				expansion(x+1, y);
			} else if(board[y][x+1].owner != NULL && board[y][x-1].owner != NULL && board[y+1][x].owner != NULL && board[y-1][x].owner != NULL) {
				board[y+1][x].owner->grids_owned--;
				board[y-1][x].owner->grids_owned--;
				board[y][x+1].owner->grids_owned--;
				board[y][x-1].owner->grids_owned--;
				board[y+1][x].owner = currTurn;
				board[y+1][x].atom_count++;
				board[y-1][x].owner = currTurn;
				board[y-1][x].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x+1].owner = currTurn;
				board[y][x+1].atom_count = 1;
				currTurn->grids_owned++;
				board[y][x-1].owner = currTurn;
				board[y][x-1].atom_count++;
				currTurn->grids_owned++;
				board[y][x].owner = NULL;
				board[y][x].atom_count = 0;
				expansion(x, y+1);
				expansion(x, y-1);
				expansion(x+1, y);
				expansion(x-1, y);
			}
		}
	}
}

void help() {
	printf("\nHELP displays this help message\nQUIT quits the current game\n\nDISPLAY draws the game board in terminal\nSTART <number of players> <width> <height> starts the game\nPLACE <x> <y> places an atom in a grid space\nUNDO undoes the last move made\nSTAT displays game statistics\n\nSAVE <filename> saves the state of the game\nLOAD <filename> loads a save file\nPLAYFROM <turn> plays from n steps into the game\n\n");
	return;
}

void display() {
	printf("\n");
	int length = 3*constWidth+1;
	char *tb = malloc((length+1)*sizeof(char));
	for (int i = 0; i <= length; i++) {
		if (i == length) {
			tb[i] = '\0';
		}else if (i == 0 || i == length -1) {
			tb[i] = '+';
		} else {
			tb[i] = '-';
		}
	}
	printf("%s\n", tb);
	char* row = malloc((length+1)* sizeof(char));
	for(int i = 0; i < constHeight; i++) {
		int p = 0;
		for (int j = 0; j < length; j++) {
			if (j == 0) {
				row[j] = '|';
			} else if (j % 3 == 0) {
				row[j] = '|';
				p = p + 1;
			} else {
				if (board[i][p].owner == NULL) {
					row[j] = ' ';
					row[j + 1] = ' ';
				} else {
					player_t *player = (board[i][p]).owner;
					row[j] = player->colour[0];
					row[j + 1] = (char)(board[i][p].atom_count + 48);
				}
				j++;
			}
		}
		row[length] = '\0';
		printf("%s\n", row);
	}
	printf("%s\n", tb);
	free(row);
	free(tb);
	printf("\n");
}
void start(int numPlayer, int width, int height) {
	if (width * height < numPlayer || numPlayer > 6) {
		printf("%s\n", "Cannot Start Game");
		return;
	}
	gameRunning = 1;
	constWidth = width;
	constHeight = height;
	constPlayNum = numPlayer;
	players = malloc(numPlayer*sizeof(player_t));
	for(int i = 0; i < numPlayer; i++) {
		players[i].colour = (colours + i);
		players[i].grids_owned = 0;
	}
	board = malloc(width*sizeof(grid_t));
	for(int i = 0; i < width; i++) {
		board[i] = malloc(height*sizeof(grid_t));
		for(int j = 0; j < height; j++) {
			(board[i][j]).owner = NULL;
			(board[i][j]).atom_count = 0;
		}
	}
	currTurn = &players[0];
	move_t *root = malloc(sizeof(move_t));
	prevMove = root;
	root->extra = NULL;
	game.moves = root;
}
void place(int x, int y) {
	if (x < 0 || y < 0) {
		printf("Invalid Coordinates");
		return;
	} else if (board[y][x].owner != NULL && board[y][x].owner != currTurn) {
		printf("Cannot Place Atom Here\n");
		return;
	}
	move_t *newMove = malloc(sizeof(struct move_t));

	newMove->x = x;
	newMove->y = y;
	newMove->parent = prevMove;
	newMove->extra = NULL;
	prevMove->extra = newMove;


	if (board[y][x].owner == NULL) {
		board[y][x].owner = currTurn;
		board[y][x].atom_count = 1;
		currTurn->grids_owned++;
	} else {
		board[y][x].atom_count++;
	}
	expansion(x, y);
	char *currPlayer = currTurn->colour;
	int currPlace = 0;
	switch (currPlayer[0]) {
		case 'R':
			currPlace = 0;
			break;
		case 'G':
			currPlace = 1;
			break;
		case 'P':
			currPlace = 2;
			break;
		case 'B':
			currPlace = 3;
			break;
		case 'Y':
			currPlace = 4;
			break;
		case 'W':
			currPlace = 5;
			break;
		default:
			break;
	}
	currPlace++;
	for(int i = 0; i < constPlayNum; i++) {
		if (currPlace >= constPlayNum) {
			currPlace = 0;
		}
		if (players[currPlace].grids_owned <= 0 && turnCount >= constPlayNum) {
			currPlace++;
		} else {
			break;
		}
	}
	currTurn = &players[currPlace];
	if (currTurn->colour[0] == 'R') {
		printf("Red's Turn\n\n");
	} else if (currTurn->colour[0] == 'G') {
		printf("Green's Turn\n\n");
	} else if (currTurn->colour[0] == 'P') {
		printf("Purple's Turn\n\n");
	} else if (currTurn->colour[0] == 'B') {
		printf("Blue's Turn\n\n");
	} else if (currTurn->colour[0] == 'Y') {
		printf("Yellow's Turn\n\n");
	} else if (currTurn->colour[0] == 'W') {
		printf("White's Turn\n\n");
	}
	prevMove = newMove;
	turnCount++;
}

void clearBoard() {
	for(int i = 0; i < constHeight; i++) {
		for (int j = 0; j < constWidth; j++) {
			board[i][j].atom_count = 0;
			board[i][j].owner = NULL;
		}
	}
}
// TODO FIX
void sim(move_t* move, char colour) {
	if(move == NULL) {
		return;
	}
	int x = move->x;
	int y = move->y;

	if (x < 0 || y < 0) {
		printf("Invalid Coordinates");
		return;
	} else if (board[y][x].owner != NULL && board[y][x].owner != currTurn) {
		printf("Cannot Place Atom Here\n");
		return;
	}

	if (board[y][x].owner == NULL) {
		switch(colour){
			case 'R':
				board[y][x].owner = &players[0];
				break;
			case 'G':
				board[y][x].owner = &players[1];
				break;
			case 'P':
				board[y][x].owner = &players[2];
				break;
			case 'B':
				board[y][x].owner = &players[3];
				break;
			case 'Y':
				board[y][x].owner = &players[4];
				break;
			case 'W':
				board[y][x].owner = &players[5];
				break;
		}
		board[y][x].atom_count = 1;
		currTurn->grids_owned++;
	} else {
		board[y][x].atom_count++;
	}
	expansion(x, y);
	return;
}
void erase(move_t *root) {
	if (root->extra == NULL) {
		free(root);
		return;
	}
	if (root->extra != NULL) {
		erase(root->extra);
	}
	free(root);
}
void playfrom(int turn) {
	if (turn == 0) {
		start(constPlayNum, constWidth, constHeight);
		printf("Red's Turn\n\n");
		return;
	}
	clearBoard();
	move_t *root = game.moves->extra;
	int colourIndex = 0;
	for(int i = 0; i < turn; i++) {
		if (i >= constPlayNum) {
			colourIndex = 0;
		}
		if (root == NULL) {
			break;
		}
		sim(root, colours[colourIndex]);
		root = root->extra;
		colourIndex++;
	}
	switch(colourIndex) {
		case 0: printf("Red's Turn\n\n"); break;
		case 1: printf("Green's Turn\n\n"); break;
		case 2: printf("Purple's Turn\n\n"); break;
		case 3: printf("Blue's Turn\n\n"); break;
		case 4: printf("Yellow's Turn\n\n"); break;
		case 5: printf("White's Turn\n\n"); break;
	}
	return;
}

void undo() {
	turnCount--;
	move_t *oldNode = game.moves;
	int count = 0;
	while (count <= turnCount) {
		oldNode = oldNode->extra;
		count++;
	}
	oldNode->parent->extra = NULL;
	free(oldNode);
	if(turnCount == 0) {
		game.moves->extra = NULL;
	}
	playfrom(turnCount);
}
void stat() {
	for (int i = 0; i < constPlayNum; i++) {
		switch(players[i].colour[0]){
			case 'R':
				if (players[i].grids_owned <= 0 && turnCount >= constPlayNum) {
					printf("%s\n\n", "Player Red:\nLost");
					break;
				}
				printf("%s%d\n\n", "Player Red:\nGrid Count: ", players[i].grids_owned);
				break;
			case 'G':
				if (players[i].grids_owned <= 0 && turnCount >= constPlayNum) {
					printf("%s\n\n", "Player Green:\nLost");
					break;
				}
				printf("%s%d\n\n", "Player Green:\nGrid Count: ", players[i].grids_owned);
				break;
			case 'P':
				if (players[i].grids_owned <= 0 && turnCount >= constPlayNum) {
					printf("%s\n\n", "Player Purple:\nLost");
					break;
				}
				printf("%s%d\n\n", "Player Purple:\nGrid Count: ", players[i].grids_owned);
				break;
			case 'B':
				if (players[i].grids_owned <= 0 && turnCount >= constPlayNum) {
					printf("%s\n\n", "Player Blue:\nLost");
					break;
				}
				printf("%s%d\n\n", "Player Blue:\nGrid Count: ", players[i].grids_owned);
				break;
			case 'Y':
				if (players[i].grids_owned <= 0 && turnCount >= constPlayNum) {
					printf("%s\n\n", "Player Yellow:\nLost");
					break;
				}
				printf("%s%d\n\n", "Player Yellow:\nGrid Count: ", players[i].grids_owned);
				break;
			case 'W':
				if (players[i].grids_owned <= 0 && turnCount >= constPlayNum) {
					printf("%s\n\n", "Player Whit:\nLost");
					break;
				}
				printf("%s%d\n\n", "Player White:\nGrid Count: ", players[i].grids_owned);
				break;
		}
	}
}
void save(char *filename) {
	/*save_t save;
	save.filename = filename;
	save_file_t saveFile;
	saveFile.height = (uint8_t)constHeight;
	saveFile.width = (uint8_t)constWidth;
	saveFile.no_players = (uint8_t)constPlayNum;
	printf("%d %d %d \n\n", saveFile.no_players, saveFile.width, saveFile.height);
	uint32_t* moves = malloc(turnCount*sizeof(uint32_t));
	move_t *root = game.moves->extra;
	char* move = malloc(turnCount*(sizeof(uint32_t)));
	for (int i = 0; i < turnCount; i++) {
		sprintf(move, "%d%d%d%d", root->x, root->y, 0, 0);
		uint32_t toArr = (uint32_t)move;
		printf("%d %d \n", move, toArr);
		moves[i] = move;
		root = root->extra;
	}
	save.data = &saveFile;
	FILE *fp = fopen(filename, "ab+");
	fwrite(&saveFile.height, sizeof(save), 1, fp);
	fclose(fp);
	free(move);
	free(moves);*/
}
void load(char *filename) {
	// TODO
}


int main(int argc, char **argv) {

	char *comd = malloc(8 * sizeof(char));
	memArr[0] = comd;
	char *x = malloc(3 * sizeof(char));
	memArr[1] = x;
	char *y = malloc(3 * sizeof(char));
	memArr[2] = y;
	char *k = malloc(50 * sizeof(char));
	memArr[3] = k;
	char *overflow = malloc(50 * sizeof(char));
	memArr[4] = overflow;
	char *input = malloc(20 * sizeof(char));
	memArr[5] = input;

	do {
		fgets(input, 21, stdin);
		sscanf(input, "%s %s %s %s %s", comd, k, x, y, overflow);
		int argCount = 1;
		for (int i = 0; i <= strlen(input); i++) {
			if (input[i] == ' ') {
				argCount = argCount + 1;
			}
		}
		if (strcmp(comd, "HELP") == 0) {
			help();
		} else if (strcmp(comd, "QUIT") == 0) {

			quit();
			break;
		} else if (strcmp(comd, "DISPLAY") == 0) {
			display();
		} else if (strcmp(comd, "START") == 0) {
			if (gameRunning == 1) {
				printf("%s\n", "Invalid Command");
				continue;
			} else if (argCount == 5) {
				printf("%s\n", "Too Many Arguments");
				continue;
			} else if (argCount < 4) {
				printf("%s\n", "Missing Argument");
				continue;
			}
			int np = (int) strtol(k, argv, 10);
			int w = (int) strtol(x, argv, 10);
			int h = (int) strtol(y, argv, 10);
			start(np, w, h);
			printf("Game Ready\n");
			printf("Red's Turn\n\n");
		} else if (strcmp(comd, "PLACE") == 0) {
			if (gameRunning == 0) {
				printf("Invalid Command\n");
				continue;
			}
			if (argCount < 3) {
				printf("%s\n", "Missing Argument");
				continue;
			} else if (argCount > 3) {
				printf("%s\n", "Too Many Arguments");
				continue;
			}
			int xcoord = (int) strtol(k, argv, 10);
			int ycoord = (int) strtol(x, argv, 10);
			place(xcoord, ycoord);
		} else if (strcmp(comd, "UNDO") == 0) {
			undo();
		} else if (strcmp(comd, "STAT") == 0) {
			stat();
		} else if (strcmp(comd, "PLAYFROM") == 0) {
			if (argCount > 2) {
				printf("%s\n", "Too Many Arguments");
				continue;
			} else if (argCount < 2) {
				printf("%s\n", "Missing Argument");
			}
			int t = (int) strtol(k, argv, 10);
			playfrom(t);
		} else if (strcmp(comd, "SAVE") == 0) {
			if (argCount > 2) {
				printf("%s\n", "Too Many Arguments");
				continue;
			} else if (argCount < 2) {
				printf("%s\n", "Missing Argument");
			}

			save(k);
		} else if (strcmp(comd, "LOAD") == 0) {
			if (argCount > 2) {
				printf("%s\n", "Too Many Arguments");
				continue;
			} else if (argCount < 2) {
				printf("%s\n", "Missing Argument");
			}

			load(k);
		} else {
			printf("Invalid Command\n");
		}
	} while (1);
	return 0;
}
