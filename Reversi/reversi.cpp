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

int putstone(int_fast8_t y, int_fast8_t x) {
	tmpy = y;
	tmpx = x;
	uint64_t put = cordinate_to_bit(&x, &y);
	legalboard = makelegalboard(&b.playerboard, &b.opponentboard);
	if(canput(&put, &legalboard)) {
		reversebit(put);
		nowIndex++;
		afterIndex++;
		return 1;
	} else {
		return 0;
	}
}

uint64_t cordinate_to_bit(int_fast8_t *x, int_fast8_t *y) {
	return 0x8000000000000000ULL >> ((*y<<3)+*x);
}

bool canput(uint64_t *put, uint64_t *legalboard) {
	return ((*put & *legalboard) == *put);
}

uint64_t makelegalboard(uint64_t *p, uint64_t *o) {
    uint64_t moves, hb, flip1, flip7, flip9, flip8, pre1, pre7, pre9, pre8;
    
    hb = *o & 0x7e7e7e7e7e7e7e7eULL;
    flip1  = hb & (*p << 1);    flip7  = hb & (*p << 7);        flip9  = hb & (*p << 9);        flip8  = *o & (*p << 8);
    flip1 |= hb & (flip1 << 1);    flip7 |= hb & (flip7 << 7);    flip9 |= hb & (flip9 << 9);    flip8 |= *o & (flip8 << 8);
    pre1 = hb & (hb << 1);         pre7 = hb & (hb << 7);        pre9 = hb & (hb << 9);        pre8 = *o & (*o << 8);
    flip1 |= pre1 & (flip1 << 2);    flip7 |= pre7 & (flip7 << 14);    flip9 |= pre9 & (flip9 << 18);    flip8 |= pre8 & (flip8 << 16);
    flip1 |= pre1 & (flip1 << 2);    flip7 |= pre7 & (flip7 << 14);    flip9 |= pre9 & (flip9 << 18);    flip8 |= pre8 & (flip8 << 16);
    moves = flip1 << 1;        moves |= flip7 << 7;        moves |= flip9 << 9;        moves |= flip8 << 8;
    flip1  = hb & (*p >> 1);        flip7  = hb & (*p >> 7);        flip9  = hb & (*p >> 9);        flip8  = *o & (*p >> 8);
    flip1 |= hb & (flip1 >> 1);    flip7 |= hb & (flip7 >> 7);    flip9 |= hb & (flip9 >> 9);    flip8 |= *o & (flip8 >> 8);
    pre1 >>= 1;            pre7 >>= 7;            pre9 >>= 9;            pre8 >>= 8;
    flip1 |= pre1 & (flip1 >> 2);    flip7 |= pre7 & (flip7 >> 14);    flip9 |= pre9 & (flip9 >> 18);    flip8 |= pre8 & (flip8 >> 16);
    flip1 |= pre1 & (flip1 >> 2);    flip7 |= pre7 & (flip7 >> 14);    flip9 |= pre9 & (flip9 >> 18);    flip8 |= pre8 & (flip8 >> 16);
    moves |= flip1 >> 1;        moves |= flip7 >> 7;        moves |= flip9 >> 9;        moves |= flip8 >> 8;
    
    return moves & ~(*p|*o);
}

