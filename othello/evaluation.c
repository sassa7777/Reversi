//
//  evaluation.c
//  othello
//
//  Created by sasa on 2024/01/15.
//

#include <stdbool.h>

#define DEPTH 10

int scoreboard[8][8] = {
    40, -12, 0, -1, -1, 0, -12, 40,
    -12, -15, -3, -3, -3, -3, -15, -12,
    0, -3, 0, -1, -1, 0, -3, 0,
    -1, -3, -1, -1, -1, -1, -3, -1,
    -1, -3, -1, -1, -1, -1, -3, -1,
    0, -3, 0, -1, -1, 0, -3, 0,
    -12, -15, -3, -3, -3, -3, -15, -12,
    40, -12, 0, -1, -1, 0, -12, 40
};

int score_stone(char board[10][10]) {
    int score = 0;
    for (char x = 1; x < 9; ++x) {
        for (char y = 1; y < 9; ++y) {
            if (board[x][y] == 2)
                score += (scoreboard[x-1][y-1]);
            else if (board[x][y] == 1)
                score -= (scoreboard[x-1][y-1]);
        }
    }
    return score;
}

int score_putable(char board[10][10]) {
    int score = 0;
    char y, x, xx, yy, xxx, yyy;
    for (y = 1; y <= 8; ++y) {
        for (x = 1; x <= 8; ++x) {
            //白
            if (board[y][x] == 2) {
                for (xx = -1; xx < 2; ++xx) {
                    for (yy = -1; yy < 2; ++yy) {
                        if (board[y + yy][x + xx] == 1) {
                            xxx = x + xx;
                            yyy = y + yy;
                            while (board[yyy][xxx] == 1) {
                                xxx += xx;
                                yyy += yy;
                            }
                            if (board[yyy][xxx] == 0)
                            {
                                score++;
                            }
                        }
                    }
                }
            }
            //黒
            else if (board[y][x] == 1) {
                for (xx = -1; xx < 2; ++xx) {
                    for (yy = -1; yy < 2; ++yy) {
                        if (board[y + yy][x + xx] == 2) {
                            xxx = x + xx;
                            yyy = y + yy;
                            while (board[yyy][xxx] == 2) {
                                xxx += xx;
                                yyy += yy;
                            }
                            if (board[yyy][xxx] == 0) score--;
                        }
                    }
                }
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
        char i;
        // 左上
        if (board[1][1] != 0) {
            i = 1;
            if (board[1][1] == 2) {
                while (board[1][i] == 2) {
                    fixedstone++;
                    i++;
                }
            } else {
                while (board[1][i] == 1) {
                    fixedstone--;
                    i++;
                }
            }
        }
        if (board[1][1] != 0) {
            i = 1;
            if (board[1][1] == 2) {
                while (board[i][1] == 2) {
                    fixedstone++;
                    i++;
                }
            } else {
                while (board[i][1] == 1) {
                    fixedstone--;
                    i++;
                }
            }
        }
        // 右上
        if (board[1][8] != 0) {
            i = 8;
            if (board[1][8] == 2) {
                while (board[1][i] == 2) {
                    fixedstone++;
                    i--;
                }
            } else {
                while (board[1][i] == 1) {
                    fixedstone--;
                    i--;
                }
            }
        }
        if (board[1][8] != 0) {
            i = 1;
            if (board[1][8] == 2) {
                while (board[i][8] == 2) {
                    fixedstone++;
                    i++;
                }
            } else {
                while (board[i][8] == 1) {
                    fixedstone--;
                    i++;
                }
            }
        }
        // 左下
        if (board[8][1] != 0) {
            i = 1;
            if (board[8][1] == 2) {
                while (board[8][i] == 2) {
                    fixedstone++;
                    i++;
                }
            } else {
                while (board[8][i] == 1) {
                    fixedstone--;
                    i++;
                }
            }
        }
        if (board[8][1] != 0) {
            i = 8;
            if (board[8][1] == 2) {
                while (board[i][1] == 2) {
                    fixedstone++;
                    i--;
                }
            } else {
                while (board[i][1] == 1) {
                    fixedstone--;
                    i--;
                }
            }
        }
        // 右下
        if (board[8][8] != 0) {
            i = 8;
            if (board[8][8] == 2) {
                while (board[8][i] == 2) {
                    fixedstone++;
                    i--;
                }
            } else {
                while (board[8][i] == 1) {
                    fixedstone--;
                    i--;
                }
            }
        }
        if (board[8][8] != 0) {
            i = 8;
            if (board[8][8] == 2) {
                while (board[i][8] == 2) {
                    fixedstone++;
                    i--;
                }
            } else {
                while (board[i][8] == 1) {
                    fixedstone--;
                    i--;
                }
            }
        }
        // 左上+右上
        if (board[1][1] != 0 && board[1][2] != 0 && board[1][3] != 0 && board[1][4] != 0 && board[1][5] != 0 && board[1][6] != 0 && board[1][7] != 0 && board[1][8] != 0) {
            i = 1;
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
            i = 1;
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
            i = 1;
            if (board[8][8] == 2) {
                while (board[8][i] == board[8][8]) {
                    fixedstone--;
                    i++;
                }
            } else {
                while (board[8][i] == board[8][8]) {
                    fixedstone++;
                    i++;
                }
            }
        }
        // 右下+右上
        if (board[8][8] != 0 && board[2][8] != 0 && board[3][8] != 0 && board[4][8] != 0 && board[5][8] != 0 && board[6][8] != 0 && board[7][8] != 0 && board[1][8] != 0) {
            i = 1;
            if (board[8][8] == 2) {
                while (board[i][8] == board[8][8]) {
                    fixedstone--;
                    i++;
                }
            } else {
                while (board[i][8] == board[8][8]) {
                    fixedstone++;
                    i++;
                }
            }
        }
    }
    return fixedstone;
}

int score_countstone(char board[10][10]) {
    int score = 0;
    char j;
    for (char i = 1; i < 9; ++i) {
        for (j = 1; j < 9; ++j) {
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
    char j;
    for (char i = 1; i <= 8; ++i) {
        for (j = 1; j <= 8; ++j) {
            if (board[i][j] == 2) {
                return false;
            }
        }
    }
    return true;
}

int countscore(char board[10][10], int *turn) {
    if(is_allblack(board)) return -9999;
    if(*turn >= 60) return 10*score_countstone(board);
    if(*turn >= 44) return 4*score_stone(board)+60*score_fixedstone(board);
    return 4*score_stone(board)+60*score_fixedstone(board)+score_putable(board);
}
