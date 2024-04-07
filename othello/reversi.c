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
	nowTurn = BLACK_TURN;
	nowIndex = 1;
	afterIndex = nowIndex+DEPTH;
	playerboard = 0x0000000810000000ULL;
	oppenentboard = 0x0000001008000000ULL;
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
		afterIndex++;
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

uint64_t cordinate_to_bit(char *x, char *y) {
	return 0x8000000000000000ULL >> ((*y*8)+*x);
}

bool canput(uint64_t *put, uint64_t *legalboard) {
	return (*put & *legalboard) == *put;
}

uint64_t makelegalBoard(uint64_t *oppenentboard, uint64_t *playerboard) {
	uint64_t horizontalboard = (*oppenentboard & 0x7e7e7e7e7e7e7e7e);
	uint64_t verticalboard = (*oppenentboard & 0x00FFFFFFFFFFFF00);
	uint64_t allsideboard = (*oppenentboard & 0x007e7e7e7e7e7e00);
	uint64_t blankboard = ~(*playerboard | *oppenentboard);
	
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
	uint64_t rev = 0;
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

uint64_t revbit(uint64_t *put, uint64_t *playerboard, uint64_t *oppenentboard) {
	uint64_t rev = 0;
	for (char i = 0; i<8; ++i) {
		uint64_t rev_ = 0;
		uint64_t mask = transfer(put, &i);
		while ((mask != 0) && ((mask & *oppenentboard) != 0)) {
			rev_ |= mask;
			mask = transfer(&mask, &i);
		}
		if((mask & *playerboard) != 0) rev |= rev_;
	}
	return rev;
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
	return ((playerlegalboard == 0) && (oppenentlegalboard != 0));
}

bool isFinished(void) {
	uint64_t playerlegalboard = makelegalBoard(&oppenentboard, &playerboard);
	uint64_t oppenentlegalboard = makelegalBoard(&playerboard, &oppenentboard);
	return ((playerlegalboard == 0) && (oppenentlegalboard == 0));
}

void swapboard(void) {
	uint64_t tmp = playerboard;
	playerboard = oppenentboard;
	oppenentboard = tmp;
	nowTurn*=(-1);
}

int bitcount(uint64_t bits) {
	return __builtin_popcountll(bits);
}

int ai(void) {
	if (nowTurn == (botplayer*-1) || isFinished() == true) {
		return 0;
	}
	isbot = true;
	printf("[*]Botが考え中..\n");
	if(DEPTH >= 10 && nowIndex >= 44) DEPTH = 20;
	tmpx = 0;
	tmpy = 0;
	think_percent = 0;
	update_hakostring();
	legalboard = makelegalBoard(&oppenentboard, &playerboard);
	think_count = 100/bitcount(legalboard);
	nega_alpha_bit(DEPTH, 1, -32767, 32767, &playerboard, &oppenentboard);
	if(tmpx == 0 || tmpy == 0) error_hakostring();
	printf("(%d, %d)\n", tmpx, tmpy);
	think_percent = 100;
	update_hakostring();
	putstone(tmpy, tmpx);
	legalboard = makelegalBoard(&oppenentboard, &playerboard);
	isbot = false;
	return 1;
}

int nega_alpha_bit(char depth, char isbotturn, int alpha, int beta, uint64_t *playerboard, uint64_t *oppenentboard) {
	if(depth == 0) return countscore(*playerboard, *oppenentboard, &isbotturn);
	uint64_t legalboard = makelegalBoard(oppenentboard, playerboard);
	if(legalboard == 0) {
		if(!(makelegalBoard(playerboard, oppenentboard))) return countscore(*playerboard, *oppenentboard, &isbotturn);
		else return -nega_alpha_bit(depth, -isbotturn, -beta, -alpha, oppenentboard, playerboard);
	}
	uint64_t rev = 0;
	int var, max_score = -32767;
	for (char i = 0; i<64; ++i) {
		if(canput(&moveorder_bit[i], &legalboard)) {
			rev = revbit(&moveorder_bit[i], playerboard, oppenentboard);
			*playerboard ^= (moveorder_bit[i] | rev);
			*oppenentboard ^= rev;
			var = -nega_alpha_bit(depth-1, -isbotturn, -beta, -alpha, oppenentboard, playerboard);
			*playerboard ^= (moveorder_bit[i] | rev);
			*oppenentboard ^= rev;
			if(depth == DEPTH) {
				think_percent += think_count;
				update_hakostring();
			}
			if(var > alpha) {
				alpha = var;
				if(depth == DEPTH) {
					tmpx = moveorder[i][1];
					tmpy = moveorder[i][0];
				}
			}
			if (var >= beta) return var;
			if(alpha > max_score) max_score = alpha;
		}
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
	} else {
		return 0;
	}
}
