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
bool canPut[10][10];
bool isfree[10][10];
int finished = 0;
int skipped = 0;
int skip = 0;
int px, py;
int player;
int turn = 0;
int whitec = 0;
int blackc = 0;
void reset(void);
void check(void);
int putstone(int px, int py);
void reverse(int px, int py);
void rebuild(void);
void countstone(void);
int finishedsw(void);
int winner(void);

#endif /* othello_h */
