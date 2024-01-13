#include <stdbool.h>
#include <stdio.h>
#include "othello.h"

int scoreboard[8][8] = {
    30, -12, 0, -1, -1, 0, -12, 30,
       -12, -15, -3, -3, -3, -3, -15, -12,
       0, -3, 0, -1, -1, 0, -3, 0,
       -1, -3, -1, -1, -1, -1, -3, -1,
       -1, -3, -1, -1, -1, -1, -3, -1,
       0, -3, 0, -1, -1, 0, -3, 0,
       -12, -15, -3, -3, -3, -3, -15, -12,
       30, -12, 0, -1, -1, 0, -12, 30
};
int scoreboard2[8][8] = {
    35, -8, 2, 0, 0, 2, -8, 35,
    -8, -15, -3, -3, -3, -3, -15, -8,
    2, -3, 1, -1, -1, 1, -3, 2,
    0, -3, -1, 2, 2, -1, -3, 0,
    0, -3, -1, 2, 2, -1, -3, 0,
    2, -3, 1, -1, -1, 1, -3, 2,
    -8, -15, -3, -3, -3, -3, -15, -8,
    35, -8, 2, 0, 0, 2, -8, 35
};

int score_wing(char board[10][10])
{
    return 0;
}

int score_putable(char board[10][10], bool canput[10][10])
{
    int score = 0;
    for(int i = 1; i < 9; ++i)
    {
        #pragma clang loop vectorize(enable)
        for(int j = 1; j < 9; ++j)
        {
            if(canput[i][j] == true)
            {
                score+=1;
            }
        }
    }
    score/=2;
    return score;
}

int score_stone(char board[10][10])
{
    int score = 0;
    for(int x = 1; x < 9; ++x)
    {
        #pragma clang loop vectorize(enable)
        for(int y = 1; y < 9; y++)
        {
            if(board[x][y] == 2) score += scoreboard2[x-1][y-1];
            if(board[x][y] == 1) score -= scoreboard2[x-1][y-1];
        }
    }
    return score;
}

int score_countstone(char board[10][10])
{
    int score = 0;
    for(int i = 1; i < 9; ++i)
    {
        #pragma clang loop vectorize(enable)
        for(int j = 1; j < 9; ++j)
        {
            if(board[i][j] == 1)
            {
                score-=2;
            }
            else if(board[i][j] == 2)
            {
                score+=2;
            }
        }
    }
    return score;
}

int countscore(char board[10][10], int turn, bool canput[10][10])
{
    int score = 0;
    if(turn < 35)
    {
        score += score_putable(board, canput);
        
    }
    if(turn < 50)
    {
        score += score_stone(board);
    }
    
    if(turn > 43)
    {
        score += score_countstone(board);
    }
    return score;
}
