//
//  othello.c
//  othello
//
//  Created by sasa on 2023/09/30.
//

#include "variables.h"
#include "Wrapper.h"

using namespace std;

void reset(void) {
	printf("[*]初期化中...\n");
    nowTurn = BLACK_TURN;
    nowIndex = 1;
    DEPTH = firstDEPTH;
    afterIndex = nowIndex+DEPTH;
    b.playerboard = 0x0000000810000000ULL;
    b.opponentboard = 0x0000001008000000ULL;
	printf("DEPTH=%d\n", DEPTH);
	printf("Player: %d\n", botplayer);
	printf("CPU Core count: %d\n", cpu_core);
	return;
}

int putstone(char y, char x) {
	tmpy = y;
	tmpx = x;
	uint64_t put = cordinate_to_bit(&x, &y);
	legalboard = makelegalBoard(&b.playerboard, &b.opponentboard);
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
	return 0x8000000000000000ULL >> ((*y<<3)+*x);
}

bool canput(uint64_t *put, uint64_t *legalboard) {
	return ((*put & *legalboard) == *put);
}

uint64_t makelegalBoard(uint64_t *playerboard, uint64_t *opponentboard) {
	uint64_t horizontalboard = (*opponentboard & 0x7e7e7e7e7e7e7e7e);
	uint64_t verticalboard = (*opponentboard & 0x00FFFFFFFFFFFF00);
	uint64_t allsideboard = (*opponentboard & 0x007e7e7e7e7e7e00);
	uint64_t blankboard = ~(*playerboard | *opponentboard);
	
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
		while ((mask != 0) && ((mask & b.opponentboard) != 0)) {
			rev_ |= mask;
			mask = transfer(&mask, &i);
		}
		if((mask & b.playerboard) != 0) rev |= rev_;
	}
	//反転
	b.playerboard ^= (put | rev);
	b.opponentboard ^= rev;
}


void revbit(uint64_t *put, uint64_t *playerboard, uint64_t *opponentboard, uint64_t *rev) {
	*rev = 0;
	for (char i = 0; i<8; ++i) {
		uint64_t rev_ = 0;
		uint64_t mask = transfer(put, &i);
		while ((mask != 0) && ((mask & *opponentboard) != 0)) {
			rev_ |= mask;
			mask = transfer(&mask, &i);
		}
		if((mask & *playerboard) != 0) *rev |= rev_;
	}
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
	return (makelegalBoard(&b.playerboard, &b.opponentboard) == 0 && makelegalBoard(&b.opponentboard, &b.playerboard) != 0);
}

bool isFinished(void) {
	return (makelegalBoard(&b.playerboard, &b.opponentboard) == 0 && makelegalBoard(&b.opponentboard, &b.playerboard) == 0);
}

void swapboard(void) {
	uint64_t tmp = b.playerboard;
	b.playerboard = b.opponentboard;
	b.opponentboard = tmp;
	nowTurn = -nowTurn;
}

int bitcount(uint64_t bits) {
	return popcount(bits);
}

uint64_t delta_swap(uint64_t x, uint64_t mask, int delta) {
    uint64_t t = (x ^ (x >> delta)) & mask;
    return x ^ t ^ (t << delta);
}

// 左循環ビットシフト
uint64_t rotateLeft(uint64_t x, int n) {
    return (x << n) | (x >> (64 - n));
}

// 右循環ビットシフト
uint64_t rotateRight(uint64_t x, int n) {
    return (x >> n) | (x << (64 - n));
}

// 水平反転
void flipHorizontal(uint64_t *x) {
    *x = delta_swap(*x, 0x5555555555555555, 1);
    *x = delta_swap(*x, 0x3333333333333333, 2);
    *x = delta_swap(*x, 0x0F0F0F0F0F0F0F0F, 4);
}

// 垂直反転
void flipVertical(uint64_t *x) {
    *x = delta_swap(*x, 0x00FF00FF00FF00FF, 8);
    *x = delta_swap(*x, 0x0000FFFF0000FFFF, 16);
    *x = delta_swap(*x, 0x00000000FFFFFFFF, 32);
}

// 00-77反転
void flipDiagonalA8H1(uint64_t *x) {
    *x = delta_swap(*x, 0x0055005500550055, 9);
    *x = delta_swap(*x, 0x0000333300003333, 18);
    *x = delta_swap(*x, 0x000000000F0F0F0F, 36);
}

