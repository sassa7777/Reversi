//
//  evaluation.c
//  othello
//
//  Created by sasa on 2024/01/15.
//

#include <stdbool.h>

//int scoreboard[8][8] = {
//    45,  -11, 4,  0,  0,  4,  -11, 45,
//    -11, -16, -1, -3, -3, -1, -16, -11,
//    4,   -1,  2,  -1, -1, 2,  -1,  4,
//    0,   -3,  -1, 0,  0,  -1, -3,  0,
//    0,   -3,  -1, 0,  0,  -1, -3,  0,
//    4,   -1,  2,  -1, -1, 2,  -1,  4,
//    -11, -16, -1, -3, -3, -1, -16, -11,
//    45,  -11, 4,  0,  0,  4,  -11, 45
//};

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


int score_stone(char board[10][10]) {
    int score = 0;
    for (char x = 1; x < 9; ++x) {
        for (char y = 1; y < 9; ++y) {
            if (board[x][y] == 2)
                score += (scoreboard[x-1][y-1]);
            if (board[x][y] == 1)
                score -= (scoreboard[x-1][y-1]);
        }
    }
    return score;
}

int score_putable(char board[10][10], bool canput[10][10]) {
    int score = 0;
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            if (canput[i][j] == true) {
                score += 1;
            }
        }
    }
    return score;
}

int score_fixedstone(char board[10][10]) {
    int fixedstone = 0;
    // ひとつでも埋まっているかどうか
    if (board[1][1] != 0 || board[1][8] != 0 || board[8][1] != 0 ||
        board[8][8] != 0) {
        // 左上
        if (board[1][1] != 0) {
            char i = 1;
            if (board[1][1] == 2) {
                while (board[1][i] == board[1][1]) {
                    fixedstone++;
                    i++;
                }
            } else {
                while (board[1][i] == board[1][1]) {
                    fixedstone--;
                    i++;
                }
            }
        }
        if (board[1][1] != 0) {
            char i = 1;
            if (board[1][1] == 2) {
                while (board[i][1] == board[1][1]) {
                    fixedstone++;
                    i++;
                }
            } else {
                while (board[i][1] == board[1][1]) {
                    fixedstone--;
                    i++;
                }
            }
        }
        // 右上
        if (board[1][8] != 0) {
            char i = 1;
            if (board[1][8] == 2) {
                while (board[1][i] == board[1][8]) {
                    fixedstone++;
                    i--;
                }
            } else {
                while (board[1][i] == board[1][8]) {
                    fixedstone--;
                    i--;
                }
            }
        }
        if (board[1][8] != 0) {
            char i = 1;
            if (board[1][8] == 2) {
                while (board[i][8] == board[1][8]) {
                    fixedstone++;
                    i++;
                }
            } else {
                while (board[i][8] == board[1][8]) {
                    fixedstone--;
                    i++;
                }
            }
        }
        // 左下
        if (board[8][1] != 0) {
            char i = 1;
            if (board[8][1] == 2) {
                while (board[8][i] == board[8][1]) {
                    fixedstone++;
                    i++;
                }
            } else {
                while (board[8][i] == board[8][1]) {
                    fixedstone--;
                    i++;
                }
            }
        }
        if (board[8][1] != 0) {
            char i = 1;
            if (board[8][1] == 2) {
                while (board[i][1] == board[8][1]) {
                    fixedstone++;
                    i--;
                }
            } else {
                while (board[i][1] == board[8][1]) {
                    fixedstone--;
                    i--;
                }
            }
        }
        // 右下
        if (board[8][8] != 0) {
            char i = 1;
            if (board[8][8] == 2) {
                while (board[8][i] == board[8][8]) {
                    fixedstone++;
                    i--;
                }
            } else {
                while (board[8][i] == board[8][8]) {
                    fixedstone--;
                    i--;
                }
            }
        }
        if (board[8][8] != 0) {
            char i = 1;
            if (board[8][8] == 2) {
                while (board[i][8] == board[8][8]) {
                    fixedstone++;
                    i--;
                }
            } else {
                while (board[i][8] == board[8][8]) {
                    fixedstone--;
                    i--;
                }
            }
        }
        // 左上+右上
        if (board[1][1] != 0 && board[1][2] != 0 && board[1][3] != 0 && board[1][4] != 0 && board[1][5] != 0 && board[1][6] != 0 && board[1][7] != 0 && board[1][8] != 0) {
            char i = 1;
            if (board[1][1] == 2) {
                while (board[1][i] == board[1][1]) {
                    fixedstone--;
                    i++;
                }
            } else {
                while (board[1][i] == board[1][1]) {
                    fixedstone++;
                    i++;
                }
            }
        }
        //左上+左下
        if (board[1][1] != 0 && board[2][1] != 0 && board[3][1] != 0 && board[4][1] != 0 && board[5][1] != 0 && board[6][1] != 0 && board[7][1] != 0 && board[8][1] != 0) {
            char i = 1;
            if (board[1][1] == 2) {
                while (board[i][1] == board[1][1]) {
                    fixedstone--;
                    i++;
                }
            } else {
                while (board[i][1] == board[1][1]) {
                    fixedstone++;
                    i++;
                }
            }
        }
        // 右下+左下
        if (board[8][8] != 0 && board[8][2] != 0 && board[8][3] != 0 && board[8][4] != 0 && board[8][5] != 0 && board[8][6] != 0 && board[8][7] != 0 && board[8][1] != 0) {
            char i = 1;
            if (board[8][8] == 2) {
                while (board[8][i] == board[8][8]) {
                    fixedstone--;
                    i--;
                }
            } else {
                while (board[8][i] == board[8][8]) {
                    fixedstone++;
                    i--;
                }
            }
        }
        // 右下+右上
        if (board[8][8] != 0 && board[2][8] != 0 && board[3][8] != 0 && board[4][8] != 0 && board[5][8] != 0 && board[6][8] != 0 && board[7][8] != 0 && board[1][8] != 0) {
            char i = 1;
            if (board[8][8] == 2) {
                while (board[i][8] == board[8][8]) {
                    fixedstone--;
                    i--;
                }
            } else {
                while (board[i][8] == board[8][8]) {
                    fixedstone++;
                    i--;
                }
            }
        }
    }
    return fixedstone;
}

int score_countstone(char board[10][10]) {
    int score = 0;
    for (char i = 1; i < 9; ++i) {
        for (char j = 1; j < 9; ++j) {
            if (board[i][j] == 1) {
                score--;
            } else if (board[i][j] == 2) {
                score++;
            }
        }
    }
    return score;
}

bool is_allblack(char board[10][10]) {
    for (char i = 1; i <= 8; ++i) {
        for (char j = 1; j <= 8; ++j) {
            if (board[i][j] == 2) {
                return false;
            }
        }
    }
    return true;
}

int countscore(char board[10][10], int *turn, bool canput[10][10],
               char *playerrn) {
    if(is_allblack(board)) return -9999;
    if(*turn >= 60) return 100*score_countstone(board);
    if(*playerrn == 2) return (3 * score_stone(board))+(55 * score_fixedstone(board))+(5*score_putable(board, canput));
    else return (3 * score_stone(board))+(55 * score_fixedstone(board))-(5*score_putable(board, canput));
}
