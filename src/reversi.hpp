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
#include <array>
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

constexpr int32_t MIN_INF = -INT_MAX;
constexpr int32_t MAX_INF = INT_MAX;

extern bool first_reset;
extern int32_t DEPTH;
extern int32_t search_depth;
extern int32_t Level;
extern int32_t px, py;
extern int32_t whitec;
extern int32_t blackc;
extern int32_t tmpx, tmpy;
extern int32_t hint_x, hint_y;
extern uint64_t tmpbit;
extern int32_t think_percent;
extern int32_t think_count;
extern int32_t AIplayer;
extern int32_t nowTurn;
extern int32_t nowIndex;
extern int32_t firstDEPTH;
extern int32_t afterIndex;
extern int32_t evaluate_ptr_num;
extern bool use_mpc;
extern double mpc_p;
extern uint64_t legalboard;
extern uint64_t rev;
extern bool search_mode_enabled;

constexpr int32_t pow3[11] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049};

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

struct search_param {
    double mpc_p[4];
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
    int32_t score;
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
    uint32_t hash() const noexcept {
        // original code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm , modified
        return static_cast<uint32_t>(crc32c_u64(crc32c_u64(0, this->p), this->o));
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
    int32_t put_x;
    int32_t put_y;
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
    int32_t u;
    int32_t l;
    int32_t depth;
};

extern board b;
extern board_back b_back;

//main functions
void reset();
int32_t winner();
int32_t ai();
int32_t ai_hint();
int32_t putstone(int32_t y, int32_t x);
uint64_t cordinate_to_bit(int32_t x, int32_t y);
bool canput(uint64_t put, uint64_t legalboard);
uint64_t makelegalboard(const board &b) noexcept;
bool isPass();
bool isFinished();
void swapboard();
uint64_t Flip(const uint64_t put, const board &b) noexcept;
void sync_model(int32_t afterIndex);
void cal_mpc();

int32_t move_ordering_value(const board &b) noexcept;

int32_t nega_alpha(int32_t depth, int32_t alpha, int32_t beta, const board &b) noexcept;
int32_t nega_alpha_moveorder(int32_t depth, int32_t alpha, int32_t beta, const board &b) noexcept;
int32_t nega_alpha_moveorder_mpc(int32_t depth, int32_t alpha, int32_t beta, const board &b) noexcept;
int32_t nega_scout(int32_t depth, int32_t alpha, int32_t beta, const board &b) noexcept;
int32_t nega_scout_finish(int32_t depth, int32_t alpha, int32_t beta, const board_finish &b) noexcept;
int32_t nega_alpha_moveorder_finish(int32_t depth, int32_t alpha, int32_t beta, const board_finish &b) noexcept;
int32_t nega_alpha_moveorder_finish_mpc(int32_t depth, int32_t alpha, int32_t beta, const board_finish &b) noexcept;
int32_t nega_alpha_finish(int32_t depth, int32_t alpha, int32_t beta, const board_finish &b) noexcept;

int32_t search_nega_scout(board b, bool hint, bool mpc);
int32_t search_finish_scout(board b, bool mpc);

std::string coordinate_to_x_y(uint64_t put);

#endif /* othello_h */
