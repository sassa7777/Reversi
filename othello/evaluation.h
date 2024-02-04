//
//  evaluation.c
//  othello
//
//  Created by sasa on 2024/02/02.
//

#ifndef evaluation_h
#define evaluation_h

#include <stdbool.h>

int scoreboard[8][8] = {
    35, -12, 1, -1, -1, 1, -12, 35,
       -12, -15, -3, -3, -3, -3, -15, -12,
       1, -3, 0, -1, -1, 0, -3, 1,
       -1, -3, -1, -1, -1, -1, -3, -1,
       -1, -3, -1, -1, -1, -1, -3, -1,
       1, -3, 0, -1, -1, 0, -3, 1,
       -12, -15, -3, -3, -3, -3, -15, -12,
       35, -12, 1, -1, -1, 1, -12, 35
};

int score_wing(char board[10][10]);
int score_mountain(char board[10][10]);
int score_pureblock(char board[10][10]);
int score_purewing(char board[10][10]);
int score_halfblock(char board[10][10]);
int score_putable(char board[10][10], bool canput[10][10]);
int score_stone(char board[10][10]);
int score_countstone(char board[10][10]);
int score_fixedstone(char board[10][10]);
int score_badcorner(char board[10][10]);
int score_goodcorner(char board[10][10]);
int countscore(char board[10][10], int turn, bool canput[10][10]);
int score_badside(char board[10][10]);

#endif /* evaluation_h */
