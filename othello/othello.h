//
//  othello.h
//  othello
//
//  Created by sasa on 2023/09/30.
//

#ifndef othello_h
#define othello_h
#define DEPTH 2

#include <stdio.h>
#include <stdbool.h>

int board[10][10];
int virtualboard[10][10];
bool canPut[10][10];
bool isfree[10][10];
int finished = 0;
int skipped = 0;
bool skip = false;
int px, py;
int player;
int turn = 0;
int whitec = 0;
int blackc = 0;
int score = 0;
int tmpboard[6][8][8];
int tmpx, tmpy;
int scoreboard[8][8] = {
        45, 3, 15, 14, 14, 15, 3, 45,
        3, 0, 12, 12, 12, 12, 0, 3,
        15,12, 15, 14, 14, 15, 12, 15,
        14, 12, 14, 14, 14, 14, 12, 14,
        14, 12, 14, 14, 14, 14, 12, 14,
        15, 12, 15, 14, 14, 15, 12, 15,
        3, 0, 12, 12, 12, 12, 0, 3,
        45, 3, 15, 14, 14, 15, 3, 45
};
int scoreboard2[8][8] = {
    30, -12, 0, -1, -1, 0, -12, 30,
    -12, -15, -3, -3, -3, -3, -15, -12,
    0, -3, 0, -1, -1, 0, -3, 0,
    -1, -3, -1, 3, 3, -1, -3, -1,
    -1, -3, -1, 3, 3, -1, -3, -1,
    0, -3, 0, -1, -1, 0, -3, 0,
    -12, -15, -3, -3, -3, -3, -15, -12,
    30, -12, 0, -1, -1, 0, -12, 30
};
void reset(void);
void check(void);
int putstone(int px, int py);
void reverse(int px, int py);
void rebuild(void);
void countstone(void);
int finishedsw(void);
int winner(void);
int countstoneswift(int c);
void ai(void);
void ai2(void);
void virtualput(int px, int py);
void virtualreverse(int x, int y);
int countscore(int board[10][10]);
void rebuild_virtual(void);
int returnplayer(void);
int minimax(int depth);

#endif /* othello_h */
