//
//  othello.h
//  othello
//
//  Created by sasa on 2023/09/30.
//

#ifndef othello_h
#define othello_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

char board[10][10];
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
int cachex[4], cachey[4], results[4];
bool isbot = false;

char moveorder[128] = {
    1,1,8,8,1,1,3,3,3,3,6,6,6,6,8,8,1,1,3,3,4,4,4,4,4,4,5,5,5,5,5,5,6,6,8,8,2,2,2,2,3,3,4,4,5,5,6,6,7,7,7,7,1,1,2,2,7,7,8,8,2,2,7,7,
    1,8,1,8,3,6,1,3,6,8,1,3,6,8,3,6,4,5,4,5,1,3,4,5,6,8,1,3,4,5,6,8,4,5,4,5,3,4,5,6,2,7,2,7,2,7,2,7,3,4,5,6,2,7,1,8,1,8,2,7,2,7,2,7
};


void reset(void);
void check(int *player);
void check2(char *player, bool canput[10][10], char board[10][10]);
int putstone(int px, int py);
int putstone2(char *py, char *px, char *player, bool canput[10][10], char board[10][10]);
void reverse(int x, int y);
void reverse2(char **x, char **y, char **player, char board[10][10]);
void rebuild(void);
void countstone(void);
int finishedsw(void);
int winner(void);
int countstoneswift(int c);
int ai(void);
int returnplayer(void);
int nega_alpha(int depth, char playerrn, int alpha, int beta, bool passed);
void negaalphaTH(void);
void* negaalphat1(void* args);
void* negaalphat2(void* args);
void* negaalphat3(void* args);
void* negaalphat4(void* args);
int nega_alphadeep(int depth, char playerrn, int alpha, int beta, bool passed, char board[10][10]);

#endif /* othello_h */
