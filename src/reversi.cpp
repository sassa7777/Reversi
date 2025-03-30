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
using bitboard = pair<uint64_t, uint64_t>;

void reset() {
    cout << "[*]初期化中..." << endl;
    nowTurn = BLACK_TURN;
    nowIndex = 1;
    DEPTH = firstDEPTH;
    afterIndex = nowIndex+DEPTH;
    b.p = 0x0000000810000000ULL;
    b.o = 0x0000001008000000ULL;
//    b.p = 9948208703946496;
//    b.o = 18436795865005588735;
    legalboard = makelegalboard(b);
    play_record = "";
    evaluate_ptr_num = 0;
    hint_x = -1;
    hint_y = -1;
    tmpx = -1;
    tmpy = -1;
    eval_num.assign(eval_num.size(), false);
    if (first_reset) {
        first_reset = false;
//        evaluate_init(U"eval15.zstd");
        
        sync_model();
//        cal_mpc();
    }
    if (book.size() == 0) book_init();
    cout << "DEPTH: " << DEPTH << endl;
    cout << "Player: " << AIplayer << endl;
    cout << "Level: " << Level << endl;
    cout << "initialized" << endl;
//    cout << evaluate(0x844c1002020009a9ULL, ~0x844c1002020009a9ULL) << endl;
    return;
}

int putstone(int y, int x) {
    uint64_t put = cordinate_to_bit(x, y);
    legalboard = makelegalboard(b);
    if (canput(put, legalboard)) {
        if (nowTurn == (1 - AIplayer)) {
            b_back.p = b.p;
            b_back.o = b.o;
            b_back.put_x = tmpx;
            b_back.put_y = tmpy;
        }
        hint_x = -1;
        hint_y = -1;
        tmpy = y;
        tmpx = x;
        uint64_t rev = Flip(put, b);
        b.p ^= (rev | put);
        b.o ^= rev;
        nowIndex++;
        afterIndex++;
        play_record += coordinate_to_x_y(put);
        cout << play_record << endl;
        return 1;
    } else {
        return 0;
    }
}

inline string coordinate_to_x_y(uint64_t put) {
    constexpr string x = "abcdefgh";
    int pos = clz_u64(put);
    return string(1, x[pos % 8]) + to_string((pos / 8) + 1);
}

inline uint64_t cordinate_to_bit(int put, int y) {
    return 0x8000000000000000ULL >> ((y<<3)+put);
}

inline bool canput(uint64_t put, uint64_t legalboard) {
    return ((put & legalboard) == put);
}

// code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm
uint64_t makelegalboard(const board &b) noexcept {
    uint64_t moves, hb, flip1, flip7, flip9, flip8, pre1, pre7, pre9, pre8;
    
    hb = b.o & 0x7e7e7e7e7e7e7e7eULL;
    flip1  = hb & (b.p << 1);    flip7  = hb & (b.p << 7);        flip9  = hb & (b.p << 9);        flip8  = b.o & (b.p << 8);
    flip1 |= hb & (flip1 << 1);    flip7 |= hb & (flip7 << 7);    flip9 |= hb & (flip9 << 9);    flip8 |= b.o & (flip8 << 8);
    pre1 = hb & (hb << 1);         pre7 = hb & (hb << 7);        pre9 = hb & (hb << 9);        pre8 = b.o & (b.o << 8);
    flip1 |= pre1 & (flip1 << 2);    flip7 |= pre7 & (flip7 << 14);    flip9 |= pre9 & (flip9 << 18);    flip8 |= pre8 & (flip8 << 16);
    flip1 |= pre1 & (flip1 << 2);    flip7 |= pre7 & (flip7 << 14);    flip9 |= pre9 & (flip9 << 18);    flip8 |= pre8 & (flip8 << 16);
    moves = flip1 << 1;        moves |= flip7 << 7;        moves |= flip9 << 9;        moves |= flip8 << 8;
    flip1  = hb & (b.p >> 1);        flip7  = hb & (b.p >> 7);        flip9  = hb & (b.p >> 9);        flip8  = b.o & (b.p >> 8);
    flip1 |= hb & (flip1 >> 1);    flip7 |= hb & (flip7 >> 7);    flip9 |= hb & (flip9 >> 9);    flip8 |= b.o & (flip8 >> 8);
    pre1 >>= 1;            pre7 >>= 7;            pre9 >>= 9;            pre8 >>= 8;
    flip1 |= pre1 & (flip1 >> 2);    flip7 |= pre7 & (flip7 >> 14);    flip9 |= pre9 & (flip9 >> 18);    flip8 |= pre8 & (flip8 >> 16);
    flip1 |= pre1 & (flip1 >> 2);    flip7 |= pre7 & (flip7 >> 14);    flip9 |= pre9 & (flip9 >> 18);    flip8 |= pre8 & (flip8 >> 16);
    moves |= flip1 >> 1;        moves |= flip7 >> 7;        moves |= flip9 >> 9;        moves |= flip8 >> 8;
    
    return moves & ~(b.p | b.o);
}

