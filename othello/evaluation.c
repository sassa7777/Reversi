//
//  evaluation.c
//  othello
//
//  Created by sasa on 2024/01/15.
//

#include "reversi.h"
#include <stdint.h>

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
    for (char x = 0; x < 8; ++x) {
        for (char y = 0; y < 8; ++y) {
            score += scoreboard[x][y] * (board[x + 1][y + 1] == 2) - scoreboard[x][y] * (board[x + 1][y + 1] == 1);
        }
    }
    return score;
}

int score_putable(char board[10][10]) {
    int score = 0;
    for (int y = 1; y <= 8; ++y) {
        for (int x = 1; x <= 8; ++x) {
            int currentCell = board[y][x];
            if (currentCell == 2 || currentCell == 1) {
                int opponentCell = (currentCell == 2) ? 1 : 2;
                for (int yy = -1; yy <= 1; ++yy) {
                    for (int xx = -1; xx <= 1; ++xx) {
                        if (board[y + yy][x + xx] == opponentCell) {
                            int xxx = x + xx;
                            int yyy = y + yy;
                            while (board[yyy][xxx] == opponentCell) {
                                xxx += xx;
                                yyy += yy;
                            }
                            if (board[yyy][xxx] == 0) {
                                if (currentCell == 2)
                                    score++;
                                else
                                    score--;
                            }
                        }
                    }
                }
            }
        }
    }
    return score;}

int score_fixedstone(char board[10][10]) {
    int fixedstone = 0;
    // ひとつでも埋まっているかどうか
    if (board[1][1] != 0 || board[1][8] != 0 || board[8][1] != 0 ||
        board[8][8] != 0) {
        char i;
        // 左上
        switch (board[1][1]) {
            case 2:
                i=1;
                while (board[1][i] == 2) {
                    fixedstone++;
                    i++;
                }
                i=1;
                while (board[i][1] == 2) {
                    fixedstone++;
                    i++;
                }
                if(board[1][2] == 2 && board[2][1] == 2 && board[2][2] == 2) fixedstone++;
                break;
            case 1:
                i=1;
                while (board[1][i] == 1) {
                    fixedstone--;
                    i++;
                }
                i=1;
                while (board[i][1] == 1) {
                    fixedstone--;
                    i++;
                }
                if(board[1][2] == 1 && board[2][1] == 1 && board[2][2] == 1) fixedstone--;
                break;
            default:
                break;
        }
        // 右上
        switch (board[1][8]) {
            case 2:
                i=8;
                while (board[1][i] == 2) {
                    fixedstone++;
                    i--;
                }
                i=1;
                while (board[i][8] == 2) {
                    fixedstone++;
                    i++;
                }
                if(board[1][7] == 2 && board[2][8] == 2 && board[2][7] == 2) fixedstone++;
                break;
            case 1:
                i=8;
                while (board[1][i] == 1) {
                    fixedstone--;
                    i--;
                }
                i=1;
                while (board[i][8] == 1) {
                    fixedstone--;
                    i++;
                }
                if(board[1][7] == 1 && board[2][8] == 1 && board[2][7] == 1) fixedstone--;
                break;
            default:
                break;
        }
        // 左下
        switch (board[8][1]) {
            case 2:
                i=1;
                while (board[8][i] == 2) {
                    fixedstone++;
                    i++;
                }
                i=8;
                while (board[i][1] == 2) {
                    fixedstone++;
                    i--;
                }
                if(board[8][2] == 2 && board[7][1] == 2 && board[7][2] == 2) fixedstone++;
                break;
            case 1:
                i=1;
                while (board[8][i] == 1) {
                    fixedstone--;
                    i++;
                }
                i=8;
                while (board[i][1] == 1) {
                    fixedstone--;
                    i--;
                }
                if(board[8][2] == 1 && board[7][1] == 1 && board[7][2] == 1) fixedstone--;
                break;
            default:
                break;
        }
        // 右下
        switch (board[8][8]) {
            case 2:
                i=8;
                while (board[8][i] == 2) {
                    fixedstone++;
                    i--;
                }
                i=8;
                while (board[i][8] == 2) {
                    fixedstone++;
                    i--;
                }
                if(board[8][7] == 2 && board[7][8] == 2 && board[7][7] == 2) fixedstone++;
                break;
            case 1:
                i=8;
                while (board[8][i] == 1) {
                    fixedstone--;
                    i--;
                }
                i=8;
                while (board[i][8] == 1) {
                    fixedstone--;
                    i--;
                }
                if(board[8][7] == 1 && board[7][8] == 1 && board[7][7] == 1) fixedstone--;
                break;
            default:
                break;
        }
        // 左上+右上
        if (board[1][1] == 1 && board[1][2] == 1 && board[1][3] == 1 && board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 && board[1][7] == 1 && board[1][8] == 1) {
            fixedstone-=8;
        }
        if (board[1][1] == 2 && board[1][2] == 2 && board[1][3] == 2 && board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 && board[1][7] == 2 && board[1][8] == 2) {
            fixedstone+=8;
        }
        //左上+左下
        if (board[1][1] == 1 && board[2][1] == 1 && board[3][1] == 1 && board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1 && board[7][1] == 1 && board[8][1] == 1) {
            fixedstone-=8;
        }
        if (board[1][1] == 2 && board[2][1] == 2 && board[3][1] == 2 && board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 && board[7][1] == 2 && board[8][1] == 2) {
            fixedstone+=8;
        }
        // 右下+左下
        if (board[8][8] == 1 && board[8][2] == 1 && board[8][3] == 1 && board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 && board[8][7] == 1 && board[8][1] == 1) {
            fixedstone-=8;
        }
        if (board[8][8] == 2 && board[8][2] == 2 && board[8][3] == 2 && board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 && board[8][7] == 2 && board[8][1] == 2) {
            fixedstone+=8;
        }
        // 右下+右上
        if (board[8][8] == 1 && board[2][8] == 1 && board[3][8] == 1 && board[4][8] == 1 && board[5][8] == 1 && board[6][8] == 1 && board[7][8] == 0 && board[1][8] == 1) {
            fixedstone-=8;
        }
        if (board[8][8] == 2 && board[2][8] == 2 && board[3][8] == 2 && board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 && board[7][8] == 2 && board[1][8] == 2) {
            fixedstone+=8;
        }
    }
    return fixedstone;
}

