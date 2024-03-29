//
//  othello.h
//  othello
//
//  Created by sasa on 2023/09/30.
//

#ifndef othello_h
#define othello_h

#define NUM_THREADS 40

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
    {1,1},{1,8},{8,1},{8,8},{1,3},{1,6},{3,1},{3,3},
    {3,6},{3,8},{6,1},{6,3},{6,6},{6,8},{8,3},{8,6},
    {1,4},{1,5},{3,4},{3,5},{4,1},{4,3},{4,4},{4,5},
    {4,6},{4,8},{5,1},{5,3},{5,4},{5,5},{5,6},{5,8},
    {6,4},{6,5},{8,4},{8,5},{2,3},{2,4},{2,5},{2,6},
    {3,2},{3,7},{4,2},{4,7},{5,2},{5,7},{6,2},{6,7},
    {7,3},{7,4},{7,5},{7,6},{1,2},{1,7},{2,1},{2,8},
    {7,1},{7,8},{8,2},{8,7},{2,2},{2,7},{7,2},{7,7}
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
void check(int *player);
void check2(char *player, bool canput[10][10], char board[10][10]);
int putstone(int px, int py);
void putstone2(char *py, char *px, char *player, char board[10][10]);
void reverse(int x, int y);
void reverse2(char **x, char **y, char **player, char board[10][10]);
void rebuild(void);
void countstone(void);
int finishedsw(void);
int winner(void);
int countstoneswift(int c);
int ai(void);
void copyboard(char src[10][10], char dest[10][10]);
int returnplayer(void);
int nega_alpha(char depth, char playerrn, int alpha, int beta, bool passed);
void negaalphaTH(void);
void* negaalphat(void* args);
int nega_alphadeep(char depth, char playerrn, int alpha, int beta, bool passed, char board[10][10]);
void negaalpha_omp(int task);
int putable_counter(int *player, char board[10][10]);
void putable_saver(int player, char board[10][10]);

#endif /* othello_h */
