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
    cout << "DEPTH=" << DEPTH << endl;
    cout << "Player: " << botplayer << endl;
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
    b.playerboard = b.playerboard ^ b.opponentboard;
    b.opponentboard = b.opponentboard ^ b.playerboard;
    b.playerboard = b.playerboard ^ b.opponentboard;
	nowTurn = -nowTurn;
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

int move_ordering_value_scout(board *b) {
    if (former_transpose_table_up.find(*b) != former_transpose_table_up.end()) {
        return 1000+former_transpose_table_up[*b];
    } else if (former_transpose_table_low.find(*b) != former_transpose_table_low.end()) {
        return 1000+former_transpose_table_low[*b];
    } else {
        return countscore(&b->playerboard, &b->opponentboard, &afterIndex);
    }
}


int move_ordering_value(board *b) {
    if (former_transpose_table.find(*b) != former_transpose_table.end()) {
        return 500+former_transpose_table[*b];
    } else {
        return countscore(&b->playerboard, &b->opponentboard, &afterIndex);
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
    transpose_table.clear();
    former_transpose_table.clear();
	legalboard = makelegalBoard(&b.playerboard, &b.opponentboard);
	int putable_count = popcount(legalboard);
    visited_nodes = 0;
    int score = 0;
    if(afterIndex >= 60) {
        think_count = 100/putable_count;
        visited_nodes = 0;
        score = nega_alpha(DEPTH, MIN_INF, MAX_INF, &b.playerboard, &b.opponentboard);
        cout << "depth: " << DEPTH << " Visited nodes: " << visited_nodes << endl;
    } else  {
        think_count = 100/(putable_count*(DEPTH+1-max(DEPTH-3, 1)));
        score = search(&b.playerboard, &b.opponentboard);
    }
	//if(tmpx == -1 || tmpy == -1) exit(1);
	think_percent = 100;
    update_think_percent();
    if(afterIndex >= 60) putstone(tmpy, tmpx);
    else {
        int count = 0;
        while (tmpbit != 0x8000000000000000) {
            tmpbit <<= 1;
            ++count;
        }
        tmpy = count/8;
        tmpx = count%8;
        putstone(tmpy, tmpx);
    }
    cout << "put on: (" << tmpx << ", " << tmpy << ")" << endl;
    if(afterIndex >= 60) cout << "Final Score" << endl;
    cout << "score: " << score << endl;
    return 1;
}

int search(uint64_t *playerboard, uint64_t *opponentboard) {
    uint64_t legalboard = makelegalBoard(playerboard, opponentboard);
    int var = 0, search_depth;
    uint64_t rev;
    board m;
    vector<board> moveorder;
    m.put = 1;
    for (int i = 0; i < 64; ++i) {
        if(legalboard & m.put) {
            revbit(&m.put, playerboard, opponentboard, &rev);
            m.playerboard = *playerboard ^ (m.put | rev);
            m.opponentboard = *opponentboard ^ rev;
            moveorder.emplace_back(m);
        }
        m.put <<= 1;
    }
    if(Level == 6) {
        for (search_depth = max(1, DEPTH-6); search_depth <= DEPTH; search_depth+=2) {
            afterIndex = nowIndex+search_depth;
            for (board m: moveorder) {
                m.score = move_ordering_value(&m);
            }
            sort(moveorder.begin(), moveorder.end());
            int alpha = MIN_INF, beta = MAX_INF;
            for (board m: moveorder) {
                var = -nega_alpha_moveorder(search_depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard);
                think_percent += think_count;
                update_think_percent();
                if(var > alpha) {
                    alpha = var;
                    if(search_depth == DEPTH) {
                        tmpbit = m.put;
                    }
                }
                alpha = max(var, alpha);
            }
            cout << "depth: " << search_depth << " Visited nodes: " << visited_nodes << endl;
            transpose_table.swap(former_transpose_table);
            transpose_table.clear();
        }
    } else {
        for (search_depth = max(1, DEPTH-3); search_depth <= DEPTH; ++search_depth) {
            afterIndex = nowIndex+search_depth;
            for (board m: moveorder) {
                m.score = move_ordering_value(&m);
            }
            sort(moveorder.begin(), moveorder.end());
            int alpha = MIN_INF, beta = MAX_INF;
            for (board m: moveorder) {
                var = -nega_alpha_moveorder(search_depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard);
                think_percent += think_count;
                update_think_percent();
                if(var > alpha) {
                    alpha = var;
                    if(search_depth == DEPTH) {
                        tmpbit = m.put;
                    }
                }
                alpha = max(var, alpha);
            }
            cout << "depth: " << search_depth << " Visited nodes: " << visited_nodes << endl;
            transpose_table.swap(former_transpose_table);
            transpose_table.clear();
        }
    }
    return var;
}

int nega_scout(char depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    board b;
    b.playerboard = *playerboard;
    b.opponentboard = *opponentboard;
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.find(b) != transpose_table_up.end()) {
        u = transpose_table_up[b];
    }
    if(transpose_table_low.find(b) != transpose_table_low.end()) {
        l = transpose_table_low[b];
    }
    if(u == l) return u;
    alpha = max(alpha, l);
    beta = min(beta, u);
    uint64_t legalboard = makelegalBoard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalBoard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_scout(depth-1, -beta, -alpha, opponentboard, playerboard);
    }
    int var, max_score = MIN_INF;
    uint64_t rev;
    vector<board> moveorder;
    b.put = 1;
    for (int i = 0; i < 64; ++i) {
        if(legalboard & b.put) {
            revbit(&b.put, playerboard, opponentboard, &rev);
            b.playerboard = *playerboard ^ (b.put | rev);
            b.opponentboard = *opponentboard ^ rev;
            b.score = move_ordering_value_scout(&b);
            moveorder.emplace_back(b);
        }
        b.put <<= 1;
    }
    b.playerboard = *playerboard;
    b.opponentboard = *opponentboard;
    sort(moveorder.begin(), moveorder.end());
    var = -nega_scout(depth-1, -beta, -alpha, &moveorder[0].opponentboard, &moveorder[0].playerboard);
    if (var >= beta) {
        if (var > l) {
            transpose_table_low[b] = var;
        }
        return var;
    }
    alpha = max(alpha, var);
    max_score = max(max_score, var);
    for (int i = 1; i < moveorder.size(); ++i) {
        var = -nega_alpha_transpose_table(depth-1, -alpha-1, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
        if (var >= beta) {
            if(var > l) {
                transpose_table_low[b] = var;
            }
            return var;
        }
        if(var > alpha) {
            alpha = var;
            var = -nega_scout(depth-1, -beta, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
            if (var >= beta) {
                if(var > l) {
                    transpose_table_low[b] = var;
                }
                return var;
            }
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
    }
    if (max_score < alpha) {
        transpose_table_up[b] = max_score;
    } else {
        transpose_table_up[b] = max_score;
        transpose_table_low[b] = max_score;
    }
    return max_score;
}

int nega_alpha_transpose_table(char depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    board b;
    b.playerboard = *playerboard;
    b.opponentboard = *opponentboard;
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.find(b) != transpose_table_up.end()) {
        u = transpose_table_up[b];
    }
    if(transpose_table_low.find(b) != transpose_table_low.end()) {
        l = transpose_table_low[b];
    }
    if(u == l) return u;
    alpha = max(u, alpha);
    beta = min(l, beta);
    uint64_t legalboard = makelegalBoard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalBoard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_alpha_transpose_table(depth-1, -beta, -alpha, opponentboard, playerboard);
    }
    int var, max_score = MIN_INF;
    uint64_t rev;
    vector<board> moveorder;
    b.put = 1;
    for (int i = 0; i < 64; ++i) {
        if(legalboard & b.put) {
            revbit(&b.put, playerboard, opponentboard, &rev);
            b.playerboard = *playerboard ^ (b.put | rev);
            b.opponentboard = *opponentboard ^ rev;
            b.score = move_ordering_value_scout(&b);
            moveorder.emplace_back(b);
        }
        b.put <<= 1;
    }
    b.playerboard = *playerboard;
    b.opponentboard = *opponentboard;
    if(moveorder.size() > 1) sort(moveorder.begin(), moveorder.end());
    for (int i = 0; i < moveorder.size(); ++i) {
        var = -nega_alpha_transpose_table(depth-1, -beta, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
        if (var >= beta) {
            transpose_table_low[b] = var;
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
    }
    //hashtableに登録
    if (max_score < alpha) {
        transpose_table_up[b] = max_score;
    } else {
        transpose_table_up[b] = max_score;
        transpose_table_low[b] = max_score;
    }
	return max_score;
}

int nega_alpha_moveorder(char depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    board b;
    b.playerboard = *playerboard;
    b.opponentboard = *opponentboard;
    if(transpose_table.find(b) != transpose_table.end()) {
        return transpose_table[b];
    }
    uint64_t legalboard = makelegalBoard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalBoard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_alpha_moveorder(depth-1, -beta, -alpha, opponentboard, playerboard);
    }
    int var, max_score = MIN_INF;
    uint64_t rev;
    vector<board> moveorder(popcount(legalboard));
    var = 0;
    for (b.put = 0x8000000000000000; b.put >= 1; b.put >>= 1) {
        if(legalboard & b.put) {
            revbit(&b.put, playerboard, opponentboard, &rev);
            moveorder[var].playerboard = *playerboard ^ (b.put | rev);
            moveorder[var].opponentboard = *opponentboard ^ rev;
            moveorder[var].score = move_ordering_value(&moveorder[var]);
            ++var;
        }
    }
    if(moveorder.size() >= 2) sort(moveorder.begin(), moveorder.end());
    for (board& m: moveorder) {
        if(depth <= 3) {
            var = -nega_alpha(depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard);
        } else {
            var = -nega_alpha_moveorder(depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard);
        }
        if (var >= beta) {
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, alpha);
    }
    //hashtableに登録
    transpose_table[b] = max_score;
    return max_score;
}

int nega_alpha(char depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    board b;
    b.playerboard = *playerboard;
    b.opponentboard = *opponentboard;
    if(transpose_table.find(b) != transpose_table.end()) {
        return transpose_table[b];
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
    transpose_table[b] = max_score;
    return max_score;
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
	
    score += scoreboard_score[0] * (popcount(*playerboard & scoreboard_weight[0])-popcount(*opponentboard & scoreboard_weight[0]));
    score += scoreboard_score[1] * (popcount(*playerboard & scoreboard_weight[1])-popcount(*opponentboard & scoreboard_weight[1]));
    score += scoreboard_score[2] * (popcount(*playerboard & scoreboard_weight[2])-popcount(*opponentboard & scoreboard_weight[2]));
    score += scoreboard_score[3] * (popcount(*playerboard & scoreboard_weight[3])-popcount(*opponentboard & scoreboard_weight[3]));
    score += scoreboard_score[4] * (popcount(*playerboard & scoreboard_weight[4])-popcount(*opponentboard & scoreboard_weight[4]));
    
	//左
    {
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
        if((*playerboard & 0xC0300C03000000) == 0xC0300C03000000) score -= -5;
        if((*opponentboard & 0xC0300C03000000) == 0xC0300C03000000) score += -5;
    }
	//右
    {
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
        if((*playerboard & 0x30303030000) == 0x30303030000) score -= -5;
        if((*opponentboard & 0x30303030000) == 0x30303030000) score += -5;
    }
	//上
    {
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
        if((*playerboard & 0x3C3C000000000000) == 0x3C3C000000000000) score -= -5;
        if((*opponentboard & 0x3C3C000000000000) == 0x3C3C000000000000) score += -5;
    }
	//下
    {
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
        if((*playerboard & 0x3C3C) == 0x3C3C) score -= -5;
        if((*opponentboard & 0x3C3C) == 0x3C3C) score += -5;
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
	if((*playerboard | *opponentboard) == UP_BOARD) {
		fixedstone += popcount(*playerboard & UP_BOARD);
		fixedstone -= popcount(*opponentboard & UP_BOARD);
	} else {
		//左上左方向
        switch (*playerboard & 0xfe00000000000000ULL) {
            case 0xfe00000000000000ULL:
                fixedstone += 7;
                break;
            case 0xfc00000000000000ULL:
                fixedstone += 6;
                break;
            case 0xf800000000000000ULL:
                fixedstone += 5;
                break;
            case 0xf000000000000000ULL:
                fixedstone += 4;
                break;
            case 0xe000000000000000ULL:
                fixedstone += 3;
                break;
            case 0xc000000000000000ULL:
                fixedstone += 2;
                break;
            case 0x8000000000000000ULL:
                fixedstone += 1;
                break;
            default:
                break;
        }
		
        switch (*opponentboard & 0xfe00000000000000ULL) {
            case 0xfe00000000000000ULL:
                fixedstone -= 7;
                break;
            case 0xfc00000000000000ULL:
                fixedstone -= 6;
                break;
            case 0xf800000000000000ULL:
                fixedstone -= 5;
                break;
            case 0xf000000000000000ULL:
                fixedstone -= 4;
                break;
            case 0xe000000000000000ULL:
                fixedstone -= 3;
                break;
            case 0xc000000000000000ULL:
                fixedstone -= 2;
                break;
            case 0x8000000000000000ULL:
                fixedstone -= 1;
                break;
            default:
                break;
        }
		//右上左方向
        switch (*playerboard & 0x7f00000000000000ULL) {
            case 0x7f00000000000000ULL:
                fixedstone += 7;
                break;
            case 0x3f00000000000000ULL:
                fixedstone += 6;
                break;
            case 0x1f00000000000000ULL:
                fixedstone += 5;
                break;
            case 0x0f00000000000000ULL:
                fixedstone += 4;
                break;
            case 0x0700000000000000ULL:
                fixedstone += 3;
                break;
            case 0x0300000000000000ULL:
                fixedstone += 2;
                break;
            case 0x0100000000000000ULL:
                fixedstone += 1;
                break;
            default:
                break;
        }
		
        switch (*opponentboard & 0x7f00000000000000ULL) {
            case 0x7f00000000000000ULL:
                fixedstone -= 7;
                break;
            case 0x3f00000000000000ULL:
                fixedstone -= 6;
                break;
            case 0x1f00000000000000ULL:
                fixedstone -= 5;
                break;
            case 0x0f00000000000000ULL:
                fixedstone -= 4;
                break;
            case 0x0700000000000000ULL:
                fixedstone -= 3;
                break;
            case 0x0300000000000000ULL:
                fixedstone -= 2;
                break;
            case 0x0100000000000000ULL:
                fixedstone -= 1;
                break;
            default:
                break;
        }
	}
	//左
	if((*playerboard | *opponentboard) == LEFT_BOARD) {
		fixedstone += popcount(*playerboard & LEFT_BOARD);
		fixedstone -= popcount(*opponentboard & LEFT_BOARD);
	} else {
		//左上下方向
        switch (*playerboard & 0x8080808080808000ULL) {
            case 0x8080808080808000ULL:
                fixedstone += 7;
                break;
            case 0x8080808080800000ULL:
                fixedstone += 6;
                break;
            case 0x8080808080000000ULL:
                fixedstone += 5;
                break;
            case 0x8080808000000000ULL:
                fixedstone += 4;
                break;
            case 0x8080800000000000ULL:
                fixedstone += 3;
                break;
            case 0x8080008000000000ULL:
                fixedstone += 2;
                break;
            default:
                break;
        }
		
        switch (*opponentboard & 0x8080808080808000ULL) {
            case 0x8080808080808000ULL:
                fixedstone -= 7;
                break;
            case 0x8080808080800000ULL:
                fixedstone -= 6;
                break;
            case 0x8080808080000000ULL:
                fixedstone -= 5;
                break;
            case 0x8080808000000000ULL:
                fixedstone -= 4;
                break;
            case 0x8080800000000000ULL:
                fixedstone -= 3;
                break;
            case 0x8080008000000000ULL:
                fixedstone -= 2;
                break;
            default:
                break;
        }
		
		//左下上方向
        switch (*playerboard & 0x0080808080808080ULL) {
            case 0x0080808080808080ULL:
                fixedstone += 7;
                break;
            case 0x0000808080808080ULL:
                fixedstone += 6;
                break;
            case 0x0000008080808080ULL:
                fixedstone += 5;
                break;
            case 0x0000000080808080ULL:
                fixedstone += 4;
                break;
            case 0x0000008000808080ULL:
                fixedstone += 3;
                break;
            case 0x0000008000008080ULL:
                fixedstone += 2;
                break;
            case 0x0000008000000080ULL:
                fixedstone += 1;
                break;
            default:
                break;
        }
		
        switch (*opponentboard & 0x0080808080808080ULL) {
            case 0x0080808080808080ULL:
                fixedstone -= 7;
                break;
            case 0x0000808080808080ULL:
                fixedstone -= 6;
                break;
            case 0x0000008080808080ULL:
                fixedstone -= 5;
                break;
            case 0x0000000080808080ULL:
                fixedstone -= 4;
                break;
            case 0x0000008000808080ULL:
                fixedstone -= 3;
                break;
            case 0x0000008000008080ULL:
                fixedstone -= 2;
                break;
            case 0x0000008000000080ULL:
                fixedstone -= 1;
                break;
            default:
                break;
        }
	}
	//右
	if((*playerboard | *opponentboard) == RIGHT_BOARD) {
		fixedstone += popcount(*playerboard & RIGHT_BOARD);
		fixedstone -= popcount(*opponentboard & RIGHT_BOARD);
	} else {
		//右上下方向
        switch (*playerboard & 0x0101010101010100ULL) {
            case 0x0101010101010100ULL:
                fixedstone += 7;
                break;
            case 0x0101010101010000ULL:
                fixedstone += 6;
                break;
            case 0x0101010101000000ULL:
                fixedstone += 5;
                break;
            case 0x0101010100000000ULL:
                fixedstone += 4;
                break;
            case 0x0101010000000000ULL:
                fixedstone += 3;
                break;
            case 0x0101000000000000ULL:
                fixedstone += 2;
                break;
            default:
                break;
        }
		
        switch (*opponentboard & 0x0101010101010100ULL) {
            case 0x0101010101010100ULL:
                fixedstone -= 7;
                break;
            case 0x0101010101010000ULL:
                fixedstone -= 6;
                break;
            case 0x0101010101000000ULL:
                fixedstone -= 5;
                break;
            case 0x0101010100000000ULL:
                fixedstone -= 4;
                break;
            case 0x0101010000000000ULL:
                fixedstone -= 3;
                break;
            case 0x0101000000000000ULL:
                fixedstone -= 2;
                break;
            default:
                break;
        }
		//右下上方向
        switch (*playerboard & 0x001010101010101ULL) {
            case 0x001010101010101ULL:
                fixedstone += 7;
                break;
            case 0x000010101010101ULL:
                fixedstone += 6;
                break;
            case 0x000000101010101ULL:
                fixedstone += 5;
                break;
            case 0x000000001010101ULL:
                fixedstone += 4;
                break;
            case 0x000000000010101ULL:
                fixedstone += 3;
                break;
            case 0x000000000000101ULL:
                fixedstone += 2;
                break;
            case 0x000000000000001ULL:
                fixedstone += 1;
                break;
            default:
                break;
        }
		
        switch (*opponentboard & 0x001010101010101ULL) {
            case 0x001010101010101ULL:
                fixedstone -= 7;
                break;
            case 0x000010101010101ULL:
                fixedstone -= 6;
                break;
            case 0x000000101010101ULL:
                fixedstone -= 5;
                break;
            case 0x000000001010101ULL:
                fixedstone -= 4;
                break;
            case 0x000000000010101ULL:
                fixedstone -= 3;
                break;
            case 0x000000000000101ULL:
                fixedstone -= 2;
                break;
            case 0x000000000000001ULL:
                fixedstone -= 1;
                break;
            default:
                break;
        }
	}
	//下
	if((*playerboard | *opponentboard) == DOWN_BOARD) {
		fixedstone += popcount(*playerboard & DOWN_BOARD);
		fixedstone -= popcount(*opponentboard & DOWN_BOARD);
	} else {
		//左下右方向
        switch (*playerboard & 0x00000000000000feULL) {
            case 0x00000000000000feULL:
                fixedstone += 7;
                break;
            case 0x00000000000000fcULL:
                fixedstone += 6;
                break;
            case 0x00000000000000f8ULL:
                fixedstone += 5;
                break;
            case 0x00000000000000f0ULL:
                fixedstone += 4;
                break;
            case 0x00000000000000e0ULL:
                fixedstone += 3;
                break;
            case 0x00000000000000c0ULL:
                fixedstone += 2;
                break;
            default:
                break;
        }
        
        switch (*opponentboard & 0x00000000000000feULL) {
            case 0x00000000000000feULL:
                fixedstone -= 7;
                break;
            case 0x00000000000000fcULL:
                fixedstone -= 6;
                break;
            case 0x00000000000000f8ULL:
                fixedstone -= 5;
                break;
            case 0x00000000000000f0ULL:
                fixedstone -= 4;
                break;
            case 0x00000000000000e0ULL:
                fixedstone -= 3;
                break;
            case 0x00000000000000c0ULL:
                fixedstone -= 2;
                break;
            default:
                break;
        }
		
		//右下左方向
        switch (*playerboard & 0x00000000000000feULL) {
            case 0x00000000000000feULL:
                fixedstone += 7;
                break;
            case 0x00000000000000fcULL:
                fixedstone += 6;
                break;
            case 0x00000000000000f8ULL:
                fixedstone += 5;
                break;
            case 0x00000000000000f0ULL:
                fixedstone += 4;
                break;
            case 0x00000000000000e0ULL:
                fixedstone += 3;
                break;
            case 0x00000000000000c0ULL:
                fixedstone += 2;
                break;
            default:
                break;
        }
        
        switch (*opponentboard & 0x00000000000000feULL) {
            case 0x00000000000000feULL:
                fixedstone -= 7;
                break;
            case 0x00000000000000fcULL:
                fixedstone -= 6;
                break;
            case 0x00000000000000f8ULL:
                fixedstone -= 5;
                break;
            case 0x00000000000000f0ULL:
                fixedstone -= 4;
                break;
            case 0x00000000000000e0ULL:
                fixedstone -= 3;
                break;
            case 0x00000000000000c0ULL:
                fixedstone -= 2;
                break;
            default:
                break;
        }
		
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
