//
//  othello.h
//  othello
//
//  Created by sasa on 2023/09/30.
//

#ifndef othello_h
#define othello_h

#define BLACK_TURN 100
#define WHITE_TURN -100

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int DEPTH;
int px, py;
int whitec = 0;
int blackc = 0;
int tmpx, tmpy;
int think_percent;
int think_count;
int botplayer;
int cpu_core;
int nowTurn;
int nowIndex;
int firstDEPTH;
int afterIndex;
uint64_t playerboard;
uint64_t oppenentboard;
uint64_t legalboard;
uint64_t rev;
int transposetable_max[11];
int transposetable_low[11];
bool isbot = false;
bool isdepth_odd;

char moveorder[64][2] = {
    {0,0}, {0,7}, {7,0}, {7,7}, {0,2}, {0,5}, {2,0}, {2,2}, {2,5}, {2,7}, {5,0}, {5,2}, {5,5}, {5,7}, {7,2}, {7,5}, {0,3}, {0,4}, {2,3}, {2,4}, {3,0}, {3,2}, {3,3}, {3,4}, {3,5}, {3,7}, {4,0}, {4,2}, {4,3}, {4,4}, {4,5}, {4,7}, {5,3}, {5,4}, {7,3}, {7,4}, {1,2}, {1,3}, {1,4}, {1,5}, {2,1}, {2,6}, {3,1}, {3,6}, {4,1}, {4,6}, {5,1}, {5,6}, {6,2}, {6,3}, {6,4}, {6,5}, {0,1}, {0,6}, {1,0}, {1,7}, {6,0}, {6,7}, {7,1}, {7,6}, {1,1}, {1,6}, {6,1}, {6,6}
};

uint64_t moveorder_bit[64] = {
    9223372036854775808, 72057594037927936, 128, 1, 2305843009213693952, 288230376151711744, 140737488355328, 35184372088832, 4398046511104, 1099511627776, 8388608, 2097152, 262144, 65536, 32, 4, 1152921504606846976, 576460752303423488, 17592186044416, 8796093022208, 549755813888, 137438953472, 68719476736, 34359738368, 17179869184, 4294967296, 2147483648, 536870912, 268435456, 134217728, 67108864, 16777216, 1048576, 524288, 16, 8, 9007199254740992, 4503599627370496, 2251799813685248, 1125899906842624, 70368744177664, 2199023255552, 274877906944, 8589934592, 1073741824, 33554432, 4194304, 131072, 8192, 4096, 2048, 1024, 4611686018427387904, 144115188075855872, 36028797018963968, 281474976710656, 32768, 256, 64, 2, 18014398509481984, 562949953421312, 16384, 512
};

char moveorder2[8][16] = {
    {1,1,3,3,4,1,4,3,4,2,2,3,4,1,2,2},
    {1,3,1,3,4,4,1,4,3,3,4,2,2,2,1,2},
    {1,1,3,3,4,1,4,3,4,2,2,3,4,1,2,2},
    {8,6,8,6,5,5,8,5,6,5,6,7,7,7,8,7},
    {8,6,8,6,5,5,8,5,6,5,6,7,7,7,8,7},
    {1,1,3,3,4,1,4,3,4,2,2,3,4,1,2,2},
    {8,8,6,6,5,5,8,5,6,5,6,7,7,8,7,7},
    {8,6,8,6,5,8,5,6,5,7,7,5,6,7,8,7}
};


void reset(void);
int winner(void);
int ai(void);
int putstone(char y, char x);
int putstone2(uint64_t *put, uint64_t* playerboard, uint64_t *oppenentboard, uint64_t *legalboard);
uint64_t cordinate_to_bit(char *x, char *y);
bool canput(uint64_t *put, uint64_t *legalboard);
uint64_t makelegalBoard(uint64_t *playerboard, uint64_t *oppenentboard);
void reversebit(uint64_t put);
void reversebit2(uint64_t *put, uint64_t *playerboard, uint64_t *oppenentboard);
uint64_t transfer(uint64_t *put, char *i);
bool isPass(void);
bool isFinished(void);
void swapboard(void);
int bitcount(uint64_t bits);
uint64_t revbit(uint64_t *put, uint64_t *playerboard, uint64_t *oppenentboard);
short nega_alpha(char depth, short alpha, short beta, uint64_t *playerboard, uint64_t *oppenentboard);
short nega_scout(char depth, short alpha, short beta, uint64_t *playerboard, uint64_t *oppenentboard);

#endif /* othello_h */