// code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm
#if __has_builtin(__builtin_subcll)
inline uint64_t OutflankToFlipmask(const uint64_t outflank) noexcept {
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

//code based on http://www.amy.hi-ho.ne.jp/okuhara/flipcuda.htm
inline uint64_t Flip(uint64_t put, const board &b) noexcept {
    uint64_t flipped, OM, outflank[4], mask[4];
    int pos = clz_u64(put);
    OM = b.o & 0x7e7e7e7e7e7e7e7eULL;
    
    mask[0] = 0x0080808080808080ULL >> (pos);
    mask[1] = 0x7f00000000000000ULL >> (pos);
    mask[2] = 0x0102040810204000ULL >> (pos);
    mask[3] = 0x0040201008040201ULL >> (pos);
    outflank[0] = (0x8000000000000000ULL >> clz_u64(~b.o & mask[0])) & b.p;
    outflank[1] = (0x8000000000000000ULL >> clz_u64(~OM & mask[1])) & b.p;
    outflank[2] = (0x8000000000000000ULL >> clz_u64(~OM & mask[2])) & b.p;
    outflank[3] = (0x8000000000000000ULL >> clz_u64(~OM & mask[3])) & b.p;
    flipped  = ((~outflank[0] + 1) << 1) & mask[0];
    flipped |= ((~outflank[1] + 1) << 1) & mask[1];
    flipped |= ((~outflank[2] + 1) << 1) & mask[2];
    flipped |= ((~outflank[3] + 1) << 1) & mask[3];

    pos = 63 - pos;
    
    mask[0] = 0x0101010101010100ULL << pos;
    mask[1] = 0x00000000000000feULL << pos;
    mask[2] = 0x0002040810204080ULL << pos;
    mask[3] = 0x8040201008040200ULL << pos;
    outflank[0] = mask[0] & ((b.o | ~mask[0]) + 1) & b.p;
    outflank[1] = mask[1] & ((OM | ~mask[1]) + 1) & b.p;
    outflank[2] = mask[2] & ((OM | ~mask[2]) + 1) & b.p;
    outflank[3] = mask[3] & ((OM | ~mask[3]) + 1) & b.p;
    flipped |= OutflankToFlipmask(outflank[0]) & mask[0];
    flipped |= OutflankToFlipmask(outflank[1]) & mask[1];
    flipped |= OutflankToFlipmask(outflank[2]) & mask[2];
    flipped |= OutflankToFlipmask(outflank[3]) & mask[3];

    return flipped;
}

bool isPass() {
    board b2 = b.flipped();
    return (makelegalboard(b) == 0 && makelegalboard(b2) != 0);
}

bool isFinished() {
    board b2 = b.flipped();
    return (makelegalboard(b) == 0 && makelegalboard(b2) == 0);
}

void swapboard() {
    swap(b.p, b.o);
    nowTurn = 1-nowTurn;
    legalboard = makelegalboard(b);
//    cout << play_record << endl;
}

void sync_model() {
    int index = min(afterIndex / 4, 14);
    if (!eval_num[index]) {
        evaluate_init(U"eval" + Format(index) + U".zstd");
        eval_num[index] = true;
    }
}

int ai_hint() {
    cout << "[*]AIが考え中.." << endl;
    tmpbit = 0;
    think_percent = 0;
    sync_model();
    transpose_table.clear();
    former_transpose_table.clear();
    legalboard = makelegalboard(b);
    int putable_count = popcnt_u64(legalboard);
    if (putable_count == 0) {
        return 0;
    }
    visited_nodes = 0;
    int score = 0;
    if (putable_count == 1) {
        tmpbit = legalboard;
    } else {
        score = search_nega_scout(b, true);
    }
    think_percent = 100;
    if (tmpbit == 0) {
        cout << "error" << endl;
        return 0;
    }
    int count = clz_u64(tmpbit);
    hint_y = count / 8;
    hint_x = count % 8;
    cout << "suggest : (" << hint_x << ", " << hint_y << ")" << endl;
    cout << "Score : " << score << endl;
    cout << "Score(stone) : " << score/256 << endl;
    return 1;
}

//void cal_mpc() {
//    const string transcripts[] = {
//        "c4c3c2d6e6f6f5e3f4f3g5c6d3c5e2b4f7f2d2b3g4g6f1h5e7h3d7g3b5d8f8c8e8g8a4b6a7a5a6c7h4d1h6a3a2b7e1g1h2h7h8g7g2h1b8a8a1b1b2c1",
//        "e6d6c6f4g3e7f6d7d8f5e8f3g4g5h4f8g8f7c7h3g6h5h6h7e3c5f2b6c3c4b4b5a4d3b3a5h2h1g2e1c2e2a6b8c8h8g7b7a8a7d1d2c1g1f1b2a3a1b1a2",
//        "f5d6c3g5g6f3f4e3d3c4h5f6g3f2e6e7c6f7c5g4h4h3h2b5d2b4e2c1d1f1c2e1d7b3a5a6a4h7a7c8a3b2h6h1a1g2g1a2b1b6d8e8f8a8b7c7b8g7g8h8",
//        "e6f4c3c4d3c2f3d6f5e3e2f1c5c6d7f2d2e1c1g3b1g4g5c7b4d1g1b3b6f6e7b5g6d8h5e8a6f7a5a3a4a7h4a2g7h3f8h6h7h8g8b7a8c8a1b2b8g2h2",
//        "e6d6c5b4c6e7b5f6e8b6d7f4g6f5c4c7c8f7g5d8f8g4a3h5c3e3d3h6e2f2a4a5a6e1f1g1f3g3g7d1h3d2h4h8c2c1b3g8b1a1g2a2b2h2b7b8h1a7a8h7",
//        "c4e3f5c6e2f3d3d2c3f4f2b4c5g4c1b6b3e6f6c2d6a3b5a4b1g3c7g5g6f7e7h5b2a1a5e8h3b8a6d7h4f1d1h2b7e1h7a7h6h8g7g2c8d8a8g8h1g1f8a2",
//        "c4e3f2c3f3c5b4d3e2d2c1f4b3e1f1d1c2b5f5a5a4a3f6e6e7g3g4g2d6c6b6d7a6f8a2b2a1b1c8c7b8h4h5h6b7g6g5f7h1g1h3h2h7a8a7d8g8e8g7h8",
//        "f5f4c3g6f3d3g4f2g3h4e6h5e3f6g5d6f1c5f7c4h6e7h3f8g7e2e1d2d1c2b3c1b4g1b6c6c7d7c8h8d8a5e8h7g8g2a4a6b7b5a7h2h1a3a2b2a1b1b8a8",
//        "c4c5f6f3d6f5c6c3e6b5d3b4g4e3f4c7f2d7e2g5c2h3b6c1g6a5d8d2c8g3d1b7b1f7e8e7b3f8h6b8a8a7h5h4h2g7h7b2g2h1g1e1f1a1a4h8g8a3a6a2",
//        "c4c3c2f4f6d6f3c5f5d3e3d2e6g4d7c6b4e7c7c8e2f2g3b5a6b3a3f1b6c1d8e8f8g8f7h4e1d1g2g6h5h6h3g5g1b8b1h1h2a1b2a2b7a8a7a5a4g7h8h7",
//        "f5f4f3g4h3f6g5e6e3g3c4f2g6e2f1c6h4c5e7c3d6d7c8f7c7d8e8g8f8b8b3b4e1g7d3d2a4g1h1b5a5h2b6h7g2a6d1c2h8b2a2a3a1h5a8b1h6b7a7c1",
//        "d3e3f2c5d6c2f3e2c3c6f5e6f6d2c4f4f1e1c1g1g5g6d1b1e7g3g4b5b3b4h3a3d7h6h5c7b6d8a5a4a6a7g2f8f7h1e8h2h4g7g8h8h7b7c8b8a8a2b2a1",
//        "c4c3e6f6f5d6c5e3d3c6f2d2c2c1d1f3f4e1e7g4g3e2h3g6g5f1h6f8d8g7f7e8d7c8c7g8h8b6b5a6a5b7b3a4b8b4a8a7b2h4a3a1h5h7g1h1h2a2b1g2",
//        "c4c3c2f4f5b2e3c5d3e2b3f2g3f6f3d6b6g4c6e6h4d2g5g6a1b4a4b5a5c7c1a3a2b1e1d1f1h5g2h3h2a7e7f7a6e8a8b7d8h1g1b8c8d7f8g7g8h8h7h6",
//        "e6f4e3d2g3g5g4f6d6d7c5f5c4f3d3c3c1c6e7c2b5f2b6c7e2b4a3d1f1b1h6d8f8h4b3e8c8h2g6h7h8f7h3h5e1b2a1a2g2a5a6h1a4g1g8g7b7b8a7a8"
//    };
//    vector<vector<double>> deviations(20, vector<double>(65)), means(20, vector<double>(65));
//    play_record_to_coordinate_init();
//    for (int i = 0; i < 13; ++i) {
//        vector<vector<int>> diff(65);
//        for (auto &transcript : transcripts) {
//            uint64_t p = 0x0000000810000000ULL;
//            uint64_t o = 0x0000001008000000ULL;
//            eval_num.assign(eval_num.size(), false);
//            for (size_t j = 0, k = transcript.length(); j < k; j+=2) {
//                string kifu = string({transcript[j], transcript[j+1]});
//                put_book_init(play_record_to_put[kifu], p, o);
//                if (mpc_depth[i] <= 0) continue;
//                transpose_table.clear();
//                int a = nega_scout(i, MIN_INF, MAX_INF, p, o);
//                transpose_table.clear();
//                int b = nega_scout(mpc_depth[i], MIN_INF, MAX_INF, p, o);
//                diff[popcnt_u64(p | o)].emplace_back(a - b);
//            }
//        }
//        vector<double> diff_average(65);
//        for (size_t j = 0, k = diff.size(); j < k; ++j) {
//            diff_average[j] = (diff[j].size() == 0) ? 0 : (double)reduce(diff[j].begin(), diff[j].end(), 0L) / diff[j].size();
//        }
//        vector<double> variance(65);
//        for (size_t j = 0, k = diff.size(); j < k; ++j) {
//            for (auto x : diff[j]) {
//                variance[j] += (x - diff_average[j]) * (x - diff_average[j]);
//            }
//        }
//        vector<double> deviation(65);
//        for (size_t j = 0, k = diff.size(); j < k; ++j) {
//            deviation[j] = (diff[j].size() == 0) ? 0 : sqrt(variance[j] / diff[j].size());
//        }
//        deviations[i] = deviation;
//        means[i] = diff_average;
//        cout << "pphase " << i << " done" << endl;
//    }
//    for (auto v1 : deviations) {
//        for (auto v2 : v1) {
//            cout << v2 << "\n";
//        }
//    }
//    for (auto v1 : means) {
//        for (auto v2 : v1) {
//            cout << v2 << "\n";
//        }
//    }
//}

int ai() {
    if (nowTurn == 1-AIplayer) {
        return 0;
    }
    cout << "[*]AIが考え中.." << endl;
    if (Level >= 5 && nowIndex >= 42) {
        DEPTH = 60;
        afterIndex=60;
    }
//    if (nowIndex >= 30) {
//        DEPTH = firstDEPTH + 2;
//        afterIndex = nowIndex + DEPTH;
//    }
    tmpbit = 0;
    think_percent = 0;
    sync_model();
    transpose_table.clear();
    former_transpose_table.clear();
    legalboard = makelegalboard(b);
    int putable_count = popcnt_u64(legalboard);
    if (putable_count == 0) {
        swapboard();
        return 0;
    }
    visited_nodes = 0;
    int score = 0;
    this_thread::sleep_for(chrono::milliseconds(20));
#if use_book
    auto book_list = book.equal_range(make_pair(b.p, b.o));
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
//    if (nowIndex <= 10) {
//        score = search_book(b.p, b.o);
//    }
#endif
    if (!tmpbit) {
        if (afterIndex >= 60) {
            think_count = 100/putable_count;
            score = search_finish_scout(b);
        } else  {
            think_count = 100/(putable_count*(DEPTH+1-max(DEPTH-3, 1)));
            score = search_nega_scout(b, false);
        }
    }
    think_percent = 100;
    if (tmpbit == 0) {
        cout << "error" << endl;
        return 0;
    }
    int count = clz_u64(tmpbit);
    tmpy = count / 8;
    tmpx = count % 8;
    putstone(tmpy, tmpx);
    cout << "suggest : (" << tmpx << ", " << tmpy << ")" << endl;
    cout << "Score : " << score << endl;
    cout << "Score(stone) : " << ((afterIndex < 60) ? score/256 : score) << endl;
    return 1;
}

inline int move_ordering_value(const board &b) noexcept {
    auto it = former_transpose_table.find(b);
    if (it != former_transpose_table.end()) {
        return (32768-max(it->second.first, it->second.second));
    } else {
        return -evaluate_moveorder(b);
    }
}

int search_nega_scout(board b, bool hint) {
    cout << "algorithm: NegaScout" << endl;
    transpose_table.clear();
    former_transpose_table.clear();
    uint64_t legalboard = makelegalboard(b);
    int var = 0;
    uint64_t rev;
    vector<board_root> moveorder(popcnt_u64(legalboard));
    board b1;
    int count = 0;
    while(legalboard) {
        moveorder[count].put = blsi_u64(legalboard);
        legalboard = blsr_u64(legalboard);
        rev = Flip(moveorder[count].put, b);
        moveorder[count].o = b.p ^ (moveorder[count].put | rev);
        moveorder[count].p = b.o ^ rev;
        b1 = moveorder[count].to_board();
        moveorder[count].score = move_ordering_value(b1);
        ++count;
    }
    int alpha = MIN_INF, beta = MAX_INF;
    think_count = 100/(popcnt_u64(legalboard)*(DEPTH-max(1, DEPTH-4)+1));
    int wave = 0;
    int end_depth;
    if (hint == true) {
        end_depth = 8;
    } else if (nowIndex <= 10) {
        end_depth = DEPTH;
    } else {
        end_depth = DEPTH;
    }
    for (search_depth = max(1, end_depth-4); search_depth <= end_depth; ++search_depth) {
        think_percent = wave*(100/(DEPTH-max(1, DEPTH-4)+1));
        ++wave;
        afterIndex = nowIndex+search_depth;
        for (auto& m: moveorder) {
            board b1 = m.to_board();
            m.score = move_ordering_value(b1);
        }
        sort(moveorder.begin(), moveorder.end());
        alpha = MIN_INF;
        beta = MAX_INF;
        board b1 = moveorder[0].to_board();
        alpha = -nega_scout(search_depth-1, -beta, -alpha, b1);
        if (search_depth == end_depth) {
            tmpbit = moveorder[0].put;
        }
        think_percent += think_count;
        for (size_t i = 1; i < moveorder.size(); ++i) {
            b1 = moveorder[i].to_board();
            var = -nega_alpha_moveorder(search_depth-1, -alpha-1, -alpha, b1);
            think_percent += think_count;
            if (var > alpha) {
                alpha = var;
                var = -nega_scout(search_depth-1, -beta, -alpha, b1);
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

int nega_scout(int depth, int alpha, int beta, const board &b) noexcept {
    ++visited_nodes;
    if (depth <= 0) {
        return evaluate(b);
    }
    int u = MAX_INF, l = MIN_INF;
    auto it = transpose_table.find(b);
    if (it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        alpha = max(l, alpha);
        beta = min(u, beta);
        if (u == l) return u;
    }
    
    uint64_t legalboard = makelegalboard(b);
    if (!legalboard) [[unlikely]] {
        board b2 = b.flipped();
        if (!(makelegalboard(b2))) [[unlikely]] return evaluate(b);
        else {
            return -nega_scout(depth, -beta, -alpha, b2);
        }
    }
    int var, max_score = MIN_INF, count = 0;
    uint64_t rev;
    board moveorder[34];
    uint64_t put;
    while(legalboard) {
        put = blsi_u64(legalboard);
        legalboard = blsr_u64(legalboard);
        rev = Flip(put, b);
        moveorder[count].o = b.p ^ (put | rev);
        moveorder[count].p = b.o ^ rev;
        moveorder[count].score = move_ordering_value(moveorder[count]);
        ++count;
    }
    
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
        
    if (depth > 3) {
        var = -nega_scout(depth-1, -beta, -alpha, moveorder[0]);
        if (var >= beta) {
            if (var > l) {
                transpose_table[b] = {u, var};
            }
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
        for (int i = 1; i < count; ++i) {
            var = -nega_alpha_moveorder(depth-1, -alpha-1, -alpha, moveorder[i]);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[b] = {u, var};
                }
                return var;
            }
            if (var > alpha) {
                alpha = var;
                var = -nega_scout(depth-1, -beta, -alpha, moveorder[i]);
                if (var >= beta) {
                    if (var > l) {
                        transpose_table[b] = {u, var};
                    }
                    return var;
                }
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    } else {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha(depth-1, -beta, -alpha, moveorder[i]);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[b] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    transpose_table[b] = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    return max_score;
}

int nega_alpha_moveorder(int depth, int alpha, int beta, const board &b) noexcept {
    ++visited_nodes;
    if (depth <= 0) {
        return evaluate(b);
    }
    int u = MAX_INF, l = MIN_INF;
    auto it = transpose_table.find(b);
    if (it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        alpha = max(l, alpha);
        beta = min(u, beta);
        if (u == l) return u;
    }
    
    uint64_t legalboard = makelegalboard(b);
    if (!legalboard) [[unlikely]] {
        board b2 = b.flipped();
        if (!(makelegalboard(b2))) [[unlikely]] return evaluate(b);
        else return -nega_alpha_moveorder(depth, -beta, -alpha, b2);
    }
    int var = 0, count = 0, max_score = MIN_INF;
    uint64_t rev;
    board moveorder[34];
    uint64_t put;
    while(legalboard) {
        put = blsi_u64(legalboard);
        legalboard = blsr_u64(legalboard);
        rev = Flip(put, b);
        moveorder[count].o = b.p ^ (put | rev);
        moveorder[count].p = b.o ^ rev;
        moveorder[count].score = move_ordering_value(moveorder[count]);
        ++count;
    }
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
        
    if (depth <= 3) {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha(depth-1, -beta, -alpha, moveorder[i]);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[b] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    } else {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha_moveorder(depth-1, -beta, -alpha, moveorder[i]);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[b] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    transpose_table[b] = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    return max_score;
}

int nega_alpha(int depth, int alpha, int beta, const board &b) noexcept {
    ++visited_nodes;
    if (depth <= 0) {
        return evaluate(b);
    }
//    bitboard board_state = make_pair(playerboard, opponentboard);
//    int u = MAX_INF, l = MIN_INF;
//    auto it = transpose_table.find(board_state);
//    if (it != transpose_table.end()) {
//        u = it->second.first;
//        l = it->second.second;
//        if (u <= alpha) return u;
//        if (l >= beta) return l;
//        alpha = max(l, alpha);
//        beta = min(u, beta);
//        if (u == l) return u;
//    }
    
    uint64_t legalboard = makelegalboard(b);
    if (!legalboard) [[unlikely]] {
        board b2 = b.flipped();
        if (!(makelegalboard(b2))) [[unlikely]] return evaluate(b);
        else return -nega_alpha(depth, -beta, -alpha, b2);
    }
    uint64_t rev = 0;
    int var, max_score = MIN_INF;
    board b1;
    for (const auto& i: moveorder_bit) {
        if (canput(i, legalboard)) {
            rev = Flip(i, b);
            b1.p = b.o ^ rev;
            b1.o = b.p ^ (i | rev);
            var = -nega_alpha(depth-1, -beta, -alpha, b1);
            if (var >= beta) {
//                if (var > l) {
//                    transpose_table[b] = {u, var};
//                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
//    transpose_table[b] = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    return max_score;
}

int search_finish_scout(board b) {
    cout << "algorithm: NegaScout" << endl;
    uint64_t legalboard = makelegalboard(b);
    int var = 0, score = 0;
    uint64_t rev;
    end_search_stone_count = 0;
    if (popcnt_u64(b.p | b.o) < 50) {
        uint64_t rev;
        vector<board_finish_root> moveorder(popcnt_u64(legalboard));
        int count = 0;
        while(legalboard) {
            moveorder[count].put = blsi_u64(legalboard);
            legalboard = blsr_u64(legalboard);
            rev = Flip(moveorder[count].put, b);
            moveorder[count].o = b.p ^ (moveorder[count].put | rev);
            moveorder[count].p = b.o ^ rev;
            board b1 = moveorder[count].to_board();
            moveorder[count].score = move_ordering_value(b1);
            ++count;
        }
        int alpha = MIN_INF, beta = MAX_INF;
        int end_depth = min(16, 64-popcnt_u64(b.p | b.o));
        end_search_stone_count = popcnt_u64(b.p | b.o)+end_depth;
        think_count = (100/(popcnt_u64(legalboard)+7));
        for (search_depth = max(1, end_depth-6); search_depth <= end_depth; ++search_depth) {
            afterIndex = nowIndex+search_depth;
            for (auto& m: moveorder) {
                board b1 = m.to_board();
                m.score = move_ordering_value(b1);
            }
            sort(moveorder.begin(), moveorder.end(), [](const auto &a, const auto &b) {
                return a.score > b.score;
            });
            alpha = MIN_INF;
            beta = MAX_INF;
            board b1 = moveorder[0].to_board();
            alpha = -nega_scout(search_depth-1, -beta, -alpha, b1);
            if (search_depth == end_depth) {
                tmpbit = moveorder[0].put;
            }
            for (size_t i = 1; i < moveorder.size(); ++i) {
                board b1 = moveorder[i].to_board();
                var = -nega_alpha_moveorder(search_depth-1, -alpha-1, -alpha, b1);
                if (var > alpha) {
                    alpha = var;
                    var = -nega_scout(search_depth-1, -beta, -alpha, b1);
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
    vector<board_finish_root> moveorder(popcnt_u64(legalboard));
    int count = 0;
    while(legalboard) {
        moveorder[count].put = blsi_u64(legalboard);
        legalboard = blsr_u64(legalboard);
        rev = Flip(moveorder[count].put, b);
        moveorder[count].o = b.p ^ (moveorder[count].put | rev);
        moveorder[count].p = b.o ^ rev;
        board b1 = moveorder[count].to_board();
        moveorder[count].score = move_ordering_value(b1);
        moveorder[count].legalboard = makelegalboard(b1);
        ++count;
    }
    sort(moveorder.begin(), moveorder.end(), [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
    int alpha = MIN_INF, beta = MAX_INF;
    board b1 = moveorder[0].to_board();
    alpha = -nega_scout_finish(-beta, -alpha, b1, moveorder[0].legalboard);
    tmpbit = moveorder[0].put;
    think_percent += think_count;
    for (int i = 1; i < var; ++i) {
        b1 = moveorder[i].to_board();
        score = -nega_alpha_moveorder_finish(-alpha-1, -alpha, b1, moveorder[i].legalboard);
        think_percent += think_count;
        if (score > alpha) {
            alpha = score;
            score = -nega_scout_finish(-beta, -alpha, b1, moveorder[i].legalboard);
            tmpbit = moveorder[i].put;
        }
        alpha = max(score, alpha);
    }
    transpose_table.swap(former_transpose_table);
    transpose_table.clear();
    cout << "Visited nodes " << visited_nodes << endl;
    return alpha;
}

int nega_scout_finish(int alpha, int beta, const board &b, uint64_t legalboard) {
    ++visited_nodes;
    int u = MAX_INF, l = MIN_INF;
    auto it = transpose_table.find(b);
    if (it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        alpha = max(l, alpha);
        beta = min(u, beta);
        if (u == l) return u;
    }
    
    if (!legalboard) [[unlikely]] {
        board b2 = b.flipped();
        uint64_t legalboard2 = makelegalboard(b2);
        if (!legalboard2) [[unlikely]] return (popcnt_u64(b.p) - popcnt_u64(b.o));
        else return -nega_scout_finish(-beta, -alpha, b2, legalboard2);
    }
    int var, max_score = MIN_INF, count = 0;
    uint64_t rev;
    board moveorder[34];
    uint64_t puts[34];
    uint64_t legalboards[34];
    int stones = popcnt_u64(b.p | b.o);
    if (stones < end_search_stone_count) {
        while(legalboard) {
            puts[count] = blsi_u64(legalboard);
            legalboard = blsr_u64(legalboard);
            rev = Flip(puts[count], b);
            moveorder[count].o = b.p ^ (puts[count] | rev);
            moveorder[count].p = b.o ^ rev;
            legalboards[count] = makelegalboard(moveorder[count]);
            moveorder[count].score = -move_ordering_value(moveorder[count]);
            ++count;
        }
    } else {
        while(legalboard) {
            puts[count] = blsi_u64(legalboard);
            legalboard = blsr_u64(legalboard);
            rev = Flip(puts[count], b);
            moveorder[count].o = b.p ^ (puts[count] | rev);
            moveorder[count].p = b.o ^ rev;
            legalboards[count] = makelegalboard(moveorder[count]);
            moveorder[count].score = popcnt_u64(legalboards[count]);
            ++count;
        }
    }
    
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score < b.score;
    });
    
    if (stones < 57) {
        var = -nega_scout_finish(-beta, -alpha, moveorder[0], legalboards[0]);
        if (var >= beta) {
            if (var > l) {
                transpose_table[b] = {u, var};
            }
            return var;
        }
        alpha = max(alpha, var);
        max_score = max(max_score, var);
        for (int i = 1; i < count; ++i) {
            var = -nega_alpha_moveorder_finish(-alpha-1, -alpha, moveorder[i], legalboards[i]);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[b] = {u, var};
                }
                return var;
            }
            if (var > alpha) {
                alpha = var;
                var = -nega_scout_finish(-beta, -alpha, moveorder[i], legalboards[i]);
                if (var >= beta) {
                    if (var > l) {
                        transpose_table[b] = {u, var};
                    }
                    return var;
                }
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    } else {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha_finish(-beta, -alpha, moveorder[i]);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[b] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    transpose_table[b] = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    return max_score;
}

int nega_alpha_moveorder_finish(int alpha, int beta, const board &b, uint64_t legalboard) {
    ++visited_nodes;
//    uint64_t legalboard = makelegalboard(playerboard, opponentboard);
    int u = MAX_INF, l = MIN_INF;
    auto it = transpose_table.find(b);
    if (it != transpose_table.end()) {
        u = it->second.first;
        l = it->second.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        alpha = max(l, alpha);
        beta = min(u, beta);
        if (u == l) return u;
    }
    
    if (!legalboard) [[unlikely]] {
        board b2 = b.flipped();
        uint64_t legalboard2 = makelegalboard(b2);
        if (!legalboard2) [[unlikely]] return (popcnt_u64(b.p) - popcnt_u64(b.o));
        else return -nega_alpha_moveorder_finish(-beta, -alpha, b2, legalboard2);
    }
    int var = 0, count = 0, max_score = MIN_INF;
    uint64_t rev;
    board moveorder[34];
    uint64_t puts[34];
    uint64_t legalboards[34];
    int stones = popcnt_u64(b.p | b.o);
    if (stones < end_search_stone_count) {
        while(legalboard) {
            puts[count] = blsi_u64(legalboard);
            legalboard = blsr_u64(legalboard);
            rev = Flip(puts[count], b);
            moveorder[count].o = b.p ^ (puts[count] | rev);
            moveorder[count].p = b.o ^ rev;
            legalboards[count] = makelegalboard(moveorder[count]);
            moveorder[count].score = -move_ordering_value(moveorder[count]);
            ++count;
        }
    } else {
        while(legalboard) {
            puts[count] = blsi_u64(legalboard);
            legalboard = blsr_u64(legalboard);
            rev = Flip(puts[count], b);
            moveorder[count].o = b.p ^ (puts[count] | rev);
            moveorder[count].p = b.o ^ rev;
            legalboards[count] = makelegalboard(moveorder[count]);
            moveorder[count].score = popcnt_u64(legalboards[count]);
            ++count;
        }
    }
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score < b.score;
    });
    
    if (stones < 57) {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha_moveorder_finish(-beta, -alpha, moveorder[i], legalboards[i]);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[b] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    } else {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha_finish(-beta, -alpha, moveorder[i]);
            if (var >= beta) {
                if (var > l) {
                    transpose_table[b] = {u, var};
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    transpose_table[b] = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    return max_score;
}

int nega_alpha_finish(int alpha, int beta, const board &b) {
    uint64_t legalboard = makelegalboard(b);
    if (!legalboard) {
        board b2 = b.flipped();
        if (!(makelegalboard(b2))) return (popcnt_u64(b.p) - popcnt_u64(b.o));
        else {
            return -nega_alpha_finish(-beta, -alpha, b2);
        }
    }
    uint64_t rev = 0;
    int var = 0, max_score = MIN_INF;
    board b1;
    for (const auto& i: moveorder_bit) {
        if (canput(i, legalboard)) {
            rev = Flip(i, b);
            b1.p = b.o ^rev;
            b1.o = b.p ^ (i | rev);
            var = -nega_alpha_finish(-beta, -alpha, b1);
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
    if (nowTurn == BLACK_TURN) {
        blackc = popcnt_u64(b.p);
        whitec = popcnt_u64(b.o);
    } else {
        whitec = popcnt_u64(b.p);
        blackc = popcnt_u64(b.o);
    }
    return (blackc > whitec) ? 1 : (blackc < whitec) ? 2 : 0;
}
