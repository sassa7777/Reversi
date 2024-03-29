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
	printf("CPU Core count: %d\n", cpu_core);
	memset(canPut, 0, sizeof(canPut));
	nowTurn = BLACK_TURN;
	nowIndex = 1;
	playerboard = 0x0000000810000000;
	oppenentboard = 0x0000001008000000;
	player = 1;
	skip = false;
	finished = 0;
	skipped = false;
	turn = 1;
	return;
}

//void print_board(uint64_t oppenentboard, uint64_t playerboard) {
//	printf("  a b c d e f g h\n");
//	uint64_t mask = 0x8000000000000000;
//	for (int i = 0; i < 8; i++) {
//		printf("%d ", i + 1);
//		for (int j = 0; j < 8; j++) {
//			if (playerboard & mask) {
//				printf("O ");
//			} else if (oppenentboard & mask) {
//				printf("X ");
//			} else {
//				printf(". ");
//			}
//			mask >>= 1;
//		}
//		printf("\n");
//	}
//}

int putstone(char y, char x) {
	y-=1;
	x-=1;
	uint64_t put = cordinate_to_bit(x, y);
	legalboard = makelegalBoard(oppenentboard, playerboard);
	if(canput(put, legalboard)) {
		reversebit(put);
		swapboard();
		player = 3-player;
		if(passorfinish() == 1) {
			swapboard();
			player = 3-player;
		}
		return 1;
	} else {
		return 0;
	}
}

//座標をbitに変換
uint64_t cordinate_to_bit(char x, char y) {
	uint64_t mask = 0x8000000000000000;
	mask = mask >> x;
	mask = mask >> (y*8);
	return mask;
}

bool canput(uint64_t put, uint64_t legalboard) {
	return (put & legalboard) == put;
}

uint64_t makelegalBoard(uint64_t oppenentboard, uint64_t playerboard) {
	const uint64_t horizontalboard = (oppenentboard & 0x7e7e7e7e7e7e7e7e);
	const uint64_t verticalboard = (oppenentboard & 0x00FFFFFFFFFFFF00);
	const uint64_t allsideboard = (oppenentboard & 0x007e7e7e7e7e7e00);
	const uint64_t blankboard = ~(playerboard | oppenentboard);
	
	uint64_t tmp;
	uint64_t legalboard;
	
	//左
	tmp = horizontalboard & (playerboard << 1);
	tmp |= horizontalboard & (tmp << 1);
	tmp |= horizontalboard & (tmp << 1);
	tmp |= horizontalboard & (tmp << 1);
	tmp |= horizontalboard & (tmp << 1);
	tmp |= horizontalboard & (tmp << 1);
	legalboard = blankboard & (tmp << 1);
	
	//右
	tmp = horizontalboard & (playerboard >> 1);
	tmp |= horizontalboard & (tmp >> 1);
	tmp |= horizontalboard & (tmp >> 1);
	tmp |= horizontalboard & (tmp >> 1);
	tmp |= horizontalboard & (tmp >> 1);
	tmp |= horizontalboard & (tmp >> 1);
	legalboard |= blankboard & (tmp >> 1);

	//上
	tmp = verticalboard & (playerboard << 8);
	tmp |= verticalboard & (tmp << 8);
	tmp |= verticalboard & (tmp << 8);
	tmp |= verticalboard & (tmp << 8);
	tmp |= verticalboard & (tmp << 8);
	tmp |= verticalboard & (tmp << 8);
	legalboard |= blankboard & (tmp << 8);
	
	//下
	tmp = verticalboard & (playerboard >> 8);
	tmp |= verticalboard & (tmp >> 8);
	tmp |= verticalboard & (tmp >> 8);
	tmp |= verticalboard & (tmp >> 8);
	tmp |= verticalboard & (tmp >> 8);
	tmp |= verticalboard & (tmp >> 8);
	legalboard |= blankboard & (tmp >> 8);
	
	//右斜め上
	tmp = allsideboard & (playerboard << 7);
	tmp |= allsideboard & (tmp << 7);
	tmp |= allsideboard & (tmp << 7);
	tmp |= allsideboard & (tmp << 7);
	tmp |= allsideboard & (tmp << 7);
	tmp |= allsideboard & (tmp << 7);
	legalboard |= blankboard & (tmp << 7);
	
	//左斜め上
	tmp = allsideboard & (playerboard << 9);
	tmp |= allsideboard & (tmp << 9);
	tmp |= allsideboard & (tmp << 9);
	tmp |= allsideboard & (tmp << 9);
	tmp |= allsideboard & (tmp << 9);
	tmp |= allsideboard & (tmp << 9);
	legalboard |= blankboard & (tmp << 9);
	
	//右斜め下
	tmp = allsideboard & (playerboard >> 9);
	tmp |= allsideboard & (tmp >> 9);
	tmp |= allsideboard & (tmp >> 9);
	tmp |= allsideboard & (tmp >> 9);
	tmp |= allsideboard & (tmp >> 9);
	tmp |= allsideboard & (tmp >> 9);
	legalboard |= blankboard & (tmp >> 9);
	
	//左斜め下
	tmp = allsideboard & (playerboard >> 7);
	tmp |= allsideboard & (tmp >> 7);
	tmp |= allsideboard & (tmp >> 7);
	tmp |= allsideboard & (tmp >> 7);
	tmp |= allsideboard & (tmp >> 7);
	tmp |= allsideboard & (tmp >> 7);
	legalboard |= blankboard & (tmp >> 7);
	
	return legalboard;
}

