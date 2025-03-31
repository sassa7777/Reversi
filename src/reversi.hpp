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

class board{
public:
    uint64_t p;
    uint64_t o;
    int score;

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
        return {this->o, this->p, this->score};
    }
    struct hash {
        uint64_t operator()(const board &b) const noexcept {
            // code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm , modified
            const uint64_t crc = crc32c_u64(0, b.p);
            return (crc << 32) | crc32c_u64(crc, b.o);
        }
    };
};

class board_root : public board {
public:
    uint64_t put;

    board_root flipped() const noexcept {
        return {{this->o, this->p, this->score}, this->put};
    }
    board to_board() const noexcept {
        return {this->p, this->o, this->score};
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
        return {{this->o, this->p, this->score}, this->legalboard};
    }
    board to_board() const noexcept {
        return {this->p, this->o, this->score};
    }
    bool operator==(const board_finish &other) const noexcept {
        return static_cast<const board&>(*this) == static_cast<const board&>(other);
    }
};

class board_finish_root : public board_finish {
public:
    uint64_t put;

    board_finish to_board_finish() const noexcept {
        return {{this->o, this->p, this->score}, this->legalboard};
    }
};

extern board b;
extern board_back b_back;

extern phmap::parallel_flat_hash_map<board, std::pair<int, int>, board::hash> transpose_table;
extern phmap::parallel_flat_hash_map<board, std::pair<int, int>, board::hash> former_transpose_table;



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
uint64_t Flip(uint64_t put, const board &b) noexcept;
void sync_model();
void cal_mpc();

int nega_alpha(int depth, int alpha, int beta, const board &b) noexcept;
int nega_alpha_moveorder(int depth, int alpha, int beta, const board &b) noexcept;
int nega_scout(int depth, int alpha, int beta, const board &b) noexcept;
int nega_scout_finish(int alpha, int beta, const board_finish &b);
int nega_alpha_moveorder_finish(int alpha, int beta, const board_finish &b);
int nega_alpha_finish(int alpha, int beta, const board_finish &b);

int search_nega_scout(board b, bool hint);
int search_finish(board b);
int search_finish_scout(board b);

std::string coordinate_to_x_y(uint64_t put);

#endif /* othello_h */
