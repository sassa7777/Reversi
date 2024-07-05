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

uint64_t cordinate_to_bit(int_fast8_t *put, int_fast8_t *y) {
	return 0x8000000000000000ULL >> ((*y<<3)+*put);
}

bool canput(uint64_t *put, uint64_t *legalboard) {
	return ((*put & *legalboard) == *put);
}

uint64_t makelegalboard(const uint64_t *p, const uint64_t *o) {
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
    uint64_t flipped, OM, outflank[4], mask[4];
    int pos = __builtin_ctzll(put);
    OM = b.opponentboard & 0x7e7e7e7e7e7e7e7eULL;

    mask[0] = 0x0080808080808080ULL >> (63 - pos);
    mask[1] = 0x7f00000000000000ULL >> (63 - pos);
    mask[2] = 0x0102040810204000ULL >> (63 - pos);
    mask[3] = 0x0040201008040201ULL >> (63 - pos);
    outflank[0] = (0x8000000000000000ULL >> __builtin_clzll(~b.opponentboard & (mask[0]))) & b.playerboard;
    outflank[1] = (0x8000000000000000ULL >> __builtin_clzll(~OM & (mask[1]))) & b.playerboard;
    outflank[2] = (0x8000000000000000ULL >> __builtin_clzll(~OM & (mask[2]))) & b.playerboard;
    outflank[3] = (0x8000000000000000ULL >> __builtin_clzll(~OM & (mask[3]))) & b.playerboard;
    flipped  = (-outflank[0] << 1) & mask[0];
    flipped |= (-outflank[1] << 1) & mask[1];
    flipped |= (-outflank[2] << 1) & mask[2];
    flipped |= (-outflank[3] << 1) & mask[3];

    mask[0] = 0x0101010101010100ULL << pos;
    mask[1] = 0x00000000000000feULL << pos;
    mask[2] = 0x0002040810204080ULL << pos;
    mask[3] = 0x8040201008040200ULL << pos;
    outflank[0] = mask[0] & ((b.opponentboard | ~mask[0]) + 1) & b.playerboard;
    outflank[1] = mask[1] & ((OM | ~mask[1]) + 1) & b.playerboard;
    outflank[2] = mask[2] & ((OM | ~mask[2]) + 1) & b.playerboard;
    outflank[3] = mask[3] & ((OM | ~mask[3]) + 1) & b.playerboard;
    flipped |= (outflank[0] - (uint32_t)(outflank[0] != 0)) & mask[0];
    flipped |= (outflank[1] - (uint32_t)(outflank[1] != 0)) & mask[1];
    flipped |= (outflank[2] - (uint32_t)(outflank[2] != 0)) & mask[2];
    flipped |= (outflank[3] - (uint32_t)(outflank[3] != 0)) & mask[3];
	//反転
    b.playerboard ^= (put | flipped);
    b.opponentboard ^= flipped;
}

uint64_t Flip(uint64_t *put, uint64_t *playerboard, uint64_t *opponentboard) {
    uint64_t flipped, OM, outflank[4], mask[4];
    int pos = __builtin_ctzll(*put);
    OM = *opponentboard & 0x7e7e7e7e7e7e7e7eULL;

    mask[0] = 0x0080808080808080ULL >> (63 - pos);
    mask[1] = 0x7f00000000000000ULL >> (63 - pos);
    mask[2] = 0x0102040810204000ULL >> (63 - pos);
    mask[3] = 0x0040201008040201ULL >> (63 - pos);
    outflank[0] = (0x8000000000000000ULL >> __builtin_clzll(~*opponentboard & (mask[0]))) & *playerboard;
    outflank[1] = (0x8000000000000000ULL >> __builtin_clzll(~OM & (mask[1]))) & *playerboard;
    outflank[2] = (0x8000000000000000ULL >> __builtin_clzll(~OM & (mask[2]))) & *playerboard;
    outflank[3] = (0x8000000000000000ULL >> __builtin_clzll(~OM & (mask[3]))) & *playerboard;
    flipped  = (-outflank[0] << 1) & mask[0];
    flipped |= (-outflank[1] << 1) & mask[1];
    flipped |= (-outflank[2] << 1) & mask[2];
    flipped |= (-outflank[3] << 1) & mask[3];

    mask[0] = 0x0101010101010100ULL << pos;
    mask[1] = 0x00000000000000feULL << pos;
    mask[2] = 0x0002040810204080ULL << pos;
    mask[3] = 0x8040201008040200ULL << pos;
    outflank[0] = mask[0] & ((*opponentboard | ~mask[0]) + 1) & *playerboard;
    outflank[1] = mask[1] & ((OM | ~mask[1]) + 1) & *playerboard;
    outflank[2] = mask[2] & ((OM | ~mask[2]) + 1) & *playerboard;
    outflank[3] = mask[3] & ((OM | ~mask[3]) + 1) & *playerboard;
    flipped |= (outflank[0] - (uint32_t)(outflank[0] != 0)) & mask[0];
    flipped |= (outflank[1] - (uint32_t)(outflank[1] != 0)) & mask[1];
    flipped |= (outflank[2] - (uint32_t)(outflank[2] != 0)) & mask[2];
    flipped |= (outflank[3] - (uint32_t)(outflank[3] != 0)) & mask[3];

    return flipped;
}

