//
//  othello.cpp
//  othello
//
//  Created by sasa on 2023/09/30.
//
#include "variables.hpp"
#pragma once
#include "evaluate.hpp"
#include "book.hpp"

using namespace std;
using bitboard = std::pair<uint64_t, uint64_t>;

void reset() {
    cout << "[*]初期化中..." << endl;
    nowTurn = BLACK_TURN;
    nowIndex = 1;
    DEPTH = firstDEPTH;
    afterIndex = nowIndex+DEPTH;
    b.playerboard = 0x0000000810000000ULL;
    b.opponentboard = 0x0000001008000000ULL;
//    b.playerboard = 9948208703946496;
//    b.opponentboard = 18436795865005588735;
    legalboard = makelegalboard(b.playerboard, b.opponentboard);
    play_record = "";
    evaluate_ptr_num = 0;
    if (first_reset) {
        evaluate_init(U"out_momentum1.txt", 0);
        first_reset = false;
        transpose_table.reserve(100000);
        former_transpose_table.reserve(100000);
        cout << "evaluation initialized" << endl;
    }
    if (book.size() == 0) book_init();
    cout << "DEPTH: " << DEPTH << endl;
    cout << "Player: " << botplayer << endl;
    cout << "Level: " << Level << endl;
    cout << "initialized" << endl;
//    cout << evaluate(0x844c1002020009a9ULL, ~0x844c1002020009a9ULL) << endl;
    return;
}

inline int putstone(int_fast8_t y, int_fast8_t x) {
    uint64_t put = cordinate_to_bit(x, y);
    legalboard = makelegalboard(b.playerboard, b.opponentboard);
    if (canput(put, legalboard)) {
        if (nowTurn == (1 - botplayer)) {
            b_back.playerboard = b.playerboard;
            b_back.opponentboard = b.opponentboard;
            b_back.put_x = tmpx;
            b_back.put_y = tmpy;
        }
        hint_x = -1;
        hint_y = -1;
        tmpy = y;
        tmpx = x;
        uint64_t rev = Flip(put, b.playerboard, b.opponentboard);
        b.playerboard ^= (rev | put);
        b.opponentboard ^= rev;
        nowIndex++;
        afterIndex++;
        if (afterIndex <= 40) {
            evaluate_ptr_num = 0;
        } else {
            evaluate_ptr_num = 0;
        }
        play_record += coordinate_to_x_y(put);
        cout << play_record << endl;
        return 1;
    } else {
        return 0;
    }
}

inline string coordinate_to_x_y(uint64_t put) {
    constexpr string x = "abcdefgh";
    int pos = countl_zero(put);
    return string(1, x[pos % 8]) + to_string((pos / 8) + 1);
}

inline uint64_t cordinate_to_bit(int_fast8_t put, int_fast8_t y) {
    return 0x8000000000000000ULL >> ((y<<3)+put);
}

inline bool canput(uint64_t put, uint64_t legalboard) {
    return ((put & legalboard) == put);
}

