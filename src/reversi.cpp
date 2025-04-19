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
#include "index.hpp"

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
    legalboard = makelegalboard(b);
    play_record = "";
    hint_x = -1;
    hint_y = -1;
    tmpx = -1;
    tmpy = -1;
    eval_num.assign(eval_num.size(), false);
    if (first_reset) {
        first_reset = false;
        //        sync_model(afterIndex);
        for (int i = 0; i <= 14; ++i) {
            evaluate_init(U"eval" + Format(i) + U".zstd", i);
        }
        //        cal_mpc();
    }
    if (book.size() == 0) book_init();
    cout << "DEPTH: " << DEPTH << endl;
    cout << "Player: " << AIplayer << endl;
    cout << "Level: " << Level << endl;
    cout << "initialized" << endl;
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

// code based on  http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm
uint64_t makelegalboard(const board &b) noexcept {
    uint64_t moves, mO, flip1, flip7, flip9, flip8, pre1, pre7, pre9, pre8;
    
    mO = b.o & 0x7e7e7e7e7e7e7e7eULL;
    flip7  = mO & (b.p << 7);        flip9  = mO & (b.p << 9);        flip8  = b.o & (b.p << 8);        flip1  = mO & (b.p << 1);
    flip7 |= mO & (flip7 << 7);    flip9 |= mO & (flip9 << 9);    flip8 |= b.o & (flip8 << 8);    moves  = mO + flip1;
    pre7 = mO & (mO << 7);        pre9 = mO & (mO << 9);        pre8 = b.o & (b.o << 8);
    flip7 |= pre7 & (flip7 << 14);    flip9 |= pre9 & (flip9 << 18);    flip8 |= pre8 & (flip8 << 16);
    flip7 |= pre7 & (flip7 << 14);    flip9 |= pre9 & (flip9 << 18);    flip8 |= pre8 & (flip8 << 16);
    moves |= flip7 << 7;        moves |= flip9 << 9;        moves |= flip8 << 8;
    flip7  = mO & (b.p >> 7);        flip9  = mO & (b.p >> 9);        flip8  = b.o & (b.p >> 8);        flip1  = mO & (b.p >> 1);
    flip7 |= mO & (flip7 >> 7);    flip9 |= mO & (flip9 >> 9);    flip8 |= b.o & (flip8 >> 8);    flip1 |= mO & (flip1 >> 1);
    pre7 >>= 7;            pre9 >>= 9;            pre8 >>= 8;            pre1 = mO & (mO >> 1);
    flip7 |= pre7 & (flip7 >> 14);    flip9 |= pre9 & (flip9 >> 18);    flip8 |= pre8 & (flip8 >> 16);    flip1 |= pre1 & (flip1 >> 2);
    flip7 |= pre7 & (flip7 >> 14);    flip9 |= pre9 & (flip9 >> 18);    flip8 |= pre8 & (flip8 >> 16);    flip1 |= pre1 & (flip1 >> 2);
    moves |= flip7 >> 7;        moves |= flip9 >> 9;        moves |= flip8 >> 8;        moves |= flip1 >> 1;
    
    return moves & ~(b.p | b.o);
}

