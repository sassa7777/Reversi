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
                int xx, yy;
                if(board[y][x] == 1)
                {
                    if(board[y - 1][x - 1] == 2)
                    {
                        xx = x - 1;
                        yy = y - 1;
                        while(board[yy][xx] == 2)
                        {
                            xx -= 1;
                            yy -= 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y - 1][x] == 2)
                    {
                        xx = x;
                        yy = y - 1;
                        while(board[yy][xx] == 2)
                        {
                            yy -= 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y][x - 1] == 2)
                    {
                        xx = x - 1;
                        yy = y;
                        while(board[yy][xx] == 2)
                        {
                            xx -= 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y - 1][x + 1] == 2)
                    {
                        xx = x + 1;
                        yy = y - 1;
                        while(board[yy][xx] == 2)
                        {
                            xx += 1;
                            yy -= 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y][x + 1] == 2)
                    {
                        xx = x + 1;
                        yy = y;
                        while(board[yy][xx] == 2)
                        {
                            xx += 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y + 1][x + 1] == 2)
                    {
                        xx = x + 1;
                        yy = y + 1;
                        while(board[yy][xx] == 2)
                        {
                            xx += 1;
                            yy += 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y + 1][x] == 2)
                    {
                        xx = x;
                        yy = y + 1;
                        while(board[yy][xx] == 2)
                        {
                            yy += 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y + 1][x - 1] == 2)
                    {
                        xx = x - 1;
                        yy = y + 1;
                        while(board[yy][xx] == 2)
                        {
                            xx -= 1;
                            yy += 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                }
            }
            else if(player == 2)
            {
                int xx, yy;
                if(board[y][x] == 2)
                {
                    
                    if(board[y - 1][x - 1] == 1)
                    {
                        xx = x - 1;
                        yy = y - 1;
                        while(board[yy][xx] == 1)
                        {
                            xx -= 1;
                            yy -= 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y - 1][x] == 1)
                    {
                        xx = x;
                        yy = y - 1;
                        while(board[yy][xx] == 1)
                        {
                            yy -= 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y][x - 1] == 1)
                    {
                        xx = x - 1;
                        yy = y;
                        while(board[yy][xx] == 1)
                        {
                            xx -= 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y - 1][x + 1] == 1)
                    {
                        xx = x + 1;
                        yy = y - 1;
                        while(board[yy][xx] == 1)
                        {
                            xx += 1;
                            yy -= 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y][x + 1] == 1)
                    {
                        xx = x + 1;
                        yy = y;
                        while(board[yy][xx] == 1)
                        {
                            xx += 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y + 1][x + 1] == 1)
                    {
                        xx = x + 1;
                        yy = y + 1;
                        while(board[yy][xx] == 1)
                        {
                            xx += 1;
                            yy += 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y + 1][x] == 1)
                    {
                        xx = x;
                        yy = y + 1;
                        while(board[yy][xx] == 1)
                        {
                            yy += 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
                        }
                    }
                    if(board[y + 1][x - 1] == 1)
                    {
                        xx = x - 1;
                        yy = y + 1;
                        while(board[yy][xx] == 1)
                        {
                            xx -= 1;
                            yy += 1;
                        }
                        if(board[yy][xx] == 0)
                        {
                            canPut[yy][xx] = true;
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
            //            if(j == 9)
            //            {
            //                if(canPut[i][j] == true)
            //                {
            //                    printf("t\n");
            //                }
            //                else
            //                {
            //                    printf("f\n");
            //                }
            //            }
            //            else
            //            {
            //                if(canPut[i][j] == true)
            //                {
            //                    printf("t ");
            //                }
            //                else
            //                {
            //                    printf("f ");
            //                }
            //            }
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
    //    int x, y;
    //    x = px;
    //    y = py;
    if(player == 1)
    {
        int xx, yy;
        if(board[y][x] == 1)
        {
            {
                if(board[y - 1][x - 1] == 2)
                {
                    xx = x - 1;
                    yy = y - 1;
                    while(board[yy][xx] == 2)
                    {
                        xx -= 1;
                        yy -= 1;
                    }
                    if(board[yy][xx] == 1)
                    {
                        xx += 1;
                        yy += 1;
                        while(board[yy][xx] == 2)
                        {
                            board[yy][xx] = 1;
                            xx++;
                            yy++;
                        }
                    }
                }
                if(board[y - 1][x] == 2)
                {
                    xx = x;
                    yy = y - 1;
                    while(board[yy][xx] == 2)
                    {
                        yy -= 1;
                    }
                    if(board[yy][xx] == 1)
                    {
                        yy += 1;
                        while(board[yy][xx] == 2)
                        {
                            board[yy][xx] = 1;
                            yy++;
                        }
                    }
                }
                if(board[y][x - 1] == 2)
                {
                    xx = x - 1;
                    yy = y;
                    while(board[yy][xx] == 2)
                    {
                        xx -= 1;
                    }
                    if(board[yy][xx] == 1)
                    {
                        xx += 1;
                        while(board[yy][xx] == 2)
                        {
                            board[yy][xx] = 1;
                            xx++;
                        }
                    }
                }
                if(board[y - 1][x + 1] == 2)
                {
                    xx = x + 1;
                    yy = y - 1;
                    while(board[yy][xx] == 2)
                    {
                        xx += 1;
                        yy -= 1;
                    }
                    if(board[yy][xx] == 1)
                    {
                        xx -= 1;
                        yy += 1;
                        while(board[yy][xx] == 2)
                        {
                            board[yy][xx] = 1;
                            xx--;
                            yy++;
                        }
                    }
                }
                if(board[y][x + 1] == 2)
                {
                    xx = x + 1;
                    yy = y;
                    while(board[yy][xx] == 2)
                    {
                        xx += 1;
                    }
                    if(board[yy][xx] == 1)
                    {
                        xx -= 1;
                        while(board[yy][xx] == 2)
                        {
                            board[yy][xx] = 1;
                            xx--;
                        }
                    }
                }
                if(board[y + 1][x + 1] == 2)
                {
                    xx = x + 1;
                    yy = y + 1;
                    while(board[yy][xx] == 2)
                    {
                        xx += 1;
                        yy += 1;
                    }
                    if(board[yy][xx] == 1)
                    {
                        xx -= 1;
                        yy -= 1;
                        while(board[yy][xx] == 2)
                        {
                            board[yy][xx] = 1;
                            xx--;
                            yy--;
                        }
                    }
                }
                if(board[y + 1][x] == 2)
                {
                    xx = x;
                    yy = y + 1;
                    while(board[yy][xx] == 2)
                    {
                        yy += 1;
                    }
                    if(board[yy][xx] == 1)
                    {
                        yy -= 1;
                        while(board[yy][xx] == 2)
                        {
                            board[yy][xx] = 1;
                            yy--;
                        }
                    }
                }
                if(board[y + 1][x - 1] == 2)
                {
                    xx = x - 1;
                    yy = y + 1;
                    while(board[yy][xx] == 2)
                    {
                        xx -= 1;
                        yy += 1;
                    }
                    if(board[yy][xx] == 1)
                    {
                        xx += 1;
                        yy -= 1;
                        while(board[yy][xx] == 2)
                        {
                            board[yy][xx] = 1;
                            xx++;
                            yy--;
                        }
                    }
                }
            }
        }
    }
    else if(player == 2)
    {
        int xx, yy;
        if(board[y][x] == 2)
        {
            
            if(board[y - 1][x - 1] == 1)
            {
                xx = x - 1;
                yy = y - 1;
                while(board[yy][xx] == 1)
                {
                    xx -= 1;
                    yy -= 1;
                }
                if(board[yy][xx] == 2)
                {
                    xx += 1;
                    yy += 1;
                    while(board[yy][xx] == 1)
                    {
                        board[yy][xx] = 2;
                        xx++;
                        yy++;
                    }
                }
            }
            if(board[y - 1][x] == 1)
            {
                xx = x;
                yy = y - 1;
                while(board[yy][xx] == 1)
                {
                    yy -= 1;
                }
                if(board[yy][xx] == 2)
                {
                    yy += 1;
                    while(board[yy][xx] == 1)
                    {
                        board[yy][xx] = 2;
                        yy++;
                    }
                }
            }
            if(board[y][x - 1] == 1)
            {
                xx = x - 1;
                yy = y;
                while(board[yy][xx] == 1)
                {
                    xx -= 1;
                }
                if(board[yy][xx] == 2)
                {
                    xx += 1;
                    while(board[yy][xx] == 1)
                    {
                        board[yy][xx] = 2;
                        xx++;
                    }
                }
            }
            if(board[y - 1][x + 1] == 1)
            {
                xx = x + 1;
                yy = y - 1;
                while(board[yy][xx] == 1)
                {
                    xx += 1;
                    yy -= 1;
                }
                if(board[yy][xx] == 2)
                {
                    xx -= 1;
                    yy += 1;
                    while(board[yy][xx] == 1)
                    {
                        board[yy][xx] = 2;
                        xx--;
                        yy++;
                    }
                }
            }
            if(board[y][x + 1] == 1)
            {
                xx = x + 1;
                yy = y;
                while(board[yy][xx] == 1)
                {
                    xx += 1;
                }
                if(board[yy][xx] == 2)
                {
                    xx -= 1;
                    while(board[yy][xx] == 1)
                    {
                        board[yy][xx] = 2;
                        xx--;
                    }
                }
            }
            if(board[y + 1][x + 1] == 1)
            {
                xx = x + 1;
                yy = y + 1;
                while(board[yy][xx] == 1)
                {
                    xx += 1;
                    yy += 1;
                }
                if(board[yy][xx] == 2)
                {
                    xx -= 1;
                    yy -= 1;
                    while(board[yy][xx] == 1)
                    {
                        board[yy][xx] = 2;
                        xx--;
                        yy--;
                    }
                }
            }
            if(board[y + 1][x] == 1)
            {
                xx = x;
                yy = y + 1;
                while(board[yy][xx] == 1)
                {
                    yy += 1;
                }
                if(board[yy][xx] == 2)
                {
                    yy -= 1;
                    while(board[yy][xx] == 1)
                    {
                        board[yy][xx] = 2;
                        yy--;
                    }
                }
            }
            if(board[y + 1][x - 1] == 1)
            {
                xx = x - 1;
                yy = y + 1;
                while(board[yy][xx] == 1)
                {
                    xx -= 1;
                    yy += 1;
                }
                if(board[yy][xx] == 2)
                {
                    xx += 1;
                    yy -= 1;
                    while(board[yy][xx] == 1)
                    {
                        board[yy][xx] = 2;
                        xx++;
                        yy--;
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
            //            if(j == 9)
            //            {
            //                printf("%d\n", board[i][9]);
            //            }
            //            else
            //            {
            //                printf("%d ", board[i][j]);
            //            }
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
    //    if(player == 2)
    //    {
    //        player = 1;
    //    }
    //    else
    //    {
    //        player = 2;
    //    }
    //    check();
    printf("Player: %d\n", player);
    //scanf("%d, %d", &px, &py);
    //    px -= 1;
    //    py -= 1;
    printf("(%d, %d)\n", px, py);
    if(px > 8 || py > 8)
    {
        printf("[*]そこには置けません\n");
        return 0;
    }
    //canPut[py][px] = true;
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
