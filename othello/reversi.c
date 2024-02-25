//
//  othello.c
//  othello
//
//  Created by sasa on 2023/09/30.
//

#include "reversi.h"
#include "evaluation.c"

void reset(void) {
	printf("[*]初期化中...\n");
	for (char i = 0; i <= 9; ++i) {
		for (char j = 0; j <= 9; ++j) {
			board[i][j] = 0;
			canPut[i][j] = false;
		}
	}
	board[4][4] = 2;
	board[4][5] = 1;
	board[5][4] = 1;
	board[5][5] = 2;
	// 白が2,黒が1
	player = 1;
	skip = false;
	finished = 0;
	skipped = false;
	turn = 1;
	DEPTH = 10;
	return;
}

void check2(int *Player) {
	char x, y;
	for (y = 1; y < 9; ++y) {
		for (x = 1; x < 9; ++x) {
			if (board[y][x] == *Player) {
				for (char xx = -1; xx < 2; ++xx) {
					for (char yy = -1; yy < 2; ++yy) {
						if (board[y + yy][x + xx] == (3 - *Player)) {
							char xxx = x + xx;
							char yyy = y + yy;
							while (board[yyy][xxx] == (3 - *Player)) {
								xxx += xx;
								yyy += yy;
							}
							if (yyy <= 8 && xxx <= 8 && yyy >= 1 && xxx >= 1 && board[yyy][xxx] == 0) {
								canPut[yyy][xxx] = true;
							}
						}
					}
				}
			}
		}
	}
	
	skip = false;
	for (y = 1; y < 9; ++y) {
		for (x = 1; x < 9; ++x) {
			if (canPut[y][x] == true) {
				skipped = false;
				skip = true;
				return;
			}
		}
	}
	
	if (skipped == true) {
		printf("終了\n");
		finished = 1;
		return;
	}
	
	if (*Player == 1) {
		printf("Player:%dをスキップ\n", player);
		skipped = true;
		player = 2;
		check2(&player);
	} else if (*Player == 2) {
		printf("Player:%dをスキップ\n", player);
		skipped = true;
		player = 1;
		check2(&player);
	}
}

void check3(char *player, bool canput[10][10]) {
	char x, y;
	for (y = 1; y < 9; ++y) {
		for (x = 1; x < 9; ++x) {
			if (board[y][x] == *player) {
				for (char xx = -1; xx < 2; ++xx) {
					for (char yy = -1; yy < 2; ++yy) {
						if (board[y + yy][x + xx] == (3 - *player)) {
							char xxx = x + xx;
							char yyy = y + yy;
							while (board[yyy][xxx] == (3 - *player)) {
								xxx += xx;
								yyy += yy;
							}
							if (yyy <= 8 && xxx <= 8 && yyy >= 1 && xxx >= 1 && board[yyy][xxx] == 0) {
								canput[yyy][xxx] = true;
							}
						}
					}
				}
			}
		}
	}
	
	skip = false;
	for (y = 1; y < 9; ++y) {
		for (char x = 1; x < 9; ++x) {
			if (canput[y][x] == true) {
				skip = true;
				return;
			}
		}
	}
}

void check4(int *player, bool canput[10][10], char board[10][10]) {
	for (char y = 1; y < 9; ++y) {
		for (char x = 1; x < 9; ++x) {
			if (board[y][x] == *player) {
				for (char xx = -1; xx < 2; ++xx) {
					for (char yy = -1; yy < 2; ++yy) {
						if (board[y + yy][x + xx] == (3 - *player)) {
							char xxx = x + xx;
							char yyy = y + yy;
							while (board[yyy][xxx] == (3 - *player)) {
								xxx += xx;
								yyy += yy;
							}
							if (board[yyy][xxx] == 0) {
								canput[yyy][xxx] = true;
							}
						}
					}
				}
			}
		}
	}
}

