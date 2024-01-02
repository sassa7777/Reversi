//
//  othello.h
//  othello
//
//  Created by sasa on 2023/09/30.
//

#ifndef othello_h
#define othello_h
#define DEPTH 7

#include <stdio.h>
#include <stdbool.h>

int board[10][10];
int virtualboard[10][10];
bool canPut[10][10];
bool isfree[10][10];
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
        0, 5, -3, 0, -1, -1, 0, -3, 5, 0,
        0, -3, -8, -3, -3, -3, -3, -8, -3, 0,
        0, 0, -3, 0, 0, 0, 0, -3, 0, 0,
        0, -1, -3, 0, -1, -1, 0, -3, -1, 0,
        0, -1, -3, 0, -1, -1, 0, -3, -1, 0,
        0, 0, -3, 0, 0, 0, 0, -3, 0, 0,
        0, -3, -8, -3, -3, -3, -3, -8, -3, 0,
        0, 5, -3, 0, -1, -1, 0, -3, 5, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int scoreboard2[10][10] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 35, -15, 0, -1, -1, 0, -15, 35, 0,
    0, -15, -18, -3, -3, -3, -3, -18, -15, 0,
    0, 0, -3, 0, -1, -1, 0, -3, 0, 0,
    0, -1, -3, -1, 3, 3, -1, -3, -1, 0,
    0, -1, -3, -1, 3, 3, -1, -3, -1, 0,
    0, 0, -3, 0, -1, -1, 0, -3, 0, 0,
    0, -15, -18, -3, -3, -3, -3, -18, -15, 0,
    0, 35, -15, 0, -1, -1, 0, -15, 35, 0,
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
void ai(void);
int ai2(void);
void virtualput(int px, int py);
void virtualreverse(int x, int y);
int countscore(int board[10][10]);
void rebuild_virtual(void);
int returnplayer(void);
int minimax(int depth, int playerrn);

#endif /* othello_h */