void reversebit(uint64_t put) {
	uint64_t rev = 0;
	for (int_fast8_t i = 0; i<8; ++i) {
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
	for (int_fast8_t i = 0; i<8; ++i) {
		uint64_t rev_ = 0;
		uint64_t mask = transfer(put, &i);
		while (mask && (mask & *opponentboard)) {
			rev_ |= mask;
			mask = transfer(&mask, &i);
		}
		if(mask & *playerboard) *rev |= rev_;
	}
}

uint64_t transfer(uint64_t *put, int_fast8_t *i) {
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
	return (makelegalboard(&b.playerboard, &b.opponentboard) == 0 && makelegalboard(&b.opponentboard, &b.playerboard) != 0);
}

bool isFinished(void) {
	return (makelegalboard(&b.playerboard, &b.opponentboard) == 0 && makelegalboard(&b.opponentboard, &b.playerboard) == 0);
}

void swapboard(void) {
    b.playerboard = b.playerboard ^ b.opponentboard;
    b.opponentboard = b.opponentboard ^ b.playerboard;
    b.playerboard = b.playerboard ^ b.opponentboard;
	nowTurn = -nowTurn;
}

int move_ordering_value_scout(uint64_t *playerboard, uint64_t *opponentboard) {
    if (former_transpose_table_up.find(to_string(*playerboard)+"&"+to_string(*opponentboard)) != former_transpose_table_up.end()) {
        return 500+former_transpose_table_up[to_string(*playerboard)+"&"+to_string(*opponentboard)];
    } else if (former_transpose_table_low.find(to_string(*playerboard)+"&"+to_string(*opponentboard)) != former_transpose_table_low.end()) {
        return 500+former_transpose_table_low[to_string(*playerboard)+"&"+to_string(*opponentboard)];
    } else {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
}


int move_ordering_value(uint64_t *playerboard, uint64_t *opponentboard) {
    return (former_transpose_table.find(to_string(*playerboard)+"&"+to_string(*opponentboard)) != former_transpose_table.end()) ? 500+former_transpose_table[to_string(*playerboard)+"&"+to_string(*opponentboard)] : countscore(playerboard, opponentboard, &afterIndex);
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
	legalboard = makelegalboard(&b.playerboard, &b.opponentboard);
	int putable_count = __builtin_popcountll(legalboard);
    visited_nodes = 0;
    int score = 0;
    if(afterIndex >= 60) {
        think_count = 100/putable_count;
        score = nega_alpha(DEPTH, MIN_INF, MAX_INF, &b.playerboard, &b.opponentboard);
        cout << "depth: " << DEPTH << " Visited nodes: " << visited_nodes << endl;
    } else  {
        think_count = 100/(putable_count*(DEPTH+1-max(DEPTH-3, 1)));
        score = search(&b.playerboard, &b.opponentboard);
//        score = search_nega_scout(&b.playerboard, &b.opponentboard);
    }
	think_percent = 100;
    update_think_percent();
    int count = __builtin_clzll(tmpbit);
    tmpy = count / 8;
    tmpx = count % 8;
    putstone(tmpy, tmpx);
    cout << "put on: (" << tmpx << ", " << tmpy << ")" << endl;
    if(afterIndex >= 60) cout << "Final Score" << endl;
    cout << "score: " << score << endl;
    return 1;
}

int search(uint64_t *playerboard, uint64_t *opponentboard) {
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
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
            for (board& m: moveorder) {
                m.score = move_ordering_value(&m.playerboard, &m.opponentboard);
            }
            sort(moveorder.begin(), moveorder.end());
            int alpha = MIN_INF, beta = MAX_INF;
            for (board& m: moveorder) {
                var = -nega_alpha_moveorder(search_depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard);
                think_percent += think_count;
                update_think_percent();
                if(var > alpha) {
                    alpha = var;
                    if(search_depth == DEPTH) {
                        tmpbit = m.put;
                    }
                }
            }
            cout << "depth: " << search_depth << " Visited nodes: " << visited_nodes << endl;
            transpose_table.swap(former_transpose_table);
            transpose_table.clear();
        }
    } else {
        for (search_depth = max(1, DEPTH-3); search_depth <= DEPTH; ++search_depth) {
            afterIndex = nowIndex+search_depth;
            for (board& m: moveorder) {
                m.score = move_ordering_value(&m.playerboard, &m.opponentboard);
            }
            sort(moveorder.begin(), moveorder.end());
            int alpha = MIN_INF, beta = MAX_INF;
            for (board& m: moveorder) {
                var = -nega_alpha_moveorder(search_depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard);
                think_percent += think_count;
                update_think_percent();
                if(var > alpha) {
                    alpha = var;
                    if(search_depth == DEPTH) {
                        tmpbit = m.put;
                    }
                }
            }
            cout << "depth: " << search_depth << " Visited nodes: " << visited_nodes << endl;
            transpose_table.swap(former_transpose_table);
            transpose_table.clear();
        }
    }
    return var;
}

int nega_alpha_moveorder(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    if(transpose_table.find(to_string(*playerboard)+"&"+to_string(*opponentboard)) != transpose_table.end()) {
        return transpose_table[to_string(*playerboard)+"&"+to_string(*opponentboard)];
    }
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalboard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_alpha_moveorder(depth-1, -beta, -alpha, opponentboard, playerboard);
    }
    int var, max_score = MIN_INF;
    uint64_t rev;
    board moveorder[__builtin_popcountll(legalboard)];
    var = 0;
    for (uint64_t put = 0x8000000000000000; put >= 1; put >>= 1) {
        if(legalboard & put) {
            revbit(&put, playerboard, opponentboard, &rev);
            moveorder[var].playerboard = *playerboard ^ (put | rev);
            moveorder[var].opponentboard = *opponentboard ^ rev;
            moveorder[var].score = move_ordering_value(&moveorder[var].playerboard, &moveorder[var].opponentboard);
            ++var;
        }
    }
    if(var >= 2) sort(moveorder, moveorder+var);
    for (board& m: moveorder) {
        var = (depth <= 3) ? -nega_alpha(depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard) : -nega_alpha_moveorder(depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard);
        if (var >= beta) {
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
    }
    //hashtableに登録
    transpose_table[to_string(*playerboard)+"&"+to_string(*opponentboard)] = max_score;
    return max_score;
}

