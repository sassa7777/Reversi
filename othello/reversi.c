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
	memset(board, 0, sizeof(board));
	memset(canPut, 0, sizeof(canPut));
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
	return;
}

void check2(int *Player) {
	char x, y, xx, yy, xxx, yyy;
	for (y = 1; y < 9; ++y) {
		for (x = 1; x < 9; ++x) {
			if (board[y][x] == *Player) {
				for (xx = -1; xx < 2; ++xx) {
					for (yy = -1; yy < 2; ++yy) {
						if (board[y + yy][x + xx] == (3 - *Player)) {
							xxx = x + xx;
							yyy = y + yy;
							while (board[yyy][xxx] == (3 - *Player)) {
								xxx += xx;
								yyy += yy;
							}
							if (board[yyy][xxx] == 0) {
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

void check4(char *player, bool canput[10][10], char board[10][10]) {
	char y, x, yy, xx, yyy, xxx;
	for (y = 1; y < 9; ++y) {
		for (x = 1; x < 9; ++x) {
			if (board[y][x] == *player) {
				for (xx = -1; xx < 2; ++xx) {
					for (yy = -1; yy < 2; ++yy) {
						if (board[y + yy][x + xx] == (3 - *player)) {
							xxx = x + xx;
							yyy = y + yy;
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

void reverse(int x, int y) {
	if (board[y][x] == player) {
		char xx, yy, xxx, yyy;
		for (xx = -1; xx < 2; ++xx) {
			for (yy = -1; yy < 2; ++yy) {
				if (xx != 0 || yy != 0) {
					if (board[y + yy][x + xx] == (3 - player)) {
						xxx = x + xx;
						yyy = y + yy;
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

void reverse2(char **x, char **y, char board[10][10]) {
	if (board[**y][**x] == player) {
		char xx, yy, xxx, yyy;
		for (xx = -1; xx < 2; ++xx) {
			for (yy = -1; yy < 2; ++yy) {
				if (xx != 0 || yy != 0) {
					if (board[**y + yy][**x + xx] == (3 - player)) {
						xxx = **x + xx;
						yyy = **y + yy;
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
		printf("[*]そこには置けません\n (%d, %d)", px, py);
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
		printf("[*]そこには置けません\n (%d, %d)", px, py);
		return 0;
	}
}

int putstone3(char *py, char *px, char *player, bool canput[10][10], char board[10][10]) {
	if (*px > 9 || *py > 9) {
		printf("[*]そこには置けません\n (%d, %d)", *px, *py);
		return 0;
	}
	if (canput[*py][*px] == true) {
		if (*player == 1) {
			board[*py][*px] = 1;
			reverse2(&px, &py, board);
			skip = false;
			return 1;
		} else if (*player == 2) {
			board[*py][*px] = 2;
			reverse2(&px, &py, board);
			skip = false;
			return 2;
		} else {
			return 0;
		}
	} else {
		printf("[*]そこには置けません\n (%d, %d)", *px, *py);
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
	tmpx = 0;
	tmpy = 0;
rerun:
	nega_alpha(DEPTH, player, -100000, 100000, turn);
	if(tmpx == 0 || tmpy == 0)
	{
		printf("ERROR\n");
		goto rerun;
	}
	putstone(tmpy, tmpx);
	check2(&player);
	isbot = false;
	return 1;
}

int nega_alpha(int depth, char playerrn, int alpha, int beta, int turn) {
	if (depth == 0) {
		bool canput[10][10] = {{false}};
		check4(&playerrn, canput, board);
		return countscore(board, &turn, canput, &playerrn);
	}
	int var;
	char tmpboard[10][10] = {{0}};
	bool canput[10][10] = {{false}};
		
	memcpy(tmpboard, board, sizeof(tmpboard));
	
	check4(&playerrn, canput, board);
	for (char i = 0; i <= 63; ++i) {
		if (canput[moveorder[0][i]][moveorder[1][i]] == true) {
			putstone3(&moveorder[0][i], &moveorder[1][i], &playerrn, canput, board);
			
			if (putableto2(&playerrn, board) == true) {
				var = -nega_alpha(depth - 1, 3 - playerrn, -beta, -alpha, turn + 1);
			} else {
				var = nega_alpha(depth, playerrn, alpha, beta, turn + 1);
			}
			
			memcpy(board, tmpboard, sizeof(tmpboard));
			
			if (var > alpha) {
				alpha = var;
				if (depth == DEPTH) {
					tmpx = moveorder[1][i];
					tmpy = moveorder[0][i];
					printf("best place is (%d, %d), score %d\n", tmpx, tmpy, var);
				}
				if (alpha >= beta) return alpha;
			}
		}
	}
	if (alpha == -100000) {
		check4(&playerrn, canput, board);
		alpha = countscore(board, &turn, canput, &playerrn);
	}
	return alpha;
}

int returnplayer(void) { return player; }

bool putableto2(char *player, char board[10][10]) {
	char y, x, xx, yy, xxx, yyy;
	bool canput[10][10] = {{false}};
	for (y = 1; y <= 8; ++y) {
		for (x = 1; x <= 8; ++x) {
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
							if (yyy <= 8 && yyy >= 1 && xxx <= 8 && xxx >= 1 && board[yyy][xxx] == 0) canput[yyy][xxx] = true;
						}
					}
				}
			}
		}
	}
	for (y = 1; y <= 8; ++y) {
		for (x = 1; x <= 8; ++x) {
			if(canput[y][x] == true) return true;
		}
	}
	return false;
}
