#include <stdbool.h>

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


int countscore(char board[10][10], int turncount, bool canput[10][10])
{
    int score = 0;
    for(int x = 1; x < 9; x++)
    {
        for(int y = 1; y < 9; y++)
        {
            if(board[x][y] == 2) score += scoreboard2[x-1][y-1];
            if(board[x][y] == 1) score -= scoreboard2[x-1][y-1];
        }
    }
//    for (int i = 1; i <= 8; i+=7)
//    {
//        if (board[i][3] == 1 && board[i][4] == 1 && board[i][5] == 1 && board[i][6] == 1) score -= 10;
//        if (board[i][3] == 2 && board[i][4] == 2 && board[i][5] == 2 && board[i][6] == 2) score += 10;
//        if (board[3][i] == 1 && board[4][i] == 1 && board[5][i] == 1 && board[6][i] == 1) score -= 10;
//        if (board[3][i] == 2 && board[4][i] == 2 && board[5][i] == 2 && board[6][i] == 2) score += 10;
//    }
    if(turncount > 45)
    {
        int score1 = 0;
        for(int i = 1; i < 9; i++)
        {
            for(int j = 1; j < 9; j++)
            {
                if(canput[i][j] == true)
                {
                    score1--;
                }
            }
        }
        score += (score1/2);
        
    }
    for(int i = 1; i < 9; i++)
    {
        for(int j = 1; j < 9; j++)
        {
            if(board[i][j] == 1)
            {
                score--;
            }
            else if(board[i][j] == 2)
            {
                score++;
            }
        }
    }
    return score;
}
