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
	DEPTH = 8;
	return;
}

void check2(int Player) {
	for (char y = 1; y < 9; ++y) {
		for (char x = 1; x < 9; ++x) {
			if (board[y][x] == Player) {
				for (char xx = -1; xx < 2; ++xx) {
					for (char yy = -1; yy < 2; ++yy) {
						if (board[y + yy][x + xx] == (3 - Player)) {
							char xxx = x + xx;
							char yyy = y + yy;
							while (board[yyy][xxx] == (3 - Player)) {
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
	for (char i = 0; i <= 9; ++i) {
		canPut[0][i] = false;
		canPut[9][i] = false;
		canPut[i][0] = false;
		canPut[i][9] = false;
	}
	
	skip = false;
	for (char i = 1; i < 9; ++i) {
		for (char j = 1; j < 9; ++j) {
			if (canPut[i][j] == true) {
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
	
	if (Player == 1) {
		printf("Player:%dをスキップ\n", player);
		skipped = true;
		player = 2;
		check2(player);
	} else if (Player == 2) {
		printf("Player:%dをスキップ\n", player);
		skipped = true;
		player = 1;
		check2(player);
	}
}

void check3(int player, bool canput[10][10]) {
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
	for (char i = 0; i <= 9; ++i) {
		canput[0][i] = false;
		canput[9][i] = false;
		canput[i][0] = false;
		canput[i][9] = false;
	}
	
	skip = false;
	for (char i = 1; i < 9; ++i) {
		for (char j = 1; j < 9; ++j) {
			if (canput[i][j] == true) {
				skip = true;
				return;
			}
		}
	}
}

void check4(int player, bool canput[10][10], char board[10][10]) {
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

void reverse2(int x, int y, char board[10][10]) {
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

void rebuild(bool bot) {
	for (char i = 0; i <= 9; ++i) {
		for (char j = 0; j <= 9; ++j) {
			canPut[i][j] = false;
		}
	}
	countstone();
	if (bot == false)
		check2(player);
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

int putstone2(int py, int px, int player, bool canput[10][10]) {
	if (px > 9 || py > 9) {
		printf("[*]そこには置けません\n");
		return 0;
	}
	if (canput[py][px] == true) {
		if (player == 1) {
			board[py][px] = 1;
			reverse(px, py);
			skip = false;
			return 1;
		} else if (player == 2) {
			board[py][px] = 2;
			reverse(px, py);
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

int putstone3(int py, int px, int player, bool canput[10][10], char board[10][10]) {
	if (px > 9 || py > 9) {
		printf("[*]そこには置けません\n");
		return 0;
	}
	if (canput[py][px] == true) {
		if (player == 1) {
			board[py][px] = 1;
			reverse2(px, py, board);
			skip = false;
			return 1;
		} else if (player == 2) {
			board[py][px] = 2;
			reverse2(px, py, board);
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
	for (char i = 0; i <= 9; ++i) {
		for (char j = 0; j <= 9; ++j) {
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
	if(turn > 43) DEPTH = 16;
	nega_alpha(DEPTH, player, -9999, 9999, turn);
	//nega_alpha_thread(DEPTH, player, -9999, 9999, turn);
	putstone(tmpy, tmpx);
	check2(1);
	isbot = false;
	return 1;
}

void nega_alpha_thread(int depth, int playerrn, int ALPHA, int BETA, int turn) {
	
	int ThreadArgs[] = {DEPTH, player, -9999, turn};
	int ret;
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;
th1:
	ret = pthread_create(&thread1, NULL, nega_alpha_thread1, (void *)&ThreadArgs);
	if (ret != 0) goto th1;
th2:
	ret = pthread_create(&thread2, NULL, nega_alpha_thread2, (void *)&ThreadArgs);
	if (ret != 0) goto th2;
th3:
	ret = pthread_create(&thread3, NULL, nega_alpha_thread3, (void *)&ThreadArgs);
	if (ret != 0) goto th3;
th4:
	ret = pthread_create(&thread4, NULL, nega_alpha_thread4, (void *)&ThreadArgs);
	if (ret != 0) goto th4;
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	int max = -99999;
	for (char i = 0; i <= 3; ++i) {
		if (result[i] > max) max = result[i];
	}
	for (char i = 0; i <= 3; ++i) {
		if (result[i] == max) {
			tmpx = cachex[i];
			tmpy = cachey[i];
			break;
		}
	}
	printf("choose (%d, %d)\n", tmpx, tmpy);
	return;
}

void *nega_alpha_thread1(void *args) {
	int var;
	bool canput[10][10] = {{false}};
	char threadboard[10][10] = {{0}};
	int depth = DEPTH;
	int playerrn = player;
	int alpha = -9999;
	int turn = 10;
	
	for (char i = 1; i <= 8; ++i) {
		for (char j = 1; j <= 8; ++j) {
			threadboard[i][j] = board[i][j];
		}
	}
	
	check4(playerrn, canput, threadboard);
	for (char i = 1; i <= 4; ++i) {
		for (char j = 1; j <= 4; ++j) {
			if (canput[i][j] == true) {
				putstone3(i, j, playerrn, canput, threadboard);
				
				if (putableto2(3 - playerrn, threadboard) == true) {
					var = -nega_alpha_deepthread(depth - 1, 3 - playerrn, -9999, 9999, turn + 1, threadboard);
				} else {
					var = -nega_alpha_deepthread(depth - 1, playerrn, -9999, 9999, turn + 1, threadboard);
				}
				
				if (var > alpha) {
					alpha = var;
					cachex[0] = j;
					cachey[0] = i;
				}
			}
		}
	}
	printf("TH1 best place is (%d, %d), score %d\n", cachex[0], cachey[1], alpha);
	result[0] = alpha;
	pthread_exit(0);
}

void *nega_alpha_thread2(void *args) {
	int var;
	bool canput[10][10] = {{false}};
	char threadboard[10][10] = {{0}};
	int depth = DEPTH;
	int playerrn = player;
	int alpha = -9999;
	int turn = 10;
	
	for (char i = 1; i <= 8; ++i) {
		for (char j = 1; j <= 8; ++j) {
			threadboard[i][j] = board[i][j];
		}
	}
	
	
	check4(playerrn, canput, threadboard);
	for (char i = 1; i <= 4; ++i) {
		for (char j = 5; j <= 8; ++j) {
			if (canput[i][j] == true) {
				putstone3(i, j, playerrn, canput, threadboard);
				
				if (putableto2(3 - playerrn, threadboard) == true) {
					var = -nega_alpha_deepthread(depth - 1, 3 - playerrn, -9999, 9999, turn + 1, threadboard);
				} else {
					var = -nega_alpha_deepthread(depth - 1, playerrn, -9999, 9999, turn + 1, threadboard);
				}
				
				if (var > alpha) {
					alpha = var;
					cachex[1] = j;
					cachey[1] = i;
				}
			}
		}
	}
	printf("TH2 best place is (%d, %d), score %d\n", cachex[1], cachey[1], alpha);
	result[1] = alpha;
	pthread_exit(0);
}

void *nega_alpha_thread3(void *args) {
	int var;
	bool canput[10][10] = {{false}};
	char threadboard[10][10] = {{0}};
	int depth = DEPTH;
	int playerrn = player;
	int alpha = -9999;
	int turn = 10;
	
	for (char i = 1; i <= 8; ++i) {
		for (char j = 1; j <= 8; ++j) {
			threadboard[i][j] = board[i][j];
		}
	}
	
	
	check4(playerrn, canput, threadboard);
	for (char i = 5; i <= 8; ++i) {
		for (char j = 1; j <= 4; ++j) {
			if (canput[i][j] == true) {
				putstone3(i, j, playerrn, canput, threadboard);
				
				if (putableto2(3 - playerrn, threadboard) == true) {
					var =  -nega_alpha_deepthread(depth - 1, 3 - playerrn, -9999, 9999, turn + 1, threadboard);
				} else {
					var = -nega_alpha_deepthread(depth - 1, playerrn, -9999, 9999, turn + 1, threadboard);
				}
				
				if (var > alpha) {
					alpha = var;
					cachex[2] = j;
					cachey[2] = i;
					printf("TH3 best place is (%d, %d), score %d\n", j, i, var);
				}
			}
		}
	}
	result[2] = alpha;
	pthread_exit(0);
}

void *nega_alpha_thread4(void *args) {
	int var;
	bool canput[10][10] = {{false}};
	char threadboard[10][10] = {{0}};
	int depth = DEPTH;
	int playerrn = player;
	int alpha = -9999;
	int turn = 10;
	
	for (char i = 1; i <= 8; ++i) {
		for (char j = 1; j <= 8; ++j) {
			threadboard[i][j] = board[i][j];
		}
	}
	
	
	check4(playerrn, canput, threadboard);
	for (char i = 5; i <= 8; ++i) {
		for (char j = 5; j <= 8; ++j) {
			if (canput[i][j] == true) {
				putstone3(i, j, playerrn, canput, threadboard);
				
				if (putableto2(3 - playerrn, threadboard) == true) {
					var =  -nega_alpha_deepthread(depth - 1, 3 - playerrn, -9999, 9999, turn + 1, threadboard);
				} else {
					var = -nega_alpha_deepthread(depth - 1, playerrn, -9999, 9999, turn + 1, threadboard);
				}
				
				if (var > alpha) {
					alpha = var;
					cachex[3] = j;
					cachey[3] = i;
				}
			}
		}
	}
	printf("TH4 best place is (%d, %d), score %d\n", cachex[3], cachey[3], alpha);
	result[3] = alpha;
	pthread_exit(0);
}

int nega_alpha_deepthread(int depth, int playerrn, int alpha, int beta, int turn, char board[10][10]) {
	if (depth == 0) {
		bool canput[10][10] = {{false}};
		check4(3 - playerrn, canput, board);
		return countscore(board, turn, canput, &playerrn);
	}
	
	int var;
	char tmpboard[10][10] = {{0}};
	bool canput[10][10] = {{false}};
	
	for (char i = 1; i <= 8; ++i) {
		for (char j = 1; j <= 8; ++j) {
			tmpboard[i][j] = board[i][j];
		}
	}
	
	check4(playerrn, canput, board);
	for (char i = 1; i <= 8; ++i) {
		for (char j = 1; j <= 8; ++j) {
			if (canput[i][j] == true) {
				putstone3(i, j, playerrn, canput, board);
				
				if (putableto2(3 - playerrn, board) == true) {
					var = -nega_alpha_deepthread(depth - 1, 3 - playerrn, -beta, -alpha,
												turn + 1, board);
				} else {
					printf("cant put\n");
					var = -nega_alpha_deepthread(depth - 1, playerrn, -beta, -alpha,
												turn + 1, board);
				}
				
				for (char i = 1; i <= 8; ++i) {
					for (char j = 1; j <= 8; ++j) {
						board[i][j] = tmpboard[i][j];
					}
				}
				
				if(var >= alpha)
				{
					alpha = var;
					if(alpha >= beta) return alpha;
				}
			}
		}
	}
	if (alpha == -9999) {
		check4(3 - playerrn, canput, board);
		return countscore(board, turn, canput, &playerrn);
	}
	return alpha;
}

int nega_alpha(int depth, int playerrn, int alpha, int beta, int turn) {
	if (depth == 0) {
		bool canput[10][10] = {{false}};
		check3(3 - playerrn, canput);
		return countscore(board, turn, canput, &playerrn);
	}
	
	int var;
	char tmpboard[10][10] = {{0}};
	bool canput[10][10] = {{false}};
	
	for (char i = 1; i <= 8; ++i) {
		for (char j = 1; j <= 8; ++j) {
			tmpboard[i][j] = board[i][j];
		}
	}
	
	check3(playerrn, canput);
	for (char i = 1; i < 9; ++i) {
		for (char j = 1; j < 9; ++j) {
			if (canput[i][j] == true) {
				putstone2(i, j, playerrn, canput);
				
				if (putableto(3 - playerrn) == true) {
					var = -nega_alpha(depth - 1, 3 - playerrn, -beta, -alpha, turn + 1);
				} else {
					printf("cant put\n");
					var = -nega_alpha(depth - 1, playerrn, -beta, -alpha, turn + 1);
				}
				
				for (char i = 1; i <= 8; ++i) {
					for (char j = 1; j <= 8; ++j) {
						board[i][j] = tmpboard[i][j];
					}
				}
				
				if (var > alpha) {
					alpha = var;
					if (depth == DEPTH) {
						tmpx = j;
						tmpy = i;
						printf("best place is (%d, %d), score %d\n", j, i, var);
					}
					if (alpha >= beta) {
						return alpha;
					}
				}
			}
		}
	}
	if (alpha == -9999) {
		check3(3 - playerrn, canput);
		alpha = countscore(board, turn, canput, &playerrn);
	}
	return alpha;
}

int returnplayer(void) { return player; }

bool putableto(int player) {
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

bool putableto2(int player, char board[10][10]) {
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
