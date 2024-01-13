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
    int wing = 0;
    //上
    //黒
    if(board[1][1] == 0 && board[1][2] == 1 && board[1][4] == 1 && board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 && board[1][7] == 0 && board[1][8] == 0) wing++;
    else if(board[1][1] == 0 && board[1][2] == 0 && board[1][4] == 1 && board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 && board[1][7] == 1 && board[1][8] == 0) wing++;
    //白
    if(board[1][1] == 0 && board[1][2] == 2 && board[1][4] == 2 && board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 && board[1][7] == 0 && board[1][8] == 0) wing--;
    else if(board[1][1] == 0 && board[1][2] == 0 && board[1][4] == 2 && board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 && board[1][7] == 2 && board[1][8] == 0) wing--;
    
    //下
    //黒
    if(board[8][1] == 0 && board[8][2] == 1 && board[8][4] == 1 && board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 && board[8][7] == 0 && board[8][8] == 0) wing++;
    else if(board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 1 && board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 && board[8][7] == 1 && board[8][8] == 0) wing++;
    //白
    if(board[8][1] == 0 && board[8][2] == 2 && board[8][4] == 2 && board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 && board[8][7] == 0 && board[8][8] == 0) wing--;
    else if(board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 2 && board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 && board[8][7] == 2 && board[8][8] == 0) wing--;
    
    //右
    //黒
    if(board[1][1] == 0 && board[2][2] == 1 && board[3][4] == 1 && board[4][4] == 1 && board[5][5] == 1 && board[6][6] == 1 && board[7][7] == 0 && board[8][8] == 0) wing++;
    else if(board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 1 && board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 && board[8][7] == 1 && board[8][8] == 0) wing++;
    //白
    if(board[1][8] == 0 && board[2][8] == 2 && board[3][8] == 2 && board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 && board[7][8] == 0 && board[8][8] == 0) wing--;
    else if(board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 2 && board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 && board[7][8] == 2 && board[8][8] == 0) wing--;
    
    //左
    //黒
    if(board[1][1] == 0 && board[2][1] == 1 && board[3][1] == 1 && board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1 && board[7][1] == 0 && board[8][1] == 0) wing++;
    else if(board[8][1] == 0 && board[8][1] == 0 && board[8][1] == 1 && board[8][1] == 1 && board[8][1] == 1 && board[8][1] == 1 && board[8][1] == 1 && board[8][1] == 0) wing++;
    //白
    if(board[1][1] == 0 && board[2][1] == 2 && board[3][1] == 2 && board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 && board[7][1] == 0 && board[8][1] == 0) wing--;
    else if(board[1][1] == 0 && board[2][1] == 0 && board[3][1] == 2 && board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 && board[7][1] == 2 && board[8][1] == 0) wing--;
    return -2*wing;
}

int score_mountain(char board[10][10])
{
    int mountain = 0;
    //上
    //黒
    if(board[1][1] == 0 && board[1][2] == 1 && board[1][4] == 1 && board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 && board[1][7] == 1 && board[1][8] == 0 && board[2][3] != 0 && board[2][4] != 0 && board[2][5] != 0 && board[2][6] != 0) mountain--;
    else if(board[1][1] == 0 && board[1][2] == 0 && board[1][4] == 1 && board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 && board[1][7] == 1 && board[1][8] == 0 && board[2][3] != 0 && board[2][4] != 0 && board[2][5] != 0 && board[2][6] != 0) mountain--;
    //白
    if(board[1][1] == 0 && board[1][2] == 2 && board[1][4] == 2 && board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 && board[1][7] == 0 && board[1][8] == 0 && board[2][3] != 0 && board[2][4] != 0 && board[2][5] != 0 && board[2][6] != 0) mountain++;
    else if(board[1][1] == 0 && board[1][2] == 0 && board[1][4] == 2 && board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 && board[1][7] == 2 && board[1][8] == 0 && board[2][3] != 0 && board[2][4] != 0 && board[2][5] != 0 && board[2][6] != 0) mountain++;
    
    //下
    //黒
    if(board[8][1] == 0 && board[8][2] == 1 && board[8][4] == 1 && board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 && board[8][7] == 0 && board[8][8] == 0 && board[7][3] != 0 && board[7][4] != 0 && board[7][5] != 0 && board[7][6] != 0) mountain--;
    else if(board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 1 && board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 && board[8][7] == 1 && board[8][8] == 0 && board[7][3] != 0 && board[7][4] != 0 && board[7][5] != 0 && board[7][6] != 0) mountain--;
    //白
    if(board[8][1] == 0 && board[8][2] == 2 && board[8][4] == 2 && board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 && board[8][7] == 0 && board[8][8] == 0 && board[7][3] != 0 && board[7][4] != 0 && board[7][5] != 0 && board[7][6] != 0) mountain++;
    else if(board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 2 && board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 && board[8][7] == 2 && board[8][8] == 0 && board[7][3] != 0 && board[7][4] != 0 && board[7][5] != 0 && board[7][6] != 0) mountain++;
    
    //右
    //黒
    if(board[1][8] == 0 && board[2][8] == 1 && board[3][8] == 1 && board[4][8] == 1 && board[5][8] == 1 && board[6][8] == 1 && board[7][8] == 0 && board[8][8] == 0 && board[3][7] != 0 && board[4][7] != 0 && board[5][7] != 0 && board[6][7] != 0) mountain--;
    else if(board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 1 && board[4][8] == 1 && board[5][8] == 1 && board[6][8] == 1 && board[7][8] == 1 && board[8][8] == 0 && board[3][7] != 0 && board[4][7] != 0 && board[5][7] != 0 && board[6][7] != 0) mountain--;
    //白
    if(board[1][8] == 0 && board[2][8] == 2 && board[3][8] == 2 && board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 && board[7][8] == 0 && board[8][8] == 0 && board[3][7] != 0 && board[4][7] != 0 && board[5][7] != 0 && board[6][7] != 0) mountain++;
    else if(board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 2 && board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 && board[7][8] == 2 && board[8][8] == 0 && board[3][7] != 0 && board[4][7] != 0 && board[5][7] != 0 && board[6][7] != 0) mountain++;
    
    //左
    //黒
    if(board[1][1] == 0 && board[2][1] == 1 && board[3][1] == 1 && board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1 && board[7][1] == 0 && board[8][1] == 0 && board[3][1] != 0 && board[4][1] != 0 && board[5][1] != 0 && board[6][1] != 0) mountain--;
    else if(board[8][1] == 0 && board[8][1] == 0 && board[8][1] == 1 && board[8][1] == 1 && board[8][1] == 1 && board[8][1] == 1 && board[8][1] == 1 && board[8][1] == 0 && board[3][1] != 0 && board[4][1] != 0 && board[5][1] != 0 && board[6][1] != 0) mountain--;
    //白
    if(board[1][1] == 0 && board[2][1] == 2 && board[3][1] == 2 && board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 && board[7][1] == 0 && board[8][1] == 0 && board[3][1] != 0 && board[4][1] != 0 && board[5][1] != 0 && board[6][1] != 0) mountain++;
    else if(board[1][1] == 0 && board[2][1] == 0 && board[3][1] == 2 && board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 && board[7][1] == 2 && board[8][1] == 0 && board[3][1] != 0 && board[4][1] != 0 && board[5][1] != 0 && board[6][1] != 0) mountain++;
    return 3*mountain;
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
        score += score_wing(board);
    }
    if(turn < 50)
    {
        score += score_stone(board);
        score += score_mountain(board);
    }
    if(turn > 43)
    {
        score += score_countstone(board);
    }
    return score;
}
