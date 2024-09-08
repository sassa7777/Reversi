//
//  othello.cpp
//  othello
//
//  Created by sasa on 2023/09/30.
//
#include "variables.h"
#include "Wrapper.h"

using namespace std;

void reset() {
	printf("[*]初期化中...\n");
    nowTurn = BLACK_TURN;
    nowIndex = 1;
    DEPTH = firstDEPTH;
    afterIndex = nowIndex+DEPTH;
    b.playerboard = 0x0000000810000000ULL;
    b.opponentboard = 0x0000001008000000ULL;
    printf("DEPTH: %d\n", DEPTH);
    printf("Player: %d\n", botplayer);
	return;
}

void fixedstone_table_init() {
    fixedstone_table.clear();
    int i;
    pair<uint64_t, uint64_t> tmp;
    for (tmp.first = 0; tmp.first <= 0xFF; ++tmp.first) {
        for (tmp.second = 0; tmp.second <= 0xFF; ++tmp.second) {
            if(tmp.first & tmp.second) continue;
            i = score_fixedstone(tmp.first, tmp.second);
            fixedstone_table[tmp] = i;
            tmp.first = rotateClockwise90(tmp.first);
            tmp.second = rotateClockwise90(tmp.second);
            fixedstone_table[tmp] = i;
            tmp.first = rotateClockwise90(tmp.first);
            tmp.second = rotateClockwise90(tmp.second);
            fixedstone_table[tmp] = i;
            tmp.first = rotateClockwise90(tmp.first);
            tmp.second = rotateClockwise90(tmp.second);
            fixedstone_table[tmp] = i;
            tmp.first = rotateClockwise90(tmp.first);
            tmp.second = rotateClockwise90(tmp.second);
        }
    }
    printf("initialized\n");
}

int putstone(int_fast8_t y, int_fast8_t x) {
	tmpy = y;
	tmpx = x;
	uint64_t put = cordinate_to_bit(x, y);
	legalboard = makelegalboard(b.playerboard, b.opponentboard);
	if(canput(put, legalboard)) {
		reversebit(put);
		nowIndex++;
		afterIndex++;
		return 1;
	} else {
		return 0;
	}
}

inline uint64_t cordinate_to_bit(int_fast8_t put, int_fast8_t y) {
	return 0x8000000000000000ULL >> ((y<<3)+put);
}

inline bool canput(const uint64_t &put, const uint64_t &legalboard) {
	return ((put & legalboard) == put);
}

inline uint64_t makelegalboard(const uint64_t &p, const uint64_t &o) {
    uint64_t moves, hb, flip1, flip7, flip9, flip8, pre1, pre7, pre9, pre8;
    
    hb = o & 0x7e7e7e7e7e7e7e7eULL;
    flip1  = hb & (p << 1);    flip7  = hb & (p << 7);        flip9  = hb & (p << 9);        flip8  = o & (p << 8);
    flip1 |= hb & (flip1 << 1);    flip7 |= hb & (flip7 << 7);    flip9 |= hb & (flip9 << 9);    flip8 |= o & (flip8 << 8);
    pre1 = hb & (hb << 1);         pre7 = hb & (hb << 7);        pre9 = hb & (hb << 9);        pre8 = o & (o << 8);
    flip1 |= pre1 & (flip1 << 2);    flip7 |= pre7 & (flip7 << 14);    flip9 |= pre9 & (flip9 << 18);    flip8 |= pre8 & (flip8 << 16);
    flip1 |= pre1 & (flip1 << 2);    flip7 |= pre7 & (flip7 << 14);    flip9 |= pre9 & (flip9 << 18);    flip8 |= pre8 & (flip8 << 16);
    moves = flip1 << 1;        moves |= flip7 << 7;        moves |= flip9 << 9;        moves |= flip8 << 8;
    flip1  = hb & (p >> 1);        flip7  = hb & (p >> 7);        flip9  = hb & (p >> 9);        flip8  = o & (p >> 8);
    flip1 |= hb & (flip1 >> 1);    flip7 |= hb & (flip7 >> 7);    flip9 |= hb & (flip9 >> 9);    flip8 |= o & (flip8 >> 8);
    pre1 >>= 1;            pre7 >>= 7;            pre9 >>= 9;            pre8 >>= 8;
    flip1 |= pre1 & (flip1 >> 2);    flip7 |= pre7 & (flip7 >> 14);    flip9 |= pre9 & (flip9 >> 18);    flip8 |= pre8 & (flip8 >> 16);
    flip1 |= pre1 & (flip1 >> 2);    flip7 |= pre7 & (flip7 >> 14);    flip9 |= pre9 & (flip9 >> 18);    flip8 |= pre8 & (flip8 >> 16);
    moves |= flip1 >> 1;        moves |= flip7 >> 7;        moves |= flip9 >> 9;        moves |= flip8 >> 8;
    
    return moves & ~(p|o);
}