// code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm
inline uint64_t makelegalboard(uint64_t p, uint64_t o) noexcept {
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

// code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm
#if __has_builtin(__builtin_subcll)
inline uint64_t OutflankToFlipmask(uint64_t outflank) noexcept {
    uint64_t flipmask, cy;
    flipmask = __builtin_subcll(outflank, 1, 0, &cy);
    return __builtin_addcll(flipmask, 0, cy, &cy);
}
#elif (defined(_M_X64) && (_MSC_VER >= 1800)) || (defined(__x86_64__) && defined(__GNUC__) && (__GNUC__ > 7 || (__GNUC__ == 7 && __GNUC_MINOR__ >= 2)))
inline uint64_t OutflankToFlipmask(uint64_t outflank) noexcept {
    uint64_t flipmask;
    unsigned char cy = _subborrow_u64(0, outflank, 1, &flipmask);
    _addcarry_u64(cy, flipmask, 0, &flipmask);
    return flipmask;
}
#else
    #define OutflankToFlipmask(outflank)    ((outflank) - (unsigned int) ((outflank) != 0))
#endif

inline uint64_t Flip(uint64_t put, uint64_t playerboard, uint64_t opponentboard) noexcept {
    uint64_t flipped, OM, outflank[4], mask[4];
    int pos = countl_zero(put);
    OM = opponentboard & 0x7e7e7e7e7e7e7e7eULL;
    
    mask[0] = 0x0080808080808080ULL >> (pos);
    mask[1] = 0x7f00000000000000ULL >> (pos);
    mask[2] = 0x0102040810204000ULL >> (pos);
    mask[3] = 0x0040201008040201ULL >> (pos);
    outflank[0] = (0x8000000000000000ULL >> countl_zero(~opponentboard & mask[0])) & playerboard;
    outflank[1] = (0x8000000000000000ULL >> countl_zero(~OM & mask[1])) & playerboard;
    outflank[2] = (0x8000000000000000ULL >> countl_zero(~OM & mask[2])) & playerboard;
    outflank[3] = (0x8000000000000000ULL >> countl_zero(~OM & mask[3])) & playerboard;
    flipped  = ((~outflank[0] + 1) << 1) & mask[0];
    flipped |= ((~outflank[1] + 1) << 1) & mask[1];
    flipped |= ((~outflank[2] + 1) << 1) & mask[2];
    flipped |= ((~outflank[3] + 1) << 1) & mask[3];

    pos = 63 - pos;
    
    mask[0] = 0x0101010101010100ULL << pos;
    mask[1] = 0x00000000000000feULL << pos;
    mask[2] = 0x0002040810204080ULL << pos;
    mask[3] = 0x8040201008040200ULL << pos;
    outflank[0] = mask[0] & ((opponentboard | ~mask[0]) + 1) & playerboard;
    outflank[1] = mask[1] & ((OM | ~mask[1]) + 1) & playerboard;
    outflank[2] = mask[2] & ((OM | ~mask[2]) + 1) & playerboard;
    outflank[3] = mask[3] & ((OM | ~mask[3]) + 1) & playerboard;
    flipped |= OutflankToFlipmask(outflank[0]) & mask[0];
    flipped |= OutflankToFlipmask(outflank[1]) & mask[1];
    flipped |= OutflankToFlipmask(outflank[2]) & mask[2];
    flipped |= OutflankToFlipmask(outflank[3]) & mask[3];

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
    legalboard = makelegalboard(b.playerboard, b.opponentboard);
//    cout << play_record << endl;
}

inline int move_ordering_value(uint64_t playerboard, uint64_t opponentboard) noexcept {
    auto it = former_transpose_table.find(make_pair(playerboard, opponentboard));
    if (it != former_transpose_table.end()) {
        return (16384-max(it->second.first, it->second.second));
    } else {
        return evaluate_moveorder(opponentboard, playerboard);
    }
}

int ai_hint() {
    cout << "[*]Botが考え中.." << endl;
    tmpbit = 0;
    think_percent = 0;
    transpose_table.clear();
    former_transpose_table.clear();
    legalboard = makelegalboard(b.playerboard, b.opponentboard);
    int putable_count = popcount(legalboard);
    if (putable_count == 0) {
        return 0;
    }
    visited_nodes = 0;
    int score = 0;
    if (putable_count == 1) {
        tmpbit = legalboard;
    } else {
        score = search_nega_scout(b.playerboard, b.opponentboard, true);
    }
    think_percent = 100;
    if (tmpbit == 0) {
        cout << "error" << endl;
        return 0;
    }
    int count = countl_zero(tmpbit);
    hint_y = count / 8;
    hint_x = count % 8;
    cout << "suggest : (" << hint_x << ", " << hint_y << ")" << endl;
    cout << "Score : " << score << endl;
    cout << "Score(stone) : " << score/256 << endl;
    return 1;
}

int ai() {
    if (nowTurn == 1-botplayer) {
        return 0;
    }
    cout << "[*]Botが考え中.." << endl;
    if (Level >= 5 && nowIndex >= 41) {
        DEPTH = 60;
        afterIndex=60;
    }
    tmpbit = 0;
    think_percent = 0;
    transpose_table.clear();
    former_transpose_table.clear();
    legalboard = makelegalboard(b.playerboard, b.opponentboard);
    int putable_count = popcount(legalboard);
    if (putable_count == 0) {
        swapboard();
        return 0;
    }
    visited_nodes = 0;
    int score = 0;
    this_thread::sleep_for(chrono::milliseconds(20));
#if use_book
    auto book_list = book.equal_range(make_pair(b.playerboard, b.opponentboard));
    if (book_list.first != book_list.second) {
        vector<multimap<bitboard, uint64_t>::const_iterator> iterators;
            for (auto it = book_list.first; it != book_list.second; ++it) {
                iterators.push_back(it);
            }
            random_device seed_gen;
            mt19937 engine {seed_gen()};
            vector<multimap<bitboard, uint64_t>::const_iterator> book_itr(1);
            sample(iterators.begin(), iterators.end(), book_itr.begin(), 1, engine);
        tmpbit = book_itr[0]->second;
        cout << "book found" << endl;
    }
#endif
    if (!tmpbit) {
        if (afterIndex >= 60) {
            think_count = 100/putable_count;
            score = search_finish_scout(b.playerboard, b.opponentboard);
        } else  {
            think_count = 100/(putable_count*(DEPTH+1-max(DEPTH-3, 1)));
            score = search_nega_scout(b.playerboard, b.opponentboard, false);
        }
    }
    think_percent = 100;
    if (tmpbit == 0) {
        cout << "error" << endl;
        return 0;
    }
    int count = countl_zero(tmpbit);
    tmpy = count / 8;
    tmpx = count % 8;
    putstone(tmpy, tmpx);
    cout << "suggest : (" << tmpx << ", " << tmpy << ")" << endl;
    cout << "Score : " << score << endl;
    cout << "Score(stone) : " << ((afterIndex < 60) ? score/256 : score) << endl;
    return 1;
}

int search_nega_scout(uint64_t playerboard, uint64_t opponentboard, bool hint) {
    cout << "algorithm: NegaScout" << endl;
    transpose_table.clear();
    former_transpose_table.clear();
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int var = 0;
    uint64_t rev;
    board_root m;
    vector<board_root> moveorder;
    moveorder.reserve(popcount(legalboard));
    m.put = 1;
    for (auto i = 0; i < 64; ++i) {
        if (legalboard & m.put) {
            rev = Flip(m.put, playerboard, opponentboard);
            m.playerboard = playerboard ^ (m.put | rev);
            m.opponentboard = opponentboard ^ rev;
            moveorder.emplace_back(m);
        }
        m.put <<= 1;
    }
    int alpha = MIN_INF, beta = MAX_INF;
    think_count = 100/(popcount(legalboard)*(DEPTH-max(1, DEPTH-4)+1));
    int wave = 0;
    int end_depth;
    if (hint == true) {
        end_depth = 10;
    } else {
        end_depth = DEPTH;
    }
    for (search_depth = max(1, end_depth-4); search_depth <= end_depth; ++search_depth) {
        think_percent = wave*(100/(DEPTH-max(1, DEPTH-4)+1));
        ++wave;
        afterIndex = nowIndex+search_depth;
        for (auto& m: moveorder) {
            m.score = move_ordering_value(m.opponentboard, m.playerboard);
        }
        sort(moveorder.begin(), moveorder.end());
        alpha = MIN_INF;
        beta = MAX_INF;
        alpha = -nega_scout(search_depth-1, -beta, -alpha, moveorder[0].opponentboard, moveorder[0].playerboard);
        if (search_depth == end_depth) {
            tmpbit = moveorder[0].put;
        }
        think_percent += think_count;
        for (size_t i = 1; i < moveorder.size(); ++i) {
            var = -nega_alpha_moveorder(search_depth-1, -alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
            think_percent += think_count;
            if (var > alpha) {
                alpha = var;
                var = -nega_scout(search_depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
                if (search_depth == end_depth) {
                    tmpbit = moveorder[i].put;
                }
            }
            alpha = max(var, alpha);
        }
        cout << "depth: " << search_depth << " Visited nodes " << visited_nodes << endl;
        transpose_table.swap(former_transpose_table);
        transpose_table.clear();
    }
    transpose_table.clear();
    former_transpose_table.clear();
    return alpha;
}

int nega_scout(int_fast8_t depth, int alpha, int beta, uint64_t playerboard, uint64_t opponentboard) noexcept {
    ++visited_nodes;
    if (depth <= 0) {
        return evaluate(playerboard, opponentboard);
    }
    bitboard board_state = make_pair(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    auto it = transpose_table.find(board_state);
    if (it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        alpha = max(l, alpha);
        beta = min(u, beta);
    }
    if (u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if (!legalboard) [[unlikely]] {
        if (!(makelegalboard(opponentboard, playerboard))) [[unlikely]] return evaluate(playerboard, opponentboard);
        else return -nega_scout(depth, -beta, -alpha, opponentboard, playerboard);
    }
    int var, max_score = MIN_INF, count = 0;
    uint64_t rev;
    board moveorder[34];
    uint64_t put;
    while(legalboard) {
        put = legalboard & (~legalboard + 1);
        legalboard ^= put;
        rev = Flip(put, playerboard, opponentboard);
        moveorder[count].playerboard = playerboard ^ (put | rev);
        moveorder[count].opponentboard = opponentboard ^ rev;
        moveorder[count].score = move_ordering_value(moveorder[count].opponentboard, moveorder[count].playerboard);
        ++count;
    }
    
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
        
    if (depth > 3) {
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
                if (var > l) {
                    transpose_table[board_state] = {u, var};
                }
                return var;
            }
            if (var > alpha) {
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
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha(depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
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

int nega_alpha_moveorder(int_fast8_t depth, int alpha, int beta, uint64_t playerboard, uint64_t opponentboard) noexcept {
    ++visited_nodes;
    if (depth <= 0) {
        return evaluate(playerboard, opponentboard);
    }
    int u = MAX_INF, l = MIN_INF;
    bitboard board_state = make_pair(playerboard, opponentboard);
    auto it = transpose_table.find(board_state);
    if (it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        alpha = max(l, alpha);
        beta = min(u, beta);
    }
    if (u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if (!legalboard) [[unlikely]] {
        if (!(makelegalboard(opponentboard, playerboard))) [[unlikely]] return evaluate(playerboard, opponentboard);
        else return -nega_alpha_moveorder(depth, -beta, -alpha, opponentboard, playerboard);
    }
    int var = 0, count = 0, max_score = MIN_INF;
    uint64_t rev;
    board moveorder[34];
    uint64_t put;
    while(legalboard) {
        put = legalboard & (~legalboard + 1);
        legalboard ^= put;
        rev = Flip(put, playerboard, opponentboard);
        moveorder[count].playerboard = playerboard ^ (put | rev);
        moveorder[count].opponentboard = opponentboard ^ rev;
        moveorder[count].score = move_ordering_value(moveorder[count].opponentboard, moveorder[count].playerboard);
        ++count;
    }
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
        
    if (depth <= 3) {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha(depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
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
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha_moveorder(depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
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

int nega_alpha(int_fast8_t depth, int alpha, int beta, uint64_t playerboard, uint64_t opponentboard) noexcept {
    ++visited_nodes;
    if (depth <= 0) {
        return evaluate(playerboard, opponentboard);
    }
    bitboard board_state = make_pair(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    auto it = transpose_table.find(board_state);
    if (it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        alpha = max(l, alpha);
        beta = min(u, beta);
    }
    if (u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if (!legalboard) [[unlikely]] {
        if (!(makelegalboard(opponentboard, playerboard))) [[unlikely]] return evaluate(playerboard, opponentboard);
        else return -nega_alpha(depth, -beta, -alpha, opponentboard, playerboard);
    }
    uint64_t rev = 0;
    int var, max_score = MIN_INF;
    for (const auto& i: moveorder_bit) {
        if (canput(i, legalboard)) {
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

int nega_alpha_moveorder_mpc(int_fast8_t depth, int alpha, int beta, uint64_t playerboard, uint64_t opponentboard) {
    ++visited_nodes;
    if (depth <= 0) {
        return evaluate(playerboard, opponentboard);
    }
    int u = MAX_INF, l = MIN_INF;
    bitboard board_state = make_pair(playerboard, opponentboard);
    auto it = transpose_table.find(board_state);
    if (it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        alpha = max(l, alpha);
        beta = min(u, beta);
    }
    if (u == l) return u;
    
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if (!legalboard) [[unlikely]] {
        if (!(makelegalboard(opponentboard, playerboard))) [[unlikely]] return evaluate(playerboard, opponentboard);
        else return -nega_alpha_moveorder(depth, -beta, -alpha, opponentboard, playerboard);
    }
    int var = 0, count = 0, max_score = MIN_INF;
    uint64_t rev;
    board moveorder[36];
    uint64_t put;
    while(legalboard) {
        put = legalboard & (~legalboard + 1);
        legalboard ^= put;
        rev = Flip(put, playerboard, opponentboard);
        moveorder[count].playerboard = playerboard ^ (put | rev);
        moveorder[count].opponentboard = opponentboard ^ rev;
        moveorder[count].score = move_ordering_value(moveorder[count].opponentboard, moveorder[count].playerboard);
        ++count;
    }
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
    
//    int64_t bound_up = (beta - 8000000000000000LL);
//    int64_t bound_low = (alpha + 8000000000000000LL);
//    for (int i = 0; i < count; ++i) {
//        if (-nega_alpha_moveorder_mpc(mpc_depth[depth]-1, -bound_up, -bound_up + 1, moveorder[i].opponentboard, moveorder[i].playerboard) >= bound_up) {
//            return beta;
//        }
//        if (-nega_alpha_moveorder_mpc(mpc_depth[depth]-1, -bound_low - 1, -bound_low, moveorder[i].opponentboard, moveorder[i].playerboard) <= bound_low) {
//            return alpha;
//        }
//    }
    
    if (depth <= 3) {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha(depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
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
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha_moveorder_mpc(depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
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

int search_finish_scout(uint64_t playerboard, uint64_t opponentboard) {
    cout << "algorithm: NegaScout" << endl;
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int var = 0, score = 0;
    uint64_t rev;
    end_search_stone_count = 0;
    if (popcount(playerboard | opponentboard) < 50) {
        board_root m;
        vector<board_root> moveorder;
        moveorder.reserve(popcount(legalboard));
        m.put = 1;
        for (auto i = 0; i < 64; ++i) {
            if (legalboard & m.put) {
                rev = Flip(m.put, playerboard, opponentboard);
                m.playerboard = playerboard ^ (m.put | rev);
                m.opponentboard = opponentboard ^ rev;
                moveorder.emplace_back(m);
            }
            m.put <<= 1;
        }
        int alpha = MIN_INF, beta = MAX_INF;
        int end_depth = min(16, 64-popcount(playerboard | opponentboard));
        end_search_stone_count = popcount(playerboard | opponentboard)+end_depth;
        think_count = (100/(popcount(legalboard)+7));
        for (search_depth = max(1, end_depth-6); search_depth <= end_depth; ++search_depth) {
            afterIndex = nowIndex+search_depth;
            for (auto& m: moveorder) {
                m.score = move_ordering_value(m.opponentboard, m.playerboard);
            }
            sort(moveorder.begin(), moveorder.end(), [](const auto &a, const auto &b) {
                return a.score > b.score;
            });
            alpha = MIN_INF;
            beta = MAX_INF;
            alpha = -nega_scout(search_depth-1, -beta, -alpha, moveorder[0].opponentboard, moveorder[0].playerboard);
            if (search_depth == end_depth) {
                tmpbit = moveorder[0].put;
            }
            for (size_t i = 1; i < moveorder.size(); ++i) {
                var = -nega_alpha_moveorder(search_depth-1, -alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
                if (var > alpha) {
                    alpha = var;
                    var = -nega_scout(search_depth-1, -beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
                }
                alpha = max(var, alpha);
            }
            cout << "depth: " << search_depth << " Visited nodes " << visited_nodes << endl;
            think_percent += think_count;
            transpose_table.swap(former_transpose_table);
            transpose_table.clear();
        }
    }
    var = 0;
    cout << "final_search" << endl;
    vector<board_finish_root> moveorder(popcount(legalboard));
    for (uint64_t put = 0x8000000000000000; put >= 1; put >>= 1) {
        if (legalboard & put) {
            rev = Flip(put, playerboard, opponentboard);
            moveorder[var].playerboard = playerboard ^ (put | rev);
            moveorder[var].opponentboard = opponentboard ^ rev;
            moveorder[var].legalboard = makelegalboard(moveorder[var].opponentboard, moveorder[var].playerboard);
            moveorder[var].score = move_ordering_value(moveorder[var].opponentboard, moveorder[var].playerboard);
            moveorder[var].put = put;
            ++var;
        }
    }
    sort(moveorder.begin(), moveorder.end(), [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
    int alpha = MIN_INF, beta = MAX_INF;
    alpha = -nega_scout_finish(-beta, -alpha, moveorder[0].opponentboard, moveorder[0].playerboard, moveorder[0].legalboard);
    tmpbit = moveorder[0].put;
    think_percent += think_count;
    for (int i = 1; i < var; ++i) {
        score = -nega_alpha_moveorder_finish(-alpha-1, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard, moveorder[i].legalboard);
        think_percent += think_count;
        if (score > alpha) {
            alpha = score;
            score = -nega_scout_finish(-beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard, moveorder[i].legalboard);
            tmpbit = moveorder[i].put;
        }
        alpha = max(score, alpha);
    }
    transpose_table.swap(former_transpose_table);
    transpose_table.clear();
    cout << "Visited nodes " << visited_nodes << endl;
    return alpha;
}

int nega_scout_finish(int alpha, int beta, uint64_t playerboard, uint64_t opponentboard, uint64_t legalboard) {
    ++visited_nodes;
    bitboard board_state = make_pair(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    auto it = transpose_table.find(board_state);
    if (it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        alpha = max(l, alpha);
        beta = min(u, beta);
    }
    if (u == l) return u;
    
    if (!legalboard) [[unlikely]] {
        uint64_t legalboard2 = makelegalboard(opponentboard, playerboard);
        if (!legalboard2) [[unlikely]] return (popcount(playerboard) - popcount(opponentboard));
        else return -nega_scout_finish(-beta, -alpha, opponentboard, playerboard, legalboard2);
    }
    int var, max_score = MIN_INF, count = 0;
    uint64_t rev;
    board_finish moveorder[34];
    uint64_t put;
    int stones = popcount(playerboard | opponentboard);
    if (stones < end_search_stone_count) {
        while(legalboard) {
            put = legalboard & (~legalboard + 1);
            legalboard ^= put;
            rev = Flip(put, playerboard, opponentboard);
            moveorder[count].playerboard = playerboard ^ (put | rev);
            moveorder[count].opponentboard = opponentboard ^ rev;
            moveorder[count].legalboard = makelegalboard(moveorder[count].opponentboard, moveorder[count].playerboard);
            moveorder[count].score = -move_ordering_value(moveorder[count].opponentboard, moveorder[count].playerboard);
            ++count;
        }
    } else {
        while(legalboard) {
            put = legalboard & (~legalboard + 1);
            legalboard ^= put;
            rev = Flip(put, playerboard, opponentboard);
            moveorder[count].playerboard = playerboard ^ (put | rev);
            moveorder[count].opponentboard = opponentboard ^ rev;
            moveorder[count].legalboard = makelegalboard(moveorder[count].opponentboard, moveorder[count].playerboard);
            moveorder[count].score = popcount(moveorder[count].legalboard);
            ++count;
        }
    }
    
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score < b.score;
    });
    
    if (stones < 57) {
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
                if (var > l) {
                    transpose_table[board_state] = {u, var};
                }
                return var;
            }
            if (var > alpha) {
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
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha_finish(-beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
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

int nega_alpha_moveorder_finish(int alpha, int beta, uint64_t playerboard, uint64_t opponentboard, uint64_t legalboard) {
    ++visited_nodes;
//    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    bitboard board_state = make_pair(playerboard, opponentboard);
    auto it = transpose_table.find(board_state);
    if (it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        alpha = max(l, alpha);
        beta = min(u, beta);
    }
    if (u == l) return u;
    
    if (!legalboard) [[unlikely]] {
        uint64_t legalboard2 = makelegalboard(opponentboard, playerboard);
        if (!legalboard2) [[unlikely]] return (popcount(playerboard) - popcount(opponentboard));
        else return -nega_alpha_moveorder_finish(-beta, -alpha, opponentboard, playerboard, legalboard2);
    }
    int var = 0, count = 0, max_score = MIN_INF;
    uint64_t rev;
    board_finish moveorder[34];
    uint64_t put;
    int stones = popcount(playerboard | opponentboard);
    if (stones < end_search_stone_count) {
        while(legalboard) {
            put = legalboard & (~legalboard + 1);
            legalboard ^= put;
            rev = Flip(put, playerboard, opponentboard);
            moveorder[count].playerboard = playerboard ^ (put | rev);
            moveorder[count].opponentboard = opponentboard ^ rev;
            moveorder[count].legalboard = makelegalboard(moveorder[count].opponentboard, moveorder[count].playerboard);
            moveorder[count].score = -move_ordering_value(moveorder[count].opponentboard, moveorder[count].playerboard);
            ++count;
        }
    } else {
        while(legalboard) {
            put = legalboard & (~legalboard + 1);
            legalboard ^= put;
            rev = Flip(put, playerboard, opponentboard);
            moveorder[count].playerboard = playerboard ^ (put | rev);
            moveorder[count].opponentboard = opponentboard ^ rev;
            moveorder[count].legalboard = makelegalboard(moveorder[count].opponentboard, moveorder[count].playerboard);
            moveorder[count].score = popcount(moveorder[count].legalboard);
            ++count;
        }
    }
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score < b.score;
    });
    
    if (stones < 57) {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha_moveorder_finish(-beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard, moveorder[i].legalboard);
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
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha_finish(-beta, -alpha, moveorder[i].opponentboard, moveorder[i].playerboard);
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

int nega_alpha_finish(int alpha, int beta, uint64_t playerboard, uint64_t opponentboard) {
    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    if (!legalboard) {
        if (!(makelegalboard(opponentboard, playerboard))) return (popcount(playerboard) - popcount(opponentboard));
        else {
            return -nega_alpha_finish(-beta, -alpha, opponentboard, playerboard);
        }
    }
    uint64_t rev = 0;
    int var = 0, max_score = MIN_INF;
    for (const auto& i: moveorder_bit) {
        if (canput(i, legalboard)) {
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
    if (nowTurn == BLACK_TURN) {
        blackc = popcount(b.playerboard);
        whitec = popcount(b.opponentboard);
    } else {
        whitec = popcount(b.playerboard);
        blackc = popcount(b.opponentboard);
    }
    return (blackc > whitec) ? 1 : (blackc < whitec) ? 2 : 0;
}
