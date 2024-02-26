//
//  othello.h
//  othello
//
//  Created by sasa on 2023/09/30.
//

#ifndef othello_h
#define othello_h

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

char board[10][10];
char threadboard[4][10][10];
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
int cachex[4], cachey[4];
bool isbot = false;
int result[4];
int DEPTH;

char moveorder[2][64] = {
    {1,1,8,8,1,1,3,3,3,3,6,6,6,6,8,8,1,1,3,3,4,4,4,4,4,4,5,5,5,5,5,5,6,6,8,8,2,2,2,2,3,3,4,4,5,5,6,6,7,7,7,7,1,1,2,2,7,7,8,8,2,2,7,7},
    {1,8,1,8,3,6,1,3,6,8,1,3,6,8,3,6,4,5,4,5,1,3,4,5,6,8,1,3,4,5,6,8,4,5,4,5,3,4,5,6,2,7,2,7,2,7,2,7,3,4,5,6,2,7,1,8,1,8,2,7,2,7,2,7}
};

void reset(void);
void check3(char *player, bool canput[10][10]);
void check2(int *player);
void check4(char *player, bool canput[10][10], char board[10][10]);
bool putableto(char *player);
bool putableto2(char *player, char board[10][10]);
int putstone(int px, int py);
int putstone2(char *py, char *px, char *player, bool canput[10][10]);
int putstone3(char *py, char *px, char *player, bool canput[10][10], char board[10][10]);
void reverse1(char **px, char **py);
void reverse(int x, int y);
void reverse2(char **x, char **y, char board[10][10]);
void rebuild(bool bot);
void countstone(void);
int finishedsw(void);
int winner(void);
int countstoneswift(int c);
int ai2(bool multi);
int returnplayer(void);
int nega_alpha(int depth, char playerrn, int alpha, int beta, int turn);
int nega_alpha_deep(int depth, char playerrn, int alpha, int beta, int turn, char board[10][10]);
void nega_alpha1(int depth, char playerrn, int alpha, int beta, int turn, char board[10][10]);
void nega_alpha2(int depth, char playerrn, int alpha, int beta, int turn, char board[10][10]);
void nega_alpha3(int depth, char playerrn, int alpha, int beta, int turn, char board[10][10]);
void nega_alpha4(int depth, char playerrn, int alpha, int beta, int turn, char board[10][10]);
void negaalphathread(int depth, char playerrn, int alpha, int beta, int turn);

#endif /* othello_h */
