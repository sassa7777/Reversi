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
        }
    }
    board[4][4] = 2;
    board[4][5] = 1;
    board[5][4] = 1;
    board[5][5] = 2;
    //白が2,黒が1
    player = 1;
    skip = false;
    finished = 0;
    skipped = false;
    for(int x = 0; x < 10; x++)
    {
        for(int y = 0; y < 10; y++)
        {
            virtualboard[y][x] = board[y][x];
        }
    }
    return;
}

void check2(int Player)
{
    for(int y = 1; y < 9; y++)
    {
        for(int x = 1; x < 9; x++)
        {
            if(board[y][x] == Player)
            {
                for(int xx = -1; xx < 2; xx++)
                {
                    for(int yy = -1; yy < 2; yy++)
                    {
                        if(board[y + yy][x + xx] == (3-Player))
                        {
                            int xxx = x + xx;
                            int yyy = y + yy;
                            while(board[yyy][xxx] == (3-Player))
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
    for(int i = 0; i < 10; i++)
    {
        canPut[0][i] = false;
        canPut[9][i] = false;
        canPut[i][0] = false;
        canPut[i][9] = false;
    }
    
    skip = false;
    for(int i = 1; i < 9; i++)
    {
        for(int j = 1; j < 9; j++)
        {
            if (canPut[i][j] == true)
            {
                skipped = false;
                skip = true;
                return;
            }
        }
    }
    
    if(skipped == true)
    {
        printf("終了\n");
        finished = 1;
        return;
    }
    
    if(Player == 1)
    {
        printf("Player:%dをスキップ\n", player);
        skipped = true;
        player = 2;
        check2(player);
    }
    else if(Player == 2)
    {
        printf("Player:%dをスキップ\n", player);
        skipped = true;
        player = 1;
        check2(player);
    }
}

void check3(int player, bool canput[10][10])
{
    for(int y = 1; y < 9; y++)
    {
        for(int x = 1; x < 9; x++)
        {
            if(board[y][x] == player)
            {
                for(int xx = -1; xx < 2; xx++)
                {
                    for(int yy = -1; yy < 2; yy++)
                    {
                        if(board[y + yy][x + xx] == (3-player))
                        {
                            int xxx = x + xx;
                            int yyy = y + yy;
                            while(board[yyy][xxx] == (3-player))
                            {
                                xxx += xx;
                                yyy += yy;
                            }
                            if(board[yyy][xxx] == 0)
                            {
                                canput[yyy][xxx] = true;
                            }
                        }
                    }
                }
            }
        }
    }
    for(int i = 0; i < 10; i++)
    {
        canput[0][i] = false;
        canput[9][i] = false;
        canput[i][0] = false;
        canput[i][9] = false;
    }
    
    skip = false;
    for(int i = 1; i < 9; i++)
    {
        for(int j = 1; j < 9; j++)
        {
            if (canput[i][j] == true)
            {
                skip = true;
                return;
            }
        }
    }
}

void reverse(int x, int y)
{
    if(board[y][x] == player)
    {
        for(int xx = -1; xx < 2; xx++)
        {
            for(int yy = -1; yy < 2; yy++)
            {
                if(xx != 0 || yy != 0)
                {
                    if(board[y + yy][x + xx] == (3-player))
                    {
                        int xxx = x + xx;
                        int yyy = y + yy;
                        while(board[yyy][xxx] == (3-player))
                        {
                            xxx += xx;
                            yyy += yy;
                        }
                        if(board[yyy][xxx] == player)
                        {
                            xxx -= xx;
                            yyy -= yy;
                            while(board[yyy][xxx] == (3-player))
                            {
                                board[yyy][xxx] = player;
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

void rebuild(bool bot)
{
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            canPut[i][j] = false;
        }
    }
    countstone();
    if(bot == false) check2(player);
}

int putstone(int py, int px)
{
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
            reverse(px, py);
            player = 2;
            skip = false;
            return 1;
        }
        else if(player == 2)
        {
            board[py][px] = 2;
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

int putstone2(int py, int px, int player, bool canput[10][10])
{
    if(px > 9 || py > 9)
    {
        printf("[*]そこには置けません\n");
        return 0;
    }
    if(canput[py][px] == true)
    {
        if(player == 1)
        {
            board[py][px] = 1;
            reverse(px, py);
            skip = false;
            return 1;
        }
        else if(player == 2)
        {
            board[py][px] = 2;
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
    check2(player);
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
}

int ai2(void)
{
    if(player == 1 || finished == 1) return 0;
    isbot = true;
    printf("[*]Botが考え中..\n");
    minimax(DEPTH, player);
    putstone(tmpy, tmpx);
    check2(1);
    isbot = false;
    return 1;
}

int minimax(int depth, int playerrn)
{
    if(depth == 0)
    {
        return countscore(board);
    }
    
    if(putableto(playerrn) == false)
    {
        if(playerrn == 1) return 99999;
        if(playerrn == 2) return -99999;
    }
    
    int score = -99999;
    int var;
    int tmpboard[10][10];
    bool canput[10][10];
    for (int i = 0; i < 10; i++)
    {
        for (int j=0; j < 10; j++)
        {
            tmpboard[i][j] = board[i][j];
            canput[i][j] = false;
        }
    }
    check3(playerrn, canput);
    for (int i=1; i<9; i++)
    {
        for (int j=1; j<9; j++)
        {
            if(canput[i][j] == true)
            {
                putstone2(i,j, playerrn, canput);
                pthread_t thread;
                if(putableto(3-playerrn) == true)
                {
                    //var = minimax(depth-1, 3-playerrn);
                    int values[2] = {depth-1, 3-playerrn};
                    pthread_create(&thread, NULL, minimax_thread, (void*)&values);
                }
                else
                {
                    printf("cant put\n");
                    //var = minimax(depth-1, playerrn);
                    int values[2] = {depth-1, playerrn};
                    pthread_create(&thread, NULL, minimax_thread, (void*)&values);
                }
                int* threadResult;
                pthread_join(thread, (void**)&threadResult);
                var = *threadResult;
                free(threadResult);
                //printf("returned score is %d\n", var);
                if(playerrn == 2 && score <= var)
                {
                    score = var;
                    if(depth == DEPTH)
                    {
                        tmpx = j;
                        tmpy = i;
                        printf("best place is (%d, %d), score %d\n", j, i, var);
                    }
                }
                
                if(playerrn == 1 && score <= -var) score = -var;
                
                for (int y = 0; y < 10; y++)
                {
                    for (int x=0; x < 10; x++)
                    {
                        board[y][x] = tmpboard[y][x];
                    }
                }
            }
        }
    }
    if(playerrn == 2) return score;
    if(playerrn == 1) return -score;
    printf("ERROR_1\n");
    return 99999;
}

void* minimax_thread(void* args)
{
    int* result = (int*)malloc(sizeof(int));
    int* arg = (int*)args;
    int depth = arg[0];
    int playerrn = arg[1];
    *result = minimax(depth, playerrn);
    //printf("score is %d\n", *result);
    pthread_exit(result);
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
    int score = 0;
    for(int x = 1; x < 9; x++)
    {
        for(int y = 1; y < 9; y++)
        {
            if(board[x][y] == 2) score += scoreboard[x][y];
            if(board[x][y] == 1) score -= scoreboard[x][y];
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
    if(virtualboard[y][x] == player)
    {
        for(int xx = -1; xx < 2; xx++)
        {
            for(int yy = -1; yy < 2; yy++)
            {
                if(xx != 0 || yy != 0)
                {
                    if(virtualboard[y + yy][x + xx] == (3-player))
                    {
                        int xxx = x + xx;
                        int yyy = y + yy;
                        while(virtualboard[yyy][xxx] == (3-player))
                        {
                            xxx += xx;
                            yyy += yy;
                        }
                        if(virtualboard[yyy][xxx] == player)
                        {
                            xxx -= xx;
                            yyy -= yy;
                            while(virtualboard[yyy][xxx] == (3-player))
                            {
                                virtualboard[yyy][xxx] = player;
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

int returnplayer(void)
{
    return player;
}

bool putableto(int player)
{
    int canput[10][10];
    for(int y = 1; y < 9; y++)
    {
        for(int x = 1; x < 9; x++)
        {
            if(board[y][x] == player)
            {
                for(int xx = -1; xx < 2; xx++)
                {
                    for(int yy = -1; yy < 2; yy++)
                    {
                        if(board[y + yy][x + xx] == (3-player))
                        {
                            int xxx = x + xx;
                            int yyy = y + yy;
                            while(board[yyy][xxx] == (3-player))
                            {
                                xxx += xx;
                                yyy += yy;
                            }
                            if(board[yyy][xxx] == 0)
                            {
                                canput[yyy][xxx] = true;
                            }
                        }
                    }
                }
            }
        }
    }
    for(int i = 0; i < 10; i++)
    {
        canput[0][i] = false;
        canput[9][i] = false;
        canput[i][0] = false;
        canput[i][9] = false;
    }
    
    for(int i = 1; i < 9; i++)
    {
        for(int j = 1; j < 9; j++)
        {
            if (canput[i][j] == true)
            {
                return true;
            }
        }
    }
    return false;
}