// A1-H8反転
void flipDiagonalA1H8(uint64_t *x) {
    *x = delta_swap(*x, 0x00AA00AA00AA00AA, 7);
    *x = delta_swap(*x, 0x0000CCCC0000CCCC, 14);
    *x = delta_swap(*x, 0x00000000F0F0F0F0, 28);
}

// 時計回りに90度回転
void rotateClockwise90(uint64_t *x) {
    flipDiagonalA1H8(x);
    flipHorizontal(x);
}

// 反時計回りに90度の回転
void rotateCounterclockwise90(uint64_t *x) {
    flipDiagonalA1H8(x);
    flipVertical(x);
}

// 180度回転
void rotate180(uint64_t *x) {
    flipHorizontal(x);
    flipVertical(x);
}

void moveordering(uint64_t moveorder[64], uint64_t *playerboard, uint64_t *opponentboard) {
	int moveorder_score[64];
	uint64_t legalboard = makelegalBoard(playerboard, opponentboard);
	int putable_count = popcount(legalboard);
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
		moveorder_score[i] = nega_alpha_move_order(DEPTH, -32767, 32767, playerboard, opponentboard, &moveorder[i]);
	}
	int temp = 0;
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
    if(DEPTH >= 10 && nowIndex >= 43) {
        DEPTH = 20;
        afterIndex+=10;
    }
	tmpx = -1;
	tmpy = -1;
	tmpbit = 0;
	think_percent = 0;
    update_think_percent();
	legalboard = makelegalBoard(&b.playerboard, &b.opponentboard);
	int putable_count = popcount(legalboard);
	think_count = 100/putable_count;
    transpose_table.clear();
    visited_nodes = 0;
    if(afterIndex >= 60) {
        nega_alpha(DEPTH, MIN_INF-1, MAX_INF+1, &b.playerboard, &b.opponentboard);
    } else {
        nega_alpha_transpose_table(DEPTH, MIN_INF-1, MAX_INF+1, &b.playerboard, &b.opponentboard);
    }
    //nega_alpha(DEPTH, MIN_INF-1, MAX_INF+1, &b.playerboard, &b.opponentboard);
	//if(tmpx == -1 || tmpy == -1) exit(1);
	printf("(%d, %d)\n", tmpx, tmpy);
    cout << "Visited nodes:" << visited_nodes << endl;
	think_percent = 100;
    update_think_percent();
	putstone(tmpy, tmpx);
	return 1;
}