void reversebit(uint64_t put) {
    uint64_t flipped, OM, outflank[4], mask[4];
    int pos = __builtin_ctzll(put);
    OM = b.opponentboard & 0x7e7e7e7e7e7e7e7eULL;

    mask[0] = 0x0080808080808080ULL >> (63 - pos);
    mask[1] = 0x7f00000000000000ULL >> (63 - pos);
    mask[2] = 0x0102040810204000ULL >> (63 - pos);
    mask[3] = 0x0040201008040201ULL >> (63 - pos);
    outflank[0] = (0x8000000000000000ULL >> __builtin_clzll(((b.opponentboard) & (((mask[0]) & ((mask[0]) - 1)))) ^ (mask[0]))) & b.playerboard;
    outflank[1] = (0x8000000000000000ULL >> __builtin_clzll(((OM) & (((mask[1]) & ((mask[1]) - 1)))) ^ (mask[1]))) & b.playerboard;
    outflank[2] = (0x8000000000000000ULL >> __builtin_clzll(((OM) & (((mask[2]) & ((mask[2]) - 1)))) ^ (mask[2]))) & b.playerboard;
    outflank[3] = (0x8000000000000000ULL >> __builtin_clzll(((OM) & (((mask[3]) & ((mask[3]) - 1)))) ^ (mask[3]))) & b.playerboard;
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

inline uint64_t Flip(const uint64_t &put, const uint64_t &playerboard, const uint64_t &opponentboard) {
    uint64_t flipped, OM, outflank[4], mask[4];
    int pos = __builtin_clzll(put);
    OM = opponentboard & 0x7e7e7e7e7e7e7e7eULL;

    mask[0] = 0x0080808080808080ULL >> (pos);
    mask[1] = 0x7f00000000000000ULL >> (pos);
    mask[2] = 0x0102040810204000ULL >> (pos);
    mask[3] = 0x0040201008040201ULL >> (pos);
    outflank[0] = (0x8000000000000000ULL >> __builtin_clzll(((opponentboard) & (((mask[0]) & ((mask[0]) - 1)))) ^ (mask[0]))) & playerboard;
    outflank[1] = (0x8000000000000000ULL >> __builtin_clzll(((OM) & (((mask[1]) & ((mask[1]) - 1)))) ^ (mask[1]))) & playerboard;
    outflank[2] = (0x8000000000000000ULL >> __builtin_clzll(((OM) & (((mask[2]) & ((mask[2]) - 1)))) ^ (mask[2]))) & playerboard;
    outflank[3] = (0x8000000000000000ULL >> __builtin_clzll(((OM) & (((mask[3]) & ((mask[3]) - 1)))) ^ (mask[3]))) & playerboard;
    flipped  = (-outflank[0] << 1) & mask[0];
    flipped |= (-outflank[1] << 1) & mask[1];
    flipped |= (-outflank[2] << 1) & mask[2];
    flipped |= (-outflank[3] << 1) & mask[3];

    pos = 63 - pos;
    
    mask[0] = 0x0101010101010100ULL << pos;
    mask[1] = 0x00000000000000feULL << pos;
    mask[2] = 0x0002040810204080ULL << pos;
    mask[3] = 0x8040201008040200ULL << pos;
    outflank[0] = mask[0] & ((opponentboard | ~mask[0]) + 1) & playerboard;
    outflank[1] = mask[1] & ((OM | ~mask[1]) + 1) & playerboard;
    outflank[2] = mask[2] & ((OM | ~mask[2]) + 1) & playerboard;
    outflank[3] = mask[3] & ((OM | ~mask[3]) + 1) & playerboard;
    flipped |= (outflank[0] - (uint32_t)(outflank[0] != 0)) & mask[0];
    flipped |= (outflank[1] - (uint32_t)(outflank[1] != 0)) & mask[1];
    flipped |= (outflank[2] - (uint32_t)(outflank[2] != 0)) & mask[2];
    flipped |= (outflank[3] - (uint32_t)(outflank[3] != 0)) & mask[3];

    return flipped;
}

bool isPass() {
	return (makelegalboard(b.playerboard, b.opponentboard) == 0 && makelegalboard(b.opponentboard, b.playerboard) != 0);
}

bool isFinished() {
	return (makelegalboard(b.playerboard, b.opponentboard) == 0 && makelegalboard(b.opponentboard, b.playerboard) == 0);
}

void swapboard() {
//    printf("現在の評価値: %d %d %d\n", score_stone(b.playerboard, b.opponentboard), score_putable(b.playerboard, b.opponentboard), score_fixedstone_table(b.playerboard, b.opponentboard));
    b.playerboard = b.playerboard ^ b.opponentboard;
    b.opponentboard = b.opponentboard ^ b.playerboard;
    b.playerboard = b.playerboard ^ b.opponentboard;
	nowTurn = -nowTurn;
}

inline int move_ordering_value(uint64_t &playerboard, uint64_t &opponentboard) {
    if(afterIndex >= 64) return -__builtin_popcountll(makelegalboard(playerboard, opponentboard));
    auto tmp = make_pair(playerboard, opponentboard);
    if(former_transpose_table_up.count(tmp)) {
        return (1000-former_transpose_table_up[tmp]);
    } else if(former_transpose_table_low.count(tmp)) {
        return (1000-former_transpose_table_low[tmp]);
    } else {
        return countscore(opponentboard, playerboard, afterIndex);
    }
}

uint64_t delta_swap(uint64_t& x, uint64_t mask, int delta) {
    uint64_t t = (x ^ (x >> delta)) & mask;
    return x ^ t ^ (t << delta);
}

// 水平反転
uint64_t flipHorizontal(uint64_t x) {
    x = delta_swap(x, 0x5555555555555555, 1);
    x = delta_swap(x, 0x3333333333333333, 2);
    return delta_swap(x, 0x0F0F0F0F0F0F0F0F, 4);
}

// 垂直反転
uint64_t flipVertical(uint64_t x) {
    x = delta_swap(x, 0x00FF00FF00FF00FF, 8);
    x = delta_swap(x, 0x0000FFFF0000FFFF, 16);
    return delta_swap(x, 0x00000000FFFFFFFF, 32);
}

// A1-H8反転
uint64_t flipDiagonalA1H8(uint64_t x) {
    x = delta_swap(x, 0x00AA00AA00AA00AA, 7);
    x = delta_swap(x, 0x0000CCCC0000CCCC, 14);
    return delta_swap(x, 0x00000000F0F0F0F0, 28);
}

// A8-H1反転
uint64_t flipDiagonalA8H1(uint64_t x) {
    x = delta_swap(x, 0x0055005500550055, 9);
    x = delta_swap(x, 0x0000333300003333, 18);
    return delta_swap(x, 0x000000000F0F0F0F, 36);
}

// 時計回りに90度回転
uint64_t rotateClockwise90(uint64_t x) {
    return flipHorizontal(flipDiagonalA1H8(x));
}

uint64_t book_finder(pair<uint64_t, uint64_t> &board) {
    if(book_usi.count(board))  {
        printf("定石: 牛\n");
        return book_usi.at(board);
    }
    if(book_tiger.count(board)) {
        printf("定石: 虎\n");
        return book_tiger.at(board);
    }
    if(book_conpos.count(board)) {
        printf("定石: コンポス\n");
        return book_conpos.at(board);
    }
    if(book_stevenson.count(board)) {
        printf("定石: スティーブンソン\n");
        return book_stevenson.at(board);
    }
    if(book_FATDraw.count(board)) {
        printf("定石: FATDraw\n");
        return book_FATDraw.at(board);
    }
    if(book_no_kan.count(board)) {
        printf("定石: ノーカン\n");
        return book_no_kan.at(board);
    }
    if(book_ura_yotto.count(board)) {
        printf("定石: 裏ヨット\n");
        return book_ura_yotto.at(board);
    }
    if(book_ura_koumori.count(board)) {
        printf("定石: 裏コウモリ\n");
        return book_ura_koumori.at(board);
    }
    if(book_kaisoku_senn.count(board)) {
        printf("定石: 快速船\n");
        return book_kaisoku_senn.at(board);
    }
    if(book_sennsya.count(board)) {
        printf("定石: 戦車\n");
        return book_sennsya.at(board);
    }
    return 0;
}

void book(uint64_t &playerboard, uint64_t &opponentboard) {
    pair<uint64_t, uint64_t> board = make_pair(playerboard, opponentboard);
    uint64_t put = 0;
    put = book_finder(board);
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = flipHorizontal(board.first);
    board.second = flipHorizontal(board.second);
    put = flipHorizontal(book_finder(board));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = rotateClockwise90(board.first);
    board.second = rotateClockwise90(board.second);
    put = rotateClockwise90(flipHorizontal(book_finder(board)));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = flipHorizontal(board.first);
    board.second = flipHorizontal(board.second);
    put = flipHorizontal(rotateClockwise90(flipHorizontal(book_finder(board))));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = rotateClockwise90(board.first);
    board.second = rotateClockwise90(board.second);
    put = rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(book_finder(board)))));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = flipHorizontal(board.first);
    board.second = flipHorizontal(board.second);
    put = flipHorizontal(rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(book_finder(board))))));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = rotateClockwise90(board.first);
    board.second = rotateClockwise90(board.second);
    put = rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(book_finder(board)))))));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = flipHorizontal(board.first);
    board.second = flipHorizontal(board.second);
    put = flipHorizontal(rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(book_finder(board))))))));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = rotateClockwise90(board.first);
    board.second = rotateClockwise90(board.second);
    board.first = flipDiagonalA1H8(board.first);
    board.second = flipDiagonalA1H8(board.second);
    put = flipDiagonalA1H8(book_finder(board));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = flipHorizontal(board.first);
    board.second = flipHorizontal(board.second);
    put = flipHorizontal(flipDiagonalA1H8(book_finder(board)));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = rotateClockwise90(board.first);
    board.second = rotateClockwise90(board.second);
    put = rotateClockwise90(flipHorizontal(flipDiagonalA1H8(book_finder(board))));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = flipHorizontal(board.first);
    board.second = flipHorizontal(board.second);
    put = flipHorizontal(rotateClockwise90(flipHorizontal(flipDiagonalA1H8(book_finder(board)))));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = rotateClockwise90(board.first);
    board.second = rotateClockwise90(board.second);
    put = rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(flipDiagonalA1H8(book_finder(board))))));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = flipHorizontal(board.first);
    board.second = flipHorizontal(board.second);
    put = flipHorizontal(rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(flipDiagonalA1H8(book_finder(board)))))));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = rotateClockwise90(board.first);
    board.second = rotateClockwise90(board.second);
    put = rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(flipDiagonalA1H8(book_finder(board))))))));
    if(put) {
        tmpbit = put;
        return;
    }
    board.first = flipHorizontal(board.first);
    board.second = flipHorizontal(board.second);
    put = flipHorizontal(rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(rotateClockwise90(flipHorizontal(flipDiagonalA1H8(book_finder(board)))))))));
    if(put) {
        tmpbit = put;
        return;
    }
    return;
}