void reversebit(uint64_t put) {
	uint64_t rev = 0;
	for (char i = 0; i<8; ++i) {
		uint64_t rev_ = 0;
		uint64_t mask = transfer(put, i);
		while ((mask != 0) && ((mask & oppenentboard) != 0)) {
			rev_ |= mask;
			mask = transfer(mask, i);
		}
		if((mask & playerboard) != 0) rev |= rev_;
	}
	//反転
	playerboard ^= (put | rev);
	oppenentboard ^= rev;
	nowIndex++;
}

uint64_t transfer(uint64_t put, char i) {
	switch (i) {
		case 0:
			return (put << 8) & 0xffffffffffffff00;
			break;
		case 1:
			return (put << 7) & 0x7f7f7f7f7f7f7f00;
			break;
		case 2:
			return (put >> 1) & 0x7f7f7f7f7f7f7f7f;
			break;
		case 3:
			return (put >> 9) & 0x007f7f7f7f7f7f7f;
			break;
		case 4:
			return (put >> 8) & 0x00ffffffffffffff;
			break;
		case 5:
			return (put >> 7) & 0x00fefefefefefefe;
			break;
		case 6:
			return (put << 1) & 0xfefefefefefefefe;
			break;
		case 7:
			return (put << 9) & 0xfefefefefefefe00;
			break;
		default:
			printf("error\n");
			return 0;
			break;
	}
}

int passorfinish(void) {
	uint64_t playerlegalboard = makelegalBoard(oppenentboard, playerboard);
	struct tmpBoard {
		int nowTurn;
		int nowIndex;
		uint64_t playerboard;
		uint64_t oppenentboard;
	};
	struct tmpBoard tmpboard;
	tmpboard.nowTurn = nowTurn;
	tmpboard.nowIndex = nowIndex;
	tmpboard.playerboard = oppenentboard;
	tmpboard.oppenentboard = playerboard;
	uint64_t oppenentlegalboard = makelegalBoard(tmpboard.oppenentboard, tmpboard.playerboard);
	if(playerlegalboard == 0x0000000000000000 && oppenentlegalboard != 0x0000000000000000) return 1;
	else if(playerlegalboard == 0x0000000000000000 && oppenentlegalboard == 0x0000000000000000) return 2;
	else return 0;
}

void swapboard(void) {
	uint64_t tmp = playerboard;
	playerboard = oppenentboard;
	oppenentboard = tmp;
	nowTurn*=(-1);
}

