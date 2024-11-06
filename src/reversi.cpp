//
//  othello.cpp
//  othello
//
//  Created by sasa on 2023/09/30.
//
#include "variables.h"

using namespace std;

void reset() {
    printf("[*]初期化中...\n");
    nowTurn = BLACK_TURN;
    nowIndex = 1;
    DEPTH = firstDEPTH;
    afterIndex = nowIndex+DEPTH;
    b.playerboard = 0x0000000810000000ULL;
    b.opponentboard = 0x0000001008000000ULL;
//    b.playerboard = 0x00141eae978bc0fe;
//    b.opponentboard = 0x2020e05068743e00;
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
        uint64_t rev = Flip(put, b.playerboard, b.opponentboard);
        b.playerboard ^= (rev | put);
        b.opponentboard ^= rev;
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
    flipped |= (outflank[0] - (uint64_t)(outflank[0] != 0)) & mask[0];
    flipped |= (outflank[1] - (uint64_t)(outflank[1] != 0)) & mask[1];
    flipped |= (outflank[2] - (uint64_t)(outflank[2] != 0)) & mask[2];
    flipped |= (outflank[3] - (uint64_t)(outflank[3] != 0)) & mask[3];

    return flipped;
}

bool isPass() {
    return (makelegalboard(b.playerboard, b.opponentboard) == 0 && makelegalboard(b.opponentboard, b.playerboard) != 0);
}

bool isFinished() {
    return (makelegalboard(b.playerboard, b.opponentboard) == 0 && makelegalboard(b.opponentboard, b.playerboard) == 0);
}

void swapboard() {
    swap(b.playerboard, b.opponentboard);
    nowTurn = 1-nowTurn;
}

inline int move_ordering_value(uint64_t &playerboard, uint64_t &opponentboard) {
    if(afterIndex >= 64) return -__builtin_popcountll(makelegalboard(playerboard, opponentboard));
    auto it = former_transpose_table.find(make_pair(playerboard, opponentboard));
    if(it != former_transpose_table.end()) {
        return (1000-max(it->second.first, it->second.second));
    } else {
        return countscore(opponentboard, playerboard);
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

int ai() {
    if (nowTurn == 1-botplayer) {
        return 0;
    }
    printf("[*]Botが考え中..\n");
    if(Level == 5 && nowIndex >= 29) {
        DEPTH = 12;
    }
    if(Level >= 6 && nowIndex >= 33) {
        DEPTH = 14;
        afterIndex=nowIndex+DEPTH;
    }
    if(Level >= 5 && nowIndex >= 39) {
        DEPTH = 20;
        afterIndex=60;
    }
    memset(box, 0, sizeof(box));
    tmpx = -1;
    tmpy = -1;
    tmpbit = 0;
    think_percent = 0;
//    update_think_percent();
    transpose_table.clear();
    former_transpose_table.clear();
    legalboard = makelegalboard(b.playerboard, b.opponentboard);
//    afterIndex = 60;
    int putable_count = __builtin_popcountll(legalboard);
    visited_nodes = 0;
    int score = 0;
    book(b.playerboard, b.opponentboard);
    if(!tmpbit) {
        if(afterIndex >= 60) {
            think_count = 100/putable_count;
            score = search_finish_scout(b.playerboard, b.opponentboard);
        } else  {
            think_count = 100/(putable_count*(DEPTH+1-max(DEPTH-3, 1)));
            score = search_nega_scout(b.playerboard, b.opponentboard);
        }
    }
    think_percent = 100;
//    update_think_percent();
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
//                update_think_percent();
                if(var > alpha) {
                    alpha = var;
                    if(search_depth == DEPTH) {
                        tmpbit = m.put;
                    }
                }
            }
            printf("depth: %d Visited nodes %d\n", search_depth, visited_nodes);
            transpose_table.swap(former_transpose_table);
            transpose_table.clear();
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
//                update_think_percent();
                if(var > alpha) {
                    alpha = var;
                    if(search_depth == DEPTH) {
                        tmpbit = m.put;
                    }
                }
            }
            printf("depth: %d Visited nodes %d\n", search_depth, visited_nodes);
            transpose_table.swap(former_transpose_table);
            transpose_table.clear();
        }
    }
    transpose_table.clear();
    former_transpose_table.clear();
    return alpha;
}