void reverse1(char **x, char **y) {
	if (board[**y][**x] == player) {
		for (char xx = -1; xx < 2; ++xx) {
			for (char yy = -1; yy < 2; ++yy) {
				if (xx != 0 || yy != 0) {
					if (board[**y + yy][**x + xx] == (3 - player)) {
						char xxx = **x + xx;
						char yyy = **y + yy;
						while (board[yyy][xxx] == (3 - player)) {
							xxx += xx;
							yyy += yy;
						}
						if (board[yyy][xxx] == player) {
							xxx -= xx;
							yyy -= yy;
							while (board[yyy][xxx] == (3 - player)) {
								board[yyy][xxx] = player;
								yyy -= yy;
								xxx -= xx;
							}
						}
					}
				}
			}
		}
	}
}

void reverse(int x, int y) {
	if (board[y][x] == player) {
		for (char xx = -1; xx < 2; ++xx) {
			for (char yy = -1; yy < 2; ++yy) {
				if (xx != 0 || yy != 0) {
					if (board[y + yy][x + xx] == (3 - player)) {
						char xxx = x + xx;
						char yyy = y + yy;
						while (board[yyy][xxx] == (3 - player)) {
							xxx += xx;
							yyy += yy;
						}
						if (board[yyy][xxx] == player) {
							xxx -= xx;
							yyy -= yy;
							while (board[yyy][xxx] == (3 - player)) {
								board[yyy][xxx] = player;
								yyy -= yy;
								xxx -= xx;
							}
						}
					}
				}
			}
		}
	}
}

void reverse2(int *x, int *y, char board[10][10]) {
	if (board[*y][*x] == player) {
		for (char xx = -1; xx < 2; ++xx) {
			for (char yy = -1; yy < 2; ++yy) {
				if (xx != 0 || yy != 0) {
					if (board[*y + yy][*x + xx] == (3 - player)) {
						char xxx = *x + xx;
						char yyy = *y + yy;
						while (board[yyy][xxx] == (3 - player)) {
							xxx += xx;
							yyy += yy;
						}
						if (board[yyy][xxx] == player) {
							xxx -= xx;
							yyy -= yy;
							while (board[yyy][xxx] == (3 - player)) {
								board[yyy][xxx] = player;
								yyy -= yy;
								xxx -= xx;
							}
						}
					}
				}
			}
		}
	}
}

void rebuild(bool bot) {
	for (char i = 0; i <= 9; ++i) {
		for (char j = 0; j <= 9; ++j) {
			canPut[i][j] = false;
		}
	}
	countstone();
	if (bot == false)
		check2(&player);
}

int putstone(int py, int px) {
	tmpx = px;
	tmpy = py;
	if (px > 8 || py > 8) {
		printf("[*]そこには置けません\n");
		return 0;
	}
	if (canPut[py][px] == true) {
		if (player == 1) {
			board[py][px] = 1;
			reverse(px, py);
			player = 2;
			turn++;
			skip = false;
			return 1;
		} else if (player == 2) {
			board[py][px] = 2;
			reverse(px, py);
			player = 1;
			turn++;
			skip = false;
			return 2;
		} else {
			return 0;
		}
	} else {
		printf("[*]そこには置けません\n");
		return 0;
	}
}

int putstone2(char *py, char *px, char *player, bool canput[10][10]) {
	if (*px > 9 || *py > 9) {
		printf("[*]そこには置けません\n");
		return 0;
	}
	if (canput[*py][*px] == true) {
		if (*player == 1) {
			board[*py][*px] = 1;
			reverse1(&px, &py);
			skip = false;
			return 1;
		} else if (*player == 2) {
			board[*py][*px] = 2;
			reverse1(&px, &py);
			skip = false;
			return 2;
		} else {
			return 0;
		}
	} else {
		printf("[*]そこには置けません\n");
		return 0;
	}
}

int putstone3(int py, int px, int *player, bool canput[10][10], char board[10][10]) {
	if (px > 9 || py > 9) {
		printf("[*]そこには置けません\n");
		return 0;
	}
	if (canput[py][px] == true) {
		if (*player == 1) {
			board[py][px] = 1;
			reverse2(&px, &py, board);
			skip = false;
			return 1;
		} else if (*player == 2) {
			board[py][px] = 2;
			reverse2(&px, &py, board);
			skip = false;
			return 2;
		} else {
			return 0;
		}
	} else {
		printf("[*]そこには置けません\n");
		return 0;
	}
}

