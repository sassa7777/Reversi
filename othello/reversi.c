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
	playerboard = 0x0000000810000000ULL;
	oppenentboard = 0x0000001008000000ULL;
	skip = false;
	finished = 0;
	skipped = false;
	turn = 1;
	return;
}

void print_board(uint64_t oppenentboard, uint64_t playerboard) {
	printf("  a b c d e f g h\n");
	uint64_t mask = 0x8000000000000000ULL;
	for (int i = 0; i < 8; i++) {
		printf("%d ", i + 1);
		for (int j = 0; j < 8; j++) {
			if (playerboard & mask) {
				printf("O ");
			} else if (oppenentboard & mask) {
				printf("X ");
			} else {
				printf(". ");
			}
			mask >>= 1;
		}
		printf("\n");
	}
}

int putstone(char y, char x) {
	tmpy = y;
	tmpx = x;
	uint64_t put = cordinate_to_bit(&x, &y);
	legalboard = makelegalBoard(&oppenentboard, &playerboard);
	if(canput(&put, &legalboard)) {
		reversebit(put);
		swapboard();
		nowIndex++;
		if(isPass()) {
			swapboard();
		}
		return 1;
	} else {
		return 0;
	}
}

int putstone2(uint64_t *put, uint64_t* playerboard, uint64_t *oppenentboard, uint64_t *legalboard) {
	if(canput(put, legalboard)) {
		reversebit2(put, playerboard, oppenentboard);
		return 1;
	} else {
		return 0;
	}
}

//座標をbitに変換
uint64_t cordinate_to_bit(char *x, char *y) {
	return 0x8000000000000000ULL >> ((*y*8)+*x);
}

bool canput(uint64_t *put, uint64_t *legalboard) {
	return (*put & *legalboard) == *put;
}

uint64_t makelegalBoard(uint64_t *oppenentboard, uint64_t *playerboard) {
	horizontalboard = (*oppenentboard & 0x7e7e7e7e7e7e7e7e);
	verticalboard = (*oppenentboard & 0x00FFFFFFFFFFFF00);
	allsideboard = (*oppenentboard & 0x007e7e7e7e7e7e00);
	blankboard = ~(*playerboard | *oppenentboard);
	
	uint64_t tmp;
	uint64_t legalboard;
	
	//左
	tmp = horizontalboard & (*playerboard << 1);
	tmp |= horizontalboard & (tmp << 1);
	tmp |= horizontalboard & (tmp << 1);
	tmp |= horizontalboard & (tmp << 1);
	tmp |= horizontalboard & (tmp << 1);
	tmp |= horizontalboard & (tmp << 1);
	legalboard = blankboard & (tmp << 1);
	
	//右
	tmp = horizontalboard & (*playerboard >> 1);
	tmp |= horizontalboard & (tmp >> 1);
	tmp |= horizontalboard & (tmp >> 1);
	tmp |= horizontalboard & (tmp >> 1);
	tmp |= horizontalboard & (tmp >> 1);
	tmp |= horizontalboard & (tmp >> 1);
	legalboard |= blankboard & (tmp >> 1);

	//上
	tmp = verticalboard & (*playerboard << 8);
	tmp |= verticalboard & (tmp << 8);
	tmp |= verticalboard & (tmp << 8);
	tmp |= verticalboard & (tmp << 8);
	tmp |= verticalboard & (tmp << 8);
	tmp |= verticalboard & (tmp << 8);
	legalboard |= blankboard & (tmp << 8);
	
	//下
	tmp = verticalboard & (*playerboard >> 8);
	tmp |= verticalboard & (tmp >> 8);
	tmp |= verticalboard & (tmp >> 8);
	tmp |= verticalboard & (tmp >> 8);
	tmp |= verticalboard & (tmp >> 8);
	tmp |= verticalboard & (tmp >> 8);
	legalboard |= blankboard & (tmp >> 8);
	
	//右斜め上
	tmp = allsideboard & (*playerboard << 7);
	tmp |= allsideboard & (tmp << 7);
	tmp |= allsideboard & (tmp << 7);
	tmp |= allsideboard & (tmp << 7);
	tmp |= allsideboard & (tmp << 7);
	tmp |= allsideboard & (tmp << 7);
	legalboard |= blankboard & (tmp << 7);
	
	//左斜め上
	tmp = allsideboard & (*playerboard << 9);
	tmp |= allsideboard & (tmp << 9);
	tmp |= allsideboard & (tmp << 9);
	tmp |= allsideboard & (tmp << 9);
	tmp |= allsideboard & (tmp << 9);
	tmp |= allsideboard & (tmp << 9);
	legalboard |= blankboard & (tmp << 9);
	
	//右斜め下
	tmp = allsideboard & (*playerboard >> 9);
	tmp |= allsideboard & (tmp >> 9);
	tmp |= allsideboard & (tmp >> 9);
	tmp |= allsideboard & (tmp >> 9);
	tmp |= allsideboard & (tmp >> 9);
	tmp |= allsideboard & (tmp >> 9);
	legalboard |= blankboard & (tmp >> 9);
	
	//左斜め下
	tmp = allsideboard & (*playerboard >> 7);
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
		uint64_t mask = transfer(&put, &i);
		while ((mask != 0) && ((mask & oppenentboard) != 0)) {
			rev_ |= mask;
			mask = transfer(&mask, &i);
		}
		if((mask & playerboard) != 0) rev |= rev_;
	}
	//反転
	playerboard ^= (put | rev);
	oppenentboard ^= rev;
}

