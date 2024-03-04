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

void rebuild(void) {
	memset(canPut, 0, sizeof(canPut));
	countstone();
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

int putstone3(char *py, char *px, char *player, bool canput[10][10], char board[10][10]) {
	if (*px > 9 || *py > 9) {
		printf("[*]そこには置けません\n");
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

int ai2(void) {
	if (player == 1 || finished == 1) return 0;
	isbot = true;
	printf("[*]Botが考え中..\n");
	tmpx = 0;
	tmpy = 0;
rerun:
	//nega_alpha(DEPTH, player, -100000, 100000, turn);
	negaalphaTH();
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

void negaalphaTH(void)
{
	memset(cachex, 0, sizeof(cachex));
	memset(cachey, 0, sizeof(cachey));
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;
	pthread_create(&thread1, NULL, negaalphat1, NULL);
	pthread_create(&thread2, NULL, negaalphat2, NULL);
	pthread_create(&thread3, NULL, negaalphat3, NULL);
	pthread_create(&thread4, NULL, negaalphat4, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	
	int max = -100001;
	for (char i = 0; i < 4; ++i) {
		if(max < results[i]) max = results[i];
	}
	for (char i = 0; i < 4; ++i) {
		if(results[i] == max)
		{
			tmpx = cachex[i];
			tmpy = cachey[i];
			break;
		}
	}
	printf("chose (%d, %d)\n", tmpx, tmpy);
}

void* negaalphat1(void* args)
{
	int var;
	char playerrn = player;
	int depth = DEPTH, alpha = -32767, beta = 32767;
	char tmpboard[10][10] = {{0}};
	bool canput[10][10] = {{false}};
		
	memcpy(tmpboard, board, sizeof(tmpboard));
	
	check4(&playerrn, canput, tmpboard);
	for (char i = 1; i <= 4; ++i) {
		for (char j = 1; j <= 4; ++j) {
			if (canput[i][j] == true) {
				putstone3(&i, &j, &playerrn, canput, tmpboard);
				
				if (putableto2(&playerrn, tmpboard) == true) {
					var = -nega_alphadeep(depth - 1, 3 - playerrn, -beta, -alpha, turn + 1, tmpboard);
				} else {
					var = nega_alphadeep(depth-1, playerrn, alpha, beta, turn + 1, tmpboard);
				}
				
				memcpy(tmpboard, board, sizeof(tmpboard));
				
				if (var > alpha) {
					alpha = var;
					cachex[0] = j;
					cachey[0] = i;
					printf("    best place is (%d, %d), score %d\n", j, i, var);
				}
			}
		}
	}
	results[0] = alpha;
	printf("TH1 best place is (%d, %d), score %d\n", cachex[0], cachey[0], alpha);
	pthread_exit(0);
}

void* negaalphat2(void* args)
{
	int var;
	char playerrn = player;
	int depth = DEPTH, alpha = -32767, beta = 32767;
	char tmpboard[10][10] = {{0}};
	bool canput[10][10] = {{false}};
		
	memcpy(tmpboard, board, sizeof(tmpboard));
	
	check4(&playerrn, canput, tmpboard);
	for (char i = 1; i <= 4; ++i) {
		for (char j = 5; j <= 8; ++j) {
			if (canput[i][j] == true) {
				putstone3(&i, &j, &playerrn, canput, tmpboard);
				
				if (putableto2(&playerrn, tmpboard) == true) {
					var = -nega_alphadeep(depth - 1, 3 - playerrn, -beta, -alpha, turn + 1, tmpboard);
				} else {
					var = nega_alphadeep(depth-1, playerrn, alpha, beta, turn + 1, tmpboard);
				}
				
				memcpy(tmpboard, board, sizeof(tmpboard));
				
				if (var > alpha) {
					alpha = var;
					cachex[1] = j;
					cachey[1] = i;
					printf("    best place is (%d, %d), score %d\n", j, i, var);
				}
			}
		}
	}
	results[1] = alpha;
	printf("TH2 best place is (%d, %d), score %d\n", cachex[1], cachey[1], alpha);
	pthread_exit(0);
}

void* negaalphat3(void* args)
{
	int var;
	char playerrn = player;
	int depth = DEPTH, alpha = -32767, beta = 32767;
	char tmpboard[10][10] = {{0}};
	bool canput[10][10] = {{false}};
		
	memcpy(tmpboard, board, sizeof(tmpboard));
	
	check4(&playerrn, canput, tmpboard);
	for (char i = 5; i <= 8; ++i) {
		for (char j = 1; j <= 4; ++j) {
			if (canput[i][j] == true) {
				putstone3(&i, &j, &playerrn, canput, tmpboard);
				
				if (putableto2(&playerrn, tmpboard) == true) {
					var = -nega_alphadeep(depth - 1, 3 - playerrn, -beta, -alpha, turn + 1, tmpboard);
				} else {
					var = nega_alphadeep(depth-1, playerrn, alpha, beta, turn + 1, tmpboard);
				}
				
				memcpy(tmpboard, board, sizeof(tmpboard));
				
				if (var > alpha) {
					alpha = var;
					cachex[2] = j;
					cachey[2] = i;
					printf("    best place is (%d, %d), score %d\n", j, i, var);
				}
			}
		}
	}
	results[2] = alpha;
	printf("TH3 best place is (%d, %d), score %d\n", cachex[2], cachey[2], alpha);
	pthread_exit(0);
}

void* negaalphat4(void* args)
{
	int var;
	char playerrn = player;
	int depth = DEPTH, alpha = -32767, beta = 32767;
	char tmpboard[10][10] = {{0}};
	bool canput[10][10] = {{false}};
		
	memcpy(tmpboard, board, sizeof(tmpboard));
	
	check4(&playerrn, canput, tmpboard);
	for (char i = 5; i <= 8; ++i) {
		for (char j = 5; j <= 8; ++j) {
			if (canput[i][j] == true) {
				putstone3(&i, &j, &playerrn, canput, tmpboard);
				
				if (putableto2(&playerrn, tmpboard) == true) {
					var = -nega_alphadeep(depth - 1, 3 - playerrn, -beta, -alpha, turn + 1, tmpboard);
				} else {
					var = nega_alphadeep(depth-1, playerrn, alpha, beta, turn + 1, tmpboard);
				}
				
				memcpy(tmpboard, board, sizeof(tmpboard));
				
				if (var > alpha) {
					alpha = var;
					cachex[3] = j;
					cachey[3] = i;
					printf("    best place is (%d, %d), score %d\n", j, i, var);
				}
			}
		}
	}
	results[3] = alpha;
	printf("TH4 best place is (%d, %d), score %d\n", cachex[3], cachey[3], alpha);
	pthread_exit(0);
}

int nega_alphadeep(int depth, char playerrn, int alpha, int beta, int turn, char board[10][10]) {
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
				var = -nega_alphadeep(depth - 1, 3 - playerrn, -beta, -alpha, turn + 1, board);
			} else {
				var = nega_alphadeep(depth-1, playerrn, alpha, beta, turn + 1, board);
			}
			
			memcpy(board, tmpboard, sizeof(tmpboard));
			
			if (var > alpha) {
				alpha = var;
				if (alpha >= beta) return alpha;
			}
		}
	}
	if (alpha == -32767) {
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
							if (board[yyy][xxx] == 0) canput[yyy][xxx] = true;
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