//code based on http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm
uint64_t Flip(const uint64_t put, const board &b) noexcept {
    uint64_t flipped, mO, flip1, flip7, flip9, flip8, pre1, pre7, pre9, pre8, outflank1, outflank7, outflank8, outflank9;
    mO = b.o & 0x7e7e7e7e7e7e7e7e;
    flip1 = (put << 1) & mO;        flip7 = (put << 7) & mO;        flip9 = (put << 9) & mO;        flip8 = (put << 8) & b.o;
    flip1 |= (flip1 << 1) & mO;   flip7 |= (flip7 << 7) & mO;   flip9 |= (flip9 << 9) & mO;   flip8 |= (flip8 << 8) & b.o;
    pre1 = mO & (mO << 1);        pre7 = mO & (mO << 7);        pre9 = mO & (mO << 9);        pre8 = b.o & (b.o << 8);
    flip1 |= (flip1 << 2) & pre1; flip7 |= (flip7 << 14) & pre7; flip9 |= (flip9 << 18) & pre9; flip8 |= (flip8 << 16) & pre8;
    flip1 |= (flip1 << 2) & pre1; flip7 |= (flip7 << 14) & pre7; flip9 |= (flip9 << 18) & pre9; flip8 |= (flip8 << 16) & pre8;
    outflank1 = b.p & (flip1 << 1); outflank7 = b.p & (flip7 << 7); outflank9 = b.p & (flip9 << 9); outflank8 = b.p & (flip8 << 8);
    flip1 &= - (int) (outflank1 != 0);  flip7 &= - (int) (outflank7 != 0);  flip9 &= - (int) (outflank9 != 0);  flip8 &= - (int) (outflank8 != 0);
    flipped = flip1 | flip7 | flip9 | flip8;
    flip1 = (put >> 1) & mO;        flip7 = (put >> 7) & mO;        flip9 = (put >> 9) & mO;        flip8 = (put >> 8) & b.o;
    flip1 |= (flip1 >> 1) & mO;   flip7 |= (flip7 >> 7) & mO;   flip9 |= (flip9 >> 9) & mO;   flip8 |= (flip8 >> 8) & b.o;
    pre1 = mO & (mO >> 1);        pre7 = mO & (mO >> 7);        pre9 = mO & (mO >> 9);        pre8 = b.o & (b.o >> 8);
    flip1 |= (flip1 >> 2) & pre1; flip7 |= (flip7 >> 14) & pre7; flip9 |= (flip9 >> 18) & pre9; flip8 |= (flip8 >> 16) & pre8;
    flip1 |= (flip1 >> 2) & pre1; flip7 |= (flip7 >> 14) & pre7; flip9 |= (flip9 >> 18) & pre9; flip8 |= (flip8 >> 16) & pre8;
    outflank1 = b.p & (flip1 >> 1); outflank7 = b.p & (flip7 >> 7); outflank9 = b.p & (flip9 >> 9); outflank8 = b.p & (flip8 >> 8);
    flip1 &= - (int) (outflank1 != 0);  flip7 &= - (int) (outflank7 != 0);  flip9 &= - (int) (outflank9 != 0);  flip8 &= - (int) (outflank8 != 0);
    flipped |= flip1 | flip7 | flip9 | flip8;
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

//void sync_model(int afterIndex) {
//    int index = min(afterIndex / 4, 14);
//    if (index != now_model_idx) {
//        now_model_idx = index;
//        evaluate_init(U"eval" + Format(index) + U".zstd");
//    }
//}

int ai_hint() {
    cout << "[*]AIが考え中.." << endl;
    tmpbit = 0;
    think_percent = 0;
    //    sync_model(nowIndex + 8);
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

void cal_mpc() {
    const string transcripts[] = {
        "c4c3c2d6e6f6f5e3f4f3g5c6d3c5e2b4f7f2d2b3g4g6f1h5e7h3d7g3b5d8f8c8e8g8a4b6a7a5a6c7h4d1h6a3a2b7e1g1h2h7h8g7g2h1b8a8a1b1b2c1",
        "e6d6c6f4g3e7f6d7d8f5e8f3g4g5h4f8g8f7c7h3g6h5h6h7e3c5f2b6c3c4b4b5a4d3b3a5h2h1g2e1c2e2a6b8c8h8g7b7a8a7d1d2c1g1f1b2a3a1b1a2",
        "f5d6c3g5g6f3f4e3d3c4h5f6g3f2e6e7c6f7c5g4h4h3h2b5d2b4e2c1d1f1c2e1d7b3a5a6a4h7a7c8a3b2h6h1a1g2g1a2b1b6d8e8f8a8b7c7b8g7g8h8",
        "e6f4c3c4d3c2f3d6f5e3e2f1c5c6d7f2d2e1c1g3b1g4g5c7b4d1g1b3b6f6e7b5g6d8h5e8a6f7a5a3a4a7h4a2g7h3f8h6h7h8g8b7a8c8a1b2b8g2h2",
        "e6d6c5b4c6e7b5f6e8b6d7f4g6f5c4c7c8f7g5d8f8g4a3h5c3e3d3h6e2f2a4a5a6e1f1g1f3g3g7d1h3d2h4h8c2c1b3g8b1a1g2a2b2h2b7b8h1a7a8h7",
        "c4e3f5c6e2f3d3d2c3f4f2b4c5g4c1b6b3e6f6c2d6a3b5a4b1g3c7g5g6f7e7h5b2a1a5e8h3b8a6d7h4f1d1h2b7e1h7a7h6h8g7g2c8d8a8g8h1g1f8a2",
        "c4e3f2c3f3c5b4d3e2d2c1f4b3e1f1d1c2b5f5a5a4a3f6e6e7g3g4g2d6c6b6d7a6f8a2b2a1b1c8c7b8h4h5h6b7g6g5f7h1g1h3h2h7a8a7d8g8e8g7h8",
        "f5f4c3g6f3d3g4f2g3h4e6h5e3f6g5d6f1c5f7c4h6e7h3f8g7e2e1d2d1c2b3c1b4g1b6c6c7d7c8h8d8a5e8h7g8g2a4a6b7b5a7h2h1a3a2b2a1b1b8a8",
        "c4c5f6f3d6f5c6c3e6b5d3b4g4e3f4c7f2d7e2g5c2h3b6c1g6a5d8d2c8g3d1b7b1f7e8e7b3f8h6b8a8a7h5h4h2g7h7b2g2h1g1e1f1a1a4h8g8a3a6a2",
        "c4c3c2f4f6d6f3c5f5d3e3d2e6g4d7c6b4e7c7c8e2f2g3b5a6b3a3f1b6c1d8e8f8g8f7h4e1d1g2g6h5h6h3g5g1b8b1h1h2a1b2a2b7a8a7a5a4g7h8h7",
        "f5f4f3g4h3f6g5e6e3g3c4f2g6e2f1c6h4c5e7c3d6d7c8f7c7d8e8g8f8b8b3b4e1g7d3d2a4g1h1b5a5h2b6h7g2a6d1c2h8b2a2a3a1h5a8b1h6b7a7c1",
        "d3e3f2c5d6c2f3e2c3c6f5e6f6d2c4f4f1e1c1g1g5g6d1b1e7g3g4b5b3b4h3a3d7h6h5c7b6d8a5a4a6a7g2f8f7h1e8h2h4g7g8h8h7b7c8b8a8a2b2a1",
        "c4c3e6f6f5d6c5e3d3c6f2d2c2c1d1f3f4e1e7g4g3e2h3g6g5f1h6f8d8g7f7e8d7c8c7g8h8b6b5a6a5b7b3a4b8b4a8a7b2h4a3a1h5h7g1h1h2a2b1g2",
        "c4c3c2f4f5b2e3c5d3e2b3f2g3f6f3d6b6g4c6e6h4d2g5g6a1b4a4b5a5c7c1a3a2b1e1d1f1h5g2h3h2a7e7f7a6e8a8b7d8h1g1b8c8d7f8g7g8h8h7h6",
        "e6f4e3d2g3g5g4f6d6d7c5f5c4f3d3c3c1c6e7c2b5f2b6c7e2b4a3d1f1b1h6d8f8h4b3e8c8h2g6h7h8f7h3h5e1b2a1a2g2a5a6h1a4g1g8g7b7b8a7a8"
    };
    vector<vector<double>> deviations(20, vector<double>(65)), means(20, vector<double>(65));
    board bb;
    play_record_to_coordinate_init();
    vector<int> stones, diffs, deep, light;
    for (int i = 0; i < 13; ++i) {
        vector<vector<int>> diff(65);
        for (auto &transcript : transcripts) {
            bb.p = 0x0000000810000000ULL;
            bb.o = 0x0000001008000000ULL;
            for (size_t j = 0, k = transcript.length(); j < k; j+=2) {
                string kifu = string({transcript[j], transcript[j+1]});
                put_book_init(play_record_to_put[kifu], bb);
                if (mpc_depth[i] <= 0) continue;
                //                sync_model(popcnt_u64(bb.p | bb.o) + i - 4);
                transpose_table.clear();
                //                int a = search_nega_scout_custom(bb, i);
                int a = nega_scout(i, MIN_INF, MAX_INF, bb);
                transpose_table.clear();
                //                int b = search_nega_scout_custom(bb, mpc_depth[i]);
                int b = nega_scout(mpc_depth[i], MIN_INF, MAX_INF, bb);
                diff[popcnt_u64(bb.p | bb.o)].emplace_back(b - a);
                stones.emplace_back(popcnt_u64(bb.p | bb.o));
                diffs.emplace_back(b - a);
                deep.emplace_back(i);
                light.emplace_back(mpc_depth[i]);
            }
        }
        vector<double> diff_average(65);
        for (size_t j = 0, k = diff.size(); j < k; ++j) {
            diff_average[j] = (diff[j].size() == 0) ? 0 : (double)reduce(diff[j].begin(), diff[j].end(), 0L) / diff[j].size();
        }
        vector<double> variance(65);
        for (size_t j = 0, k = diff.size(); j < k; ++j) {
            for (auto x : diff[j]) {
                variance[j] += (x - diff_average[j]) * (x - diff_average[j]);
            }
        }
        vector<double> deviation(65);
        for (size_t j = 0, k = diff.size(); j < k; ++j) {
            deviation[j] = (diff[j].size() == 0) ? 0 : sqrt(variance[j] / diff[j].size());
        }
        deviations[i] = deviation;
        means[i] = diff_average;
        cout << "pphase " << i << " done" << endl;
        mpc_mean[i] = diff_average;
        mpc_deviation[i] = deviation;
    }
    for (auto v1 : stones) {
        cout << v1 << ", ";
    }
    cout << endl;
    cout << endl;
    for (size_t i = 0, j = diffs.size(); i < j; ++i) {
        cout << mpc_deviation[deep[i]][stones[i]] << ", ";
    }
    cout << endl;
    cout << endl;
    for (auto v1 : deep) {
        cout << v1 << ", ";
    }
    cout << endl;
    cout << endl;
    for (auto v1 : light) {
        cout << v1 << ", ";
    }
    cout << endl;
    cout << endl;
}

int ai() {
    if (nowTurn == 1-AIplayer) {
        return 0;
    }
    cout << "[*]AIが考え中.." << endl;
    if (Level >= 5 && nowIndex >= 41) {
        DEPTH = 60;
        afterIndex=60;
    }
    //    if (nowIndex >= 30) {
    //        DEPTH = firstDEPTH + 2;
    //        afterIndex = nowIndex + DEPTH;
    //    }
    tmpbit = 0;
    think_percent = 0;
    //    sync_model(afterIndex);
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
#endif
    auto start = std::chrono::high_resolution_clock::now();
    if (!tmpbit) {
        if (afterIndex >= 60) {
            score = search_finish_scout(b);
        } else  {
            score = search_nega_scout(b, false);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
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
    cout << "Time: " << duration.count() << " ms" << endl;
    return 1;
}

inline int move_ordering_value(const board &b) noexcept {
    pair<int, int> val;
    auto get_value = [&val](const MAP::value_type& v) {
        val = v.second;
    };
    if (former_transpose_table.if_contains(b, get_value)) {
        return (32768 - val.first);
    } else {
        return -evaluate_moveorder(b);
    }
}

int search_nega_scout(board b, bool hint) {
    cout << "algorithm: NegaScout" << endl;
    transpose_table.clear();
    former_transpose_table.clear();
    INIT_INDEX(b);
    uint64_t legalboard = makelegalboard(b);
    uint64_t rev;
    vector<board_root> moveorder(popcnt_u64(legalboard));
    int count = 0;
    while(legalboard) {
        moveorder[count].put = blsi_u64(legalboard);
        legalboard = blsr_u64(legalboard);
        rev = Flip(moveorder[count].put, b);
        moveorder[count].p = b.p ^ (moveorder[count].put | rev);
        moveorder[count].o = b.o ^ rev;
        moveorder[count].index_o = b.index_o;
        moveorder[count].index_p = b.index_p;
        SYNC_INDEX(moveorder[count].put, rev, moveorder[count]);
        swap(moveorder[count].p, moveorder[count].o);
        swap(moveorder[count].index_p, moveorder[count].index_o);
        moveorder[count].score = move_ordering_value(moveorder[count]);
        ++count;
    }
    int alpha = MIN_INF, beta = MAX_INF;
    think_count = 100/(popcnt_u64(legalboard)*(DEPTH-max(1, DEPTH-4)+1));
    int wave = 0;
    int end_depth;
    if (hint == true) {
        end_depth = 8;
    } else {
        end_depth = DEPTH;
    }
    for (search_depth = max(1, end_depth-4); search_depth <= end_depth; ++search_depth) {
        think_percent = wave*(100/(DEPTH-max(1, DEPTH-4)+1));
        ++wave;
        afterIndex = nowIndex+search_depth;
        //        sync_model(afterIndex);
        for (auto& m: moveorder) {
            m.score = move_ordering_value(m);
        }
        sort(moveorder.begin(), moveorder.end(), [](const auto &a, const auto &b) {
            return a.score > b.score;
        });
        alpha = MIN_INF;
        beta = MAX_INF;
        future<int> futures[34];
        int vars[34];
        alpha = -nega_scout(search_depth-1, -beta, -alpha, moveorder[0]);
        vars[0] = alpha;
        if (search_depth == end_depth) {
            tmpbit = moveorder[0].put;
        }
        for (size_t i = 1; i < moveorder.size(); ++i) {
            futures[i] = async(launch::async, [&, i]() -> int {
                return -nega_alpha_moveorder(search_depth-1, -alpha - 1, -alpha, moveorder[i]);
            });
        }
        for (size_t i = 1; i < moveorder.size(); ++i) {
            vars[i] = futures[i].get();
        }
        for (size_t i = 1; i < moveorder.size(); ++i) {
            if (vars[i] > alpha) {
                vars[i] = -nega_scout(search_depth-1, -beta, -vars[i], moveorder[i]);
            }
        }
        size_t index_max_score = max_element(vars, vars+moveorder.size()) - vars;
        tmpbit = moveorder[index_max_score].put;
        alpha = vars[index_max_score];
        cout << "depth: " << search_depth << " Visited nodes " << visited_nodes << endl;
        //        transpose_table.swap(former_transpose_table);
        swap(transpose_table, former_transpose_table);
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
    pair<int, int> val;
    auto get_value = [&val](const MAP::value_type& v) {
        val = v.second;
    };
    
    if (transpose_table.if_contains(b, get_value)) {
        u = val.first;
        l = val.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        if (u == l) return u;
        alpha = max(l, alpha);
        beta = min(u, beta);
    }
    
    uint64_t legalboard = makelegalboard(b);
    if (!legalboard) [[unlikely]] {
        board b2 = b.flipped();
        if (!(makelegalboard(b2))) [[unlikely]] return evaluate(b);
        else {
            return -nega_scout(depth, -beta, -alpha, b2);
        }
    }
    int max_score = MIN_INF, count = 0;
    uint64_t rev;
    board moveorder[34];
    uint64_t put;
    while(legalboard) {
        put = blsi_u64(legalboard);
        legalboard = blsr_u64(legalboard);
        rev = Flip(put, b);
        moveorder[count].p = b.p ^ (put | rev);
        moveorder[count].o = b.o ^ rev;
        moveorder[count].index_o = b.index_o;
        moveorder[count].index_p = b.index_p;
        SYNC_INDEX(put, rev, moveorder[count]);
        swap(moveorder[count].p, moveorder[count].o);
        swap(moveorder[count].index_p, moveorder[count].index_o);
        moveorder[count].score = move_ordering_value(moveorder[count]);
        ++count;
    }
    
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
    
    //    if (mpc_depth[depth] > 0 && depth <= MAX_MPC_DEPTH) {
    //        double A = aa*popcnt_u64(b.p | b.o) + bb*depth + cc*mpc_depth[depth];
    //        double dev = dd*A*A*A + ee*A*A + ff*A + gg;
    //        int bound_up = beta + 1.6 * dev;
    //        int bound_low = alpha - 1.6 * dev;
    //        for (int i = 0; i < count; ++i) {
    //            if (-nega_alpha_moveorder_mpc(mpc_depth[depth]-1, -bound_low-1, -bound_low, moveorder[i]) <= bound_low) {
    //                return bound_low;
    //            }
    //            if (-nega_alpha_moveorder_mpc(mpc_depth[depth]-1, -bound_up, -bound_up+1, moveorder[i]) >= bound_up) {
    //                return bound_up;
    //            }
    //        }
    //    }
    
    future<int> futures[34];
    int vars[34];
    if (depth > 3) {
        vars[0] = -nega_scout(depth-1, -beta, -alpha, moveorder[0]);
        if (vars[0] >= beta) {
            if (vars[0] > l) {
                transpose_table[b] = {u, vars[0]};
            }
            return vars[0];
        }
        alpha = max(alpha, vars[0]);
        max_score = max(max_score, vars[0]);
        for (int i = 1; i < count; ++i) {
            futures[i] = async(launch::async, [&, i]() -> int {
                return -nega_alpha_moveorder(depth - 1, -alpha - 1, -alpha, moveorder[i]);
            });
        }
        for (int i = 1; i < count; ++i) {
            vars[i] = futures[i].get();
        }
        for (int i = 1; i < count; ++i) {
            if (vars[i] >= beta) {
                if (vars[i] > l) {
                    transpose_table[b] = {u, vars[i]};
                }
                return vars[i];
            }
            
            if (vars[i] > alpha) {
                vars[i] = -nega_scout(depth - 1, -beta, -vars[i], moveorder[i]);
                if (vars[i] >= beta) {
                    {
                        if (vars[i] > l) {
                            transpose_table[b] = {u, vars[i]};
                        }
                    }
                    return vars[i];
                }
            }
        }
        int max_var = *max_element(vars, vars+count);
        alpha = max(alpha, max_var);
        max_score = max(max_score, max_var);
    } else {
        vars[0] = -nega_alpha(depth-1, -beta, -alpha, moveorder[0]);
        alpha = max(vars[0], alpha);
        if (vars[0] >= beta) {
            if (vars[0] > l) {
                transpose_table[b] = {u, vars[0]};
            }
            return vars[0];
        }
        for (int i = 1; i < count; ++i) {
            futures[i] = async(launch::async, [&, i]() -> int {
                return -nega_alpha(depth-1, -beta, -alpha, moveorder[i]);
            });
        }
        for (int i = 1; i < count; ++i) {
            vars[i] = futures[i].get();
        }
        for (int i = 1; i < count; ++i) {
            if (vars[i] >= beta) {
                if (vars[i] > l) {
                    transpose_table[b] = {u, vars[i]};
                }
                return vars[i];
            }
        }
        int max_var = *max_element(vars, vars+count);
        alpha = max(alpha, max_var);
        max_score = max(max_score, max_var);
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
    pair<int, int> val;
    auto get_value = [&val](const MAP::value_type& v) {
        val = v.second;
    };
    
    if (transpose_table.if_contains(b, get_value)) {
        u = val.first;
        l = val.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        if (u == l) return u;
        alpha = max(l, alpha);
        beta = min(u, beta);
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
        moveorder[count].p = b.p ^ (put | rev);
        moveorder[count].o = b.o ^ rev;
        moveorder[count].index_o = b.index_o;
        moveorder[count].index_p = b.index_p;
        SYNC_INDEX(put, rev, moveorder[count]);
        swap(moveorder[count].p, moveorder[count].o);
        swap(moveorder[count].index_p, moveorder[count].index_o);
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
                    pair<int, int> it = {u, var};
                    transpose_table.lazy_emplace_l(b, [&it](MAP::value_type& v) {
                        v.second = it;
                    },
                                                   [&it, &b](const MAP::constructor& ctor) { ctor(b, it); });
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
                    pair<int, int> it = {u, var};
                    transpose_table.lazy_emplace_l(b, [&it](MAP::value_type& v) {
                        v.second = it;
                    },
                                                   [&it, &b](const MAP::constructor& ctor) { ctor(b, it); });
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    auto it = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    transpose_table.lazy_emplace_l(b, [&it](MAP::value_type& v) {
        v.second = it;
    },
                                   [&it, &b](const MAP::constructor& ctor) { ctor(b, it); });
    return max_score;
}

int nega_alpha(int depth, int alpha, int beta, const board &b) noexcept {
    ++visited_nodes;
    if (depth <= 0) {
        return evaluate(b);
    }
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
            b1.o = b.o ^ rev;
            b1.p = b.p ^ (i | rev);
            b1.index_o = b.index_o;
            b1.index_p = b.index_p;
            SYNC_INDEX(i, rev, b1);
            swap(b1.p, b1.o);
            swap(b1.index_p, b1.index_o);
            var = -nega_alpha(depth-1, -beta, -alpha, b1);
            if (var >= beta) {
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    return max_score;
}

int nega_alpha_moveorder_mpc(int depth, int alpha, int beta, const board &b) noexcept {
    ++visited_nodes;
    if (depth <= 0) {
        return evaluate(b);
    }
    int u = MAX_INF, l = MIN_INF;
    pair<int, int> val;
    auto get_value = [&val](const MAP::value_type& v) {
        val = v.second;
    };
    
    if (transpose_table.if_contains(b, get_value)) {
        u = val.first;
        l = val.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        if (u == l) return u;
        alpha = max(l, alpha);
        beta = min(u, beta);
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
        moveorder[count].p = b.p ^ (put | rev);
        moveorder[count].o = b.o ^ rev;
        moveorder[count].index_o = b.index_o;
        moveorder[count].index_p = b.index_p;
        SYNC_INDEX(put, rev, moveorder[count]);
        swap(moveorder[count].p, moveorder[count].o);
        swap(moveorder[count].index_p, moveorder[count].index_o);
        moveorder[count].score = move_ordering_value(moveorder[count]);
        ++count;
    }
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
    
    if (mpc_depth[depth] > 0 && depth <= MAX_MPC_DEPTH) {
        double A = aa*popcnt_u64(b.p | b.o) + bb*depth + cc*mpc_depth[depth];
        double dev = dd*A*A*A + ee*A*A + ff*A + gg;
        int bound_up = beta + 1.6 * dev;
        int bound_low = alpha - 1.6 * dev;
        for (int i = 0; i < count; ++i) {
            if (-nega_alpha_moveorder_mpc(mpc_depth[depth]-1, -bound_low-1, -bound_low, moveorder[i]) <= bound_low) {
                return bound_low;
            }
            if (-nega_alpha_moveorder_mpc(mpc_depth[depth]-1, -bound_up, -bound_up+1, moveorder[i]) >= bound_up) {
                return bound_up;
            }
        }
    }
    
    if (depth <= 3) {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha(depth-1, -beta, -alpha, moveorder[i]);
            if (var >= beta) {
                if (var > l) {
                    pair<int, int> it = {u, var};
                    transpose_table.lazy_emplace_l(b, [&it](MAP::value_type& v) {
                        v.second = it;
                    },
                                                   [&it, &b](const MAP::constructor& ctor) { ctor(b, it); });
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    } else {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha_moveorder_mpc(depth-1, -beta, -alpha, moveorder[i]);
            if (var >= beta) {
                if (var > l) {
                    pair<int, int> it = {u, var};
                    transpose_table.lazy_emplace_l(b, [&it](MAP::value_type& v) {
                        v.second = it;
                    },
                                                   [&it, &b](const MAP::constructor& ctor) { ctor(b, it); });
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    auto it = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    transpose_table.lazy_emplace_l(b, [&it](MAP::value_type& v) {
        v.second = it;
    },
                                   [&it, &b](const MAP::constructor& ctor) { ctor(b, it); });
    return max_score;
}

int search_finish_scout(board b) {
    cout << "algorithm: NegaScout" << endl;
    uint64_t legalboard = makelegalboard(b);
    uint64_t rev;
    
    INIT_INDEX(b);
    end_search_stone_count = 0;
    if (popcnt_u64(b.p | b.o) < 50) {
        uint64_t rev;
        vector<board_finish_root> moveorder(popcnt_u64(legalboard));
        int count = 0;
        while(legalboard) {
            moveorder[count].put = blsi_u64(legalboard);
            legalboard = blsr_u64(legalboard);
            rev = Flip(moveorder[count].put, b);
            moveorder[count].p = b.p ^ (moveorder[count].put | rev);
            moveorder[count].o = b.o ^ rev;
            moveorder[count].index_o = b.index_o;
            moveorder[count].index_p = b.index_p;
            SYNC_INDEX(moveorder[count].put, rev, moveorder[count]);
            swap(moveorder[count].p, moveorder[count].o);
            swap(moveorder[count].index_p, moveorder[count].index_o);
            moveorder[count].score = move_ordering_value(moveorder[count]);
            ++count;
        }
        int alpha = MIN_INF, beta = MAX_INF;
        int end_depth = min(16, 64-popcnt_u64(b.p | b.o));
        end_search_stone_count = popcnt_u64(b.p | b.o)+end_depth;
        think_count = (100/(count+(end_depth-max(1, min(10, end_depth-6)))));
        for (search_depth = max(1, min(10, end_depth-6)); search_depth <= end_depth; ++search_depth) {
            afterIndex = nowIndex+search_depth;
            //            sync_model(afterIndex);
            for (auto& m: moveorder) {
                m.score = move_ordering_value(m);
            }
            sort(moveorder.begin(), moveorder.end(), [](const auto &a, const auto &b) {
                return a.score > b.score;
            });
            alpha = MIN_INF;
            beta = MAX_INF;
            future<int> futures[34];
            int vars[34];
            alpha = -nega_scout(search_depth-1, -beta, -alpha, moveorder[0]);
            vars[0] = alpha;
            if (search_depth == end_depth) {
                tmpbit = moveorder[0].put;
            }
            for (int i = 1; i < count; ++i) {
                futures[i] = async(launch::async, [&, i]() -> int {
                    return -nega_alpha_moveorder(search_depth-1, -alpha - 1, -alpha, moveorder[i]);
                });
            }
            for (int i = 1; i < count; ++i) {
                vars[i] = futures[i].get();
            }
            for (int i = 1; i < count; ++i) {
                if (vars[i] > alpha) {
                    vars[i] = -nega_scout(search_depth-1, -beta, -vars[i], moveorder[i]);
                }
            }
            size_t index_max_score = max_element(vars, vars+count) - vars;
            tmpbit = moveorder[index_max_score].put;
            alpha = vars[index_max_score];
            cout << "depth: " << search_depth << " Visited nodes " << visited_nodes << endl;
            think_percent += think_count;
            swap(transpose_table, former_transpose_table);
            transpose_table.clear();
        }
    }
    cout << "final_search" << endl;
    afterIndex = 60;
    //    sync_model(afterIndex);
    legalboard = makelegalboard(b);
    vector<board_finish_root> moveorder(popcnt_u64(legalboard));
    int count = 0;
    while(legalboard) {
        moveorder[count].put = blsi_u64(legalboard);
        legalboard = blsr_u64(legalboard);
        rev = Flip(moveorder[count].put, b);
        moveorder[count].p = b.p ^ (moveorder[count].put | rev);
        moveorder[count].o = b.o ^ rev;
        moveorder[count].index_o = b.index_o;
        moveorder[count].index_p = b.index_p;
        SYNC_INDEX(moveorder[count].put, rev, moveorder[count]);
        swap(moveorder[count].p, moveorder[count].o);
        swap(moveorder[count].index_p, moveorder[count].index_o);
        moveorder[count].score = move_ordering_value(moveorder[count]);
        moveorder[count].legalboard = makelegalboard(moveorder[count]);
        ++count;
    }
    sort(moveorder.begin(), moveorder.end(), [](const auto &a, const auto &b) {
        return a.score > b.score;
    });
    int alpha = MIN_INF, beta = MAX_INF;
    future<int> futures[34];
    int vars[34];
    alpha = -nega_scout_finish(-beta, -alpha, moveorder[0]);
    vars[0] = alpha;
    tmpbit = moveorder[0].put;
    for (int i = 1; i <count; ++i) {
        futures[i] = async(launch::async, [&, i]() -> int {
            return -nega_alpha_moveorder_finish(-alpha - 1, -alpha, moveorder[i]);
        });
    }
    for (int i = 1; i < count; ++i) {
        vars[i] = futures[i].get();
    }
    for (int i = 1; i < count; ++i) {
        if (vars[i] > alpha) {
            vars[i] = -nega_scout_finish(-beta, -vars[i], moveorder[i]);
        }
    }
    size_t index_max_score = max_element(vars, vars+count) - vars;
    tmpbit = moveorder[index_max_score].put;
    alpha = vars[index_max_score];
    swap(transpose_table, former_transpose_table);
    transpose_table.clear();
    think_percent += think_count;
    cout << "Visited nodes " << visited_nodes << endl;
    return alpha;
}

int nega_scout_finish(int alpha, int beta, const board_finish &b) {
    ++visited_nodes;
    int u = MAX_INF, l = MIN_INF;
    pair<int, int> val;
    auto get_value = [&val](const MAP::value_type& v) {
        val = v.second;
    };
    if (transpose_table.if_contains(b, get_value)) {
        u = val.first;
        l = val.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        if (u == l) return u;
        alpha = max(l, alpha);
        beta = min(u, beta);
    }
    if (!b.legalboard) [[unlikely]] {
        board_finish b2 = b.flipped();
        b2.legalboard = makelegalboard(b2);
        if (!b2.legalboard) [[unlikely]] return (popcnt_u64(b.p) - popcnt_u64(b.o));
        else return -nega_scout_finish(-beta, -alpha, b2);
    }
    int max_score = MIN_INF, count = 0;
    uint64_t rev;
    board_finish moveorder[34];
    int stones = popcnt_u64(b.p | b.o);
    if (stones < end_search_stone_count) {
        for (uint64_t put = 0x8000000000000000; put > 0; put >>= 1) {
            if (b.legalboard & put) {
                rev = Flip(put, b);
                moveorder[count].p = b.p ^ (put | rev);
                moveorder[count].o = b.o ^ rev;
                moveorder[count].index_o = b.index_o;
                moveorder[count].index_p = b.index_p;
                SYNC_INDEX(put, rev, moveorder[count]);
                swap(moveorder[count].p, moveorder[count].o);
                swap(moveorder[count].index_p, moveorder[count].index_o);
                moveorder[count].legalboard = makelegalboard(moveorder[count]);
                moveorder[count].score = -move_ordering_value(moveorder[count]);
                ++count;
            }
        }
    } else {
        for (uint64_t put = 0x8000000000000000; put > 0; put >>= 1) {
            if (b.legalboard & put) {
                rev = Flip(put, b);
                moveorder[count].p = b.p ^ (put | rev);
                moveorder[count].o = b.o ^ rev;
                moveorder[count].index_o = b.index_o;
                moveorder[count].index_p = b.index_p;
                SYNC_INDEX(put, rev, moveorder[count]);
                swap(moveorder[count].p, moveorder[count].o);
                swap(moveorder[count].index_p, moveorder[count].index_o);
                moveorder[count].legalboard = makelegalboard(moveorder[count]);
                moveorder[count].score = popcnt_u64(moveorder[count].legalboard);
                ++count;
            }
        }
    }
    
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score < b.score;
    });
    future<int> futures[34];
    int vars[34];
    if (stones < 57) {
        vars[0] = -nega_scout_finish(-beta, -alpha, moveorder[0]);
        if (vars[0] >= beta) {
            if (vars[0] > l) {
                transpose_table[b] = {u, vars[0]};
            }
            return vars[0];
        }
        alpha = max(alpha, vars[0]);
        max_score = max(max_score, vars[0]);
        for (int i = 1; i < count; ++i) {
            futures[i] = async(launch::async, [&, i]() -> int {
                return -nega_alpha_moveorder_finish(-alpha - 1, -alpha, moveorder[i]);
            });
        }
        for (int i = 1; i < count; ++i) {
            vars[i] = futures[i].get();
        }
        for (int i = 1; i < count; ++i) {
            if (vars[i] >= beta) {
                if (vars[i] > l) {
                    transpose_table[b] = {u, vars[i]};
                }
                return vars[i];
            }
            
            if (vars[i] > alpha) {
                vars[i] = -nega_scout_finish(-beta, -vars[i], moveorder[i]);
                if (vars[i] >= beta) {
                    if (vars[i] > l) {
                        transpose_table[b] = {u, vars[i]};
                    }
                    return vars[i];
                }
            }
        }
        int max_var = *max_element(vars, vars+count);
        alpha = max(alpha, max_var);
        max_score = max(max_score, max_var);
    } else {
        for (int i = 0; i < count; ++i) {
            futures[i] = async(launch::async, [&, i]() -> int {
                int var = -nega_alpha_finish(-beta, -alpha, moveorder[i]);
                return var;
            });
        }
        for (int i = 0; i < count; ++i) {
            vars[i] = futures[i].get();
        }
        for (int i = 0; i < count; ++i) {
            if (vars[i] >= beta) {
                if (vars[i] > l) {
                    transpose_table[b] = {u, vars[i]};
                }
                return vars[i];
            }
        }
        int max_var = *max_element(vars, vars+count);
        alpha = max(alpha, max_var);
        max_score = max(max_score, max_var);
    }
    transpose_table[b] = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    return max_score;
}

int nega_alpha_moveorder_finish(int alpha, int beta, const board_finish &b) {
    ++visited_nodes;
    int u = MAX_INF, l = MIN_INF;
    pair<int, int> val;
    auto get_value = [&val](const MAP::value_type& v) {
        val = v.second;
    };
    
    if (transpose_table.if_contains(b, get_value)) {
        u = val.first;
        l = val.second;
        if (u <= alpha) return u;
        if (l >= beta) return l;
        if (u == l) return u;
        alpha = max(l, alpha);
        beta = min(u, beta);
    }
    if (!b.legalboard) [[unlikely]] {
        board_finish b2 = b.flipped();
        b2.legalboard = makelegalboard(b2);
        if (!b2.legalboard) [[unlikely]] return (popcnt_u64(b.p) - popcnt_u64(b.o));
        else return -nega_alpha_moveorder_finish(-beta, -alpha, b2);
    }
    int var = 0, count = 0, max_score = MIN_INF;
    uint64_t rev;
    board_finish moveorder[34];
    int stones = popcnt_u64(b.p | b.o);
    if (stones < end_search_stone_count) {
        for (uint64_t put = 0x8000000000000000; put > 0; put >>= 1) {
            if (b.legalboard & put) {
                rev = Flip(put, b);
                moveorder[count].p = b.p ^ (put | rev);
                moveorder[count].o = b.o ^ rev;
                moveorder[count].index_o = b.index_o;
                moveorder[count].index_p = b.index_p;
                SYNC_INDEX(put, rev, moveorder[count]);
                swap(moveorder[count].p, moveorder[count].o);
                swap(moveorder[count].index_p, moveorder[count].index_o);
                moveorder[count].legalboard = makelegalboard(moveorder[count]);
                moveorder[count].score = -move_ordering_value(moveorder[count]);
                ++count;
            }
        }
    } else {
        for (uint64_t put = 0x8000000000000000; put > 0; put >>= 1) {
            if (b.legalboard & put) {
                rev = Flip(put, b);
                moveorder[count].p = b.p ^ (put | rev);
                moveorder[count].o = b.o ^ rev;
                moveorder[count].index_o = b.index_o;
                moveorder[count].index_p = b.index_p;
                SYNC_INDEX(put, rev, moveorder[count]);
                swap(moveorder[count].p, moveorder[count].o);
                swap(moveorder[count].index_p, moveorder[count].index_o);
                moveorder[count].legalboard = makelegalboard(moveorder[count]);
                moveorder[count].score = popcnt_u64(moveorder[count].legalboard);
                ++count;
            }
        }
    }
    sort(moveorder, moveorder+count, [](const auto &a, const auto &b) {
        return a.score < b.score;
    });
    
    if (stones < 57) {
        for (int i = 0; i < count; ++i) {
            var = -nega_alpha_moveorder_finish(-beta, -alpha, moveorder[i]);
            if (var >= beta) {
                if (var > l) {
                    pair<int, int> it = {u, var};
                    transpose_table.lazy_emplace_l(b, [&it](MAP::value_type& v) {
                        v.second = it;
                    },
                                                   [&it, &b](const MAP::constructor& ctor) { ctor(b, it); });
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
                    pair<int, int> it = {u, var};
                    transpose_table.lazy_emplace_l(b, [&it](MAP::value_type& v) {
                        v.second = it;
                    },
                                                   [&it, &b](const MAP::constructor& ctor) { ctor(b, it); });
                }
                return var;
            }
            alpha = max(alpha, var);
            max_score = max(max_score, var);
        }
    }
    auto it = make_pair(max_score, ((max_score > alpha) ? max_score : l));
    transpose_table.lazy_emplace_l(b, [&it](MAP::value_type& v) {
        v.second = it;
    },
                                   [&it, &b](const MAP::constructor& ctor) { ctor(b, it); });
    return max_score;
}

int nega_alpha_finish(int alpha, int beta, const board_finish &b) {
    if (!b.legalboard) {
        board_finish b2 = b.flipped();
        b2.legalboard = makelegalboard(b2);
        if (!(b2.legalboard)) return (popcnt_u64(b.p) - popcnt_u64(b.o));
        else {
            return -nega_alpha_finish(-beta, -alpha, b2);
        }
    }
    uint64_t rev = 0;
    int var = 0, max_score = MIN_INF;
    board_finish b1;
    for (const auto& i: moveorder_bit) {
        if (canput(i, b.legalboard)) {
            rev = Flip(i, b);
            b1.o = b.o ^rev;
            b1.p = b.p ^ (i | rev);
            b1.index_p = b.index_p;
            b1.index_o = b.index_o;
            SYNC_INDEX(i, rev, b1);
            swap(b1.p, b1.o);
            swap(b1.index_p, b1.index_o);
            b1.legalboard = makelegalboard(b1);
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
