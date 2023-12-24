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
    skip = false;
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            virtualboard[y][x] = board[y][x];
        }
    }
    return;
}
void check(void)
{
    for(int y = 1; y < 9; y++)
    {
        for(int x = 1; x < 9; x++)
        {
            if(player == 1 && board[y][x] == 1)
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
    
    skip = false;
    for(int i = 1; i < 9; i++)
    {
        for(int j = 1; j < 9; j++)
        {
            if (canPut[i][j] == true)
            {
                skip = true;
                return;
            }
        }
    }
    
    if(skip == false)
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

void check2(int player)
{
    for(int y = 1; y < 9; y++)
    {
        for(int x = 1; x < 9; x++)
        {
            if(player == 1 && board[y][x] == 1)
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
    
    skip = false;
    for(int i = 1; i < 9; i++)
    {
        for(int j = 1; j < 9; j++)
        {
            if (canPut[i][j] == true)
            {
                skip = true;
                return;
            }
        }
    }
    
    if(skip == false)
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
            check2(player);
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
            check2(player);
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
            skip = false;
            return 1;
        }
        else if(player == 2)
        {
            board[py][px] = 2;
            isfree[py][px] = false;
            reverse(px, py);
            player = 1;
            skip = false;
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

int putstone2(int py, int px, int player)
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
            skip = false;
            return 1;
        }
        else if(player == 2)
        {
            board[py][px] = 2;
            isfree[py][px] = false;
            reverse(px, py);
            skip = false;
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

int countstoneswift(int c)
{
    if(c == 1)
    {
        return blackc;
    }
    else if(c == 2)
    {
        return whitec;
    }
    else
    {
        return 0;
    }
}

void ai(void)
{
    int tmpscore = -9999;
    int tmpx = 0, tmpy = 0;
    check();
    turn++;
    for(int y = 1; y < 9; y++)
    {
        for(int x = 1; x < 9; x++)
        {
            if(canPut[y][x] == true)
            {
                virtualput(x, y);
                printf("%d\n", countscore(virtualboard));
                if(turn <= 5)
                {
                    if((countscore(virtualboard) - (whitec)) > tmpscore)
                    {
                        tmpx = x; tmpy = y;
                        tmpscore = countscore(virtualboard);
                    }
                }
                else
                {
                    if(countscore(virtualboard) > tmpscore)
                    {
                        tmpx = x; tmpy = y;
                        tmpscore = countscore(virtualboard);
                    }
                }
                rebuild_virtual();
            }
        }
    }
    putstone(tmpy, tmpx);
    printf("sroreL: %d\n", scoreboard[2][2]);
}

void ai2(void)
{
    isbot = true;
    minimax(DEPTH, player);
    player = 2;
    putstone(tmpy, tmpx);
    isbot = false;
}

int minimax(int depth, int playerrn)
{
    if(depth == 0) return countscore(board);
    
    int score = -99999;
    int var;
    
    for (int i = 0; i < 8; i++)
    {
        for (int j=0; j < 8; j++)
        {
            tmpboard[depth][i][j] = board[i+1][j+1];
        }
    }
    check2(playerrn);
    //printf("checked %d\n", playerrn);
    for (int i=1; i<9; i++)
    {
        for (int j=1; j<9; j++)
        {
            if(canPut[i][j] == true)
            {
                printf("putable: (%d, %d)\n", j, i);
                putstone2(i,j, playerrn);
                
                if(playerrn == 2)
                {
                    var = -minimax(depth-1, 1);
                }
                else
                {
                    var = minimax(depth-1, 2);
                }
                
                if(score <= var)
                {
                    score = var;
                    if(depth == DEPTH)
                    {
                        tmpx = j;
                        tmpy = i;
                    }
                }
                
                for (int y = 0; y < 8; y++)
                {
                    for (int x=0; x < 8; x++)
                    {
                        board[y+1][x+1] = tmpboard[depth][y][x];
                    }
                }
            }
        }
    }
    return 0;
}

void virtualput(int px, int py)
{
    printf("Player: bot %d\n", player);
    printf("(%d, %d)\n", px, py);
    if(player == 1)
    {
        virtualboard[py][px] = 1;
        virtualreverse(py, px);
    }
    else if(player == 2)
    {
        virtualboard[py][px] = 2;
        virtualreverse(py, px);
    }
    else
    {
        printf("[*]そこには置けません\n");
    }
    return;
}

int countscore(int board[10][10])
{
    score = 0;
    for(int x = 1; x < 9; x++)
    {
        for(int y = 1; y < 9; y++)
        {
            if(board[x][y] == 2)
            {
                score += scoreboard[x-1][y-1];
            }
        }
    }
    return score;
}

void rebuild_virtual(void)
{
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            virtualboard[y][x] = board[y][x];
        }
    }
}

void virtualreverse(int y, int x)
{
    if(player == 1)
    {
        if(virtualboard[y][x] == 1)
        {
            for(int xx = -1; xx < 2; xx++)
            {
                for(int yy = -1; yy < 2; yy++)
                {
                    if(xx != 0 || yy != 0)
                    {
                        if(virtualboard[y + yy][x + xx] == 2)
                        {
                            int xxx = x + xx;
                            int yyy = y + yy;
                            while(virtualboard[yyy][xxx] == 2)
                            {
                                xxx += xx;
                                yyy += yy;
                            }
                            if(virtualboard[yyy][xxx] == 1)
                            {
                                xxx -= xx;
                                yyy -= yy;
                                while(virtualboard[yyy][xxx] == 2)
                                {
                                    virtualboard[yyy][xxx] = 1;
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
        if(virtualboard[y][x] == 2)
        {
            for(int xx = -1; xx < 2; xx++)
            {
                for(int yy = -1; yy < 2; yy++)
                {
                    if(xx != 0 || yy != 0)
                    {
                        if(virtualboard[y + yy][x + xx] == 1)
                        {
                            int xxx = x + xx;
                            int yyy = y + yy;
                            while(virtualboard[yyy][xxx] == 1)
                            {
                                xxx += xx;
                                yyy += yy;
                            }
                            if(virtualboard[yyy][xxx] == 2)
                            {
                                xxx -= xx;
                                yyy -= yy;
                                while(virtualboard[yyy][xxx] == 1)
                                {
                                    virtualboard[yyy][xxx] = 2;
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

int returnplayer(void)
{
    return player;
}
