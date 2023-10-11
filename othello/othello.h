//
//  othello.h
//  othello
//
//  Created by sasa on 2023/09/30.
//

#ifndef othello_h
#define othello_h

#include <stdio.h>
#include <stdbool.h>

int board[10][10];
int virtualboard[10][10];
int virtualboard2[10][10];
bool canPut[10][10];
bool isfree[10][10];
int finished = 0;
int skipped = 0;
int skip = 0;
int px, py;
int player;
int player_bot;
int turn = 0;
int whitec = 0;
int blackc = 0;
int score = 0;
bool bot = false;
int scoreboard[10][10] = {
    -100, -100, -100, -100, -100, -100, -100, -100, -100, -100,
    -100, 30, -12, 0, -1, -1, 0, -12, 30, -100,
        -100, -12, -15, -3, -3, -3, -3, -15, -12, -100,
        -100, 0, -3, 0, -1, -1, 0, -3, 0, -100,
       -100, -1, -3, -1, -1, -1, -1, -3, -1, -100,
        -100, -1, -3, -1, -1, -1, -1, -3, -1, -100,
        -100, 0, -3, 0, -1, -1, 0, -3, 0, -100,
        -100, -12, -15, -3, -3, -3, -3, -15, -12, -100,
        -100, 30, -12, 0, -1, -1, 0, -12, 30, -100,
    -100, -100, -100, -100, -100, -100, -100, -100, -100, -100
};

bool skipb = false;
void reset(void);
void read_lev(void);
int eval(void);
void check(void);
void check2(void);
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
int countscore(void);
void rebuild_virtual(void);
void checkbot(void);
void reset_virtualboard(void);
int minimax(int depth, int playerrn, int playeraf);
int skipsw(void);
#endif /* othello_h */
