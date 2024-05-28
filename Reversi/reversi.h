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
#include <unordered_map>
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
//extern uint64_t playerboard;
//extern uint64_t oppenentboard;
extern uint64_t legalboard;
extern uint64_t rev;

class board{
public:
    uint64_t playerboard;
    uint64_t oppenentboard;
    
public:
    board(uint64_t player, uint64_t opponent) : playerboard(player), oppenentboard(opponent) {}

    board() : playerboard(0), oppenentboard(0) {}
    // Convert the playerboard and opponentboard to string and concatenate them
    std::string to_string() const {
        return std::to_string(playerboard) + std::to_string(oppenentboard);
    }

    // Override hash function to be used in unordered_map
    struct hash {
        size_t operator()(const board& b) const {
            // Hash the concatenated string representation of playerboard and opponentboard
            return std::hash<std::string>{}(b.to_string());
        }
    };

    // Override equality operator to be used in unordered_map
    bool operator==(const board& other) const {
        return playerboard == other.playerboard && oppenentboard == other.oppenentboard;
    }
};

extern board b;


//main functions
void reset(void);
int winner(void);
int ai(void);
int putstone(char y, char x);
uint64_t cordinate_to_bit(char *x, char *y);
bool canput(uint64_t *put, uint64_t *legalboard);
uint64_t makelegalBoard(uint64_t *playerboard, uint64_t *oppenentboard);
void reversebit(uint64_t put);
uint64_t transfer(uint64_t *put, char *i);
bool isPass(void);
bool isFinished(void);
void swapboard(void);
int bitcount(uint64_t bits);
uint64_t revbit(uint64_t *put, uint64_t *playerboard, uint64_t *oppenentboard, uint64_t *rev);
void moveordering(uint64_t moveorder[64], uint64_t *playerboard, uint64_t *oppenentboard);
int nega_alpha(char depth, int alpha, int beta, uint64_t *playerboard, uint64_t *oppenentboard);
int nega_alpha_move_order(char depth, int alpha, int beta, uint64_t *playerboard, uint64_t *oppenentboard, uint64_t *put);

//evaluation
int score_stone(uint64_t *playerboard, uint64_t *oppenentboard);
int score_putable(uint64_t *playerboard, uint64_t *oppenentboard);
int score_fixedstone(uint64_t *playerboard, uint64_t *oppenentboard);
int countscore(uint64_t *playerboard, uint64_t *oppenentboard, int *afterIndex);

#endif /* othello_h */