int nega_alpha_transpose_table(char depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    if(transpose_table.find(b) != transpose_table.end()) {
        return transpose_table[b];
    }
	uint64_t legalboard = makelegalBoard(playerboard, opponentboard);
	if(!legalboard) {
		if(!(makelegalBoard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
		else return -nega_alpha_transpose_table(depth-1, -beta, -alpha, opponentboard, playerboard);
	}
    int var, max_score = MIN_INF;
    uint64_t rev;
    for (int i = 0; i < 64; ++i) {
        if(canput(&moveorder_bit[i], &legalboard)) {
            revbit(&moveorder_bit[i], playerboard, opponentboard, &rev);
            *playerboard ^= (moveorder_bit[i] | rev);
            *opponentboard ^= rev;
            if(depth <= 3) {
                var = -nega_alpha(depth-1, -beta, -alpha, opponentboard, playerboard);
            } else {
                var = -nega_alpha_transpose_table(depth-1, -beta, -alpha, opponentboard, playerboard);
            }
            *playerboard ^= (moveorder_bit[i] | rev);
            *opponentboard ^= rev;
            if(depth == DEPTH) {
                think_percent += think_count;
                update_think_percent();
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
            max_score = max(max_score, alpha);
        }
    }
    //hashtableに登録
    transpose_table[b] = max_score;
    *playerboard = __builtin_bswap64(*playerboard);
    *opponentboard = __builtin_bswap64(*opponentboard);
    transpose_table[b] = max_score;
    *playerboard = __builtin_bswap64(*playerboard);
    *opponentboard = __builtin_bswap64(*opponentboard);
    rotateClockwise90(playerboard);
    rotateClockwise90(opponentboard);
    transpose_table[b] = max_score;
    *playerboard = __builtin_bswap64(*playerboard);
    *opponentboard = __builtin_bswap64(*opponentboard);
    transpose_table[b] = max_score;
    *playerboard = __builtin_bswap64(*playerboard);
    *opponentboard = __builtin_bswap64(*opponentboard);
    rotateClockwise90(playerboard);
    rotateClockwise90(opponentboard);
    transpose_table[b] = max_score;
    *playerboard = __builtin_bswap64(*playerboard);
    *opponentboard = __builtin_bswap64(*opponentboard);
    transpose_table[b] = max_score;
    *playerboard = __builtin_bswap64(*playerboard);
    *opponentboard = __builtin_bswap64(*opponentboard);
    rotateClockwise90(playerboard);
    rotateClockwise90(opponentboard);
    transpose_table[b] = max_score;
    *playerboard = __builtin_bswap64(*playerboard);
    *opponentboard = __builtin_bswap64(*opponentboard);
    transpose_table[b] = max_score;
    *playerboard = __builtin_bswap64(*playerboard);
    *opponentboard = __builtin_bswap64(*opponentboard);
    rotateClockwise90(playerboard);
    rotateClockwise90(opponentboard);
	return max_score;
}

int nega_alpha(char depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    
    uint64_t legalboard = makelegalBoard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalBoard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_alpha(depth-1, -beta, -alpha, opponentboard, playerboard);
    }
    uint64_t rev = 0;
    int var, max_score = MIN_INF;
    for (char i = 0; i<64; ++i) {
        if(canput(&moveorder_bit[i], &legalboard)) {
            revbit(&moveorder_bit[i], playerboard, opponentboard, &rev);
            *playerboard ^= (moveorder_bit[i] | rev);
            *opponentboard ^= rev;
            var = -nega_alpha(depth-1, -beta, -alpha, opponentboard, playerboard);
            *playerboard ^= (moveorder_bit[i] | rev);
            *opponentboard ^= rev;
            if(depth == DEPTH) {
                think_percent += think_count;
                update_think_percent();
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
            max_score = max(max_score, alpha);
        }
    }
    return max_score;
}

int nega_alpha_move_order(char depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard, uint64_t *put) {
	if(depth == 0) return countscore(playerboard, opponentboard, &afterIndex);
	uint64_t rev = 0;
	int var;
    revbit(put, playerboard, opponentboard, &rev);
	*playerboard ^= (*put | rev);
	*opponentboard ^= rev;
	var = countscore(playerboard, opponentboard, &afterIndex);
	*playerboard ^= (*put | rev);
	*opponentboard ^= rev;
	return var;
}

int winner(void) {
	if(nowTurn == BLACK_TURN) {
		blackc = popcount(b.playerboard);
		whitec = popcount(b.opponentboard);
	} else {
		whitec = popcount(b.playerboard);
		blackc = popcount(b.opponentboard);
	}
	if (blackc > whitec) {
		return 1;
	} else if (blackc < whitec) {
		return 2;
	} else {
		return 0;
	}
}

//evaluation

int score_stone(uint64_t *playerboard, uint64_t *opponentboard) {
	int score = 0;
	
	for (char i = 0; i < 5; ++i) {
		score += scoreboard_score[i] * popcount(*playerboard & scoreboard_weight[i]);
		score -= scoreboard_score[i] * popcount(*opponentboard & scoreboard_weight[i]);
	}
	
	//左
	switch (*playerboard & LEFT_BOARD) {
		case 0x0000808080800000ULL:
			score += 2;
			break;
		case 0x0000800000800000ULL:
			if((*opponentboard & LEFT_BOARD) == 0x0000008080000000ULL) score -= 4;
			break;
		case 0x0000800080800000ULL:
			score -= 2;
			break;
		case 0x0000808000800000ULL:
			score -= 2;
			break;
		case 0x0080808080808000ULL:
			score += 25;
			break;
		default:
			break;
	}
	switch (*opponentboard & LEFT_BOARD) {
		case 0x0000808080800000ULL:
			score -= 2;
			break;
		case 0x0000800000800000ULL:
			if((*playerboard & LEFT_BOARD) == 0x0000008080000000ULL) score += 4;
			break;
		case 0x0000800080800000ULL:
			score += 2;
			break;
		case 0x0000808000800000ULL:
			score += 2;
			break;
		case 0x0080808080808000ULL:
			score -= 25;
			break;
		default:
			break;
	}
	//右
	switch (*playerboard & RIGHT_BOARD) {
		case 0x0000010101010000ULL:
			score += 2;
			break;
		case 0x0000010000010000ULL:
			if((*opponentboard & RIGHT_BOARD) == 0x0000000101000000ULL) score -= 4;
			break;
		case 0x0000010001010000ULL:
			score -= 2;
			break;
		case 0x0000010100010000ULL:
			score -= 2;
			break;
		case 0x0001010101010100ULL:
			score += 25;
			break;
		default:
			break;
	}
	switch (*opponentboard & RIGHT_BOARD) {
		case 0x0000010101010000ULL:
			score -= 2;
			break;
		case 0x0000010000010000ULL:
			if((*playerboard & RIGHT_BOARD) == 0x0000000101000000ULL) score += 4;
			break;
		case 0x0000010001010000ULL:
			score += 2;
			break;
		case 0x0000010100010000ULL:
			score += 2;
			break;
		case 0x0001010101010100ULL:
			score -= 25;
			break;
		default:
			break;
	}
	//上
	switch (*playerboard & UP_BOARD) {
		case 0x3c00000000000000ULL:
			score += 2;
			break;
		case 0x2400000000000000ULL:
			if((*opponentboard & UP_BOARD) == 0x1800000000000000ULL) score -= 4;
			break;
		case 0x2c00000000000000ULL:
			score -= 2;
			break;
		case 0x1c00000000000000ULL:
			score -= 2;
			break;
		case 0x7e00000000000000ULL:
			score += 25;
			break;
		default:
			break;
	}
	switch (*opponentboard & UP_BOARD) {
		case 0x3c00000000000000ULL:
			score -= 2;
			break;
		case 0x2400000000000000ULL:
			if((*playerboard & UP_BOARD) == 0x1800000000000000ULL) score += 4;
			break;
		case 0x2c00000000000000ULL:
			score += 2;
			break;
		case 0x1c00000000000000ULL:
			score += 2;
			break;
		case 0x7e00000000000000ULL:
			score -= 25;
			break;
		default:
			break;
	}
	//下
	switch (*playerboard & DOWN_BOARD) {
		case 0x000000000000003cULL:
			score += 2;
			break;
		case 0x0000000000000024ULL:
			if((*opponentboard & DOWN_BOARD) == 0x0000000000000018ULL) score += 4;
			break;
		case 0x000000000000002cULL:
			score -= 2;
			break;
		case 0x000000000000001cULL:
			score -= 2;
			break;
		case 0x000000000000007eULL:
			score += 25;
			break;
		default:
			break;
	}
	switch (*opponentboard & DOWN_BOARD) {
		case 0x000000000000003cULL:
			score -= 2;
			break;
		case 0x0000000000000024ULL:
			if((*playerboard & DOWN_BOARD) == 0x0000000000000018ULL) score -= 4;
			break;
		case 0x000000000000002cULL:
			score += 2;
			break;
		case 0x000000000000001cULL:
			score += 2;
			break;
		case 0x000000000000007eULL:
			score -= 25;
			break;
		default:
			break;
	}
	
	return score;
}

int score_stone2(uint64_t *playerboard, uint64_t *opponentboard) {
    int score = 0;
    
    for (char i = 0; i < 10; ++i) {
        score += cell_weight_score[i] * popcount(*playerboard & cell_weight_mask[i]);
        score -= cell_weight_score[i] * popcount(*opponentboard & cell_weight_mask[i]);
    }
    return score;
}

int score_putable(uint64_t *playerboard, uint64_t *opponentboard) {
	return ((popcount(makelegalBoard(playerboard, opponentboard)))-(popcount(makelegalBoard(opponentboard, playerboard))));
}

int score_fixedstone(uint64_t *playerboard, uint64_t *opponentboard) {
	int fixedstone = 0;
	
	//上
	if((*playerboard | *opponentboard) & UP_BOARD) {
		fixedstone += popcount(*playerboard & UP_BOARD);
		fixedstone -= popcount(*opponentboard & UP_BOARD);
	} else {
		//左上左方向
		if(*playerboard & 0xfe00000000000000ULL) fixedstone+=7;
		else if(*playerboard & 0xfc00000000000000ULL) fixedstone+=6;
		else if(*playerboard & 0xf800000000000000ULL) fixedstone+=5;
		else if(*playerboard & 0xf000000000000000ULL) fixedstone+=4;
		else if(*playerboard & 0xe000000000000000ULL) fixedstone+=3;
		else if(*playerboard & 0xc000000000000000ULL) fixedstone+=2;
		else if(*playerboard & 0x8000000000000000ULL) fixedstone+=1;
		
		if(*opponentboard & 0xfe00000000000000ULL) fixedstone-=7;
		else if(*opponentboard & 0xfc00000000000000ULL) fixedstone-=6;
		else if(*opponentboard & 0xf800000000000000ULL) fixedstone-=5;
		else if(*opponentboard & 0xf000000000000000ULL) fixedstone-=4;
		else if(*opponentboard & 0xe000000000000000ULL) fixedstone-=3;
		else if(*opponentboard & 0xc000000000000000ULL) fixedstone-=2;
		else if(*opponentboard & 0x8000000000000000ULL) fixedstone-=1;
		//右上左方向
		if(*playerboard & 0x7f00000000000000ULL) fixedstone+=7;
		else if(*playerboard & 0x3f00000000000000ULL) fixedstone+=6;
		else if(*playerboard & 0x1f00000000000000ULL) fixedstone+=5;
		else if(*playerboard & 0x0f00000000000000ULL) fixedstone+=4;
		else if(*playerboard & 0x0700000000000000ULL) fixedstone+=3;
		else if(*playerboard & 0x0300000000000000ULL) fixedstone+=2;
		else if(*playerboard & 0x0100000000000000ULL) fixedstone+=1;
		
		if(*opponentboard & 0x7f00000000000000ULL) fixedstone-=7;
		else if(*opponentboard & 0x3f00000000000000ULL) fixedstone-=6;
		else if(*opponentboard & 0x1f00000000000000ULL) fixedstone-=5;
		else if(*opponentboard & 0x0f00000000000000ULL) fixedstone-=4;
		else if(*opponentboard & 0x0700000000000000ULL) fixedstone-=3;
		else if(*opponentboard & 0x0300000000000000ULL) fixedstone-=2;
		else if(*opponentboard & 0x0100000000000000ULL) fixedstone-=1;
	}
	//左
	if((*playerboard | *opponentboard) & LEFT_BOARD) {
		fixedstone += popcount(*playerboard & LEFT_BOARD);
		fixedstone -= popcount(*opponentboard & LEFT_BOARD);
	} else {
		//左上下方向
		if(*playerboard & 0x8080808080808000ULL) fixedstone+=7;
		else if(*playerboard & 0x8080808080800000ULL) fixedstone+=6;
		else if(*playerboard & 0x8080808080000000ULL) fixedstone+=5;
		else if(*playerboard & 0x8080808000000000ULL) fixedstone+=4;
		else if(*playerboard & 0x8080800000000000ULL) fixedstone+=3;
		else if(*playerboard & 0x8080008000000000ULL) fixedstone+=2;
		
		if(*opponentboard & 0x8080808080808000ULL) fixedstone-=7;
		else if(*opponentboard & 0x8080808080800000ULL) fixedstone-=6;
		else if(*opponentboard & 0x8080808080000000ULL) fixedstone-=5;
		else if(*opponentboard & 0x8080808000000000ULL) fixedstone-=4;
		else if(*opponentboard & 0x8080800000000000ULL) fixedstone-=3;
		else if(*opponentboard & 0x8080008000000000ULL) fixedstone-=2;
		
		//左下上方向
		if(*playerboard & 0x0080808080808080ULL) fixedstone+=7;
		else if(*playerboard & 0x0000808080808080ULL) fixedstone+=6;
		else if(*playerboard & 0x0000008080808080ULL) fixedstone+=5;
		else if(*playerboard & 0x0000000080808080ULL) fixedstone+=4;
		else if(*playerboard & 0x0000008000808080ULL) fixedstone+=3;
		else if(*playerboard & 0x0000008000008080ULL) fixedstone+=2;
		else if(*playerboard & 0x0000008000000080ULL) fixedstone+=1;
		
		if(*opponentboard & 0x0080808080808080ULL) fixedstone-=7;
		else if(*opponentboard & 0x0000808080808080ULL) fixedstone-=6;
		else if(*opponentboard & 0x0000008080808080ULL) fixedstone-=5;
		else if(*opponentboard & 0x0000000080808080ULL) fixedstone-=4;
		else if(*opponentboard & 0x0000008000808080ULL) fixedstone-=3;
		else if(*opponentboard & 0x0000008000008080ULL) fixedstone-=2;
		else if(*opponentboard & 0x0000008000000080ULL) fixedstone-=1;
	}
	//右
	if((*playerboard | *opponentboard) & RIGHT_BOARD) {
		fixedstone += popcount(*playerboard & RIGHT_BOARD);
		fixedstone -= popcount(*opponentboard & RIGHT_BOARD);
	} else {
		//右上下方向
		if(*playerboard & 0x0101010101010100ULL) fixedstone+=7;
		else if(*playerboard & 0x0101010101010000ULL) fixedstone+=6;
		else if(*playerboard & 0x0101010101000000ULL) fixedstone+=5;
		else if(*playerboard & 0x0101010100000000ULL) fixedstone+=4;
		else if(*playerboard & 0x0101010000000000ULL) fixedstone+=3;
		else if(*playerboard & 0x0101000000000000ULL) fixedstone+=2;
		
		if(*opponentboard & 0x0101010101010100ULL) fixedstone-=7;
		else if(*opponentboard & 0x0101010101010000ULL) fixedstone-=6;
		else if(*opponentboard & 0x0101010101000000ULL) fixedstone-=5;
		else if(*opponentboard & 0x0101010100000000ULL) fixedstone-=4;
		else if(*opponentboard & 0x0101010000000000ULL) fixedstone-=3;
		else if(*opponentboard & 0x0101000000000000ULL) fixedstone-=2;
		//右下上方向
		if(*playerboard & 0x001010101010101ULL) fixedstone+=7;
		else if(*playerboard & 0x000010101010101ULL) fixedstone+=6;
		else if(*playerboard & 0x000000101010101ULL) fixedstone+=5;
		else if(*playerboard & 0x000000001010101ULL) fixedstone+=4;
		else if(*playerboard & 0x000000000010101ULL) fixedstone+=3;
		else if(*playerboard & 0x000000000000101ULL) fixedstone+=2;
		else if(*playerboard & 0x000000000000001ULL) fixedstone+=1;
		
		if(*opponentboard & 0x001010101010101ULL) fixedstone-=7;
		else if(*opponentboard & 0x000010101010101ULL) fixedstone-=6;
		else if(*opponentboard & 0x000000101010101ULL) fixedstone-=5;
		else if(*opponentboard & 0x000000001010101ULL) fixedstone-=4;
		else if(*opponentboard & 0x000000000010101ULL) fixedstone-=3;
		else if(*opponentboard & 0x000000000000101ULL) fixedstone-=2;
		else if(*opponentboard & 0x000000000000001ULL) fixedstone-=1;
	}
	//下
	if((*playerboard | *opponentboard) & DOWN_BOARD) {
		fixedstone += popcount(*playerboard & DOWN_BOARD);
		fixedstone -= popcount(*opponentboard & DOWN_BOARD);
	} else {
		//左下右方向
		if(*playerboard & 0x00000000000000feULL) fixedstone+=7;
		else if(*playerboard & 0x00000000000000fcULL) fixedstone+=6;
		else if(*playerboard & 0x00000000000000f8ULL) fixedstone+=5;
		else if(*playerboard & 0x00000000000000f0ULL) fixedstone+=4;
		else if(*playerboard & 0x00000000000000e0ULL) fixedstone+=3;
		else if(*playerboard & 0x00000000000000c0ULL) fixedstone+=2;
		
		//右下左方向
		if(*playerboard & 0x000000000000007fULL) fixedstone+=7;
		else if(*playerboard & 0x000000000000003fULL) fixedstone+=6;
		else if(*playerboard & 0x000000000000001fULL) fixedstone+=5;
		else if(*playerboard & 0x000000000000000fULL) fixedstone+=4;
		else if(*playerboard & 0x0000000000000007ULL) fixedstone+=3;
		else if(*playerboard & 0x0000000000000003ULL) fixedstone+=2;
		
	}
	return fixedstone;
}

int countscore(uint64_t *playerboard, uint64_t *opponentboard, int *afterIndex) {
    if(*afterIndex >= 60) return (popcount(*playerboard)-popcount(*opponentboard));
	if(!(*playerboard)) return MIN_INF;
	if(!(*opponentboard)) return MAX_INF;
	if(*afterIndex >= 40) return ((score_stone(playerboard, opponentboard))+(score_fixedstone(playerboard, opponentboard)*55));
	return ((score_stone(playerboard, opponentboard)*3)+(score_fixedstone(playerboard, opponentboard)*55)+(score_putable(playerboard, opponentboard)));
}

