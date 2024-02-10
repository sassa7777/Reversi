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

char board[10][10];
bool canPut[10][10];
char threadboard[4][10][10];
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

void reset(void);
void check3(int player, bool canput[10][10]);
void check2(int player);
void check4(int player, bool canput[10][10], char board[10][10]);
bool putableto(int player);
bool putableto2(int player, char board[10][10]);
int putstone(int px, int py);
int putstone2(int py, int px, int player, bool canput[10][10]);
int putstone3(int py, int px, int player, bool canput[10][10], char board[10][10]);
void reverse(int px, int py);
void reverse2(int x, int y, char board[10][10]);
void rebuild(bool bot);
void countstone(void);
int finishedsw(void);
int winner(void);
int countstoneswift(int c);
int ai2(bool multi);
int returnplayer(void);
int minimax(int depth, int playerrn);
int nega_alpha(int depth, int playerrn, int alpha, int beta, int turn);
void nega_alpha_thread(int depth, int playerrn, int ALPHA, int BETA, int turn);
void *nega_alpha_thread1(void *args);
void *nega_alpha_thread2(void *args);
void *nega_alpha_thread3(void *args);
void *nega_alpha_thread4(void *args);
int nega_alpha_deepthread(int depth, int playerrn, int alpha, int beta, int turn, char board[10][10]);

#endif /* othello_h */