int bitcount(uint64_t board) {
	uint64_t mask = 0x8000000000000000;
	int count = 0;
	for (char i = 0; i<64; ++i) {
		if((mask & board) != 0) count++;
		mask = mask >> 1;
	}
	return count;
}

int ai(void) {
	if (player == (3-botplayer) || finished == 1) {
		return 0;
	}
	isbot = true;
	printf("[*]Botが考え中..\n");
	tmpx = 0;
	tmpy = 0;
	think_percent = 0;
	update_hakostring();
	//think_count = 100/putable_counter(&player, board);
	nega_alpha_bit(DEPTH, player, -32767, 32767, false, playerboard, oppenentboard);
	if(tmpx == 0 || tmpy == 0) error_hakostring();
	printf("(%d, %d)\n", tmpx, tmpy);
	think_percent = 100;
	update_hakostring();
	putstone(tmpy, tmpx);
	legalboard = makelegalBoard(oppenentboard, playerboard);
	isbot = false;
	return 1;
}

int nega_alpha_bit(char depth, char playerrn, int alpha, int beta,  bool passed, uint64_t playerboard, uint64_t oppenentboard) {
	if(depth == 0) return countscore(playerboard, oppenentboard, &playerrn);
	int var, max_score = -32767;
	uint64_t tmpplayerboard, tmpoppenentboard;
	tmpplayerboard = playerboard;
	tmpoppenentboard = oppenentboard;
	uint64_t put;
	uint64_t legalboard = makelegalBoard(oppenentboard, playerboard);
	for (char i = 0; i<64; ++i) {
		put = cordinate_to_bit(moveorder[i][0], moveorder[i][1]);
		if(canput(put, legalboard)) {
			reversebit(put);
			var = -nega_alpha_bit(depth-1, 3-playerrn, -beta, -alpha, false, oppenentboard, playerboard);
			playerboard = tmpplayerboard;
			oppenentboard = tmpoppenentboard;
			if(var > alpha) {
				alpha = var;
				if(depth == DEPTH) {
					tmpx = moveorder[i][0];
					tmpy = moveorder[i][1];
				}
			}
			if (var >= beta) return var;
			if(alpha > max_score) max_score = alpha;
		}
	}
	if (max_score == -32767) {
		if(passed) countscore(playerboard, oppenentboard, &playerrn);
		return -nega_alpha_bit(depth, 3-playerrn, -beta, -alpha, true, oppenentboard, playerboard);
	}
	return max_score;
}