int search_nega_scout_beta(uint64_t &playerboard, uint64_t &opponentboard) {
    printf("algorithm: NegaScout\n");
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int var = 0;
    uint64_t rev;
    board_root m;
    vector<board_root> moveorder;
    moveorder.reserve(__builtin_popcountll(legalboard));
    m.put = 1;
    for (auto i = 0; i < 64; ++i) {
        if(legalboard & m.put) {
            rev = Flip(m.put, playerboard, opponentboard);
            m.playerboard = playerboard ^ (m.put | rev);
            m.opponentboard = opponentboard ^ rev;
            moveorder.emplace_back(m);
        }
        m.put <<= 1;
    }
    DEPTH = 64-nowIndex;
    afterIndex = 64;
    search_depth = DEPTH;
    int alpha = MIN_INF, beta = MAX_INF;
    think_count = 100/(__builtin_popcountll(legalboard));
    for (auto& m: moveorder) {
        m.score = move_ordering_value(m.opponentboard, m.playerboard);
    }
    sort(moveorder.begin(), moveorder.end());
    alpha = MIN_INF;
    beta = MAX_INF;
    alpha = -nega_scout(search_depth-1, -beta, -alpha, moveorder[0].opponentboard, moveorder[0].playerboard);
    if(search_depth == DEPTH) {
        tmpbit = moveorder[0].put;
    }
    think_percent += think_count;
    update_think_percent();
    for (auto i = 1; i < moveorder.size(); ++i) {
        var = -nega_alpha_moveorder(search_depth-1, -alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
        think_percent += think_count;
        update_think_percent();
        if(var > alpha) {
            alpha = var;
            var = -nega_scout(search_depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
            if(search_depth == DEPTH) {
                tmpbit = moveorder[i].put;
            }
        }
        alpha = max(var, alpha);
    }
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
    return alpha;
}

int ai() {
	if (nowTurn == -botplayer) {
		return 0;
	}
	printf("[*]Botが考え中..\n");
    if(Level == 5 && nowIndex >= 29) {
        DEPTH = 12;
    }
//    if(Level >= 6 && nowIndex >= 33) {
//        DEPTH = 13;
//        afterIndex=nowIndex+DEPTH;
//    }
    if(Level >= 5 && nowIndex >= 40) {
        DEPTH = 20;
        afterIndex=60;
    }
	tmpx = -1;
	tmpy = -1;
	tmpbit = 0;
	think_percent = 0;
    update_think_percent();
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
	legalboard = makelegalboard(b.playerboard, b.opponentboard);
	int putable_count = __builtin_popcountll(legalboard);
    visited_nodes = 0;
    int score = 0;
    if(Level >= 4) book(b.playerboard, b.opponentboard);
    if(!tmpbit) {
        if(afterIndex >= 60) {
            think_count = 100/putable_count;
//            score = search_finish(b.playerboard, b.opponentboard);
            score = search_finish_scout(b.playerboard, b.opponentboard);
//            score = search_nega_scout_beta(b.playerboard, b.opponentboard);
        } else  {
            think_count = 100/(putable_count*(DEPTH+1-max(DEPTH-3, 1)));
//            score = search(b.playerboard, b.opponentboard);
            score = search_nega_scout(b.playerboard, b.opponentboard);
        }
    }
	think_percent = 100;
    update_think_percent();
    int count = __builtin_clzll(tmpbit);
    tmpy = count / 8;
    tmpx = count % 8;
    putstone(tmpy, tmpx);
    printf("put : (%d, %d)\n", tmpx, tmpy);
    if(afterIndex >= 60) printf("Final Score\n");
    printf("Score : %d\n", score);
    return 1;
}

int search(uint64_t &playerboard, uint64_t &opponentboard) {
    printf("algorithm: NegaAlpha\n");
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int var = 0;
    uint64_t rev;
    board_root m;
    vector<board_root> moveorder;
    moveorder.reserve(__builtin_popcountll(legalboard));
    m.put = 1;
    for (auto i = 0; i < 64; ++i) {
        if(legalboard & m.put) {
            rev = Flip(m.put, playerboard, opponentboard);
            m.playerboard = playerboard ^ (m.put | rev);
            m.opponentboard = opponentboard ^ rev;
            moveorder.emplace_back(m);
        }
        m.put <<= 1;
    }
    int alpha = MIN_INF, beta = MAX_INF;
    if(Level == 7) {
        think_count = 100/(__builtin_popcountll(legalboard)*4);
        for (search_depth = DEPTH-4; search_depth <= DEPTH; search_depth+=2) {
            afterIndex = nowIndex+search_depth;
            for (auto& m: moveorder) {
                m.score = move_ordering_value(m.opponentboard, m.playerboard);
            }
            sort(moveorder.begin(), moveorder.end());
            alpha = MIN_INF;
            beta = MAX_INF;
            for (auto& m: moveorder) {
                var = -nega_alpha_moveorder(search_depth-1, -beta, -alpha, m.opponentboard, m.playerboard);
                think_percent += think_count;
                update_think_percent();
                if(var > alpha) {
                    alpha = var;
                    if(search_depth == DEPTH) {
                        tmpbit = m.put;
                    }
                }
            }
            printf("depth: %d Visited nodes %d\n", search_depth, visited_nodes);
            transpose_table_low.swap(former_transpose_table_low);
            transpose_table_up.swap(former_transpose_table_up);
            transpose_table_low.clear();
            transpose_table_up.clear();
        }
    } else {
        for (search_depth = max(1, DEPTH-3); search_depth <= DEPTH; ++search_depth) {
            afterIndex = nowIndex+search_depth;
            for (auto& m: moveorder) {
                m.score = move_ordering_value(m.opponentboard, m.playerboard);
            }
            sort(moveorder.begin(), moveorder.end());
            if(search_depth == DEPTH-2 && moveorder.size() >= 3) moveorder.pop_back();
            alpha = MIN_INF;
            beta = MAX_INF;
            for (auto& m: moveorder) {
                var = -nega_alpha_moveorder(search_depth-1, -beta, -alpha, m.opponentboard, m.playerboard);
                think_percent += think_count;
                update_think_percent();
                if(var > alpha) {
                    alpha = var;
                    if(search_depth == DEPTH) {
                        tmpbit = m.put;
                    }
                }
            }
            printf("depth: %d Visited nodes %d\n", search_depth, visited_nodes);
            transpose_table_low.swap(former_transpose_table_low);
            transpose_table_up.swap(former_transpose_table_up);
            transpose_table_low.clear();
            transpose_table_up.clear();
        }
    }
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
    return alpha;
}

int search_nega_scout(uint64_t &playerboard, uint64_t &opponentboard) {
    printf("algorithm: NegaScout\n");
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int var = 0;
    uint64_t rev;
    board_root m;
    vector<board_root> moveorder;
    moveorder.reserve(__builtin_popcountll(legalboard));
    m.put = 1;
    for (auto i = 0; i < 64; ++i) {
        if(legalboard & m.put) {
            rev = Flip(m.put, playerboard, opponentboard);
            m.playerboard = playerboard ^ (m.put | rev);
            m.opponentboard = opponentboard ^ rev;
            moveorder.emplace_back(m);
        }
        m.put <<= 1;
    }
    int alpha = MIN_INF, beta = MAX_INF;
    think_count = 100/(__builtin_popcountll(legalboard)*(DEPTH-max(1, DEPTH-4)+1));
    int wave = 0;
    for (search_depth = max(1, DEPTH-4); search_depth <= DEPTH; ++search_depth) {
        think_percent = wave*(100/(DEPTH-max(1, DEPTH-4)+1));
        update_think_percent();
        ++wave;
        afterIndex = nowIndex+search_depth;
        for (auto& m: moveorder) {
            m.score = move_ordering_value(m.opponentboard, m.playerboard);
        }
        sort(moveorder.begin(), moveorder.end());
        alpha = MIN_INF;
        beta = MAX_INF;
        alpha = -nega_scout(search_depth-1, -beta, -alpha, moveorder[0].opponentboard, moveorder[0].playerboard);
        if(search_depth == DEPTH) {
            tmpbit = moveorder[0].put;
        }
        think_percent += think_count;
        update_think_percent();
        for (auto i = 1; i < moveorder.size(); ++i) {
            var = -nega_alpha_moveorder(search_depth-1, -alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
            think_percent += think_count;
            update_think_percent();
            if(var > alpha) {
                alpha = var;
                var = -nega_scout(search_depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
                if(search_depth == DEPTH) {
                    tmpbit = moveorder[i].put;
                }
            }
            alpha = max(var, alpha);
        }
        printf("depth: %d Visited nodes %d\n", search_depth, visited_nodes);
        transpose_table_up.swap(former_transpose_table_up);
        transpose_table_up.clear();
        transpose_table_low.swap(former_transpose_table_low);
        transpose_table_low.clear();
    }
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
    return alpha;
}

int nega_scout(int_fast8_t depth, int alpha, int beta, uint64_t &playerboard, uint64_t &opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, afterIndex);
    }
    auto board_state = make_pair(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.count(board_state)) {
        u = transpose_table_up[board_state];
        if(u <= alpha) return u;
        beta = min(u, beta);
    }
    if(transpose_table_low.count(board_state)) {
        l = transpose_table_low[board_state];
        if(l >= beta) return l;
        alpha = max(l, alpha);
    }
    if(u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) [[unlikely]] {
        if(!(makelegalboard(opponentboard, playerboard))) [[unlikely]] return countscore(playerboard, opponentboard, afterIndex);
        else return -nega_scout(depth, -beta, -alpha, opponentboard, playerboard);
    }
    int var, max_score = MIN_INF, count = 0;
    uint64_t rev;
    board moveorder[__builtin_popcountll(legalboard)];
    uint64_t put;
    while(legalboard) {
        put = legalboard & -legalboard;
        legalboard ^= put;
        rev = Flip(put, playerboard, opponentboard);
        moveorder[count].playerboard = playerboard ^ (put | rev);
        moveorder[count].opponentboard = opponentboard ^ rev;
        moveorder[count].score = move_ordering_value(moveorder[count].opponentboard, moveorder[count].playerboard);
        ++count;
    }
    
    sort(moveorder, moveorder+count);
    
    if(depth > 3) {
        var = -nega_scout(depth-1, -beta, -alpha, moveorder[0].opponentboard, moveorder[0].playerboard);
        if (var >= beta) {
            if (var > l) {
                transpose_table_low[board_state] = var;
            }
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
        for (auto i = 1; i < count; ++i) {
            var = -nega_alpha_moveorder(depth-1, -alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
            if (var >= beta) {
                if(var > l) {
                    transpose_table_low[board_state] = var;
                }
                return var;
            }
            if(var > alpha) {
                alpha = var;
                var = -nega_scout(depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
                if (var >= beta) {
                    if (var > l) {
                        transpose_table_low[board_state] = var;
                    }
                    return var;
                }
            }
            alpha = max(alpha, var);
            max_score = max(var, max_score);
        }
    } else {
        for (auto& m: moveorder) {
            var = -nega_alpha(depth-1, -beta, -alpha, m.opponentboard, m.playerboard);
            if (var >= beta) {
                if (var > l) {
                    transpose_table_low[board_state] = var;
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    if(max_score > alpha) {
        transpose_table_up[board_state] = max_score;
        transpose_table_low[board_state] = max_score;
    } else {
        transpose_table_up[board_state] = max_score;
    }
    return max_score;
}


int nega_alpha_moveorder(int_fast8_t depth, int alpha, int beta, uint64_t &playerboard, uint64_t &opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, afterIndex);
    }
    int u = MAX_INF, l = MIN_INF;
    auto board_state = make_pair(playerboard, opponentboard);
    if(transpose_table_up.count(board_state)) {
        u = transpose_table_up[board_state];
        if(u <= alpha) return u;
        beta = min(u, beta);
    }
    if(transpose_table_low.count(board_state)) {
        l = transpose_table_low[board_state];
        if(l >= beta) return l;
        alpha = max(l, alpha);
    }
    if(u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) [[unlikely]] {
        if(!(makelegalboard(opponentboard, playerboard))) [[unlikely]] return countscore(playerboard, opponentboard, afterIndex);
        else return -nega_alpha_moveorder(depth, -beta, -alpha, opponentboard, playerboard);
    }
    int var = 0, count = 0, max_score = MIN_INF;
    uint64_t rev;
    board moveorder[__builtin_popcountll(legalboard)];
    uint64_t put;
    while(legalboard) {
        put = legalboard & -legalboard;
        legalboard ^= put;
        rev = Flip(put, playerboard, opponentboard);
        moveorder[count].playerboard = playerboard ^ (put | rev);
        moveorder[count].opponentboard = opponentboard ^ rev;
        moveorder[count].score = move_ordering_value(moveorder[count].opponentboard, moveorder[count].playerboard);
        ++count;
    }
    sort(moveorder, moveorder+count);
    
    for (auto& m: moveorder) {
        var = (depth <= 3) ? -nega_alpha(depth-1, -beta, -alpha, m.opponentboard, m.playerboard) :
        -nega_alpha_moveorder(depth-1, -beta, -alpha, m.opponentboard, m.playerboard);
        if (var >= beta) {
            if (var > l) {
                transpose_table_low[board_state] = var;
            }
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
    }
    if(max_score > alpha) {
        transpose_table_up[board_state] = max_score;
        transpose_table_low[board_state] = max_score;
    } else {
        transpose_table_up[board_state] = max_score;
    }
    return max_score;
}

int nega_alpha(int_fast8_t depth, int alpha, int beta, uint64_t &playerboard, uint64_t &opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard, afterIndex);
    }
    auto board_state = make_pair(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.count(board_state)) {
        u = transpose_table_up[board_state];
        if(u <= alpha) return u;
        beta = min(u, beta);
    }
    if(transpose_table_low.count(board_state)) {
        l = transpose_table_low[board_state];
        if(l >= beta) return l;
        alpha = max(l, alpha);
    }
    if(u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) [[unlikely]] {
        if(!(makelegalboard(opponentboard, playerboard))) [[unlikely]] return countscore(playerboard, opponentboard, afterIndex);
        else return -nega_alpha(depth, -beta, -alpha, opponentboard, playerboard);
    }
    uint64_t rev = 0;
    int var, max_score = MIN_INF;
    for (const auto& i: moveorder_bit) {
        if(canput(i, legalboard)) {
            rev = Flip(i, playerboard, opponentboard);
            playerboard ^= (i | rev);
            opponentboard ^= rev;
            var = -nega_alpha(depth-1, -beta, -alpha, opponentboard, playerboard);
            playerboard ^= (i | rev);
            opponentboard ^= rev;
            if (var >= beta) {
                if (var > l) {
                    transpose_table_low[board_state] = var;
                }
                return var;
            }
            alpha = max(var, alpha);
            max_score = max(max_score, var);
        }
    }
    if(max_score > alpha) {
        transpose_table_up[board_state] = max_score;
        transpose_table_low[board_state] = max_score;
    } else {
        transpose_table_up[board_state] = max_score;
    }
    return max_score;
    
}

int search_finish(uint64_t &playerboard, uint64_t &opponentboard) {
    cout << "algorithm: NegaAlpha" << endl;
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int var = 0;
    uint64_t rev;
    board_finish_root moveorder[__builtin_popcountll(legalboard)];
    var = 0;
    for (uint64_t put = 0x8000000000000000; put >= 1; put >>= 1) {
        if(legalboard & put) {
            rev = Flip(put, playerboard, opponentboard);
            moveorder[var].playerboard = playerboard ^ (put | rev);
            moveorder[var].opponentboard = opponentboard ^ rev;
            moveorder[var].legalboard = makelegalboard(moveorder[var].opponentboard, moveorder[var].playerboard);
            moveorder[var].score = __builtin_popcountll(moveorder[var].legalboard);
            moveorder[var].put = put;
            ++var;
        }
    }
    think_count = 100/var;
    sort(moveorder, moveorder+var);
    int alpha = MIN_INF, beta = MAX_INF;
    for (board_finish_root& m: moveorder) {
        var = -nega_alpha_moveorder_finish(-beta, -alpha, m.opponentboard, m.playerboard);
//        var = -nega_scout_finish(-beta, -alpha, m.opponentboard, m.playerboard, m.legalboard);
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

int search_finish_scout(uint64_t &playerboard, uint64_t &opponentboard) {
    cout << "algorithm: NegaScout" << endl;
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int var = 0, score = 0;
    uint64_t rev;
    board_finish_root moveorder[__builtin_popcountll(legalboard)];
    for (uint64_t put = 0x8000000000000000; put >= 1; put >>= 1) {
        if(legalboard & put) {
            rev = Flip(put, playerboard, opponentboard);
            moveorder[var].playerboard = playerboard ^ (put | rev);
            moveorder[var].opponentboard = opponentboard ^ rev;
            moveorder[var].legalboard = makelegalboard(moveorder[var].opponentboard, moveorder[var].playerboard);
            moveorder[var].score = __builtin_popcountll(moveorder[var].legalboard);
            moveorder[var].put = put;
            ++var;
        }
    }
    think_count = 100/var;
    sort(moveorder, moveorder+var);
    int alpha = MIN_INF, beta = MAX_INF;
    alpha = -nega_scout_finish(-beta, -alpha, moveorder[0].opponentboard, moveorder[0].playerboard);
    tmpbit = moveorder[0].put;
    think_percent += think_count;
    update_think_percent();
    for (int i = 1; i < var; ++i) {
        score = -nega_alpha_moveorder_finish(-alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
        think_percent += think_count;
        update_think_percent();
        if(score > alpha) {
            alpha = score;
            score = -nega_scout_finish(-beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
            tmpbit = moveorder[i].put;
        }
        alpha = max(score, alpha);
    }
    transpose_table_up.swap(former_transpose_table_up);
    transpose_table_up.clear();
    transpose_table_low.swap(former_transpose_table_low);
    transpose_table_low.clear();
    cout << "final_search" << endl;
    return alpha;
}

int nega_scout_finish(int alpha, int beta, uint64_t &playerboard, uint64_t &opponentboard) {
    ++visited_nodes;
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    auto board_state = make_pair(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    if(transpose_table_up.count(board_state)) {
        u = transpose_table_up[board_state];
        if(u <= alpha) return u;
        beta = min(u, beta);
    }
    if(transpose_table_low.count(board_state)) {
        l = transpose_table_low[board_state];
        if(l >= beta) return l;
        alpha = max(l, alpha);
    }
    if(u == l) return u;
    
    if(legalboard == 0) [[unlikely]] {
        if(!makelegalboard(opponentboard, playerboard)) [[unlikely]] return (__builtin_popcountll(playerboard) - __builtin_popcountll(opponentboard));
        else return -nega_scout_finish(-beta, -alpha, opponentboard, playerboard);
    }
    int var, max_score = MIN_INF, count = 0;
    uint64_t rev;
    board moveorder[__builtin_popcountll(legalboard)];
    uint64_t put;
    while(legalboard) {
        put = legalboard & -legalboard;
        legalboard ^= put;
        rev = Flip(put, playerboard, opponentboard);
        moveorder[count].playerboard = playerboard ^ (put | rev);
        moveorder[count].opponentboard = opponentboard ^ rev;
        moveorder[count].score = __builtin_popcountll(makelegalboard(moveorder[count].opponentboard, moveorder[count].playerboard));
        ++count;
    }
    
    sort(moveorder, moveorder+count);
    
    var = -nega_scout_finish(-beta, -alpha, moveorder[0].opponentboard, moveorder[0].playerboard);
    if (var >= beta) {
        if (var > l) {
            transpose_table_low[board_state] = var;
        }
        return var;
    }
    alpha = max(alpha, var);
    max_score = max(max_score, var);
    if(__builtin_popcountll(playerboard | opponentboard) <= 56) {
        for (auto i = 1; i < count; ++i) {
            var = -nega_alpha_moveorder_finish(-alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
            if (var >= beta) {
                if(var > l) {
                    transpose_table_low[board_state] = var;
                }
                return var;
            }
            if(var > alpha) {
                alpha = var;
                var = -nega_scout_finish(-beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
                if (var >= beta) {
                    if (var > l) {
                        transpose_table_low[board_state] = var;
                    }
                    return var;
                }
            }
            alpha = max(alpha, var);
            max_score = max(var, max_score);
        }
    } else {
        for (auto &m : moveorder) {
            var = -nega_alpha_finish(-beta, -alpha, m.opponentboard, m.playerboard);
            if (var >= beta) {
                if(var > l) {
                    transpose_table_low[board_state] = var;
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    if(max_score > alpha) {
        transpose_table_up[board_state] = max_score;
        transpose_table_low[board_state] = max_score;
    } else {
        transpose_table_up[board_state] = max_score;
    }
    return max_score;
}

int nega_alpha_moveorder_finish(int alpha, int beta, uint64_t &playerboard, uint64_t &opponentboard) {
    ++visited_nodes;
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    auto board_state = make_pair(playerboard, opponentboard);
    if(transpose_table_up.count(board_state)) {
        u = transpose_table_up[board_state];
        if(u <= alpha) return u;
        beta = min(u, beta);
    }
    if(transpose_table_low.count(board_state)) {
        l = transpose_table_low[board_state];
        if(l >= beta) return l;
        alpha = max(l, alpha);
    }
    if(u == l) return u;
    
    if(legalboard == 0) [[unlikely]] {
        if(!makelegalboard(opponentboard, playerboard)) [[unlikely]] return (__builtin_popcountll(playerboard) - __builtin_popcountll(opponentboard));
        else return -nega_alpha_moveorder_finish(-beta, -alpha, opponentboard, playerboard);
    }
    int var = 0, count = 0, max_score = MIN_INF;
    uint64_t rev;
    board_finish moveorder[__builtin_popcountll(legalboard)];
    uint64_t put;
    while(legalboard) {
        put = legalboard & -legalboard;
        legalboard ^= put;
        rev = Flip(put, playerboard, opponentboard);
        moveorder[count].playerboard = playerboard ^ (put | rev);
        moveorder[count].opponentboard = opponentboard ^ rev;
        moveorder[count].legalboard = makelegalboard(moveorder[count].opponentboard, moveorder[count].playerboard);
        moveorder[count].score = __builtin_popcountll(moveorder[count].legalboard);
        ++count;
    }
    sort(moveorder, moveorder+count);
    
    if(__builtin_popcountll(playerboard | opponentboard) <= 56) {
        for (auto& m: moveorder) {
            var = -nega_alpha_moveorder_finish(-beta, -alpha, m.opponentboard, m.playerboard);
            if (var >= beta) {
                if (var > l) {
                    transpose_table_low[board_state] = var;
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    } else {
        for (auto &m : moveorder) {
            var = -nega_alpha_finish(-beta, -alpha, m.opponentboard, m.playerboard);
            if (var >= beta) {
                if(var > l) {
                    transpose_table_low[board_state] = var;
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    if(max_score > alpha) {
        transpose_table_up[board_state] = max_score;
        transpose_table_low[board_state] = max_score;
    } else {
        transpose_table_up[board_state] = max_score;
    }
    return max_score;
}

int nega_alpha_finish(int alpha, int beta, uint64_t &playerboard, uint64_t &opponentboard) {
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) {
        if(!(makelegalboard(opponentboard, playerboard))) return (__builtin_popcountll(playerboard) - __builtin_popcountll(opponentboard));
        else {
            return -nega_alpha_finish(-beta, -alpha, opponentboard, playerboard);
        }
    }
    uint64_t rev = 0;
    int var = 0, max_score = MIN_INF;
    for (const auto& i: moveorder_bit) {
        if(canput(i, legalboard)) {
            rev = Flip(i, playerboard, opponentboard);
            playerboard ^= (i | rev);
            opponentboard ^= rev;
            var = -nega_alpha_finish(-beta, -alpha, opponentboard, playerboard);
            playerboard ^= (i | rev);
            opponentboard ^= rev;
            if (var >= beta) {
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    return max_score;
}

int winner() {
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

inline int score_stone(const uint64_t &playerboard, const uint64_t &opponentboard) {
    int score = 0;
	
//    score += (__builtin_popcountll(playerboard & 0x8100000000000081ULL)-__builtin_popcountll(opponentboard & 0x8100000000000081ULL));
//    score -= (__builtin_popcountll(playerboard & 0x183C3C180000ULL)-__builtin_popcountll(opponentboard & 0x183C3C180000ULL));
    score -= (__builtin_popcountll(playerboard & 0x182424180000ULL)-__builtin_popcountll(opponentboard & 0x182424180000ULL));
    score -= 3 * (__builtin_popcountll(playerboard & 0x003C424242423C00ULL)-__builtin_popcountll(opponentboard & 0x003C424242423C00ULL));
//    score -= 3 * (__builtin_popcountll(playerboard & 0x24420000422400ULL)-__builtin_popcountll(opponentboard & 0x24420000422400ULL));
//    score -= 2 * (__builtin_popcountll(playerboard & 0x18004242001800ULL)-__builtin_popcountll(opponentboard & 0x18004242001800ULL));
    score -= 6 * (__builtin_popcountll(playerboard & 0x4281000000008142ULL)-__builtin_popcountll(opponentboard & 0x4281000000008142ULL));
    score -= 7 * (__builtin_popcountll(playerboard & 0x0042000000004200ULL)-__builtin_popcountll(opponentboard & 0x0042000000004200ULL));
    
//    for (int i = 0; i < 10; ++i) score += cell_weight_score[i] * (__builtin_popcountll(playerboard & cell_weight_mask[i])-__builtin_popcountll(opponentboard & cell_weight_mask[i]));
    
	//左
    {
        switch (playerboard & LEFT_BOARD) {
            case 0x0000808080800000ULL:
                score += 2;
                break;
            case 0x0080808080808000ULL:
                score += 13;
                break;
            case 0x0000808080808000ULL:
                score += 4;
                break;
            case 0x0080808080800000ULL:
                score += 4;
                break;
            default:
                break;
        }
        switch (opponentboard & LEFT_BOARD) {
            case 0x0000808080800000ULL:
                score -= 2;
                break;
            case 0x0080808080808000ULL:
                score -= 13;
                break;
            case 0x0000808080808000ULL:
                score -= 4;
                break;
            case 0x0080808080800000ULL:
                score -= 4;
                break;
            default:
                break;
        }
    }
	//右
    {
        switch (playerboard & RIGHT_BOARD) {
            case 0x0000010101010000ULL:
                score += 2;
                break;
            case 0x0001010101010100ULL:
                score += 13;
                break;
            case 0x0000010101010100ULL:
                score += 4;
                break;
            case 0x0001010101010000ULL:
                score += 4;
                break;
            default:
                break;
        }
        switch (opponentboard & RIGHT_BOARD) {
            case 0x0000010101010000ULL:
                score -= 2;
                break;
            case 0x0001010101010100ULL:
                score -= 13;
                break;
            case 0x0000010101010100ULL:
                score -= 4;
                break;
            case 0x0001010101010000ULL:
                score -= 4;
                break;
            default:
                break;
        }
    }
	//上
    {
        switch (playerboard & UP_BOARD) {
            case 0x3c00000000000000ULL:
                score += 2;
                break;
            case 0x7e00000000000000ULL:
                score += 13;
                break;
            case 0x3E00000000000000ULL:
                score += 4;
                break;
            case 0x7C00000000000000ULL:
                score += 4;
                break;
            default:
                break;
        }
        switch (opponentboard & UP_BOARD) {
            case 0x3c00000000000000ULL:
                score -= 2;
                break;
            case 0x7e00000000000000ULL:
                score -= 13;
                break;
            case 0x3E00000000000000ULL:
                score -= 4;
                break;
            case 0x7C00000000000000ULL:
                score -= 4;
                break;
            default:
                break;
        }
    }
	//下
    {
        switch (playerboard & DOWN_BOARD) {
            case 0x000000000000003cULL:
                score += 2;
                break;
            case 0x000000000000007eULL:
                score += 13;
                break;
            case 0x000000000000003eULL:
                score += 4;
                break;
            case 0x000000000000007cULL:
                score += 4;
                break;
            default:
                break;
        }
        switch (opponentboard & DOWN_BOARD) {
            case 0x000000000000003cULL:
                score -= 2;
                break;
            case 0x000000000000007eULL:
                score -= 13;
                break;
            case 0x000000000000003eULL:
                score += 4;
                break;
            case 0x000000000000007cULL:
                score += 4;
                break;
            default:
                break;
        }
    }
    
    //左上
    if((playerboard & 0x4000000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score += 6;
    if((playerboard & 0x0080000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score += 6;
    if((playerboard & 0x0040000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score += 7;
    if((opponentboard & 0x4000000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score -= 6;
    if((opponentboard & 0x0080000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score -= 6;
    if((opponentboard & 0x0040000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score -= 7;
    
    //右上
    if((playerboard & 0x0200000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score += 6;
    if((playerboard & 0x0001000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score += 6;
    if((playerboard & 0x0002000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score += 7;
    if((opponentboard & 0x0200000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score -= 6;
    if((opponentboard & 0x0001000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score -= 6;
    if((opponentboard & 0x0002000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score -= 7;
    
    //左下
    if((playerboard & 0x0000000000000040ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score += 6;
    if((playerboard & 0x0000000000008000ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score += 6;
    if((playerboard & 0x0000000000004000ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score += 7;
    if((opponentboard & 0x0000000000000040ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score -= 6;
    if((opponentboard & 0x0000000000008000ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score -= 6;
    if((opponentboard & 0x0000000000004000ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score -= 7;
    
    //右下
    if((playerboard & 0x0000000000000002ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score += 6;
    if((playerboard & 0x0000000000000100ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score += 6;
    if((playerboard & 0x0000000000000200ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score += 7;
    if((opponentboard & 0x0000000000000002ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score -= 6;
    if((opponentboard & 0x0000000000000100ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score -= 6;
    if((opponentboard & 0x0000000000000200ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score -= 7;
    
    
    //上
    if((playerboard & UP_BOARD) == 0xBE00000000000000ULL && (opponentboard & UP_BOARD) == 0x4000000000000000ULL) score-=10;
    if((playerboard & UP_BOARD) == 0x7D00000000000000ULL && (opponentboard & UP_BOARD) == 0x0200000000000000ULL) score-=10;
    //右
    if((playerboard & RIGHT_BOARD) == 0x100010101010100ULL && (opponentboard & RIGHT_BOARD) == 0x001000000000000ULL) score-=10;
    if((playerboard & RIGHT_BOARD) == 0x001010101010001ULL && (opponentboard & RIGHT_BOARD) == 0x000000000000100ULL) score-=10;
    //左
    if((playerboard & LEFT_BOARD) == 0x8000808080808000ULL && (opponentboard & LEFT_BOARD) == 0x080000000000000ULL) score-=10;
    if((playerboard & LEFT_BOARD) == 0x0080808080800080ULL && (opponentboard & LEFT_BOARD) == 0x000000000008000ULL) score-=10;
    //下
    if((playerboard & DOWN_BOARD) == 0x00000000000000BEULL && (opponentboard & DOWN_BOARD) == 0x0000000000000040ULL) score-=10;
    if((playerboard & DOWN_BOARD) == 0x000000000000007DULL && (opponentboard & DOWN_BOARD) == 0x0000000000000002ULL) score-=10;
    
    //上
    if((opponentboard & UP_BOARD) == 0xBE00000000000000ULL && (playerboard & UP_BOARD) == 0x4000000000000000ULL) score+=10;
    if((opponentboard & UP_BOARD) == 0x7D00000000000000ULL && (playerboard & UP_BOARD) == 0x0200000000000000ULL) score+=10;
    //右
    if((opponentboard & RIGHT_BOARD) == 0x100010101010100ULL && (playerboard & RIGHT_BOARD) == 0x001000000000000ULL) score+=10;
    if((opponentboard & RIGHT_BOARD) == 0x001010101010001ULL && (playerboard & RIGHT_BOARD) == 0x000000000000100ULL) score+=10;
    //左
    if((opponentboard & LEFT_BOARD) == 0x8000808080808000ULL && (playerboard & LEFT_BOARD) == 0x080000000000000ULL) score+=10;
    if((opponentboard & LEFT_BOARD) == 0x0080808080800080ULL && (playerboard & LEFT_BOARD) == 0x000000000008000ULL) score+=10;
    //下
    if((opponentboard & DOWN_BOARD) == 0x00000000000000BEULL && (playerboard & DOWN_BOARD) == 0x0000000000000040ULL) score+=10;
    if((opponentboard & DOWN_BOARD) == 0x000000000000007DULL && (playerboard & DOWN_BOARD) == 0x0000000000000002ULL) score+=10;
    
    
    //真ん中斜め
    if((playerboard & 0x201008040000ULL) == 0x201008040000ULL) score += 2;
    if((opponentboard & 0x201008040000ULL) == 0x201008040000ULL) score -= 2;
    
    if((playerboard & 0x40810200000ULL) == 0x40810200000ULL) score += 2;
    if((opponentboard & 0x40810200000ULL) == 0x40810200000ULL) score -= 2;
    
	return score;
}

inline int score_stone2(const uint64_t &playerboard, const uint64_t &opponentboard) {
    int score = 0;
    
//    score += (__builtin_popcountll(playerboard & 0x8100000000000081ULL)-__builtin_popcountll(opponentboard & 0x8100000000000081ULL));
//    score -= (__builtin_popcountll(playerboard & 0x183C3C180000ULL)-__builtin_popcountll(opponentboard & 0x183C3C180000ULL));
//    score -= (__builtin_popcountll(playerboard & 0x182424180000ULL)-__builtin_popcountll(opponentboard & 0x182424180000ULL));
    score -= 2 * (__builtin_popcountll(playerboard & 0x003C424242423C00ULL)-__builtin_popcountll(opponentboard & 0x003C424242423C00ULL));
//    score -= 3 * (__builtin_popcountll(playerboard & 0x24420000422400ULL)-__builtin_popcountll(opponentboard & 0x24420000422400ULL));
//    score -= 2 * (__builtin_popcountll(playerboard & 0x18004242001800ULL)-__builtin_popcountll(opponentboard & 0x18004242001800ULL));
    score -= 5 * (__builtin_popcountll(playerboard & 0x4281000000008142ULL)-__builtin_popcountll(opponentboard & 0x4281000000008142ULL));
    score -= 6 * (__builtin_popcountll(playerboard & 0x0042000000004200ULL)-__builtin_popcountll(opponentboard & 0x0042000000004200ULL));
    
//    for (int i = 0; i < 10; ++i) score += cell_weight_score[i] * (__builtin_popcountll(playerboard & cell_weight_mask[i])-__builtin_popcountll(opponentboard & cell_weight_mask[i]));
    
    //左
    {
        switch (playerboard & LEFT_BOARD) {
            case 0x0000808080800000ULL:
                score += 2;
                break;
            case 0x0080808080808000ULL:
                score += 13;
                break;
            case 0x0000808080808000ULL:
                score += 4;
                break;
            case 0x0080808080800000ULL:
                score += 4;
                break;
            default:
                break;
        }
        switch (opponentboard & LEFT_BOARD) {
            case 0x0000808080800000ULL:
                score -= 2;
                break;
            case 0x0080808080808000ULL:
                score -= 13;
                break;
            case 0x0000808080808000ULL:
                score -= 4;
                break;
            case 0x0080808080800000ULL:
                score -= 4;
                break;
            default:
                break;
        }
    }
    //右
    {
        switch (playerboard & RIGHT_BOARD) {
            case 0x0000010101010000ULL:
                score += 2;
                break;
            case 0x0001010101010100ULL:
                score += 13;
                break;
            case 0x0000010101010100ULL:
                score += 4;
                break;
            case 0x0001010101010000ULL:
                score += 4;
                break;
            default:
                break;
        }
        switch (opponentboard & RIGHT_BOARD) {
            case 0x0000010101010000ULL:
                score -= 2;
                break;
            case 0x0001010101010100ULL:
                score -= 13;
                break;
            case 0x0000010101010100ULL:
                score -= 4;
                break;
            case 0x0001010101010000ULL:
                score -= 4;
                break;
            default:
                break;
        }
    }
    //上
    {
        switch (playerboard & UP_BOARD) {
            case 0x3c00000000000000ULL:
                score += 2;
                break;
            case 0x7e00000000000000ULL:
                score += 13;
                break;
            case 0x3E00000000000000ULL:
                score += 4;
                break;
            case 0x7C00000000000000ULL:
                score += 4;
                break;
            default:
                break;
        }
        switch (opponentboard & UP_BOARD) {
            case 0x3c00000000000000ULL:
                score -= 2;
                break;
            case 0x7e00000000000000ULL:
                score -= 13;
                break;
            case 0x3E00000000000000ULL:
                score -= 4;
                break;
            case 0x7C00000000000000ULL:
                score -= 4;
                break;
            default:
                break;
        }
    }
    //下
    {
        switch (playerboard & DOWN_BOARD) {
            case 0x000000000000003cULL:
                score += 2;
                break;
            case 0x000000000000007eULL:
                score += 13;
                break;
            case 0x000000000000003eULL:
                score += 4;
                break;
            case 0x000000000000007cULL:
                score += 4;
                break;
            default:
                break;
        }
        switch (opponentboard & DOWN_BOARD) {
            case 0x000000000000003cULL:
                score -= 2;
                break;
            case 0x000000000000007eULL:
                score -= 13;
                break;
            case 0x000000000000003eULL:
                score += 4;
                break;
            case 0x000000000000007cULL:
                score += 4;
                break;
            default:
                break;
        }
    }
    
    //左上
    if((playerboard & 0x4000000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score += 6;
    if((playerboard & 0x0080000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score += 6;
    if((playerboard & 0x0040000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score += 7;
    if((opponentboard & 0x4000000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score -= 6;
    if((opponentboard & 0x0080000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score -= 6;
    if((opponentboard & 0x0040000000000000ULL) && ((playerboard | opponentboard) & 0x8000000000000000ULL)) score -= 7;
    
    //右上
    if((playerboard & 0x0200000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score += 6;
    if((playerboard & 0x0001000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score += 6;
    if((playerboard & 0x0002000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score += 7;
    if((opponentboard & 0x0200000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score -= 6;
    if((opponentboard & 0x0001000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score -= 6;
    if((opponentboard & 0x0002000000000000ULL) && ((playerboard | opponentboard) & 0x0100000000000000ULL)) score -= 7;
    
    //左下
    if((playerboard & 0x0000000000000040ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score += 6;
    if((playerboard & 0x0000000000008000ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score += 6;
    if((playerboard & 0x0000000000004000ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score += 7;
    if((opponentboard & 0x0000000000000040ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score -= 6;
    if((opponentboard & 0x0000000000008000ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score -= 6;
    if((opponentboard & 0x0000000000004000ULL) && ((playerboard | opponentboard) & 0x0000000000000080ULL)) score -= 7;
    
    //右下
    if((playerboard & 0x0000000000000002ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score += 6;
    if((playerboard & 0x0000000000000100ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score += 6;
    if((playerboard & 0x0000000000000200ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score += 7;
    if((opponentboard & 0x0000000000000002ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score -= 6;
    if((opponentboard & 0x0000000000000100ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score -= 6;
    if((opponentboard & 0x0000000000000200ULL) && ((playerboard | opponentboard) & 0x0000000000000001ULL)) score -= 7;
    
    
    //上
    if((playerboard & UP_BOARD) == 0xBE00000000000000ULL && (opponentboard & UP_BOARD) == 0x4000000000000000ULL) score-=10;
    if((playerboard & UP_BOARD) == 0x7D00000000000000ULL && (opponentboard & UP_BOARD) == 0x0200000000000000ULL) score-=10;
    //右
    if((playerboard & RIGHT_BOARD) == 0x100010101010100ULL && (opponentboard & RIGHT_BOARD) == 0x001000000000000ULL) score-=10;
    if((playerboard & RIGHT_BOARD) == 0x001010101010001ULL && (opponentboard & RIGHT_BOARD) == 0x000000000000100ULL) score-=10;
    //左
    if((playerboard & LEFT_BOARD) == 0x8000808080808000ULL && (opponentboard & LEFT_BOARD) == 0x080000000000000ULL) score-=10;
    if((playerboard & LEFT_BOARD) == 0x0080808080800080ULL && (opponentboard & LEFT_BOARD) == 0x000000000008000ULL) score-=10;
    //下
    if((playerboard & DOWN_BOARD) == 0x00000000000000BEULL && (opponentboard & DOWN_BOARD) == 0x0000000000000040ULL) score-=10;
    if((playerboard & DOWN_BOARD) == 0x000000000000007DULL && (opponentboard & DOWN_BOARD) == 0x0000000000000002ULL) score-=10;
    
    //上
    if((opponentboard & UP_BOARD) == 0xBE00000000000000ULL && (playerboard & UP_BOARD) == 0x4000000000000000ULL) score+=10;
    if((opponentboard & UP_BOARD) == 0x7D00000000000000ULL && (playerboard & UP_BOARD) == 0x0200000000000000ULL) score+=10;
    //右
    if((opponentboard & RIGHT_BOARD) == 0x100010101010100ULL && (playerboard & RIGHT_BOARD) == 0x001000000000000ULL) score+=10;
    if((opponentboard & RIGHT_BOARD) == 0x001010101010001ULL && (playerboard & RIGHT_BOARD) == 0x000000000000100ULL) score+=10;
    //左
    if((opponentboard & LEFT_BOARD) == 0x8000808080808000ULL && (playerboard & LEFT_BOARD) == 0x080000000000000ULL) score+=10;
    if((opponentboard & LEFT_BOARD) == 0x0080808080800080ULL && (playerboard & LEFT_BOARD) == 0x000000000008000ULL) score+=10;
    //下
    if((opponentboard & DOWN_BOARD) == 0x00000000000000BEULL && (playerboard & DOWN_BOARD) == 0x0000000000000040ULL) score+=10;
    if((opponentboard & DOWN_BOARD) == 0x000000000000007DULL && (playerboard & DOWN_BOARD) == 0x0000000000000002ULL) score+=10;
    
    
    //真ん中斜め
    if((playerboard & 0x201008040000ULL) == 0x201008040000ULL) score += 2;
    if((opponentboard & 0x201008040000ULL) == 0x201008040000ULL) score -= 2;
    
    if((playerboard & 0x40810200000ULL) == 0x40810200000ULL) score += 2;
    if((opponentboard & 0x40810200000ULL) == 0x40810200000ULL) score -= 2;
    
    return score;
}

inline int score_putable(const uint64_t &playerboard, const uint64_t &opponentboard) {
    return __builtin_popcountll(makelegalboard(playerboard, opponentboard))-__builtin_popcountll(makelegalboard(opponentboard, playerboard));
}

inline int score_fixedstone(const uint64_t &playerboard, const uint64_t &opponentboard) {
    int fixedstone = 0;
    uint64_t mask = 0x80;
    if(((playerboard | opponentboard) & DOWN_BOARD) == DOWN_BOARD) {
        fixedstone += __builtin_popcountll(playerboard & DOWN_BOARD);
        fixedstone -= __builtin_popcountll(opponentboard & DOWN_BOARD);
    } else {
        if(mask & playerboard) {
            fixedstone++;
            mask>>=1;
            if(mask & playerboard) {
                fixedstone++;
                mask>>=1;
                if(mask & playerboard) {
                    fixedstone++;
                    mask>>=1;
                    if(mask & playerboard) {
                        fixedstone++;
                        mask>>=1;
                        if(mask & playerboard) {
                            fixedstone++;
                            mask>>=1;
                            if(mask & playerboard) {
                                fixedstone++;
                                mask>>=1;
                                if(mask & playerboard) {
                                    fixedstone++;
                                    mask>>=1;
                                    if(mask & playerboard) {
                                        fixedstone++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        mask = 0x80;
        if(mask & opponentboard) {
            fixedstone--;
            mask>>=1;
            if(mask & opponentboard) {
                fixedstone--;
                mask>>=1;
                if(mask & opponentboard) {
                    fixedstone--;
                    mask>>=1;
                    if(mask & opponentboard) {
                        fixedstone--;
                        mask>>=1;
                        if(mask & opponentboard) {
                            fixedstone--;
                            mask>>=1;
                            if(mask & opponentboard) {
                                fixedstone--;
                                mask>>=1;
                                if(mask & opponentboard) {
                                    fixedstone--;
                                    mask>>=1;
                                    if(mask & opponentboard) {
                                        fixedstone--;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        mask = 0x1;
        if(mask & playerboard) {
            fixedstone++;
            mask<<=1;
            if(mask & playerboard) {
                fixedstone++;
                mask<<=1;
                if(mask & playerboard) {
                    fixedstone++;
                    mask<<=1;
                    if(mask & playerboard) {
                        fixedstone++;
                        mask<<=1;
                        if(mask & playerboard) {
                            fixedstone++;
                            mask<<=1;
                            if(mask & playerboard) {
                                fixedstone++;
                                mask<<=1;
                                if(mask & playerboard) {
                                    fixedstone++;
                                    mask<<=1;
                                    if(mask & playerboard) {
                                        fixedstone++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        mask = 0x1;
        if(mask & opponentboard) {
            fixedstone--;
            mask<<=1;
            if(mask & opponentboard) {
                fixedstone--;
                mask<<=1;
                if(mask & opponentboard) {
                    fixedstone--;
                    mask<<=1;
                    if(mask & opponentboard) {
                        fixedstone--;
                        mask<<=1;
                        if(mask & opponentboard) {
                            fixedstone--;
                            mask<<=1;
                            if(mask & opponentboard) {
                                fixedstone--;
                                mask<<=1;
                                if(mask & opponentboard) {
                                    fixedstone--;
                                    mask<<=1;
                                    if(mask & opponentboard) {
                                        fixedstone--;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }
    return fixedstone;
}

inline int score_fixedstone_table(const uint64_t &playerboard, const uint64_t &opponentboard) {
    return fixedstone_table[make_pair(playerboard & UP_BOARD, opponentboard & UP_BOARD)] + fixedstone_table[make_pair(playerboard & RIGHT_BOARD, opponentboard & RIGHT_BOARD)] + fixedstone_table[make_pair(playerboard & DOWN_BOARD, opponentboard & DOWN_BOARD)] + fixedstone_table[make_pair(playerboard & LEFT_BOARD, opponentboard & LEFT_BOARD)] - __builtin_popcountll(playerboard & 0x8100000000000081ULL) + __builtin_popcountll(opponentboard & 0x8100000000000081ULL);
}

inline int countscore(const uint64_t &playerboard, const uint64_t &opponentboard, const char &afterIndex) {
    if(!playerboard) [[unlikely]] return MIN_INF;
    if(!opponentboard) [[unlikely]] return MAX_INF;
    if(afterIndex >= 64) return (__builtin_popcountll(playerboard)-__builtin_popcountll(opponentboard));
    if(afterIndex >= 45) return (score_stone(playerboard, opponentboard)*3+score_fixedstone_table(playerboard, opponentboard)*15);
    if(afterIndex >= 41) return (score_stone(playerboard, opponentboard)*3+score_fixedstone_table(playerboard, opponentboard)*15 + score_putable(playerboard, opponentboard));
    return (score_stone(playerboard, opponentboard)*3 + score_fixedstone_table(playerboard, opponentboard)*15 + score_putable(playerboard, opponentboard)*2);
}
