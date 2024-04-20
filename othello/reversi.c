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
	//printf("CPU Core count: %d\n", cpu_core);
	nowTurn = BLACK_TURN;
	nowIndex = 1;
	DEPTH = firstDEPTH;
	afterIndex = nowIndex+DEPTH;
	playerboard = 0x0000000810000000ULL;
	oppenentboard = 0x0000001008000000ULL;
	return;
}

int putstone(char y, char x) {
	tmpy = y;
	tmpx = x;
	uint64_t put = cordinate_to_bit(&x, &y);
	legalboard = makelegalBoard(&playerboard, &oppenentboard);
	if(canput(&put, &legalboard)) {
		reversebit(put);
		nowIndex++;
		afterIndex++;
		return 1;
	} else {
		return 0;
	}
}

uint64_t cordinate_to_bit(char *x, char *y) {
	return 0x8000000000000000ULL >> ((*y*8)+*x);
}

bool canput(uint64_t *put, uint64_t *legalboard) {
	return ((*put & *legalboard) == *put);
}

uint64_t makelegalBoard(uint64_t *playerboard, uint64_t *oppenentboard) {
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


uint64_t revbit(uint64_t *put, uint64_t *playerboard, uint64_t *oppenentboard) {
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
	return (makelegalBoard(&playerboard, &oppenentboard) == 0 && makelegalBoard(&oppenentboard, &playerboard) != 0);
}

bool isFinished(void) {
	return (makelegalBoard(&playerboard, &oppenentboard) == 0 && makelegalBoard(&oppenentboard, &playerboard) == 0);
}

void swapboard(void) {
	uint64_t tmp = playerboard;
	playerboard = oppenentboard;
	oppenentboard = tmp;
	nowTurn = -nowTurn;
}

int bitcount(uint64_t bits) {
	return __builtin_popcountll(bits);
}

void moveordering(uint64_t moveorder[64], short moveorder_score[64], uint64_t *playerboard, uint64_t *oppenentboard) {
	uint64_t legalboard = makelegalBoard(playerboard, oppenentboard);
	int putable_count = (int)__builtin_popcountll(legalboard);
	char j=0;
	uint64_t mask = 0x8000000000000000ULL;
	for (char i=0; i<64; ++i) {
		if(canput(&mask, &legalboard)) {
			moveorder[j] = mask;
			++j;
		} else {
			moveorder[i] = 0;
		}
		mask >>= 1;
	}
	for (char i=0; i<putable_count; ++i) {
		moveorder_score[i] = nega_alpha_move_order(DEPTH, -32767, 32767, playerboard, oppenentboard, &moveorder[i]);
	}
	short temp = 0;
	uint64_t temp2 = 0;
	for (char i = 0; i < putable_count-1; i++) {
		for (char j = 0; j < putable_count-1; j++) {
			if (moveorder_score[j+1] < moveorder_score[j]) {
				temp = moveorder_score[j];
				moveorder_score[j] = moveorder_score[j+1];
				moveorder_score[j+1] = temp;
				temp2 = moveorder[j];
				moveorder[j] = moveorder[j+1];
				moveorder[j+1] = temp2;
			}
		}
	}
}

int ai(void) {
	if (nowTurn == -botplayer) {
		return 0;
	}
	printf("[*]Botが考え中..\n");
	if(DEPTH >= 10 && nowIndex >= 44) DEPTH = 20;
	tmpx = -1;
	tmpy = -1;
	tmpbit = 0;
	think_percent = 0;
	update_hakostring();
	legalboard = makelegalBoard(&playerboard, &oppenentboard);
	int putable_count = (int)__builtin_popcountll(legalboard);
	think_count = 100/putable_count;
	if(DEPTH != 20) {
		nega_alpha_root(DEPTH, -32767, 32767, &playerboard, &oppenentboard);
		int shiftcount = 0;
		while (tmpbit != 0x8000000000000000ULL) {
			tmpbit <<= 1;
			++shiftcount;
		}
		tmpy = shiftcount/8;
		tmpx = shiftcount%8;
	} else {
		nega_alpha(DEPTH, -32767, 32767, &playerboard, &oppenentboard);
	}
	if(tmpx == -1 || tmpy == -1) error_hakostring();
	printf("(%d, %d)\n", tmpx, tmpy);
	think_percent = 100;
	update_hakostring();
	putstone(tmpy, tmpx);
	return 1;
}

short nega_alpha_root(char depth, short alpha, short beta, uint64_t *playerboard, uint64_t *oppenentboard) {
	if(depth == 0) return countscore(playerboard, oppenentboard);
	int putable_count = (int)__builtin_popcountll(legalboard);
	uint64_t moveorder2_bit[64];
	short moveorder2_score[64];
	moveordering(moveorder2_bit, moveorder2_score, playerboard, oppenentboard);
	uint64_t rev = 0;
	short var = 0;
	for (char i = 0; i<putable_count; ++i) {
		rev = revbit(&moveorder2_bit[i], playerboard, oppenentboard);
		*playerboard ^= (moveorder2_bit[i] | rev);
		*oppenentboard ^= rev;
		var = -nega_alpha(depth-1, -beta, -alpha, oppenentboard, playerboard);
		*playerboard ^= (moveorder2_bit[i] | rev);
		*oppenentboard ^= rev;
		if(depth == DEPTH) {
			think_percent += think_count;
			update_hakostring();
		}
		if(var > alpha) {
			alpha = var;
			if(depth == DEPTH) {
				tmpbit = moveorder2_bit[i];
			}
		}
	}
	return alpha;
}


short nega_alpha(char depth, short alpha, short beta, uint64_t *playerboard, uint64_t *oppenentboard) {
	if(depth == 0) return countscore(playerboard, oppenentboard);
	
	uint64_t legalboard = makelegalBoard(playerboard, oppenentboard);
	if(!(legalboard)) {
		if(!(makelegalBoard(oppenentboard, playerboard))) return countscore(playerboard, oppenentboard);
		else return -nega_alpha(depth-1, -beta, -alpha, oppenentboard, playerboard);
	}
	uint64_t rev = 0;
	short var, max_score = -32767;
	for (char i = 0; i<64; ++i) {
		if(canput(&moveorder_bit[i], &legalboard)) {
			rev = revbit(&moveorder_bit[i], playerboard, oppenentboard);
			*playerboard ^= (moveorder_bit[i] | rev);
			*oppenentboard ^= rev;
			var = -nega_alpha(depth-1, -beta, -alpha, oppenentboard, playerboard);
			*playerboard ^= (moveorder_bit[i] | rev);
			*oppenentboard ^= rev;
			if(depth == DEPTH) {
				think_percent += think_count;
				update_hakostring();
			}
			if (var >= beta) {
				return var;
			}
			if(var > alpha) {
				alpha = var;
				if(depth == DEPTH) {
					tmpx = moveorder[i][1];
					tmpy = moveorder[i][0];
				}
			}
		}
	}
	if(alpha > max_score) max_score = alpha;
	return max_score;
}

short nega_alpha_move_order(char depth, short alpha, short beta, uint64_t *playerboard, uint64_t *oppenentboard, uint64_t *put) {
	if(depth == 0) return countscore(playerboard, oppenentboard);
	uint64_t rev = 0;
	short var;
	rev = revbit(put, playerboard, oppenentboard);
	*playerboard ^= (*put | rev);
	*oppenentboard ^= rev;
	var = countscore(playerboard, oppenentboard);
	*playerboard ^= (*put | rev);
	*oppenentboard ^= rev;
	return var;
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
