//
//  othello.h
//  othello
//
//  Created by sasa on 2023/09/30.
//

#ifndef othello_h
#define othello_h

#define NUM_THREADS 40
#define BLACK_TURN 100
#define WHITE_TURN -100

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <omp.h>

int DEPTH;
char board[10][10];
bool canPut[10][10];
char putable_list[50][2];
int finished = 0;
bool skipped = false;
bool skip = false;
int px, py;
int player;
int turn = 0;
int whitec = 0;
int blackc = 0;
int tmpx, tmpy;
int cachex[NUM_THREADS], cachey[NUM_THREADS], results[NUM_THREADS];
int ALPHA;
int think_percent;
int think_count;
int botplayer;
int cpu_core;
bool isbot = false;

char moveorder[64][2] = {
    {0,0}, {0,7}, {7,0}, {7,7}, {0,2}, {0,5}, {2,0}, {2,2}, {2,5}, {2,7}, {5,0}, {5,2}, {5,5}, {5,7}, {7,2}, {7,5}, {0,3}, {0,4}, {2,3}, {2,4}, {3,0}, {3,2}, {3,3}, {3,4}, {3,5}, {3,7}, {4,0}, {4,2}, {4,3}, {4,4}, {4,5}, {4,7}, {5,3}, {5,4}, {7,3}, {7,4}, {1,2}, {1,3}, {1,4}, {1,5}, {2,1}, {2,6}, {3,1}, {3,6}, {4,1}, {4,6}, {5,1}, {5,6}, {6,2}, {6,3}, {6,4}, {6,5}, {0,1}, {0,6}, {1,0}, {1,7}, {6,0}, {6,7}, {7,1}, {7,6}, {1,1}, {1,6}, {6,1}, {6,6}
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
int returnplayer(void);



int nowTurn;
int nowIndex;
uint64_t playerboard;
uint64_t oppenentboard;
uint64_t legalboard;

int ai(void);
int putstone(char y, char x);
int putstone2(char *y, char *x, uint64_t* playerboard, uint64_t *oppenentboard, uint64_t *legalboard);
uint64_t cordinate_to_bit(char *x, char *y);
bool canput(uint64_t *put, uint64_t *legalboard);
uint64_t makelegalBoard(uint64_t *oppenentboard, uint64_t *playerboard);
void reversebit(uint64_t put);
void reversebit2(uint64_t *put, uint64_t *playerboard, uint64_t *oppenentboard);
uint64_t transfer(uint64_t *put, char *i);
bool isPass(void);
bool isFinished(void);
void swapboard(void);
uint64_t bitcount(uint64_t bits);
int nega_alpha_bit(char depth, int alpha, int beta,  bool passed, uint64_t *playerboard, uint64_t *oppenentboard);


#endif /* othello_h */