int nega_alpha(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    if(DEPTH != 20 && transpose_table.find(to_string(*playerboard)+"&"+to_string(*opponentboard)) != transpose_table.end()) {
        return transpose_table[to_string(*playerboard)+"&"+to_string(*opponentboard)];
    }
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalboard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_alpha(depth-1, -beta, -alpha, opponentboard, playerboard);
    }
    uint64_t rev = 0;
    int var, max_score = MIN_INF;
    for (uint64_t& i: moveorder_bit) {
        if(canput(&i, &legalboard)) {
            revbit(&i, playerboard, opponentboard, &rev);
            *playerboard ^= (i | rev);
            *opponentboard ^= rev;
            var = -nega_alpha(depth-1, -beta, -alpha, opponentboard, playerboard);
            *playerboard ^= (i | rev);
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
                    tmpbit = i;
                }
            }
            max_score = max(max_score, alpha);
        }
    }
    if(DEPTH != 20) {
        transpose_table[to_string(*playerboard)+"&"+to_string(*opponentboard)] = max_score;
    }
    return max_score;
}

int search_nega_scout(uint64_t *playerboard, uint64_t *opponentboard) {
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
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
            for (board& m: moveorder) {
                m.score = move_ordering_value_scout(&m.playerboard, &m.opponentboard);
            }
            sort(moveorder.begin(), moveorder.end());
            int alpha = MIN_INF, beta = MAX_INF;
            alpha = -nega_scout(search_depth-1, -beta, -alpha, &moveorder[0].opponentboard, &moveorder[0].playerboard);
            if(search_depth == DEPTH) {
                tmpbit = moveorder[0].put;
            }
            for (int i = 1; i < moveorder.size(); ++i) {
                var = -nega_alpha_moveorder_scout(search_depth-1, -alpha-1, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
                think_percent += think_count;
                update_think_percent();
                if(var > alpha) {
                    alpha = var;
                    var = -nega_scout(search_depth-1, -beta, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
                    if(search_depth == DEPTH) {
                        tmpbit = moveorder[i].put;
                    }
                }
                alpha = max(var, alpha);
            }
            cout << "depth: " << search_depth << " Visited nodes: " << visited_nodes << endl;
            transpose_table_up.swap(former_transpose_table_up);
            transpose_table_up.clear();
            transpose_table_low.swap(former_transpose_table_low);
            transpose_table_low.clear();
        }
    } else {
        for (search_depth = max(1, DEPTH-3); search_depth <= DEPTH; ++search_depth) {
            afterIndex = nowIndex+search_depth;
            for (board& m: moveorder) {
                m.score = move_ordering_value(&m.playerboard, &m.opponentboard);
            }
            sort(moveorder.begin(), moveorder.end());
            int alpha = MIN_INF, beta = MAX_INF;
            alpha = -nega_scout(search_depth-1, -beta, -alpha, &moveorder[0].opponentboard, &moveorder[0].playerboard);
            if(search_depth == DEPTH) {
                tmpbit = moveorder[0].put;
            }
            for (int i = 1; i < moveorder.size(); ++i) {
                var = -nega_alpha_moveorder_scout(search_depth-1, -alpha-1, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
                think_percent += think_count;
                update_think_percent();
                if(var > alpha) {
                    alpha = var;
                    var = -nega_scout(search_depth-1, -beta, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
                    if(search_depth == DEPTH) {
                        tmpbit = moveorder[i].put;
                    }
                }
                alpha = max(var, alpha);
            }
            cout << "depth: " << search_depth << " Visited nodes: " << visited_nodes << endl;
            transpose_table_up.swap(former_transpose_table_up);
            transpose_table_up.clear();
            transpose_table_low.swap(former_transpose_table_low);
            transpose_table_low.clear();
        }
    }
    return var;
}

int nega_scout(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.find(to_string(*playerboard)+"&"+to_string(*opponentboard)) != transpose_table_up.end()) {
        u = transpose_table_up[to_string(*playerboard)+"&"+to_string(*opponentboard)];
    }
    if(transpose_table_low.find(to_string(*playerboard)+"&"+to_string(*opponentboard)) != transpose_table_low.end()) {
        l = transpose_table_low[to_string(*playerboard)+"&"+to_string(*opponentboard)];
    }
    if(u == l) return u;
    alpha = max(l, alpha);
    beta = min(u, beta);
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalboard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_alpha_moveorder(depth-1, -beta, -alpha, opponentboard, playerboard);
    }
    int var, max_score = MIN_INF, canput = 0;
    uint64_t rev;
    board moveorder[__builtin_popcountll(legalboard)];
    for (uint64_t put = 0x8000000000000000; put >= 1; put >>= 1) {
        if(legalboard & put) {
            revbit(&put, playerboard, opponentboard, &rev);
            moveorder[canput].playerboard = *playerboard ^ (put | rev);
            moveorder[canput].opponentboard = *opponentboard ^ rev;
            moveorder[canput].score = move_ordering_value_scout(&moveorder[canput].playerboard, &moveorder[canput].opponentboard);
            ++canput;
        }
    }
    if(canput >= 2) sort(moveorder, moveorder+canput);
    var = -nega_scout(depth-1, -beta, -alpha, &moveorder[0].opponentboard, &moveorder[0].playerboard);
    if (var >= beta) {
        if (var > l) {
            transpose_table_low[to_string(*playerboard)+"&"+to_string(*opponentboard)] = var;
        }
        return var;
    }
    alpha = max(alpha, var);
    max_score = max(max_score, var);
    
    for (int i = 1; i < canput; ++i) {
        var = -nega_alpha_moveorder_scout(depth-1, -alpha-1, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
        if (var >= beta) {
            if(var > l) {
                transpose_table_low[to_string(*playerboard)+"&"+to_string(*opponentboard)] = var;
            }
            return var;
        }
        if(var > alpha) {
            alpha = var;
            var = -nega_scout(depth-1, -beta, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
            if (var >= beta) {
                if (var > l) {
                    transpose_table_low[to_string(*playerboard)+"&"+to_string(*opponentboard)] = var;
                }
                return var;
            }
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
    }
    //hashtableに登録
    if(max_score < alpha) {
        transpose_table_up[to_string(*playerboard)+"&"+to_string(*opponentboard)] = max_score;
    } else {
        transpose_table_up[to_string(*playerboard)+"&"+to_string(*opponentboard)] = max_score;
        transpose_table_low[to_string(*playerboard)+"&"+to_string(*opponentboard)] = max_score;
    }
    
    return max_score;
}

int nega_alpha_moveorder_scout(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.find(to_string(*playerboard)+"&"+to_string(*opponentboard)) != transpose_table_up.end()) {
        u = transpose_table_up[to_string(*playerboard)+"&"+to_string(*opponentboard)];
    }
    if(transpose_table_low.find(to_string(*playerboard)+"&"+to_string(*opponentboard)) != transpose_table_low.end()) {
        l = transpose_table_low[to_string(*playerboard)+"&"+to_string(*opponentboard)];
    }
    if(u == l) return u;
    alpha = max(l, alpha);
    beta = min(u, beta);
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalboard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_alpha_moveorder(depth-1, -beta, -alpha, opponentboard, playerboard);
    }
    int var, max_score = MIN_INF;
    uint64_t rev;
    board moveorder[__builtin_popcountll(legalboard)];
    var = 0;
    for (uint64_t put = 0x8000000000000000; put >= 1; put >>= 1) {
        if(legalboard & put) {
            revbit(&put, playerboard, opponentboard, &rev);
            moveorder[var].playerboard = *playerboard ^ (put | rev);
            moveorder[var].opponentboard = *opponentboard ^ rev;
            moveorder[var].score = move_ordering_value(&moveorder[var].playerboard, &moveorder[var].opponentboard);
            ++var;
        }
    }
    if(var >= 2) sort(moveorder, moveorder+var);
    for (board& m: moveorder) {
        var = (depth <= 3) ? -nega_alpha_scout(depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard) : -nega_alpha_moveorder(depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard);
        if (var >= beta) {
            if(var > l) {
                transpose_table_low[to_string(*playerboard)+"&"+to_string(*opponentboard)] = var;
            }
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
    }
    //hashtableに登録
    if(max_score < alpha) {
        transpose_table_up[to_string(*playerboard)+"&"+to_string(*opponentboard)] = max_score;
    } else {
        transpose_table_up[to_string(*playerboard)+"&"+to_string(*opponentboard)] = max_score;
        transpose_table_low[to_string(*playerboard)+"&"+to_string(*opponentboard)] = max_score;
    }
    
    return max_score;
}

int nega_alpha_scout(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalboard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_alpha(depth-1, -beta, -alpha, opponentboard, playerboard);
    }
    uint64_t rev = 0;
    int var, max_score = MIN_INF;
    for (uint64_t& i: moveorder_bit) {
        if(canput(&i, &legalboard)) {
            revbit(&i, playerboard, opponentboard, &rev);
            *playerboard ^= (i | rev);
            *opponentboard ^= rev;
            var = -nega_alpha_scout(depth-1, -beta, -alpha, opponentboard, playerboard);
            *playerboard ^= (i | rev);
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
                    tmpbit = i;
                }
            }
            max_score = max(max_score, var);
        }
    }
    if(max_score < alpha) {
        transpose_table_up[to_string(*playerboard)+"&"+to_string(*opponentboard)] = max_score;
    } else {
        transpose_table_up[to_string(*playerboard)+"&"+to_string(*opponentboard)] = max_score;
        transpose_table_low[to_string(*playerboard)+"&"+to_string(*opponentboard)] = max_score;
    }
    return max_score;
}

