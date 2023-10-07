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
bool finished = false;
bool skipped = false;
int skip = 1;
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
extern void finish(void);
void countstone(void);

#endif /* othello_h */