int winner(void) {
	if(nowTurn == BLACK_TURN) {
		blackc = bitcount(playerboard);
		whitec = bitcount(oppenentboard);
	} else {
		whitec = bitcount(playerboard);
		blackc = bitcount(oppenentboard);
	}
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

//int nega_alpha(char depth, char playerrn, int alpha, int beta,  bool passed) {
//	if (depth == 0) return countscore(board, &playerrn);
//	int var, max_score = -32767;
//	char tmpboard[10][10];
//	bool canput[10][10] = {{false}};
//	copyboard(board, tmpboard);
//	
//	check2(&playerrn, canput, board);
//	for (char i = 0; i <= 63; ++i) {
//		if (canput[moveorder[i][1]][moveorder[i][0]]) {
//			putstone2(&moveorder[i][1], &moveorder[i][0], &playerrn, board);
//			var = -nega_alpha(depth-1, 3-playerrn, -beta, -alpha, false);
//			copyboard(tmpboard, board);
//			if (var > alpha) {
//				alpha = var;
//				if (depth == DEPTH) {
//					tmpx = moveorder[i][0];
//					tmpy = moveorder[i][1];
//				}
//			}
//			if(depth == DEPTH) {
//				think_percent += think_count;
//				update_hakostring();
//			}
//			if (var >= beta) return var;
//			if(alpha > max_score) max_score = alpha;
//		}
//	}
//	if (max_score == -32767) {
//		if(passed) return countscore(board, &playerrn);
//		return -nega_alpha(depth, 3-playerrn, -beta, -alpha, true);
//	}
//	return max_score;
//}

//void negaalphaTH(void) {
//	memset(cachex, 0, sizeof(cachex));
//	memset(cachey, 0, sizeof(cachey));
//	memset(putable_list, 0, sizeof(putable_list));
//	ALPHA = -32767;
//	char putable_num = putable_counter(&player, board);
//	putable_saver(player, board);
//#pragma omp parallel for num_threads(cpu_core)
//	for (int i=0; i<putable_num; ++i) {
//		negaalpha_omp(i);
//	}
//	int max = -100001;
//	for (char i = 0; i < putable_num; ++i) {
//		if(max < results[i]) max = results[i];
//	}
//	for (char i = 0; i < putable_num; ++i) {
//		if(results[i] == max)
//		{
//			tmpx = cachex[i];
//			tmpy = cachey[i];
//			break;
//		}
//	}
//	printf("chose (%d, %d)\n", tmpx, tmpy);
//}

//int nega_alphadeep(char depth, char playerrn, int alpha, int beta, bool passed, char board[10][10]) {
//	int var, max_score = -32767;
//	char tmpboard[10][10];
//	bool canput[10][10] = {{false}};
//	copyboard(board, tmpboard);
//	check2(&playerrn, canput, board);
//	if(depth == 1) {
//		for (char i = 0; i <= 63; ++i) {
//			if (canput[moveorder[i][0]][moveorder[i][1]]) {
//				putstone2(&moveorder[i][0], &moveorder[i][1], &playerrn, board);
//				var = countscore(board, &playerrn);
//				copyboard(tmpboard, board);
//				
//				if(var >= beta) return var;
//				if(var > alpha) alpha = var;
//				if(alpha > max_score) max_score = alpha;
//			}
//		}
//	} else {
//		for (char i = 0; i <= 63; ++i) {
//			if (canput[moveorder[i][0]][moveorder[i][1]]) {
//				putstone2(&moveorder[i][0], &moveorder[i][1], &playerrn, board);
//				var = -nega_alphadeep(depth-1, 3-playerrn, -beta, -alpha, false, board);
//				copyboard(tmpboard, board);
//				
//				if(var >= beta) return var;
//				if(var > alpha) alpha = var;
//				if(alpha > max_score) max_score = alpha;
//			}
//		}
//	}
//	
//	if (max_score == -32767) {
//		if(passed) return countscore(board, &playerrn);
//		return -nega_alphadeep(depth, 3-playerrn, -beta, -alpha, true, board);
//	}
//	return max_score;
//}

//void* negaalphat(void* args) {
//	int task = *((int*)args);
//	int var;
//	char playerrn = player;
//	int  maxscore = -32767;
//	char tmpboard[10][10] = {{0}};
//		
//	copyboard(board, tmpboard);
//	
//	putstone2(&putable_list[task][1], &putable_list[task][0], &playerrn, tmpboard);
//	
//	var = -nega_alphadeep(DEPTH-1, 3-playerrn, -32767, -ALPHA, false, tmpboard);
//	
//	copyboard(board, tmpboard);
//	
//	if (var > ALPHA) {
//		ALPHA = var;
//		maxscore = var;
//		cachex[task] = putable_list[task][0];
//		cachey[task] = putable_list[task][1];
//	}
//	think_percent += think_count;
//	update_hakostring();
//	results[task] = maxscore;
//	pthread_exit(0);
//}

//void negaalpha_omp(int task) {
//	int var;
//	char playerrn = player;
//	int  maxscore = -32767;
//	char tmpboard[10][10] = {{0}};
//		
//	copyboard(board, tmpboard);
//	
//	putstone2(&putable_list[task][1], &putable_list[task][0], &playerrn, tmpboard);
//	
//	var = -nega_alphadeep(DEPTH-1, 3-playerrn, -32767, -ALPHA, false, tmpboard);
//	
//	copyboard(board, tmpboard);
//	
//	if (var > ALPHA) {
//		ALPHA = var;
//		maxscore = var;
//		cachex[task] = putable_list[task][0];
//		cachey[task] = putable_list[task][1];
//	}
//	think_percent += think_count;
//	update_hakostring();
//	results[task] = maxscore;
//	return;
//}

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