int winner(void) {
	if(nowTurn == BLACK_TURN) {
		blackc = __builtin_popcountll(b.playerboard);
		whitec = __builtin_popcountll(b.opponentboard);
	} else {
		whitec = __builtin_popcountll(b.playerboard);
		blackc = __builtin_popcountll(b.opponentboard);
	}
    return (blackc > whitec) ? 1 : (blackc < whitec) ? 2 : 0;
}

//evaluation

int score_stone(uint64_t *playerboard, uint64_t *opponentboard) {
    int score = 0;
	
    score += scoreboard_score[0] * (__builtin_popcountll(*playerboard & scoreboard_weight[0])-__builtin_popcountll(*opponentboard & scoreboard_weight[0]));
    score += scoreboard_score[1] * (__builtin_popcountll(*playerboard & scoreboard_weight[1])-__builtin_popcountll(*opponentboard & scoreboard_weight[1]));
    score += scoreboard_score[2] * (__builtin_popcountll(*playerboard & scoreboard_weight[2])-__builtin_popcountll(*opponentboard & scoreboard_weight[2]));
    score += scoreboard_score[3] * (__builtin_popcountll(*playerboard & scoreboard_weight[3])-__builtin_popcountll(*opponentboard & scoreboard_weight[3]));
    score += scoreboard_score[4] * (__builtin_popcountll(*playerboard & scoreboard_weight[4])-__builtin_popcountll(*opponentboard & scoreboard_weight[4]));
    
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
    
    for (int_fast8_t i = 0; i < 10; ++i) {
        score += cell_weight_score[i] * __builtin_popcountll(*playerboard & cell_weight_mask[i]);
        score -= cell_weight_score[i] * __builtin_popcountll(*opponentboard & cell_weight_mask[i]);
    }
    return score;
}

