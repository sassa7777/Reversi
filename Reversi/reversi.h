//
//  reversi.h
//  othello
//
//  Created by sasa on 2023/09/30.
//

#ifndef reversi_h
#define reversi_h

#define BLACK_TURN 100
#define WHITE_TURN -100
#define UP_BOARD 0xFF00000000000000ULL
#define DOWN_BOARD 0x00000000000000FFULL
#define LEFT_BOARD 0x8080808080808080ULL
#define RIGHT_BOARD 0x0101010101010101ULL
#define MIN_INF -2147483646
#define MAX_INF 2147483646

#include <iostream>
#include <bit>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <string>

extern int DEPTH;
extern int Level;
extern int px, py;
extern int whitec;
extern int blackc;
extern int tmpx, tmpy;
extern uint64_t tmpbit;
extern int think_percent;
extern int think_count;
extern int botplayer;
extern int cpu_core;
extern int nowTurn;
extern int nowIndex;
extern int firstDEPTH;
extern int afterIndex;
extern uint64_t legalboard;
extern uint64_t rev;
struct board{
public:
    uint64_t playerboard;
    uint64_t opponentboard;
    uint64_t put;
    int score;
    struct hash {
        size_t operator()(const board& b) const {
            return std::hash<std::string>{}(std::to_string(b.playerboard)+"&"+std::to_string(b.opponentboard));
        }
    };
    bool operator<(const board& b) const {
        return score > b.score;
    }
    bool operator==(const board& other) const {
        return this->playerboard == other.playerboard && this->opponentboard == other.opponentboard;
    }
};
extern board b;



//main functions
void reset(void);
int winner(void);
int ai(void);
int putstone(int_fast8_t y, int_fast8_t x);
uint64_t cordinate_to_bit(int_fast8_t *x, int_fast8_t *y);
bool canput(uint64_t *put, uint64_t *legalboard);
uint64_t makelegalboard(const uint64_t *p, const uint64_t *o);
void reversebit(uint64_t put);
uint64_t transfer(uint64_t *put, int_fast8_t *i);
bool isPass(void);
bool isFinished(void);
void swapboard(void);
uint64_t Flip(uint64_t *put, uint64_t *playerboard, uint64_t *opponentboard);
int nega_alpha(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard);
int nega_alpha_moveorder(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard);
int nega_alpha_moveorder_finish(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard);
int nega_scout(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard);
int nega_alpha_scout(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard);
int nega_alpha_moveorder_scout(int_fast8_t depth, int alpha, int beta, uint64_t *playerboard, uint64_t *opponentboard);
int search(uint64_t *playerboard, uint64_t *opponentboard);
int search_finish(uint64_t *playerboard, uint64_t *opponentboard);
int search_nega_scout(uint64_t *playerboard, uint64_t *opponentboard);

//evaluation
int score_stone(const uint64_t *playerboard, const uint64_t *opponentboard);
int score_stone2(const uint64_t *playerboard, const uint64_t *opponentboard);
int score_putable(const uint64_t *playerboard, const uint64_t *opponentboard);
int score_fixedstone(const uint64_t *playerboard, const uint64_t *opponentboard);
int countscore(const uint64_t *playerboard, const uint64_t *opponentboard, const int *afterIndex);

#endif /* othello_h */