int score_countstone(char board[10][10]) {
    int score = 0;
    char j;
    for (char i = 1; i < 9; ++i) {
        for (j = 1; j < 9; ++j) {
            switch (board[i][j]) {
                case 1:
                    score--;
                    break;
                case 2:
                    score++;
                    break;
                default:
                    break;
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

int countscore(uint64_t playerboard, uint64_t oppenentboard, char *playerrn) {
//    if(DEPTH == 1) {
//        if(*playerrn == 1) {
//            return -score_countstone(board);
//        } else {
//            return score_countstone(board);
//        }
//    } else {
//        if(*playerrn == 1) {
//            if(is_allblack(board)) return 9999;
//            else if(turn+DEPTH >= 60) return -(score_countstone(board));
//            else if(turn+DEPTH >= 44) return -(score_stone(board)+50*score_fixedstone(board));
//            else return -(3*score_stone(board)+50*score_fixedstone(board)+2*score_putable(board));
//        } else {
//            if(is_allblack(board)) return -9999;
//            else if(turn+DEPTH >= 60) return (score_countstone(board));
//            else if(turn+DEPTH >= 44) return (score_stone(board)+50*score_fixedstone(board));
//            else return (3*score_stone(board)+50*score_fixedstone(board)+2*score_putable(board));
//        }
//    }
//    if(*playerrn == 1) {
//        return bitcount(*oppenentboard)-bitcount(*playerboard);
//    } else {
//        return bitcount(*playerboard)-bitcount(*oppenentboard);
//    }
    return bitcount(playerboard)-bitcount(oppenentboard);
}