void countstone(void) {
	blackc = 0;
	whitec = 0;
	for (char i = 1; i <= 8; ++i) {
		for (char j = 1; j <= 8; ++j) {
			if (board[i][j] == 1) {
				blackc++;
			} else if (board[i][j] == 2) {
				whitec++;
			}
		}
	}
}

int winner(void) {
	if (blackc > whitec) {
		return 1;
	} else if (blackc < whitec) {
		return 2;
	} else if (blackc == whitec) {
		return 0;
	} else {
		return 0;
	}
}

int finishedsw(void) {
	printf("黒: %d 白: %d\n", blackc, whitec);
	if (finished == 1) {
		return 1;
	} else {
		return 0;
	}
}

int countstoneswift(int c) {
	if (c == 1) {
		return blackc;
	} else if (c == 2) {
		return whitec;
	} else {
		return 0;
	}
}

int ai2(bool multi) {
	if (player == 1 || finished == 1) return 0;
	isbot = true;
	printf("[*]Botが考え中..\n");
	if(turn > 47) DEPTH = 12;
	nega_alpha(DEPTH, player, -9999, 9999, turn);
	putstone(tmpy, tmpx);
	check2(&player);
	isbot = false;
	return 1;
}

int nega_alpha(int depth, char playerrn, int alpha, int beta, int turn) {
	if (depth == 0) {
		bool canput[10][10] = {{false}};
		check3(&playerrn, canput);
		return countscore(board, &turn, canput, &playerrn);
	}
	
	int var;
	char i, j;
	char tmpboard[10][10] = {{0}};
	bool canput[10][10] = {{false}};
	
	memcpy(tmpboard, board, sizeof(board));
	
	check3(&playerrn, canput);
	for (i = 1; i <= 8; ++i) {
		for (j = 1; j <= 8; ++j) {
			if (canput[i][j] == true) {
				putstone2(&i, &j, &playerrn, canput);
				
				if (putableto(&playerrn) == true) {
					var = -nega_alpha(depth - 1, 3 - playerrn, -beta, -alpha, turn + 1);
				} else {
					var = -nega_alpha(depth, playerrn, -beta, -alpha, turn + 1);
				}
			
				memcpy(board, tmpboard, sizeof(board));
				
				if (var > alpha) {
					alpha = var;
					if (depth == DEPTH) {
						tmpx = j;
						tmpy = i;
						printf("best place is (%d, %d), score %d\n", j, i, var);
					}
					if (alpha >= beta) return alpha;
				}
			}
		}
	}
	if (alpha == -9999) {
		check3(&playerrn, canput);
		alpha = countscore(board, &turn, canput, &playerrn);
	}
	return alpha;
}

int returnplayer(void) { return player; }

bool putableto(char *player) {
	char y, x, xx, yy, xxx, yyy;
	for (y = 1; y < 9; ++y) {
		for (x = 1; x < 9; ++x) {
			if (board[y][x] == 3-*player) {
				for (xx = -1; xx < 2; ++xx) {
					for (yy = -1; yy < 2; ++yy) {
						if (board[y + yy][x + xx] == (*player)) {
							xxx = x + xx;
							yyy = y + yy;
							while (board[yyy][xxx] == (*player)) {
								xxx += xx;
								yyy += yy;
							}
							if (yyy <= 8 && xxx <= 8 && yyy >= 1 && xxx >= 1 && board[yyy][xxx] == 0) return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool putableto2(char player, char board[10][10]) {
	bool canput[10][10];
	for (char y = 1; y < 9; ++y) {
		for (char x = 1; x < 9; ++x) {
			if (board[y][x] == player) {
				for (char xx = -1; xx < 2; ++xx) {
					for (char yy = -1; yy < 2; ++yy) {
						if (board[y + yy][x + xx] == (3 - player)) {
							char xxx = x + xx;
							char yyy = y + yy;
							while (board[yyy][xxx] == (3 - player)) {
								xxx += xx;
								yyy += yy;
							}
							if (board[yyy][xxx] == 0) {
								canput[yyy][xxx] = true;
							}
						}
					}
				}
			}
		}
	}
	for (char i = 1; i < 9; ++i) {
		for (char j = 1; j < 9; ++j) {
			if (canput[i][j] == true) {
				return true;
			}
		}
	}
	return false;
}
