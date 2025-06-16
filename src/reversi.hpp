//
//  reversi.h
//  othello
//
//  Created by sasa on 2023/09/30.
//

#ifndef reversi_h
#define reversi_h

#define BLACK_TURN 0
#define WHITE_TURN 1
#define UP_BOARD 0xFF00000000000000ULL
#define DOWN_BOARD 0x00000000000000FFULL
#define LEFT_BOARD 0x8080808080808080ULL
#define RIGHT_BOARD 0x0101010101010101ULL

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <bit>
#include <numeric>
#include <atomic>
#include <cmath>
#include <execution>
#include <thread>
#include <numeric>
#include <Siv3D.hpp>
#include "bit.hpp"

constexpr int MIN_INF = -INT_MAX;
constexpr int MAX_INF = INT_MAX;

extern bool first_reset;
extern int DEPTH;
extern int search_depth;
extern int Level;
extern int px, py;
extern int whitec;
extern int blackc;
extern int tmpx, tmpy;
extern int hint_x, hint_y;
extern uint64_t tmpbit;
extern int think_percent;
extern int think_count;
extern int AIplayer;
extern int nowTurn;
extern int nowIndex;
extern int firstDEPTH;
extern int afterIndex;
extern int evaluate_ptr_num;
extern bool use_mpc;
extern double mpc_p;
extern uint64_t legalboard;
extern uint64_t rev;
extern bool search_mode_enabled;

constexpr int pow3[11] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049};

union features {
    uint16_t indexes_1[56];
    __m128i indexes_8[7];
};

struct Pattern_Eval {
    int16_t diagonal8[pow3[10]];
    int16_t diagonal7[pow3[10]];
    int16_t diagonal6[pow3[10]];
    int16_t diagonal5[pow3[10]];
    int16_t edge_2x[pow3[10]];
    int16_t h_v_2[pow3[10]];
    int16_t h_v_3[pow3[10]];
    int16_t h_v_4[pow3[10]];
    int16_t corner_3x3[pow3[10]];
    int16_t edge_x_side[pow3[10]];
    int16_t edge_block[pow3[10]];
    int16_t triangle[pow3[10]];
    int16_t corner_2x5[pow3[10]];
};

class spinlock {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)) {
             std::this_thread::yield();
        }
    }
    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

class board{
public:
    features index;
    uint64_t p;
    uint64_t o;
    int score;
    uint8_t player;
    bool operator<(const auto& b) const noexcept {
        return this->score > b.score;
    }
    bool operator==(const auto& other) const noexcept {
        return this->p == other.p && this->o == other.o;
    }
    bool operator!=(const auto& other) const noexcept {
        return this->p != other.p || this->o != other.o;
    }
    board flipped() const noexcept {
        return {this->index, this->o, this->p, this->score, static_cast<uint8_t>(this->player ^ 1)};
    }
    uint64_t hash() const noexcept {
        // original code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm , modified
        return crc32c_u64(crc32c_u64(0, this->p), this->o);
    }
};

class board_root : public board {
public:
    uint64_t put;
    
    board_root flipped() const noexcept {
        return {{this->index, this->o, this->p, this->score, static_cast<uint8_t>(this->player ^ 1)}, this->put};
    }
};

class board_back{
public:
    uint64_t p;
    uint64_t o;
    int put_x;
    int put_y;
};

class board_finish : public board {
public:
    uint64_t legalboard;
    
    board_finish flipped() const noexcept {
        return {{this->index, this->o, this->p, this->score, static_cast<uint8_t>(this->player ^ 1)}, this->legalboard};
    }
};

class board_finish_root : public board_finish {
public:
    uint64_t put;
};

class table_data {
public:
    uint64_t p;
    uint64_t o;
    int u;
    int l;
    int depth;
};

extern board b;
extern board_back b_back;

//main functions
void reset();
int winner();
int ai();
int ai_hint();
int putstone(int y, int x);
uint64_t cordinate_to_bit(int x, int y);
bool canput(uint64_t put, uint64_t legalboard);
uint64_t makelegalboard(const board &b) noexcept;
bool isPass();
bool isFinished();
void swapboard();
uint64_t Flip(const uint64_t put, const board &b) noexcept;
void sync_model(int afterIndex);
void cal_mpc();

int move_ordering_value(const board &b) noexcept;

int nega_alpha(int depth, int alpha, int beta, const board &b) noexcept;
int nega_alpha_moveorder(int depth, int alpha, int beta, const board &b) noexcept;
int nega_alpha_moveorder_mpc(int depth, int alpha, int beta, const board &b) noexcept;
int nega_scout(int depth, int alpha, int beta, const board &b) noexcept;
int nega_scout_finish(int alpha, int beta, const board_finish &b);
int nega_alpha_moveorder_finish(int alpha, int beta, const board_finish &b);
int nega_alpha_finish(int alpha, int beta, const board_finish &b);

int search_nega_scout(board b, bool hint, bool mpc);
int search_finish(board b);
int search_finish_scout(board b);

std::string coordinate_to_x_y(uint64_t put);

#endif /* othello_h */
