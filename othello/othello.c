#include "othello.h"

void reset(void)
{
    printf("[*]初期化中...\n");
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            board[i][j] = 0;
            canPut[i][j] = false;
            isfree[i][j] = true;
        }
    }
    board[4][4] = 2;
    board[4][5] = 1;
    board[5][4] = 1;
    board[5][5] = 2;
    isfree[4][4] = false;
    isfree[4][5] = false;
    isfree[5][4] = false;
    isfree[5][5] = false;
    //白が2,黒が1
    player = 1;
    skip = 0;
    return;
}
void check(void)
{
    for(int y = 1; y < 9; y++)
    {
        for(int x = 1; x < 9; x++)
        {
            if(player == 1)
            {
                if(board[y][x] == 1)
                {
                    for(int xx = -1; xx < 2; xx++)
                    {
                        for(int yy = -1; yy < 2; yy++)
                        {
                            if(board[y + yy][x + xx] == 2)
                            {
                                int xxx = x + xx;
                                int yyy = y + yy;
                                while(board[yyy][xxx] == 2)
                                {
                                    xxx += xx;
                                    yyy += yy;
                                }
                                if(board[yyy][xxx] == 0)
                                {
                                    canPut[yyy][xxx] = true;
                                }
                            }
                        }
                    }
                }
            }
            else if(player == 2)
            {
                if(board[y][x] == 2)
                {
                    for(int xx = -1; xx < 2; xx++)
                    {
                        for(int yy = -1; yy < 2; yy++)
                        {
                            if(board[y + yy][x + xx] == 1)
                            {
                                int xxx = x + xx;
                                int yyy = y + yy;
                                while(board[yyy][xxx] == 1)
                                {
                                    xxx += xx;
                                    yyy += yy;
                                }
                                if(board[yyy][xxx] == 0)
                                {
                                    canPut[yyy][xxx] = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for(int i = 0; i < 10; i++)
    {
        canPut[0][i] = false;
        canPut[9][i] = false;
    }
    for(int i = 1; i < 9; i++)
    {
        canPut[i][0] = false;
        canPut[i][9] = false;
    }
    
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if(isfree[i][j] == true)
            {
                board[i][j] = 0;
            }
        }
    }
    
    skip = 0;
    for(int i = 1; i < 9; i++)
    {
        for(int j = 1; j < 9; j++)
        {
            if (canPut[i][j] == true)
            {
                skip++;
                break;
            }
        }
    }
    
    if(skip == 0)
    {
        if(player == 1)
        {
            player = 2;
            printf("スキップ\n");
            skipped++;
            if(skipped > 60)
            {
                printf("終了\n");
                finished = 1;
                return;
            }
            check();
        }
        else if(player == 2)
        {
            player = 1;
            printf("スキップ\n");
            skipped++;
            if(skipped > 60)
            {
                printf("終了\n");
                finished = 1;
                return;
            }
            check();
        }
    }
}

void reverse(int x, int y)
{
    if(player == 1)
    {
        if(board[y][x] == 1)
        {
            for(int xx = -1; xx < 2; xx++)
            {
                for(int yy = -1; yy < 2; yy++)
                {
                    if(xx != 0 || yy != 0)
                    {
                        if(board[y + yy][x + xx] == 2)
                        {
                            int xxx = x + xx;
                            int yyy = y + yy;
                            while(board[yyy][xxx] == 2)
                            {
                                xxx += xx;
                                yyy += yy;
                            }
                            if(board[yyy][xxx] == 1)
                            {
                                xxx -= xx;
                                yyy -= yy;
                                while(board[yyy][xxx] == 2)
                                {
                                    board[yyy][xxx] = 1;
                                    yyy -= yy;
                                    xxx -= xx;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if(player == 2)
    {
        if(board[y][x] == 2)
        {
            for(int xx = -1; xx < 2; xx++)
            {
                for(int yy = -1; yy < 2; yy++)
                {
                    if(xx != 0 || yy != 0)
                    {
                        if(board[y + yy][x + xx] == 1)
                        {
                            int xxx = x + xx;
                            int yyy = y + yy;
                            while(board[yyy][xxx] == 1)
                            {
                                xxx += xx;
                                yyy += yy;
                            }
                            if(board[yyy][xxx] == 2)
                            {
                                xxx -= xx;
                                yyy -= yy;
                                while(board[yyy][xxx] == 1)
                                {
                                    board[yyy][xxx] = 2;
                                    yyy -= yy;
                                    xxx -= xx;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void rebuild(void)
{
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if(isfree[i][j] == true)
            {
                board[i][j] = 0;
            }
        }
    }
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            canPut[i][j] = false;
        }
    }
    countstone();
    check();
}

int putstone(int py, int px)
{
    printf("Player: %d\n", player);
    printf("(%d, %d)\n", px, py);
    if(px > 8 || py > 8)
    {
        printf("[*]そこには置けません\n");
        return 0;
    }
    if(canPut[py][px] == true)
    {
        if(player == 1)
        {
            board[py][px] = 1;
            isfree[py][px] = false;
            reverse(px, py);
            player = 2;
            return 1;
        }
        else if(player == 2)
        {
            board[py][px] = 2;
            isfree[py][px] = false;
            reverse(px, py);
            player = 1;
            return 2;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        printf("[*]そこには置けません\n");
        return 0;
    }
    
}

void countstone(void)
{
    blackc = 0;
    whitec = 0;
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if(board[i][j] == 1)
            {
                blackc++;
            }
            else if(board[i][j] == 2)
            {
                whitec++;
            }
        }
    }
}

int winner(void)
{
    if(blackc > whitec)
    {
        return 1;
    }
    else if(blackc < whitec)
    {
        return 2;
    }
    else if(blackc == whitec)
    {
        return 0;
    }
    else
    {
        return 0;
    }
}

int finishedsw(void)
{
    printf("黒: %d 白: %d\n", blackc, whitec);
    if (finished == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
