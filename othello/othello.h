//
//  othello.h
//  othello
//
//  Created by sasa on 2023/09/30.
//

#ifndef othello_h
#define othello_h
#define DEPTH 6

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

char board[10][10];
//int virtualboard[10][10];
bool canPut[10][10];
int finished = 0;
bool skipped = false;
bool skip = false;
int px, py;
int player;
int turn = 0;
int whitec = 0;
int blackc = 0;
int tmpx, tmpy;
bool isbot = false;
int scoreboard[10][10] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 8, -3, 0, -1, -1, 0, -3, 8, 0,
        0, -3, -8, -3, -3, -3, -3, -8, -3, 0,
        0, 0, -3, 0, 0, 0, 0, -3, 0, 0,
        0, -1, -3, 0, -1, -1, 0, -3, -1, 0,
        0, -1, -3, 0, -1, -1, 0, -3, -1, 0,
        0, 0, -3, 0, 0, 0, 0, -3, 0, 0,
        0, -3, -8, -3, -3, -3, -3, -8, -3, 0,
        0, 8, -3, 0, -1, -1, 0, -3, 8, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int scoreboard2[10][10] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 30, -12, 0, -1, -1, 0, -12, 30, 0,
    0, -12, -15, -3, -3, -3, -3, -15, -12, 0,
    0, 0, -3, 0, -1, -1, 0, -3, 0, 0,
    0, -1, -3, -1, 3, 3, -1, -3, -1, 0,
    0, -1, -3, -1, 3, 3, -1, -3, -1, 0,
    0, 0, -3, 0, -1, -1, 0, -3, 0, 0,
    0, -12, -15, -3, -3, -3, -3, -15, -12, 0,
    0, 30, -12, 0, -1, -1, 0, -12, 30, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void reset(void);
void check3(int player, bool canput[10][10]);
void check2(int player);
bool putableto(int player);
int putstone(int px, int py);
void reverse(int px, int py);
void rebuild(bool bot);
void countstone(void);
int finishedsw(void);
int winner(void);
int countstoneswift(int c);
int ai2(bool multi);
int countscore(char board[10][10]);
int returnplayer(void);
int minimax(int depth, int playerrn);
void* minimax_thread(void* minimaxargs);
int minimax_multi(int depth, int playerrn);
int returnrundom(void);

#endif /* othello_h */