uint64_t transfer(uint64_t *put, int_fast8_t *i) {
	switch (*i) {
		case 0:
			return (*put << 8) & 0xffffffffffffff00;
		case 1:
			return (*put << 7) & 0x7f7f7f7f7f7f7f00;
		case 2:
			return (*put >> 1) & 0x7f7f7f7f7f7f7f7f;
		case 3:
			return (*put >> 9) & 0x007f7f7f7f7f7f7f;
		case 4:
			return (*put >> 8) & 0x00ffffffffffffff;
		case 5:
			return (*put >> 7) & 0x00fefefefefefefe;
		case 6:
			return (*put << 1) & 0xfefefefefefefefe;
		case 7:
			return (*put << 9) & 0xfefefefefefefe00;
		default:
			printf("error\n");
			return 0;
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

int move_ordering_value(uint64_t *playerboard, uint64_t *opponentboard) {
    if(former_transpose_table_up.find(*playerboard) != former_transpose_table_up.end() && former_transpose_table_up[*playerboard].find(*opponentboard) != former_transpose_table_up[*playerboard].end()) {
        return (1000-former_transpose_table_up[*playerboard][*opponentboard]);
    } else if(former_transpose_table_low.find(*playerboard) != former_transpose_table_low.end() && former_transpose_table_low[*playerboard].find(*opponentboard) != former_transpose_table_low[*playerboard].end()) {
            return (1000-former_transpose_table_low[*playerboard][*opponentboard]);
    } else {
        return -countscore(playerboard, opponentboard, &afterIndex);
    }
}

int ai(void) {
	if (nowTurn == -botplayer) {
		return 0;
	}
	printf("[*]Botが考え中..\n");
    if(DEPTH == 12 && nowIndex >= 38) {
//        DEPTH = 20;
//        afterIndex=60;
        DEPTH = 14;
        afterIndex=nowIndex+DEPTH;
    }
    if(DEPTH >= 10 && nowIndex >= 40) {
        DEPTH = 20;
        afterIndex=60;
    }
	tmpx = -1;
	tmpy = -1;
	tmpbit = 0;
	think_percent = 0;
    update_think_percent();
    transpose_table.clear();
    former_transpose_table.clear();
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
	legalboard = makelegalboard(&b.playerboard, &b.opponentboard);
	int putable_count = __builtin_popcountll(legalboard);
    visited_nodes = 0;
    int score = 0;
    if(afterIndex >= 60) {
        think_count = 100/putable_count;
        score = search_finish(&b.playerboard, &b.opponentboard);
//        score = search_finish_scout(&b.playerboard, &b.opponentboard);
    } else  {
        think_count = 100/(putable_count*(DEPTH+1-max(DEPTH-3, 1)));
//        score = search(&b.playerboard, &b.opponentboard);
        score = search_nega_scout(&b.playerboard, &b.opponentboard);
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
    cout << "algorithm: NegaAlpha" << endl;
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int var = 0, search_depth;
    uint64_t rev;
    board_root m;
    vector<board_root> moveorder;
    m.put = 1;
    for (auto i = 0; i < 64; ++i) {
        if(legalboard & m.put) {
            rev = Flip(&m.put, playerboard, opponentboard);
            m.playerboard = *playerboard ^ (m.put | rev);
            m.opponentboard = *opponentboard ^ rev;
            moveorder.emplace_back(m);
        }
        m.put <<= 1;
    }
    int alpha = MIN_INF, beta = MAX_INF;
    if(Level == 6) {
        think_count = 100/(__builtin_popcountll(legalboard)*5);
        for (search_depth = max(1, DEPTH-4); search_depth <= DEPTH; ++search_depth) {
            afterIndex = nowIndex+search_depth;
            for (board_root& m: moveorder) {
                m.score = move_ordering_value(&m.opponentboard, &m.playerboard);
            }
            sort(moveorder.begin(), moveorder.end());
            alpha = MIN_INF;
            beta = MAX_INF;
            for (board_root& m: moveorder) {
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
            transpose_table_low.swap(former_transpose_table_low);
            transpose_table_up.swap(former_transpose_table_up);
            transpose_table_low.clear();
            transpose_table_up.clear();
        }
    } else {
        for (search_depth = max(1, DEPTH-3); search_depth <= DEPTH; ++search_depth) {
            afterIndex = nowIndex+search_depth;
            for (board_root& m: moveorder) {
                m.score = move_ordering_value(&m.opponentboard, &m.playerboard);
            }
            sort(moveorder.begin(), moveorder.end());
            alpha = MIN_INF;
            beta = MAX_INF;
            for (board_root& m: moveorder) {
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
            transpose_table_low.swap(former_transpose_table_low);
            transpose_table_up.swap(former_transpose_table_up);
            transpose_table_low.clear();
            transpose_table_up.clear();
        }
    }
    former_transpose_table.clear();
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
    return alpha;
}

int search_nega_scout(uint64_t *playerboard, uint64_t *opponentboard) {
    cout << "algorithm: NegaScout" << endl;
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int var = 0, search_depth;
    uint64_t rev;
    board_root m;
    vector<board_root> moveorder;
    m.put = 1;
    for (auto i = 0; i < 64; ++i) {
        if(legalboard & m.put) {
            rev = Flip(&m.put, playerboard, opponentboard);
            m.playerboard = *playerboard ^ (m.put | rev);
            m.opponentboard = *opponentboard ^ rev;
            moveorder.emplace_back(m);
        }
        m.put <<= 1;
    }
    int alpha = MIN_INF, beta = MAX_INF;
    if(Level == 6) {
        think_count = 100/(__builtin_popcountll(legalboard)*5);
        for (search_depth = max(1, DEPTH-4); search_depth <= DEPTH; ++search_depth) {
            afterIndex = nowIndex+search_depth;
            for (board_root& m: moveorder) {
                m.score = move_ordering_value(&m.opponentboard, &m.playerboard);
            }
            sort(moveorder.begin(), moveorder.end());
            alpha = MIN_INF;
            beta = MAX_INF;
            alpha = -nega_scout(search_depth-1, -beta, -alpha, &moveorder[0].opponentboard, &moveorder[0].playerboard);
            if(search_depth == DEPTH) {
                tmpbit = moveorder[0].put;
            }
            for (int i = 1; i < moveorder.size(); ++i) {
                var = -nega_alpha_moveorder(search_depth-1, -alpha-1, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
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
            for (board_root& m: moveorder) {
                m.score = move_ordering_value(&m.opponentboard, &m.playerboard);
            }
            sort(moveorder.begin(), moveorder.end());
            alpha = MIN_INF;
            beta = MAX_INF;
            alpha = -nega_scout(search_depth-1, -beta, -alpha, &moveorder[0].opponentboard, &moveorder[0].playerboard);
            if(search_depth == DEPTH) {
                tmpbit = moveorder[0].put;
            }
            for (uint_fast8_t i = 1; i < moveorder.size(); ++i) {
                var = -nega_alpha_moveorder(search_depth-1, -alpha-1, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
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
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
    return alpha;
}

int nega_scout(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.find(*playerboard) != transpose_table_up.end() && transpose_table_up[*playerboard].find(*opponentboard) != transpose_table_up[*playerboard].end()) {
        u = transpose_table_up[*playerboard][*opponentboard];
        if(u <= alpha) return u;
        beta = min(u, beta);
    }
    if(transpose_table_low.find(*playerboard) != transpose_table_low.end() && transpose_table_low[*playerboard].find(*opponentboard) != transpose_table_low[*playerboard].end()) {
        l = transpose_table_low[*playerboard][*opponentboard];
        if(l >= beta) return l;
        alpha = max(l, alpha);
    }
    if(u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalboard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_scout(depth, -beta, -alpha, opponentboard, playerboard);
    }
    int var, max_score = MIN_INF, canput = 0, a = alpha;
    uint64_t rev;
    board moveorder[__builtin_popcountll(legalboard)];
    uint64_t put;
    while(legalboard) {
        put = legalboard & -legalboard;
        legalboard ^= put;
        rev = Flip(&put, playerboard, opponentboard);
        moveorder[canput].playerboard = *playerboard ^ (put | rev);
        moveorder[canput].opponentboard = *opponentboard ^ rev;
        moveorder[canput].score = move_ordering_value(&moveorder[canput].opponentboard, &moveorder[canput].playerboard);
        ++canput;
    }
    if(canput >= 2) sort(moveorder, moveorder+canput);
    if(depth > 3) {
        var = -nega_scout(depth-1, -beta, -alpha, &moveorder[0].opponentboard, &moveorder[0].playerboard);
        if (var >= beta) {
            if (var > l) {
                transpose_table_low[*playerboard][*opponentboard] = var;
            }
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
        
        for (uint_fast8_t i = 1; i < canput; ++i) {
            var = -nega_alpha_moveorder(depth-1, -alpha-1, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
            if (var >= beta) {
                if(var > l) {
                    transpose_table_low[*playerboard][*opponentboard] = var;
                }
                return var;
            }
            if(var > alpha) {
                alpha = var;
                var = -nega_scout(depth-1, -beta, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard);
                if (var >= beta) {
                    if (var > l) {
                        transpose_table_low[*playerboard][*opponentboard] = var;
                    }
                    return var;
                }
            }
            alpha = max(alpha, var);
            max_score = max(var, max_score);
        }
    } else {
        for (board& m: moveorder) {
            var = -nega_alpha(depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard);
            if (var >= beta) {
                if (var > l) {
                    transpose_table_low[*playerboard][*opponentboard] = var;
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    //hashtableに登録
    if(max_score > a) {
        transpose_table_up[*playerboard][*opponentboard] = max_score;
        transpose_table_low[*playerboard][*opponentboard] = max_score;
    } else {
        transpose_table_up[*playerboard][*opponentboard] = max_score;
    }
    return max_score;
}


int nega_alpha_moveorder(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.find(*playerboard) != transpose_table_up.end() && transpose_table_up[*playerboard].find(*opponentboard) != transpose_table_up[*playerboard].end()) {
        u = transpose_table_up[*playerboard][*opponentboard];
        if(u <= alpha) return u;
        beta = min(u, beta);
    }
    if(transpose_table_low.find(*playerboard) != transpose_table_low.end() && transpose_table_low[*playerboard].find(*opponentboard) != transpose_table_low[*playerboard].end()) {
        l = transpose_table_low[*playerboard][*opponentboard];
        if(l >= beta) return l;
        alpha = max(l, alpha);
    }
    if(u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalboard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_alpha_moveorder(depth, -beta, -alpha, opponentboard, playerboard);
    }
    int var, max_score = MIN_INF, a = alpha;
    uint64_t rev;
    board moveorder[__builtin_popcountll(legalboard)];
    var = 0;
    uint64_t put;
    while(legalboard) {
        put = legalboard & -legalboard;
        legalboard ^= put;
        rev = Flip(&put, playerboard, opponentboard);
        moveorder[var].playerboard = *playerboard ^ (put | rev);
        moveorder[var].opponentboard = *opponentboard ^ rev;
        moveorder[var].score = move_ordering_value(&moveorder[var].opponentboard, &moveorder[var].playerboard);
        ++var;
    }
    if(var >= 2) sort(moveorder, moveorder+var);
    for (board& m: moveorder) {
        var = (depth <= 3) ? -nega_alpha(depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard) : -nega_alpha_moveorder(depth-1, -beta, -alpha, &m.opponentboard, &m.playerboard);
        if (var >= beta) {
            if (var > l) {
                transpose_table_low[*playerboard][*opponentboard] = var;
            }
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
    }
    //hashtableに登録
    if(max_score > a) {
        transpose_table_up[*playerboard][*opponentboard] = max_score;
        transpose_table_low[*playerboard][*opponentboard] = max_score;
    } else {
        transpose_table_up[*playerboard][*opponentboard] = max_score;
    }
    return max_score;
}

int nega_alpha(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, &afterIndex);
    }
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.find(*playerboard) != transpose_table_up.end() && transpose_table_up[*playerboard].find(*opponentboard) != transpose_table_up[*playerboard].end()) {
        u = transpose_table_up[*playerboard][*opponentboard];
        if(u <= alpha) return u;
        beta = min(u, beta);
    }
    if(transpose_table_low.find(*playerboard) != transpose_table_low.end() && transpose_table_low[*playerboard].find(*opponentboard) != transpose_table_low[*playerboard].end()) {
        l = transpose_table_low[*playerboard][*opponentboard];
        if(l >= beta) return l;
        alpha = max(l, alpha);
    }
    if(u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalboard(opponentboard, playerboard))) return countscore(playerboard, opponentboard, &afterIndex);
        else return -nega_alpha(depth, -beta, -alpha, opponentboard, playerboard);
    }
    uint64_t rev = 0;
    int var, max_score = MIN_INF, a = alpha;
    for (uint64_t& i: moveorder_bit) {
        if(canput(&i, &legalboard)) {
            rev = Flip(&i, playerboard, opponentboard);
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
                if (var > l) {
                    transpose_table_low[*playerboard][*opponentboard] = var;
                }
                return var;
            }
            alpha = max(var, alpha);
            max_score = max(max_score, var);
        }
    }
    if(max_score > a) {
        transpose_table_up[*playerboard][*opponentboard] = max_score;
        transpose_table_low[*playerboard][*opponentboard] = max_score;
    } else {
        transpose_table_up[*playerboard][*opponentboard] = max_score;
    }
    return max_score;
}

int search_finish(uint64_t *playerboard, uint64_t *opponentboard) {
    cout << "algorithm: NegaAlpha" << endl;
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int var = 0;
    uint64_t rev;
    board_finish_root moveorder[__builtin_popcountll(legalboard)];
    var = 0;
    for (uint64_t put = 0x8000000000000000; put >= 1; put >>= 1) {
        if(legalboard & put) {
            rev = Flip(&put, playerboard, opponentboard);
            moveorder[var].playerboard = *playerboard ^ (put | rev);
            moveorder[var].opponentboard = *opponentboard ^ rev;
            moveorder[var].legalboard = makelegalboard(&moveorder[var].opponentboard, &moveorder[var].playerboard);
            moveorder[var].score = __builtin_popcountll(moveorder[var].legalboard);
            moveorder[var].put = put;
            ++var;
        }
    }
    think_count = 100/var;
    sort(moveorder, moveorder+var);
    int alpha = MIN_INF, beta = MAX_INF;
    for (board_finish_root& m: moveorder) {
        var = -nega_alpha_moveorder_finish(-beta, -alpha, &m.opponentboard, &m.playerboard, &m.legalboard);
        think_percent += think_count;
        update_think_percent();
        if(var > alpha) {
            alpha = var;
            tmpbit = m.put;
        }
    }
    cout << "final_search" << endl;
    return var;
}

int search_finish_scout(uint64_t *playerboard, uint64_t *opponentboard) {
    cout << "algorithm: NegaScout" << endl;
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int var = 0, score = 0;
    uint64_t rev;
    board_finish_root moveorder[__builtin_popcountll(legalboard)];
    var = 0;
    for (uint64_t put = 0x8000000000000000; put >= 1; put >>= 1) {
        if(legalboard & put) {
            rev = Flip(&put, playerboard, opponentboard);
            moveorder[var].playerboard = *playerboard ^ (put | rev);
            moveorder[var].opponentboard = *opponentboard ^ rev;
            moveorder[var].legalboard = makelegalboard(&moveorder[var].opponentboard, &moveorder[var].playerboard);
            moveorder[var].score = __builtin_popcountll(moveorder[var].legalboard);
            moveorder[var].put = put;
            ++var;
        }
    }
    think_count = 100/var;
    sort(moveorder, moveorder+var);
    int alpha = MIN_INF, beta = MAX_INF;
    alpha = -nega_scout_finish(-beta, -alpha, &moveorder[0].opponentboard, &moveorder[0].playerboard, &moveorder[0].legalboard);
    tmpbit = moveorder[0].put;
    think_percent += think_count;
    update_think_percent();
    for (int i = 1; i < var; ++i) {
        score = -nega_alpha_moveorder_finish(-alpha-1, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard, &moveorder[i].legalboard);
        think_percent += think_count;
        update_think_percent();
        if(score > alpha) {
            alpha = var;
            score = -nega_scout_finish(-beta, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard, &moveorder[i].legalboard);
            tmpbit = moveorder[i].put;
        }
        alpha = max(score, alpha);
    }
    transpose_table_up.swap(former_transpose_table_up);
    transpose_table_up.clear();
    transpose_table_low.swap(former_transpose_table_low);
    transpose_table_low.clear();
    cout << "final_search" << endl;
    return score;
}

int nega_scout_finish(int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard, uint64_t *legalboard) {
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.find(*playerboard) != transpose_table_up.end() && transpose_table_up[*playerboard].find(*opponentboard) != transpose_table_up[*playerboard].end()) {
        u = transpose_table_up[*playerboard][*opponentboard];
        if(u <= alpha) return u;
        beta = min(u, beta);
    }
    if(transpose_table_low.find(*playerboard) != transpose_table_low.end() && transpose_table_low[*playerboard].find(*opponentboard) != transpose_table_low[*playerboard].end()) {
        l = transpose_table_low[*playerboard][*opponentboard];
        if(l >= beta) return l;
        alpha = max(l, alpha);
    }
    if(u == l) return u;
    
    if(!*legalboard) {
        uint64_t legalboard2 = makelegalboard(opponentboard, playerboard);
        if(!legalboard2) return __builtin_popcountll(*playerboard) - __builtin_popcountll(*opponentboard);
        else {
            return -nega_scout_finish(-beta, -alpha, opponentboard, playerboard, &legalboard2);
        }
    }
    int var, max_score = MIN_INF, canput = 0, a = alpha;
    uint64_t rev;
    board_finish moveorder[__builtin_popcountll(*legalboard)];
    var = 0;
    uint64_t put;
    while(*legalboard) {
        put = *legalboard & -*legalboard;
        *legalboard ^= put;
        rev = Flip(&put, playerboard, opponentboard);
        moveorder[var].playerboard = *playerboard ^ (put | rev);
        moveorder[var].opponentboard = *opponentboard ^ rev;
        moveorder[var].legalboard = makelegalboard(&moveorder[var].opponentboard, &moveorder[var].playerboard);
        moveorder[var].score = __builtin_popcountll(moveorder[var].legalboard);
        ++canput;
    }
    if(canput >= 2) sort(moveorder, moveorder+canput);
    if(__builtin_popcountll(*playerboard | *opponentboard) < 56) {
        var = -nega_scout_finish(-beta, -alpha, &moveorder[0].opponentboard, &moveorder[0].playerboard, &moveorder[0].legalboard);
        if (var >= beta) {
            if (var > l) {
                transpose_table_low[*playerboard][*opponentboard] = var;
            }
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
        
        for (int i = 1; i < canput; ++i) {
            var = -nega_alpha_moveorder_finish(-alpha-1, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard, &moveorder[i].legalboard);
            if (var >= beta) {
                if(var > l) {
                    transpose_table_low[*playerboard][*opponentboard] = var;
                }
                return var;
            }
            if(var > alpha) {
                alpha = var;
                var = -nega_scout_finish(-beta, -alpha, &moveorder[i].opponentboard, &moveorder[i].playerboard, &moveorder[i].legalboard);
                if (var >= beta) {
                    if (var > l) {
                        transpose_table_low[*playerboard][*opponentboard] = var;
                    }
                    return var;
                }
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    } else {
        for (board_finish& m: moveorder) {
            var = -nega_alpha_finish(-beta, -alpha, &m.opponentboard, &m.playerboard);
            if (var >= beta) {
                if (var > l) {
                    transpose_table_low[*playerboard][*opponentboard] = var;
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    //hashtableに登録
    if(max_score > a) {
        transpose_table_up[*playerboard][*opponentboard] = max_score;
        transpose_table_low[*playerboard][*opponentboard] = max_score;
    } else {
        transpose_table_up[*playerboard][*opponentboard] = max_score;
    }
    return max_score;
}

int nega_alpha_moveorder_finish(int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard, uint64_t *legalboard) {
    if(!*legalboard) {
        uint64_t legalboard2 = makelegalboard(opponentboard, playerboard);
        if(!legalboard2) return (__builtin_popcountll(*playerboard) - __builtin_popcountll(*opponentboard));
        else {
            return -nega_alpha_moveorder_finish(-beta, -alpha, opponentboard, playerboard, &legalboard2);
        }
    }
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.find(*playerboard) != transpose_table_up.end() && transpose_table_up[*playerboard].find(*opponentboard) != transpose_table_up[*playerboard].end()) {
        u = transpose_table_up[*playerboard][*opponentboard];
        if(u <= alpha) return u;
        beta = min(u, beta);
    }
    if(transpose_table_low.find(*playerboard) != transpose_table_low.end() && transpose_table_low[*playerboard].find(*opponentboard) != transpose_table_low[*playerboard].end()) {
        l = transpose_table_low[*playerboard][*opponentboard];
        if(l >= beta) return l;
        alpha = max(l, alpha);
    }
    if(u == l) return u;
    
    int var, max_score = MIN_INF, a = alpha;
    uint64_t rev;
    board_finish moveorder[__builtin_popcountll(*legalboard)];
    var = 0;
    for (uint64_t put = 0x8000000000000000; put >= 1; put >>= 1) {
        if(*legalboard & put) {
            rev = Flip(&put, playerboard, opponentboard);
            moveorder[var].playerboard = *playerboard ^ (put | rev);
            moveorder[var].opponentboard = *opponentboard ^ rev;
            moveorder[var].legalboard = makelegalboard(&moveorder[var].opponentboard, &moveorder[var].playerboard);
            moveorder[var].score = __builtin_popcountll(moveorder[var].legalboard);
            ++var;
        }
    }
    if(var >= 2) sort(moveorder, moveorder+var);
    if(__builtin_popcountll(*playerboard | *opponentboard) < 56) {
        for (board_finish& m: moveorder) {
            var = -nega_alpha_moveorder_finish(-beta, -alpha, &m.opponentboard, &m.playerboard, &m.legalboard);
            if (var >= beta) {
                if (var > l) {
                    transpose_table_low[*playerboard][*opponentboard] = var;
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    } else {
        for (board_finish& m: moveorder) {
            var = -nega_alpha_finish(-beta, -alpha, &m.opponentboard, &m.playerboard);
            if (var >= beta) {
                if (var > l) {
                    transpose_table_low[*playerboard][*opponentboard] = var;
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    //hashtableに登録
    if(max_score > a) {
        transpose_table_up[*playerboard][*opponentboard] = max_score;
        transpose_table_low[*playerboard][*opponentboard] = max_score;
    } else {
        transpose_table_up[*playerboard][*opponentboard] = max_score;
    }
    return max_score;
}

int nega_alpha_finish(int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard) {
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalboard(opponentboard, playerboard))) return (__builtin_popcountll(*playerboard) - __builtin_popcountll(*opponentboard));
        else {
            return -nega_alpha_finish(-beta, -alpha, opponentboard, playerboard);
        }
    }
    uint64_t rev = 0;
    int var = 0, max_score = MIN_INF;
    for (uint64_t& i: moveorder_bit) {
        if(canput(&i, &legalboard)) {
            rev = Flip(&i, playerboard, opponentboard);
            *playerboard ^= (i | rev);
            *opponentboard ^= rev;
            var = -nega_alpha_finish(-beta, -alpha, opponentboard, playerboard);
            *playerboard ^= (i | rev);
            *opponentboard ^= rev;
            if (var >= beta) {
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
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

int score_stone(const uint64_t *playerboard, const uint64_t *opponentboard) {
    int score = 0;
	
    score += 30 * (__builtin_popcountll(*playerboard & 0x8100000000000081ULL)-__builtin_popcountll(*opponentboard & 0x8100000000000081ULL));
    score += -3 * (__builtin_popcountll(*playerboard & 0x003C424242423C00ULL)-__builtin_popcountll(*opponentboard & 0x003C424242423C00ULL));
    score += -12 * (__builtin_popcountll(*playerboard & 0x4281000000008142ULL)-__builtin_popcountll(*opponentboard & 0x4281000000008142ULL));
    score += -15 * (__builtin_popcountll(*playerboard & 0x0042000000004200ULL)-__builtin_popcountll(*opponentboard & 0x0042000000004200ULL));
    
	//左
    {
        switch (*playerboard & LEFT_BOARD) {
            case 0x0000808080800000ULL:
                score += 2;
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
            case 0x0080808080808000ULL:
                score -= 25;
                break;
            default:
                break;
        }
    }
	//右
    {
        switch (*playerboard & RIGHT_BOARD) {
            case 0x0000010101010000ULL:
                score += 2;
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
            case 0x0001010101010100ULL:
                score -= 25;
                break;
            default:
                break;
        }
    }
	//上
    {
        switch (*playerboard & UP_BOARD) {
            case 0x3c00000000000000ULL:
                score += 2;
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
            case 0x7e00000000000000ULL:
                score -= 25;
                break;
            default:
                break;
        }
    }
	//下
    {
        switch (*playerboard & DOWN_BOARD) {
            case 0x000000000000003cULL:
                score += 2;
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
            case 0x000000000000007eULL:
                score -= 25;
                break;
            default:
                break;
        }
    }
    
    //左上
    if((*playerboard & 0x4000000000000000ULL) && ((*playerboard | *opponentboard) & 0x8000000000000000ULL)) score += 12;
    if((*playerboard & 0x0080000000000000ULL) && ((*playerboard | *opponentboard) & 0x8000000000000000ULL)) score += 12;
    if((*playerboard & 0x0040000000000000ULL) && ((*playerboard | *opponentboard) & 0x8000000000000000ULL)) score += 15;
    if((*opponentboard & 0x4000000000000000ULL) && ((*playerboard | *opponentboard) & 0x8000000000000000ULL)) score -= 12;
    if((*opponentboard & 0x0080000000000000ULL) && ((*playerboard | *opponentboard) & 0x8000000000000000ULL)) score -= 12;
    if((*opponentboard & 0x0040000000000000ULL) && ((*playerboard | *opponentboard) & 0x8000000000000000ULL)) score -= 15;
    
    //右上
    if((*playerboard & 0x0200000000000000ULL) && ((*playerboard | *opponentboard) & 0x0100000000000000ULL)) score += 12;
    if((*playerboard & 0x0001000000000000ULL) && ((*playerboard | *opponentboard) & 0x0100000000000000ULL)) score += 12;
    if((*playerboard & 0x0002000000000000ULL) && ((*playerboard | *opponentboard) & 0x0100000000000000ULL)) score += 15;
    if((*opponentboard & 0x0200000000000000ULL) && ((*playerboard | *opponentboard) & 0x0100000000000000ULL)) score -= 12;
    if((*opponentboard & 0x0001000000000000ULL) && ((*playerboard | *opponentboard) & 0x0100000000000000ULL)) score -= 12;
    if((*opponentboard & 0x0002000000000000ULL) && ((*playerboard | *opponentboard) & 0x0100000000000000ULL)) score -= 15;
    
    //左下
    if((*playerboard & 0x0000000000000040ULL) && ((*playerboard | *opponentboard) & 0x0000000000000080ULL)) score += 12;
    if((*playerboard & 0x0000000000008000ULL) && ((*playerboard | *opponentboard) & 0x0000000000000080ULL)) score += 12;
    if((*playerboard & 0x0000000000004000ULL) && ((*playerboard | *opponentboard) & 0x0000000000000080ULL)) score += 15;
    if((*opponentboard & 0x0000000000000040ULL) && ((*playerboard | *opponentboard) & 0x0000000000000080ULL)) score -= 12;
    if((*opponentboard & 0x0000000000008000ULL) && ((*playerboard | *opponentboard) & 0x0000000000000080ULL)) score -= 12;
    if((*opponentboard & 0x0000000000004000ULL) && ((*playerboard | *opponentboard) & 0x0000000000000080ULL)) score -= 15;
    
    //右下
    if((*playerboard & 0x0000000000000002ULL) && ((*playerboard | *opponentboard) & 0x0000000000000001ULL)) score += 12;
    if((*playerboard & 0x0000000000000100ULL) && ((*playerboard | *opponentboard) & 0x0000000000000001ULL)) score += 12;
    if((*playerboard & 0x0000000000000200ULL) && ((*playerboard | *opponentboard) & 0x0000000000000001ULL)) score += 15;
    if((*opponentboard & 0x0000000000000002ULL) && ((*playerboard | *opponentboard) & 0x0000000000000001ULL)) score -= 12;
    if((*opponentboard & 0x0000000000000100ULL) && ((*playerboard | *opponentboard) & 0x0000000000000001ULL)) score -= 12;
    if((*opponentboard & 0x0000000000000200ULL) && ((*playerboard | *opponentboard) & 0x0000000000000001ULL)) score -= 15;
	return score;
}

int score_putable(const uint64_t *playerboard, const uint64_t *opponentboard) {
	return (__builtin_popcountll(makelegalboard(playerboard, opponentboard))-__builtin_popcountll(makelegalboard(opponentboard, playerboard)));
}

int score_fixedstone(const uint64_t *playerboard, const uint64_t *opponentboard) {
    int fixedstone = 0;
	//上
    if(((*playerboard | *opponentboard) & UP_BOARD) == UP_BOARD) {
		fixedstone += __builtin_popcountll(*playerboard & UP_BOARD);
		fixedstone -= __builtin_popcountll(*opponentboard & UP_BOARD);
	} else {
		//左上左方向
        if((*playerboard & 0xfe00000000000000ULL) == 0xfe00000000000000ULL) fixedstone+=7;
        else if((*playerboard & 0xfc00000000000000ULL) == 0xfc00000000000000ULL) fixedstone+=6;
        else if((*playerboard & 0xf800000000000000ULL) == 0xf800000000000000ULL) fixedstone+=5;
        else if((*playerboard & 0xf000000000000000ULL) == 0xf000000000000000ULL) fixedstone+=4;
        else if((*playerboard & 0xe000000000000000ULL) == 0xe000000000000000ULL) fixedstone+=3;
        else if((*playerboard & 0xc000000000000000ULL) == 0xc000000000000000ULL) fixedstone+=2;
        else if((*playerboard & 0x8000000000000000ULL) == 0x8000000000000000ULL) fixedstone+=1;
		
        if((*opponentboard & 0xfe00000000000000ULL) == 0xfe00000000000000ULL) fixedstone-=7;
        else if((*opponentboard & 0xfc00000000000000ULL) == 0xfc00000000000000ULL) fixedstone-=6;
        else if((*opponentboard & 0xf800000000000000ULL) == 0xf800000000000000ULL) fixedstone-=5;
        else if((*opponentboard & 0xf000000000000000ULL) == 0xf000000000000000ULL) fixedstone-=4;
        else if((*opponentboard & 0xe000000000000000ULL) == 0xe000000000000000ULL) fixedstone-=3;
        else if((*opponentboard & 0xc000000000000000ULL) == 0xc000000000000000ULL) fixedstone-=2;
        else if((*opponentboard & 0x8000000000000000ULL) == 0x8000000000000000ULL) fixedstone-=1;
		//右上左方向
        if((*playerboard & 0x7f00000000000000ULL) == 0x7f00000000000000ULL) fixedstone+=7;
        else if((*playerboard & 0x3f00000000000000ULL) == 0x3f00000000000000ULL) fixedstone+=6;
        else if((*playerboard & 0x1f00000000000000ULL) == 0x1f00000000000000ULL) fixedstone+=5;
        else if((*playerboard & 0x0f00000000000000ULL) == 0x0f00000000000000ULL) fixedstone+=4;
        else if((*playerboard & 0x0700000000000000ULL) == 0x0700000000000000ULL) fixedstone+=3;
        else if((*playerboard & 0x0300000000000000ULL) == 0x0300000000000000ULL) fixedstone+=2;
        else if((*playerboard & 0x0100000000000000ULL) == 0x0100000000000000ULL) fixedstone+=1;
		
        if((*opponentboard & 0x7f00000000000000ULL) == 0x7f00000000000000ULL) fixedstone-=7;
        else if((*opponentboard & 0x3f00000000000000ULL) == 0x3f00000000000000ULL) fixedstone-=6;
        else if((*opponentboard & 0x1f00000000000000ULL) == 0x1f00000000000000ULL) fixedstone-=5;
        else if((*opponentboard & 0x0f00000000000000ULL) == 0x0f00000000000000ULL) fixedstone-=4;
        else if((*opponentboard & 0x0700000000000000ULL) == 0x0700000000000000ULL) fixedstone-=3;
        else if((*opponentboard & 0x0300000000000000ULL) == 0x0300000000000000ULL) fixedstone-=2;
        else if((*opponentboard & 0x0100000000000000ULL) == 0x0100000000000000ULL) fixedstone-=1;
	}
	//左
    if(((*playerboard | *opponentboard) & LEFT_BOARD) == LEFT_BOARD) {
		fixedstone += __builtin_popcountll(*playerboard & LEFT_BOARD);
		fixedstone -= __builtin_popcountll(*opponentboard & LEFT_BOARD);
	} else {
		//左上下方向
        if((*playerboard & 0x8080808080808000ULL) == 0x8080808080808000ULL) fixedstone+=7;
        else if((*playerboard & 0x8080808080800000ULL) == 0x8080808080800000ULL) fixedstone+=6;
        else if((*playerboard & 0x8080808080000000ULL) == 0x8080808080000000ULL) fixedstone+=5;
        else if((*playerboard & 0x8080808000000000ULL) == 0x8080808000000000ULL) fixedstone+=4;
        else if((*playerboard & 0x8080800000000000ULL) == 0x8080800000000000ULL) fixedstone+=3;
        else if((*playerboard & 0x8080008000000000ULL) == 0x8080008000000000ULL) fixedstone+=2;
        else if((*playerboard & 0x8080008000000000ULL) == 0x8000008000000000ULL) fixedstone+=1;
		
        if((*opponentboard & 0x8080808080808000ULL) == 0x8080808080808000ULL) fixedstone-=7;
        else if((*opponentboard & 0x8080808080800000ULL) == 0x8080808080800000ULL) fixedstone-=6;
        else if((*opponentboard & 0x8080808080000000ULL) == 0x8080808080000000ULL) fixedstone-=5;
        else if((*opponentboard & 0x8080808000000000ULL) == 0x8080808000000000ULL) fixedstone-=4;
        else if((*opponentboard & 0x8080800000000000ULL) == 0x8080800000000000ULL) fixedstone-=3;
        else if((*opponentboard & 0x8080008000000000ULL) == 0x8080008000000000ULL) fixedstone-=2;
        else if((*opponentboard & 0x8080008000000000ULL) == 0x8000008000000000ULL) fixedstone-=1;
        
		//左下上方向
        if((*playerboard & 0x0080808080808080ULL) == 0x0080808080808080ULL) fixedstone+=7;
        else if((*playerboard & 0x0000808080808080ULL) == 0x0000808080808080ULL) fixedstone+=6;
        else if((*playerboard & 0x0000008080808080ULL) == 0x0000008080808080ULL) fixedstone+=5;
        else if((*playerboard & 0x0000000080808080ULL) == 0x0000000080808080ULL) fixedstone+=4;
        else if((*playerboard & 0x0000008000808080ULL) == 0x0000008000808080ULL) fixedstone+=3;
        else if((*playerboard & 0x0000008000008080ULL) == 0x0000008000008080ULL) fixedstone+=2;
        else if((*playerboard & 0x0000008000000080ULL) == 0x0000008000000080ULL) fixedstone+=1;
		
        if((*opponentboard & 0x0080808080808080ULL) == 0x0080808080808080ULL) fixedstone-=7;
        else if((*opponentboard & 0x0000808080808080ULL) == 0x0000808080808080ULL) fixedstone-=6;
        else if((*opponentboard & 0x0000008080808080ULL) == 0x0000008080808080ULL) fixedstone-=5;
        else if((*opponentboard & 0x0000000080808080ULL) == 0x0000000080808080ULL) fixedstone-=4;
        else if((*opponentboard & 0x0000008000808080ULL) == 0x0000008000808080ULL) fixedstone-=3;
        else if((*opponentboard & 0x0000008000008080ULL) == 0x0000008000008080ULL) fixedstone-=2;
        else if((*opponentboard & 0x0000008000000080ULL) == 0x0000008000000080ULL) fixedstone-=1;
	}
	//右
    if(((*playerboard | *opponentboard) & RIGHT_BOARD) == RIGHT_BOARD) {
		fixedstone += __builtin_popcountll(*playerboard & RIGHT_BOARD);
		fixedstone -= __builtin_popcountll(*opponentboard & RIGHT_BOARD);
	} else {
		//右上下方向
        if((*playerboard & 0x0101010101010100ULL) == 0x0101010101010100ULL) fixedstone+=7;
        else if((*playerboard & 0x0101010101010000ULL) == 0x0101010101010000ULL) fixedstone+=6;
        else if((*playerboard & 0x0101010101000000ULL) == 0x0101010101000000ULL) fixedstone+=5;
        else if((*playerboard & 0x0101010100000000ULL) == 0x0101010100000000ULL) fixedstone+=4;
        else if((*playerboard & 0x0101010000000000ULL) == 0x0101010000000000ULL) fixedstone+=3;
        else if((*playerboard & 0x0101000000000000ULL) == 0x0101000000000000ULL) fixedstone+=2;
        else if((*playerboard & 0x0100000000000000ULL) == 0x0100000000000000ULL) fixedstone+=1;
		
        if((*opponentboard & 0x0101010101010100ULL) == 0x0101010101010100ULL) fixedstone-=7;
        else if((*opponentboard & 0x0101010101010000ULL) == 0x0101010101010000ULL) fixedstone-=6;
        else if((*opponentboard & 0x0101010101000000ULL) == 0x0101010101000000ULL) fixedstone-=5;
        else if((*opponentboard & 0x0101010100000000ULL) == 0x0101010100000000ULL) fixedstone-=4;
        else if((*opponentboard & 0x0101010000000000ULL) == 0x0101010000000000ULL) fixedstone-=3;
        else if((*opponentboard & 0x0101000000000000ULL) == 0x0101000000000000ULL) fixedstone-=2;
        else if((*opponentboard & 0x0100000000000000ULL) == 0x0100000000000000ULL) fixedstone-=1;
        
		//右下上方向
        if((*playerboard & 0x001010101010101ULL) == 0x001010101010101ULL) fixedstone+=7;
        else if((*playerboard & 0x000010101010101ULL) == 0x000010101010101ULL) fixedstone+=6;
        else if((*playerboard & 0x000000101010101ULL) == 0x000000101010101ULL) fixedstone+=5;
        else if((*playerboard & 0x000000001010101ULL) == 0x000000001010101ULL) fixedstone+=4;
        else if((*playerboard & 0x000000000010101ULL) == 0x000000000010101ULL) fixedstone+=3;
        else if((*playerboard & 0x000000000000101ULL) == 0x000000000000101ULL) fixedstone+=2;
        else if((*playerboard & 0x000000000000001ULL) == 0x000000000000001ULL) fixedstone+=1;
		
        if((*opponentboard & 0x001010101010101ULL) == 0x001010101010101ULL) fixedstone-=7;
        else if((*opponentboard & 0x000010101010101ULL) == 0x000010101010101ULL) fixedstone-=6;
        else if((*opponentboard & 0x000000101010101ULL) == 0x000000101010101ULL) fixedstone-=5;
        else if((*opponentboard & 0x000000001010101ULL) == 0x000000001010101ULL) fixedstone-=4;
        else if((*opponentboard & 0x000000000010101ULL) == 0x000000000010101ULL) fixedstone-=3;
        else if((*opponentboard & 0x000000000000101ULL) == 0x000000000000101ULL) fixedstone-=2;
        else if((*opponentboard & 0x000000000000001ULL) == 0x000000000000001ULL) fixedstone-=1;
	}
	//下
    if(((*playerboard | *opponentboard) & DOWN_BOARD) == DOWN_BOARD) {
		fixedstone += __builtin_popcountll(*playerboard & DOWN_BOARD);
		fixedstone -= __builtin_popcountll(*opponentboard & DOWN_BOARD);
	} else {
		//左下右方向
        if((*playerboard & 0x00000000000000feULL) == 0x00000000000000feULL) fixedstone+=7;
        else if((*playerboard & 0x00000000000000fcULL) == 0x00000000000000fcULL) fixedstone+=6;
        else if((*playerboard & 0x00000000000000f8ULL) == 0x00000000000000f8ULL) fixedstone+=5;
        else if((*playerboard & 0x00000000000000f0ULL) == 0x00000000000000f0ULL) fixedstone+=4;
        else if((*playerboard & 0x00000000000000e0ULL) == 0x00000000000000e0ULL) fixedstone+=3;
        else if((*playerboard & 0x00000000000000c0ULL) == 0x00000000000000c0ULL) fixedstone+=2;
        else if((*playerboard & 0x0000000000000080ULL) == 0x0000000000000080ULL) fixedstone+=1;
        
        if((*opponentboard & 0x00000000000000feULL) == 0x00000000000000feULL) fixedstone-=7;
        else if((*opponentboard & 0x00000000000000fcULL) == 0x00000000000000fcULL) fixedstone-=6;
        else if((*opponentboard & 0x00000000000000f8ULL) == 0x00000000000000f8ULL) fixedstone-=5;
        else if((*opponentboard & 0x00000000000000f0ULL) == 0x00000000000000f0ULL) fixedstone-=4;
        else if((*opponentboard & 0x00000000000000e0ULL) == 0x00000000000000e0ULL) fixedstone-=3;
        else if((*opponentboard & 0x00000000000000c0ULL) == 0x00000000000000c0ULL) fixedstone-=2;
        else if((*opponentboard & 0x0000000000000080ULL) == 0x0000000000000080ULL) fixedstone-=1;
		
		//右下左方向
        if((*playerboard & 0x000000000000007fULL) == 0x000000000000007fULL) fixedstone+=7;
        else if((*playerboard & 0x000000000000003fULL) == 0x000000000000003fULL) fixedstone+=6;
        else if((*playerboard & 0x000000000000001fULL) == 0x000000000000001fULL) fixedstone+=5;
        else if((*playerboard & 0x000000000000000fULL) == 0x000000000000000fULL) fixedstone+=4;
        else if((*playerboard & 0x0000000000000007ULL) == 0x0000000000000007ULL) fixedstone+=3;
        else if((*playerboard & 0x0000000000000003ULL) == 0x0000000000000003ULL) fixedstone+=2;
        else if((*playerboard & 0x0000000000000001ULL) == 0x0000000000000001ULL) fixedstone+=1;
        
        if((*opponentboard & 0x000000000000007fULL) == 0x000000000000007fULL) fixedstone-=7;
        else if((*opponentboard & 0x000000000000003fULL) == 0x000000000000003fULL) fixedstone-=6;
        else if((*opponentboard & 0x000000000000001fULL) == 0x000000000000001fULL) fixedstone-=5;
        else if((*opponentboard & 0x000000000000000fULL) == 0x000000000000000fULL) fixedstone-=4;
        else if((*opponentboard & 0x0000000000000007ULL) == 0x0000000000000007ULL) fixedstone-=3;
        else if((*opponentboard & 0x0000000000000003ULL) == 0x0000000000000003ULL) fixedstone-=2;
        else if((*opponentboard & 0x0000000000000001ULL) == 0x0000000000000001ULL) fixedstone-=1;
		
	}
    if(*playerboard & 0x8000000000000000ULL) fixedstone--;
    if(*playerboard & 0x0100000000000000ULL) fixedstone--;
    if(*playerboard & 0x0000000000000080ULL) fixedstone--;
    if(*playerboard & 0x0000000000000001ULL) fixedstone--;
    
    if(*opponentboard & 0x8000000000000000ULL) fixedstone++;
    if(*opponentboard & 0x0100000000000000ULL) fixedstone++;
    if(*opponentboard & 0x0000000000000080ULL) fixedstone++;
    if(*opponentboard & 0x0000000000000001ULL) fixedstone++;
        
	return fixedstone;
}

int countscore(const uint64_t *playerboard, const uint64_t *opponentboard, const int *afterIndex) {
    return (*afterIndex >= 60) ? __builtin_popcountll(*playerboard) - __builtin_popcountll(*opponentboard) :
           (!*playerboard) ? MIN_INF :
           (!*opponentboard) ? MAX_INF :
           (*afterIndex >= 45) ? score_stone(playerboard, opponentboard)*6 + score_fixedstone(playerboard, opponentboard) * 55:
           score_stone(playerboard, opponentboard)*6 + score_fixedstone(playerboard, opponentboard)*55 + score_putable(playerboard, opponentboard)*11;
}
