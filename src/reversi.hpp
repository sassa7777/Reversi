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
#include <cmath>
#include <execution>
#include <thread>
#include <numeric>
#include <parallel_hashmap/phmap.h>
#include <Siv3D.hpp>
#include "bit.hpp"

constexpr int MIN_INF = -32768;
constexpr int MAX_INF = 32768;

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
extern uint64_t legalboard;
extern uint64_t rev;
extern bool search_mode_enabled;

constexpr int pow3[11] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049};

struct PATTERN_INDEX {
    uint16_t diagonal8_0;
    uint16_t diagonal8_1;
    uint16_t diagonal7_0;
    uint16_t diagonal7_1;
    uint16_t diagonal7_2;
    uint16_t diagonal7_3;
    uint16_t diagonal6_0;
    uint16_t diagonal6_1;
    uint16_t diagonal6_2;
    uint16_t diagonal6_3;
    uint16_t diagonal5_0;
    uint16_t diagonal5_1;
    uint16_t diagonal5_2;
    uint16_t diagonal5_3;
    uint16_t edge_2x_0;
    uint16_t edge_2x_1;
    uint16_t edge_2x_2;
    uint16_t edge_2x_3;
    uint16_t h_v_2_0;
    uint16_t h_v_2_1;
    uint16_t h_v_2_2;
    uint16_t h_v_2_3;
    uint16_t h_v_3_0;
    uint16_t h_v_3_1;
    uint16_t h_v_3_2;
    uint16_t h_v_3_3;
    uint16_t h_v_4_0;
    uint16_t h_v_4_1;
    uint16_t h_v_4_2;
    uint16_t h_v_4_3;
    uint16_t corner_3x3_0;
    uint16_t corner_3x3_1;
    uint16_t corner_3x3_2;
    uint16_t corner_3x3_3;
    uint16_t edge_x_side_0;
    uint16_t edge_x_side_1;
    uint16_t edge_x_side_2;
    uint16_t edge_x_side_3;
    uint16_t edge_block_0;
    uint16_t edge_block_1;
    uint16_t edge_block_2;
    uint16_t edge_block_3;
    uint16_t triangle_0;
    uint16_t triangle_1;
    uint16_t triangle_2;
    uint16_t triangle_3;
    uint16_t corner_2x5_0;
    uint16_t corner_2x5_1;
    uint16_t corner_2x5_2;
    uint16_t corner_2x5_3;
    uint16_t corner_2x5_4;
    uint16_t corner_2x5_5;
    uint16_t corner_2x5_6;
    uint16_t corner_2x5_7;
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

constexpr int PatternSizes[] = {
    pow3[8],
    pow3[7],
    pow3[6],
    pow3[5],
    pow3[10],
    pow3[8],
    pow3[8],
    pow3[8],
    pow3[10],
    pow3[10],
    pow3[10],
    pow3[10],
    pow3[10],
    pow3[10]
};

class board{
public:
    uint64_t p;
    uint64_t o;
    int score;
    PATTERN_INDEX index_p;
    PATTERN_INDEX index_o;
    bool operator<(const board& b) const noexcept {
        return this->score > b.score;
    }
    bool operator==(const board& other) const noexcept {
        return this->p == other.p && this->o == other.o;
    }
    bool operator!=(const board& other) const noexcept {
        return this->p != other.p || this->o != other.o;
    }
    board flipped() const noexcept {
        return {this->o, this->p, this->score, this->index_o, this->index_p};
    }
    struct hash {
        uint64_t operator()(const board &b) const noexcept {
            // original code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm , modified
            const uint64_t crc = crc32c_u64(0, b.p);
            return (crc << 32) | crc32c_u64(crc, b.o);
        }
    };
};

class board_root : public board {
public:
    uint64_t put;
    
    board_root flipped() const noexcept {
        return {{this->o, this->p, this->score, this->index_o, this->index_p}, this->put};
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
        return {{this->o, this->p, this->score, this->index_o, this->index_p}, this->legalboard};
    }
    
    bool operator==(const board_finish &other) const noexcept {
        return static_cast<const board&>(*this) == static_cast<const board&>(other);
    }
};

class board_finish_root : public board_finish {
public:
    uint64_t put;
};

extern board b;
extern board_back b_back;
using MAP = phmap::parallel_flat_hash_map<board, std::pair<int, int>, board::hash, std::equal_to<board>,
std::allocator<std::pair<const board, std::pair<int, int>>>, 4, std::mutex>;
extern MAP transpose_table;
extern MAP former_transpose_table;



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

int search_nega_scout(board b, bool hint);
int search_finish(board b);
int search_finish_scout(board b);

std::string coordinate_to_x_y(uint64_t put);

#endif /* othello_h */