int score_putable(uint64_t *playerboard, uint64_t *opponentboard) {
	return (__builtin_popcountll(makelegalboard(playerboard, opponentboard))-__builtin_popcountll(makelegalboard(opponentboard, playerboard)));
}

int score_fixedstone(uint64_t *playerboard, uint64_t *opponentboard) {
    int fixedstone = 0;
	//上
	if((*playerboard | *opponentboard) == UP_BOARD) {
		fixedstone += __builtin_popcountll(*playerboard & UP_BOARD);
		fixedstone -= __builtin_popcountll(*opponentboard & UP_BOARD);
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
		fixedstone += __builtin_popcountll(*playerboard & LEFT_BOARD);
		fixedstone -= __builtin_popcountll(*opponentboard & LEFT_BOARD);
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
		fixedstone += __builtin_popcountll(*playerboard & RIGHT_BOARD);
		fixedstone -= __builtin_popcountll(*opponentboard & RIGHT_BOARD);
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
		fixedstone += __builtin_popcountll(*playerboard & DOWN_BOARD);
		fixedstone -= __builtin_popcountll(*opponentboard & DOWN_BOARD);
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
    return (*afterIndex >= 60) ? (__builtin_popcountll(*playerboard) - __builtin_popcountll(*opponentboard)) :
           (!*playerboard) ? MIN_INF :
           (!*opponentboard) ? MAX_INF :
           (*afterIndex >= 44) ? (score_stone(playerboard, opponentboard) * 3 + score_fixedstone(playerboard, opponentboard) * 55) :
           (score_stone(playerboard, opponentboard) * 3 + score_fixedstone(playerboard, opponentboard) * 55 + score_putable(playerboard, opponentboard));
}
