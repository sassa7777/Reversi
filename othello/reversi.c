//
//  othello.c
//  othello
//
//  Created by sasa on 2023/09/30.
//

#include "reversi.h"
#include "evaluation.c"
#include "Objective-C-Wrapper.h"

void reset(void) {
	printf("[*]初期化中...\n");
	printf("DEPTH=%d\n", DEPTH);
	printf("Player: %d\n", botplayer);
	memset(canPut, 0, sizeof(canPut));
	for (char i = 0; i <= 9; ++i) {
		for (char j = 0; j <= 9; ++j) {
			if (i == 0 || i == 9 || j == 0 || j == 9) {
				board[i][j] = 3;
			} else {
				board[i][j] = 0;
			}
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
	return;
}

void check(int *Player) {
	char x, y, xx, yy, xxx, yyy;
	skip = false;
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
								skip = true;
								skipped = false;
							}
						}
					}
				}
			}
		}
	}
	
	if(skip) return;
	
	if (skipped) {
		printf("終了\n");
		finished = 1;
		return;
	}
	
	if (*Player == 1) {
		printf("Player:%dをスキップ\n", player);
		skipped = true;
		player = 2;
		check(&player);
	} else if (*Player == 2) {
		printf("Player:%dをスキップ\n", player);
		skipped = true;
		player = 1;
		check(&player);
	}
}