int search_nega_scout(uint64_t &playerboard, uint64_t &opponentboard) {
    printf("algorithm: NegaScout\n");
    transpose_table.clear();
    former_transpose_table.clear();
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
//        update_think_percent();
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
//        update_think_percent();
        for (size_t i = 1; i < moveorder.size(); ++i) {
            var = -nega_alpha_moveorder(search_depth-1, -alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
            think_percent += think_count;
//            update_think_percent();
            if(var > alpha) {
                alpha = var;
                var = -nega_scout(search_depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
                if(search_depth == DEPTH) {
                    tmpbit = moveorder[i].put;
                }
            }
            int pos = __builtin_clzll(moveorder[i].put);
            box[pos / 8][pos % 8] = var;
            alpha = max(var, alpha);
        }
        printf("depth: %d Visited nodes %d\n", search_depth, visited_nodes);
        transpose_table.swap(former_transpose_table);
        transpose_table.clear();
    }
    transpose_table.clear();
    former_transpose_table.clear();
    return alpha;
}

int nega_scout(int_fast8_t depth, int alpha, int beta, uint64_t &playerboard, uint64_t &opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard);
    }
    auto board_state = make_pair(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    auto it = transpose_table.find(board_state);
    if(it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if(u <= alpha) return u;
        if(l >= beta) return l;
        alpha = max(l, alpha);
        beta = min(u, beta);
    }
    if(u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) [[unlikely]] {
        if(!(makelegalboard(opponentboard, playerboard))) [[unlikely]] return countscore(playerboard, opponentboard);
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
    
    sort(execution::unseq, moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
    
    if(depth > 3) {
        var = -nega_scout(depth-1, -beta, -alpha, moveorder[0].opponentboard, moveorder[0].playerboard);
        if (var >= beta) {
            if (var > l) {
                transpose_table[board_state] = {u, var};
            }
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
        for (int i = 1; i < count; ++i) {
            var = -nega_alpha_moveorder(depth-1, -alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
            if (var >= beta) {
                if(var > l) {
                    transpose_table[board_state] = {u, var};
                }
                return var;
            }
            if(var > alpha) {
                alpha = var;
                var = -nega_scout(depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
                if (var >= beta) {
                    if (var > l) {
                        transpose_table[board_state] = {u, var};
                    }
                    return var;
                }
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    } else {
        for (auto& m: moveorder) {
            var = -nega_alpha(depth-1, -beta, -alpha, m.opponentboard, m.playerboard);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[board_state] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    transpose_table[board_state] = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    return max_score;
}


int nega_alpha_moveorder(int_fast8_t depth, int alpha, int beta, uint64_t &playerboard, uint64_t &opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard);
    }
    int u = MAX_INF, l = MIN_INF;
    auto board_state = make_pair(playerboard, opponentboard);
    auto it = transpose_table.find(board_state);
    if(it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if(u <= alpha) return u;
        if(l >= beta) return l;
        alpha = (l > alpha) ? l : alpha;
        beta = (u < beta) ? u : beta;
    }
    if(u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) [[unlikely]] {
        if(!(makelegalboard(opponentboard, playerboard))) [[unlikely]] return countscore(playerboard, opponentboard);
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
    sort(execution::unseq, moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
    if(depth <= 3) {
        for (auto& m: moveorder) {
            var = -nega_alpha(depth-1, -beta, -alpha, m.opponentboard, m.playerboard);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[board_state] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    } else {
        for (auto& m: moveorder) {
            var = -nega_alpha_moveorder(depth-1, -beta, -alpha, m.opponentboard, m.playerboard);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[board_state] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    transpose_table[board_state] = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    return max_score;
}

int nega_alpha(int_fast8_t depth, int alpha, int beta, uint64_t &playerboard, uint64_t &opponentboard) {
    ++visited_nodes;
    if(!depth) {
        return countscore(playerboard, opponentboard);
    }
    auto board_state = make_pair(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    auto it = transpose_table.find(board_state);
    if(it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if(u <= alpha) return u;
        if(l >= beta) return l;
        alpha = (l > alpha) ? l : alpha;
        beta = (u < beta) ? u : beta;
    }
    if(u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if(!legalboard) [[unlikely]] {
        if(!(makelegalboard(opponentboard, playerboard))) [[unlikely]] return countscore(playerboard, opponentboard);
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
                    transpose_table[board_state] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    transpose_table[board_state] = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    return max_score;
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
    alpha = -nega_scout_finish(-beta, -alpha, moveorder[0].opponentboard, moveorder[0].playerboard, moveorder[0].legalboard);
    tmpbit = moveorder[0].put;
    think_percent += think_count;
//    update_think_percent();
    for (int i = 1; i < var; ++i) {
        score = -nega_alpha_moveorder_finish(-alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard, moveorder[i].legalboard);
        think_percent += think_count;
//        update_think_percent();
        if(score > alpha) {
            alpha = score;
            score = -nega_scout_finish(-beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard, moveorder[i].legalboard);
            tmpbit = moveorder[i].put;
        }
        alpha = max(score, alpha);
    }
    transpose_table.swap(former_transpose_table);
    transpose_table.clear();
    cout << "final_search" << endl;
    printf("Visited nodes %d\n", visited_nodes);
    return alpha;
}

int nega_scout_finish(int alpha, int beta, uint64_t &playerboard, uint64_t &opponentboard, uint64_t legalboard) {
    ++visited_nodes;
//    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    auto board_state = make_pair(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    auto it = transpose_table.find(board_state);
    if(it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if(u <= alpha) return u;
        if(l >= beta) return l;
        alpha = (l > alpha) ? l : alpha;
        beta = (u < beta) ? u : beta;
    }
    if(u == l) return u;
    
    if(!legalboard) [[unlikely]] {
        uint64_t legalboard2 = makelegalboard(opponentboard, playerboard);
        if(!legalboard2) [[unlikely]] return (__builtin_popcountll(playerboard) - __builtin_popcountll(opponentboard));
        else return -nega_scout_finish(-beta, -alpha, opponentboard, playerboard, legalboard2);
    }
    int var, max_score = MIN_INF, count = 0;
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
    
    sort(execution::unseq, moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score < b.score;
    });
    
    if(__builtin_popcountll(playerboard | opponentboard) <= 56) {
        var = -nega_scout_finish(-beta, -alpha, moveorder[0].opponentboard, moveorder[0].playerboard, moveorder[0].legalboard);
        if (var >= beta) {
            if (var > l) {
                transpose_table[board_state] = {u, var};
            }
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
        for (int i = 1; i < count; ++i) {
            var = -nega_alpha_moveorder_finish(-alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard, moveorder[i].legalboard);
            if (var >= beta) {
                if(var > l) {
                    transpose_table[board_state] = {u, var};
                }
                return var;
            }
            if(var > alpha) {
                alpha = var;
                var = -nega_scout_finish(-beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard, moveorder[i].legalboard);
                if (var >= beta) {
                    if (var > l) {
                        transpose_table[board_state] = {u, var};
                    }
                    return var;
                }
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    } else {
        for (auto &m : moveorder) {
            var = -nega_alpha_finish(-beta, -alpha, m.opponentboard, m.playerboard);
            if (var >= beta) {
                if(var > l) {
                    transpose_table[board_state] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    transpose_table[board_state] = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    return max_score;
}

int nega_alpha_moveorder_finish(int alpha, int beta, uint64_t &playerboard, uint64_t &opponentboard, uint64_t legalboard) {
    ++visited_nodes;
//    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    auto board_state = make_pair(playerboard, opponentboard);
    auto it = transpose_table.find(board_state);
    if(it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if(u <= alpha) return u;
        if(l >= beta) return l;
        alpha = (l > alpha) ? l : alpha;
        beta = (u < beta) ? u : beta;
    }
    if(u == l) return u;
    
    if(!legalboard) [[unlikely]] {
        uint64_t legalboard2 = makelegalboard(opponentboard, playerboard);
        if(!legalboard2) [[unlikely]] return (__builtin_popcountll(playerboard) - __builtin_popcountll(opponentboard));
        else return -nega_alpha_moveorder_finish(-beta, -alpha, opponentboard, playerboard, legalboard2);
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
    sort(execution::unseq, moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score < b.score;
    });
    
    if(__builtin_popcountll(playerboard | opponentboard) <= 56) {
        for (auto& m: moveorder) {
            var = -nega_alpha_moveorder_finish(-beta, -alpha, m.opponentboard, m.playerboard, m.legalboard);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[board_state] = {u, var};
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
                    transpose_table[board_state] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    transpose_table[board_state] = make_pair(max_score, ((max_score > alpha) ? max_score : l));
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
            alpha = (var > alpha) ? var : alpha;
            max_score = (var > max_score) ? var : max_score;
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
    constexpr uint64_t corner_masks[] = {
        0x4000000000000000ULL, 0x0080000000000000ULL, 0x0040000000000000ULL,
        0x0200000000000000ULL, 0x0001000000000000ULL, 0x0002000000000000ULL,
        0x0000000000000040ULL, 0x0000000000008000ULL, 0x0000000000004000ULL,
        0x0000000000000002ULL, 0x0000000000000100ULL, 0x0000000000000200ULL
    };
    constexpr int corner_scores[] = {6, 6, 7, 6, 6, 7, 6, 6, 7, 6, 6, 7};
    constexpr uint64_t opponent_checks[] = {
        0x8000000000000000ULL, 0x8000000000000000ULL, 0x8000000000000000ULL,
        0x0100000000000000ULL, 0x0100000000000000ULL, 0x0100000000000000ULL,
        0x0000000000000080ULL, 0x0000000000000080ULL, 0x0000000000000080ULL,
        0x0000000000000001ULL, 0x0000000000000001ULL, 0x0000000000000001ULL
    };

    constexpr int mask_scores[] = {1, 13, 4, 4};
    constexpr uint64_t left_cases[] = {0x0000808080800000ULL, 0x0080808080808000ULL, 0x0000808080808000ULL, 0x0080808080800000ULL};
    constexpr uint64_t right_cases[] = {0x0000010101010000ULL, 0x0001010101010100ULL, 0x0000010101010100ULL, 0x0001010101010000ULL};
    constexpr uint64_t up_cases[] = {0x3c00000000000000ULL, 0x7e00000000000000ULL, 0x3E00000000000000ULL, 0x7C00000000000000ULL};
    constexpr uint64_t down_cases[] = {0x000000000000003cULL, 0x000000000000007eULL, 0x000000000000003eULL, 0x000000000000007cULL};

    constexpr uint64_t patterns[] = {
        0x182424180000ULL, 0x003C424242423C00ULL, 0x4281000000008142ULL, 0x0042000000004200ULL
    };
    constexpr int multipliers[] = {1, 3, 6, 7};

    int score = 0;
#pragma clang loop vectorize(enable)
    for (int i = 0; i < 4; ++i) {
        score -= multipliers[i] * (__builtin_popcountll(playerboard & patterns[i])-__builtin_popcountll(opponentboard & patterns[i]));
    }
#pragma clang loop vectorize(enable)
    for (int i = 0; i < 4; ++i) {
        if ((playerboard & LEFT_BOARD) == left_cases[i]) score += mask_scores[i];
        if ((opponentboard & LEFT_BOARD) == left_cases[i]) score -= mask_scores[i];

        if ((playerboard & RIGHT_BOARD) == right_cases[i]) score += mask_scores[i];
        if ((opponentboard & RIGHT_BOARD) == right_cases[i]) score -= mask_scores[i];

        if ((playerboard & UP_BOARD) == up_cases[i]) score += mask_scores[i];
        if ((opponentboard & UP_BOARD) == up_cases[i]) score -= mask_scores[i];

        if ((playerboard & DOWN_BOARD) == down_cases[i]) score += mask_scores[i];
        if ((opponentboard & DOWN_BOARD) == down_cases[i]) score -= mask_scores[i];
    }
#pragma clang loop vectorize(enable)
    for (int i = 0; i < 12; ++i) {
        if ((playerboard & corner_masks[i]) && ((playerboard | opponentboard) & opponent_checks[i])) {
            score += corner_scores[i];
        }
        if ((opponentboard & corner_masks[i]) && ((playerboard | opponentboard) & opponent_checks[i])) {
            score -= corner_scores[i];
        }
    }
    
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
    if(((playerboard | opponentboard) & DOWN_BOARD) == DOWN_BOARD) {
        fixedstone += __builtin_popcountll(playerboard & DOWN_BOARD);
        fixedstone -= __builtin_popcountll(opponentboard & DOWN_BOARD);
    } else {
        uint8_t mask = 0x80;
        while(mask & playerboard) {
            fixedstone++;
            mask>>=1;
        }
        mask = 0x80;
        while(mask & opponentboard) {
            fixedstone--;
            mask>>=1;
        }
        mask = 0x1;
        while(mask & playerboard) {
            fixedstone++;
            mask<<=1;
        }
        mask = 0x1;
        while(mask & opponentboard) {
            fixedstone--;
            mask<<=1;
        }
    }
    return fixedstone;
}

inline int score_fixedstone_table(const uint64_t &playerboard, const uint64_t &opponentboard) {
    return (!(0x8100000000000081ULL & (playerboard | opponentboard))) ? 0 :
    (fixedstone_table[make_pair(playerboard & UP_BOARD, opponentboard & UP_BOARD)] + fixedstone_table[make_pair(playerboard & RIGHT_BOARD, opponentboard & RIGHT_BOARD)] + fixedstone_table[make_pair(playerboard & DOWN_BOARD, opponentboard & DOWN_BOARD)] + fixedstone_table[make_pair(playerboard & LEFT_BOARD, opponentboard & LEFT_BOARD)] - __builtin_popcountll(playerboard & 0x8100000000000081ULL) + __builtin_popcountll(opponentboard & 0x8100000000000081ULL));
}

inline int score_null_place(const uint64_t &playerboard, const uint64_t &opponentboard) {
    uint64_t free_board = ~(playerboard | opponentboard);
    constexpr uint64_t LEFT_MASK = 0x7F7F7F7F7F7F7F7FULL;
    constexpr uint64_t RIGHT_MASK = 0xFEFEFEFEFEFEFEFEULL;
    uint64_t free_mask = 0;
    free_mask |= (free_board & LEFT_MASK) << 1;
    free_mask |= (free_board & LEFT_MASK) << 9;
    free_mask |= (free_board & RIGHT_MASK) << 7;
    free_mask |= free_board << 8;
    free_mask |= (free_board & RIGHT_MASK) >> 1;
    free_mask |= (free_board & RIGHT_MASK) >> 9;
    free_mask |= (free_board & LEFT_MASK) >> 7;
    free_mask |= free_board >> 8;
    
    return __builtin_popcountll(opponentboard & free_mask)-__builtin_popcountll(playerboard & free_mask);
}

inline int countscore(const uint64_t &playerboard, const uint64_t &opponentboard) {
    if(!playerboard) [[unlikely]] return MIN_INF;
    if(!opponentboard) [[unlikely]] return MAX_INF;
    if(afterIndex >= 64) return (__builtin_popcountll(playerboard)-__builtin_popcountll(opponentboard));
    if(afterIndex >= 45) return (score_stone(playerboard, opponentboard)+score_fixedstone_table(playerboard, opponentboard)*4);
    if(afterIndex >= 41) return (score_stone(playerboard, opponentboard)*4+score_fixedstone_table(playerboard, opponentboard)*16 + score_putable(playerboard, opponentboard));
    else return (score_stone(playerboard, opponentboard)*6 + score_fixedstone_table(playerboard, opponentboard)*24 + score_putable(playerboard, opponentboard)*2 + score_null_place(playerboard, opponentboard)/2);
    
}