void reversebit2(uint64_t *put, uint64_t *playerboard, uint64_t *oppenentboard) {
	rev = 0;
	for (char i = 0; i<8; ++i) {
		uint64_t rev_ = 0;
		uint64_t mask = transfer(put, &i);
		while ((mask != 0) && ((mask & *oppenentboard) != 0)) {
			rev_ |= mask;
			mask = transfer(&mask, &i);
		}
		if((mask & *playerboard) != 0) rev |= rev_;
	}
	//反転
	*playerboard ^= (*put | rev);
	*oppenentboard ^= rev;
}

uint64_t transfer(uint64_t *put, char *i) {
	switch (*i) {
		case 0:
			return (*put << 8) & 0xffffffffffffff00;
			break;
		case 1:
			return (*put << 7) & 0x7f7f7f7f7f7f7f00;
			break;
		case 2:
			return (*put >> 1) & 0x7f7f7f7f7f7f7f7f;
			break;
		case 3:
			return (*put >> 9) & 0x007f7f7f7f7f7f7f;
			break;
		case 4:
			return (*put >> 8) & 0x00ffffffffffffff;
			break;
		case 5:
			return (*put >> 7) & 0x00fefefefefefefe;
			break;
		case 6:
			return (*put << 1) & 0xfefefefefefefefe;
			break;
		case 7:
			return (*put << 9) & 0xfefefefefefefe00;
			break;
		default:
			printf("error\n");
			return 0;
			break;
	}
}

bool isPass(void) {
	uint64_t playerlegalboard = makelegalBoard(&oppenentboard, &playerboard);
	uint64_t oppenentlegalboard = makelegalBoard(&playerboard, &oppenentboard);
	if((playerlegalboard == 0x0000000000000000ULL) && (oppenentlegalboard != 0x0000000000000000ULL)) return 1;
	return ((playerlegalboard == 0x0000000000000000ULL) && (oppenentlegalboard != 0x0000000000000000ULL));
}

bool isFinished(void) {
	uint64_t playerlegalboard = makelegalBoard(&oppenentboard, &playerboard);
	uint64_t oppenentlegalboard = makelegalBoard(&playerboard, &oppenentboard);
	if((playerlegalboard == 0x0000000000000000ULL) && (oppenentlegalboard != 0x0000000000000000ULL)) return 1;
	return ((playerlegalboard == 0x0000000000000000ULL) && (oppenentlegalboard == 0x0000000000000000ULL));
}

void swapboard(void) {
	uint64_t tmp = playerboard;
	playerboard = oppenentboard;
	oppenentboard = tmp;
	nowTurn*=(-1);
}

uint64_t bitcount(uint64_t bits) {
	bits = (bits & 0x5555555555555555ULL) + (bits >> 1 & 0x5555555555555555ULL);
	bits = (bits & 0x3333333333333333ULL) + (bits >> 2 & 0x3333333333333333ULL);
	bits = (bits & 0x0f0f0f0f0f0f0f0fULL) + (bits >> 4 & 0x0f0f0f0f0f0f0f0fULL);
	bits = (bits & 0x00ff00ff00ff00ffULL) + (bits >> 8 & 0x00ff00ff00ff00ffULL);
	bits = (bits & 0x0000ffff0000ffffULL) + (bits >> 16 & 0x0000ffff0000ffffULL);
	return (bits & 0x00000000ffffffffULL) + (bits >> 32 & 0x00000000ffffffffULL);
}

int ai(void) {
	if (nowTurn == (botplayer*-1) || isFinished() == true) {
		return 0;
	}
	isbot = true;
	printf("[*]Botが考え中..\n");
	tmpx = 0;
	tmpy = 0;
	think_percent = 0;
	update_hakostring();
	legalboard = makelegalBoard(&oppenentboard, &playerboard);
	think_count = 100/bitcount(legalboard);
	nega_alpha_bit(DEPTH, -32767, 32767, false, &playerboard, &oppenentboard);
	if(tmpx == 0 || tmpy == 0) error_hakostring();
	printf("(%d, %d)\n", tmpx, tmpy);
	think_percent = 100;
	update_hakostring();
	putstone(tmpy, tmpx);
	legalboard = makelegalBoard(&oppenentboard, &playerboard);
	isbot = false;
	return 1;
}

int nega_alpha_bit(char depth, int alpha, int beta,  bool passed, uint64_t *playerboard, uint64_t *oppenentboard) {
	if(depth == 0) return countscore(playerboard, oppenentboard);
	int var, max_score = -32767;
	uint64_t playerboard2 = *playerboard, oppenentboard2 = *oppenentboard;
	uint64_t legalboard = makelegalBoard(oppenentboard, playerboard);
	if(legalboard == 0) {
		if(passed) return countscore(playerboard, oppenentboard);
		return -nega_alpha_bit(depth, -beta, -alpha, true, oppenentboard, playerboard);
	}
	for (char i = 0; i<64; ++i) {
		if(putstone2(&moveorder_bit[i], playerboard, oppenentboard, &legalboard)) {
			var = -nega_alpha_bit(depth-1, -beta, -alpha, false, oppenentboard, playerboard);
			*playerboard = playerboard2;
			*oppenentboard = oppenentboard2;
			if(var > alpha) {
				alpha = var;
				if(depth == DEPTH) {
					tmpx = moveorder[i][1];
					tmpy = moveorder[i][0];
					printf("score = %d\n", var);
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
	return max_score;
}

int winner(void) {
	if(nowTurn == BLACK_TURN) {
		blackc = (int)bitcount(playerboard);
		whitec = (int)bitcount(oppenentboard);
	} else {
		whitec = (int)bitcount(playerboard);
		blackc = (int)bitcount(oppenentboard);
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

int returnplayer(void) { return player; }