void check2(char *player, bool canput[10][10], char board[10][10]) {
	char yy, xx, yyy, xxx;
	char playerVal = *player;
	char opponentVal = 3 - *player;
	for (char y = 1; y <= 8; ++y) {
		for (char x = 1; x <= 8; ++x) {
			if (board[y][x] == playerVal) {
				for (yy = -1; yy <= 1; ++yy) {
					for (xx = -1; xx <= 1; ++xx) {
						if (xx == 0 && yy == 0) continue;
						if (board[y + yy][x + xx] == opponentVal) {
							xxx = x + xx;
							yyy = y + yy;
							while (board[yyy][xxx] == opponentVal) {
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

void reverse2(char **x, char **y, char **player, char board[10][10]) {
	char xx, yy, xxx, yyy, playerVal = **player, opponentVal = 3 - **player;
	for (xx = -1; xx <= 1; ++xx) {
		for (yy = -1; yy <= 1; ++yy) {
			if (xx == 0 && yy == 0) continue;
			if (board[**y + yy][**x + xx] == opponentVal) {
				xxx = **x + xx;
				yyy = **y + yy;
				while (board[yyy][xxx] == opponentVal) {
					xxx += xx;
					yyy += yy;
				}
				if (board[yyy][xxx] == playerVal) {
					xxx -= xx;
					yyy -= yy;
					while (board[yyy][xxx] == opponentVal) {
						board[yyy][xxx] = playerVal;
						xxx -= xx;
						yyy -= yy;
					}
				}
			}
		}
	}
}


void rebuild(void) {
	memset(canPut, 0, sizeof(canPut));
	countstone();
	check(&player);
}

int putstone(int py, int px) {
	tmpx = px;
	tmpy = py;
	if (px > 8 || py > 8) {
		printf("[*]そこには置けません\n");
		return 0;
	}
	if (canPut[py][px]) {
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

void putstone2(char *py, char *px, char *player, char board[10][10]) {
		board[*py][*px] = *player;
		reverse2(&px, &py, &player, board);
		skip = false;
		return;
}

void countstone(void) {
	blackc = 0;
	whitec = 0;
	for (char i = 1; i <= 8; ++i) {
		for (char j = 1; j <= 8; ++j) {
			switch (board[i][j]) {
				case 1:
					blackc++;
					break;
				case 2:
					whitec++;
					break;
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

int ai(void) {
	if (player == (3-botplayer) || finished == 1) return 0;
	isbot = true;
	printf("[*]Botが考え中..\n");
	tmpx = 0;
	tmpy = 0;
	think_percent = 0;
	update_hakostring();
	think_count = 100/putable_counter(&player, board);
	//nega_alpha(DEPTH, player, -32767, 32767, false);
	negaalphaTH();
	if(tmpx == 0 || tmpy == 0) error_hakostring();
	think_percent = 100;
	update_hakostring();
	putstone(tmpy, tmpx);
	check(&player);
	isbot = false;
	return 1;
}

void copyboard(char src[10][10], char dest[10][10]) {
	for (char i = 0; i<10; ++i) {
		for (char j = 0; j<10; ++j) {
			dest[i][j] = src[i][j];
		}
	}
}

int nega_alpha(char depth, char playerrn, int alpha, int beta,  bool passed) {
	if (depth == 0) return countscore(board, &playerrn);
	int var, max_score = -32767;
	char tmpboard[10][10];
	bool canput[10][10] = {{false}};
	copyboard(board, tmpboard);
	
	check2(&playerrn, canput, board);
	for (char i = 0; i <= 63; ++i) {
		if (canput[moveorder[i][1]][moveorder[i][0]]) {
			putstone2(&moveorder[i][1], &moveorder[i][0], &playerrn, board);
			var = -nega_alpha(depth-1, 3-playerrn, -beta, -alpha, false);
			copyboard(tmpboard, board);
			if (var > alpha) {
				alpha = var;
				if (depth == DEPTH) {
					tmpx = moveorder[i][0];
					tmpy = moveorder[i][1];
				}
			}
			if(depth == DEPTH) {
				think_percent += think_count;
				update_hakostring();
			}
			if (var >= beta) return var;
			if(alpha > max_score) max_score = alpha;
		}
	}
	if (max_score == -32767) {
		if(passed) return countscore(board, &playerrn);
		return -nega_alpha(depth, 3-playerrn, -beta, -alpha, true);
	}
	return max_score;
}

void negaalphaTH(void) {
	memset(cachex, 0, sizeof(cachex));
	memset(cachey, 0, sizeof(cachey));
	memset(putable_list, 0, sizeof(putable_list));
	ALPHA = -32767;
	char putable_num = putable_counter(&player, board);
	putable_saver(player, board);
//	pthread_t thread[putable_num];
//	pthread_attr_t attr[putable_num];
//	for (int i=0; i<putable_num; ++i) {
//		pthread_attr_init(&attr[i]);
//		pthread_attr_set_qos_class_np(&attr[i], QOS_CLASS_USER_INTERACTIVE, 1);
//	}
//	pthread_create(&thread[0], NULL, negaalphat1, NULL);
//	pthread_create(&thread[1], NULL, negaalphat2, NULL);
//	pthread_create(&thread[2], NULL, negaalphat3, NULL);
//	pthread_create(&thread[3], NULL, negaalphat4, NULL);
//	pthread_join(thread[0], NULL);
//	pthread_join(thread[1], NULL);
//	pthread_join(thread[2], NULL);
//	pthread_join(thread[3], NULL);
//	int task[putable_num];
#pragma omp parallel for num_threads(6)
	for (int i=0; i<putable_num; ++i) {
		//task[i] = i;
		//pthread_create(&thread[i], &attr[i], negaalphat, (void*)&task[i]);
		negaalpha_omp(i);
	}
//	for (int i=0; i<putable_num; ++i) {
//		pthread_join(thread[i], NULL);
//	}
//	for (int i=0; i<64; i+=4) {
//		pthread_join(thread[i], NULL);
//		pthread_join(thread[i+1], NULL);
//		pthread_join(thread[i+2], NULL);
//		pthread_join(thread[i+3], NULL);
//	}
	int max = -100001;
	for (char i = 0; i < putable_num; ++i) {
		if(max < results[i]) max = results[i];
	}
	for (char i = 0; i < putable_num; ++i) {
		if(results[i] == max)
		{
			tmpx = cachex[i];
			tmpy = cachey[i];
			break;
		}
	}
	printf("chose (%d, %d)\n", tmpx, tmpy);
}

int nega_alphadeep(char depth, char playerrn, int alpha, int beta, bool passed, char board[10][10]) {
	int var, max_score = -32767;
	char tmpboard[10][10];
	bool canput[10][10] = {{false}};
	copyboard(board, tmpboard);
	check2(&playerrn, canput, board);
	if(depth == 1) {
		for (char i = 0; i <= 63; ++i) {
			if (canput[moveorder[i][0]][moveorder[i][1]]) {
				putstone2(&moveorder[i][0], &moveorder[i][1], &playerrn, board);
				var = countscore(board, &playerrn);
				copyboard(tmpboard, board);
				
				if(var >= beta) return var;
				if(var > alpha) alpha = var;
				if(alpha > max_score) max_score = alpha;
			}
		}
	} else {
		for (char i = 0; i <= 63; ++i) {
			if (canput[moveorder[i][0]][moveorder[i][1]]) {
				putstone2(&moveorder[i][0], &moveorder[i][1], &playerrn, board);
				var = -nega_alphadeep(depth-1, 3-playerrn, -beta, -alpha, false, board);
				copyboard(tmpboard, board);
				
				if(var >= beta) return var;
				if(var > alpha) alpha = var;
				if(alpha > max_score) max_score = alpha;
			}
		}
	}
	
	if (max_score == -32767) {
		if(passed) return countscore(board, &playerrn);
		return -nega_alphadeep(depth, 3-playerrn, -beta, -alpha, true, board);
	}
	return max_score;
}

void* negaalphat(void* args) {
	int task = *((int*)args);
	int var;
	char playerrn = player;
	int  maxscore = -32767;
	char tmpboard[10][10] = {{0}};
		
	copyboard(board, tmpboard);
	
	putstone2(&putable_list[task][1], &putable_list[task][0], &playerrn, tmpboard);
	
	var = -nega_alphadeep(DEPTH-1, 3-playerrn, -32767, -ALPHA, false, tmpboard);
	
	copyboard(board, tmpboard);
	
	if (var > ALPHA) {
		ALPHA = var;
		maxscore = var;
		cachex[task] = putable_list[task][0];
		cachey[task] = putable_list[task][1];
	}
	think_percent += think_count;
	update_hakostring();
	results[task] = maxscore;
	pthread_exit(0);
}

void negaalpha_omp(int task) {
	int var;
	char playerrn = player;
	int  maxscore = -32767;
	char tmpboard[10][10] = {{0}};
		
	copyboard(board, tmpboard);
	
	putstone2(&putable_list[task][1], &putable_list[task][0], &playerrn, tmpboard);
	
	var = -nega_alphadeep(DEPTH-1, 3-playerrn, -32767, -ALPHA, false, tmpboard);
	
	copyboard(board, tmpboard);
	
	if (var > ALPHA) {
		ALPHA = var;
		maxscore = var;
		cachex[task] = putable_list[task][0];
		cachey[task] = putable_list[task][1];
	}
	think_percent += think_count;
	update_hakostring();
	results[task] = maxscore;
	return;
}

int returnplayer(void) { return player; }


int putable_counter(int *player, char board[10][10]) {
	char y, x, yy, xx, yyy, xxx;
	int count = 0;
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
								count++;
							}
						}
					}
				}
			}
		}
	}
	return count;
}

void putable_saver(int player, char board[10][10]) {
	char y, x, yy, xx, yyy, xxx;
	char i=0;
	for (y = 1; y < 9; ++y) {
		for (x = 1; x < 9; ++x) {
			if (board[y][x] == player) {
				for (xx = -1; xx < 2; ++xx) {
					for (yy = -1; yy < 2; ++yy) {
						if (board[y + yy][x + xx] == (3 - player)) {
							xxx = x + xx;
							yyy = y + yy;
							while (board[yyy][xxx] == (3 - player)) {
								xxx += xx;
								yyy += yy;
							}
							if (board[yyy][xxx] == 0) {
								putable_list[i][0]=xxx;
								putable_list[i][1]=yyy;
								i++;
							}
						}
					}
				}
			}
		}
	}
}
