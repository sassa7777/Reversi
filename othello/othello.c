#include "othello.h"
#include "evaluation.h"

void reset(void)
{
    printf("[*]初期化中...\n");
    memset(board, 0, sizeof(board));
    memset(canPut, 0, sizeof(canPut));
    board[4][4] = 2;
    board[4][5] = 1;
    board[5][4] = 1;
    board[5][5] = 2;
    //白が2,黒が1
    player = 1;
    skip = false;
    finished = 0;
    skipped = false;
    turn = 1;
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
    #pragma clang loop vectorize(enable)
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
        #pragma clang loop vectorize(enable)
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
    #pragma clang loop vectorize(enable)
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
        #pragma clang loop vectorize(enable)
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
            #pragma clang loop vectorize(enable)
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
            turn++;
            skip = false;
            return 1;
        }
        else if(player == 2)
        {
            board[py][px] = 2;
            reverse(px, py);
            player = 1;
            turn++;
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

int ai2(bool multi)
{
    if(player == 1 || finished == 1) return 0;
    isbot = true;
    printf("[*]Botが考え中..\n");
    if(multi == true)
    {
        printf("Warning: using pthread\n");
        //minimax_multi(DEPTH, player);
    }
    //else minimax(DEPTH, player);
    else alphabeta(DEPTH, player, -9999, 9999, turn);
    putstone(tmpy, tmpx);
    check2(1);
    isbot = false;
    return 1;
}

int alphabeta(int depth, int playerrn, int alpha, int beta, int turn)
{
    if(depth == 0)
    {
        bool canput[10][10];
        memset(canput, 0, sizeof(canput));

        check3(3-playerrn, canput);
        return countscore(board, turn, canput);
    }
    
    int var;
    char tmpboard[10][10];
    bool canput[10][10];
    
    memcpy(tmpboard, board, sizeof(board));
    memset(canput, 0, sizeof(canput));
    
    check3(playerrn, canput);
    for (int i=1; i<9; i++)
    {
        for (int j=1; j<9; j++)
        {
            if(canput[i][j] == true)
            {
                putstone2(i,j, playerrn, canput);
                
                if(putableto(3-playerrn) == true)
                {
                    var = alphabeta(depth-1, 3-playerrn, alpha, beta, turn+1);
                }
                else
                {
                    printf("cant put\n");
                    var = alphabeta(depth-1, playerrn, alpha, beta, turn+1);
                }
                
                if(playerrn == 2 && alpha < var)
                {
                    alpha = var;
                    if(depth == DEPTH)
                    {
                        tmpx = j;
                        tmpy = i;
                        printf("best place is (%d, %d), score %d\n", j, i, var);
                    }
                }
                
                if(playerrn == 1 && beta > var) beta = var;
                memcpy(board, tmpboard, sizeof(board));
                if(alpha >= beta)
                {
                    if(playerrn == 2) return alpha;
                    if(playerrn == 1) return beta;
                }
            }
        }
    }
    if(alpha == -9999)
    {
        check3(3-playerrn, canput);
        alpha = countscore(board, turn, canput);
    }
    if(beta == 9999)
    {
        check3(3-playerrn, canput);
        beta = countscore(board, turn, canput);
    }
    if(playerrn == 2) return alpha;
    if(playerrn == 1) return beta;
    printf("ERROR_1\n");
    return 99999;
}

int returnplayer(void)
{
    return player;
}

bool putableto(int player)
{
    bool canput[10][10];
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


