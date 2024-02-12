//
//  evaluation.c
//  othello
//
//  Created by sasa on 2024/01/15.
//

#include <stdbool.h>
//
// int scoreboard[8][8] = {
//    50, -15, 2, -1, -1, 2, -15, 50,
//    -15, -18, -3, -2, -2, -3, -18, -15,
//    2, -3, 1, -1, -1, 1, -3, 2,
//    -1, -2, -1, 0, 0, -1, -2, -1,
//    -1, -2, -1, 0, 0, -1, -2, -1,
//    2, -3, 1, -1, -1, 1, -3, 2,
//    -15, -18, -3, -2, -2, -3, -18, -15,
//    50, -15, 2, -1, -1, 2, -15, 50
//};

int scoreboard[8][8] = {
    45,  -11, 4,  0,  0,  4,  -11, 45,  -11, -16, -1, -3, -3, -1, -16, -11,
    4,   -1,  2,  -1, -1, 2,  -1,  4,   0,   -3,  -1, 0,  0,  -1, -3,  0,
    0,   -3,  -1, 0,  0,  -1, -3,  0,   4,   -1,  2,  -1, -1, 2,  -1,  4,
    -11, -16, -1, -3, -3, -1, -16, -11, 45,  -11, 4,  0,  0,  4,  -11, 45};

int score_stone(char board[10][10]) {
  int score = 0;
  for (char x = 1; x < 9; ++x) {
    for (char y = 1; y < 9; ++y) {
      if (board[x][y] == 2)
        score += (scoreboard[x - 1][y - 1] * 3);
      if (board[x][y] == 1)
        score -= (scoreboard[x - 1][y - 1] * 3);
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
  score *= 10;
  return score;
}

int score_fixedstone(char board[10][10]) {
  int fixedstone = 0;
  // ひとつでも埋まっているかどうか
  if (board[1][1] != 0 || board[1][8] != 0 || board[8][1] != 0 ||
      board[8][8] != 0) {
    // 全て埋まっている
    if (board[1][1] != 0 && board[1][8] != 0 && board[8][1] != 0 &&
        board[8][8] != 0) {
      for (char i = 1; i <= 8; ++i) {
        if (board[1][i] == 2)
          fixedstone++;
        else if (board[1][i] == 1)
          fixedstone--;
        if (board[8][i] == 2)
          fixedstone++;
        else if (board[8][i] == 1)
          fixedstone--;
        if (board[i][1] == 2)
          fixedstone++;
        else if (board[i][1] == 1)
          fixedstone--;
        if (board[i][8] == 2)
          fixedstone++;
        else if (board[i][8] == 1)
          fixedstone--;
      }
    } else // 全ては埋まっていない
    {
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
      // 右上
      if (board[1][8] != 0) {
        char i = 2;
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
      // 左下
      if (board[8][1] != 0) {
        char i = 2;
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
      // 右下
      if (board[8][8] != 0) {
        char i = 2;
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
    }
  }
  return 11 * fixedstone;
}

int score_wing(char board[10][10]) {
  int wing = 0;
  // 上
  // 黒
  if (board[1][1] == 0 && board[1][2] == 1 && board[1][4] == 1 &&
      board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 &&
      board[1][7] == 0 && board[1][8] == 0)
    wing++;
  else if (board[1][1] == 0 && board[1][2] == 0 && board[1][4] == 1 &&
           board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 &&
           board[1][7] == 1 && board[1][8] == 0)
    wing++;
  // 白
  if (board[1][1] == 0 && board[1][2] == 2 && board[1][4] == 2 &&
      board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 &&
      board[1][7] == 0 && board[1][8] == 0)
    wing--;
  else if (board[1][1] == 0 && board[1][2] == 0 && board[1][4] == 2 &&
           board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 &&
           board[1][7] == 2 && board[1][8] == 0)
    wing--;

  // 下
  // 黒
  if (board[8][1] == 0 && board[8][2] == 1 && board[8][4] == 1 &&
      board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 &&
      board[8][7] == 0 && board[8][8] == 0)
    wing++;
  else if (board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 1 &&
           board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 &&
           board[8][7] == 1 && board[8][8] == 0)
    wing++;
  // 白
  if (board[8][1] == 0 && board[8][2] == 2 && board[8][4] == 2 &&
      board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 &&
      board[8][7] == 0 && board[8][8] == 0)
    wing--;
  else if (board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 2 &&
           board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 &&
           board[8][7] == 2 && board[8][8] == 0)
    wing--;

  // 右
  // 黒
  if (board[1][1] == 0 && board[2][2] == 1 && board[3][4] == 1 &&
      board[4][4] == 1 && board[5][5] == 1 && board[6][6] == 1 &&
      board[7][7] == 0 && board[8][8] == 0)
    wing++;
  else if (board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 1 &&
           board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 &&
           board[8][7] == 1 && board[8][8] == 0)
    wing++;
  // 白
  if (board[1][8] == 0 && board[2][8] == 2 && board[3][8] == 2 &&
      board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 &&
      board[7][8] == 0 && board[8][8] == 0)
    wing--;
  else if (board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 2 &&
           board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 &&
           board[7][8] == 2 && board[8][8] == 0)
    wing--;

  // 左
  // 黒
  if (board[1][1] == 0 && board[2][1] == 1 && board[3][1] == 1 &&
      board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1 &&
      board[7][1] == 0 && board[8][1] == 0)
    wing++;
  else if (board[8][1] == 0 && board[8][1] == 0 && board[8][1] == 1 &&
           board[8][1] == 1 && board[8][1] == 1 && board[8][1] == 1 &&
           board[8][1] == 1 && board[8][1] == 0)
    wing++;
  // 白
  if (board[1][1] == 0 && board[2][1] == 2 && board[3][1] == 2 &&
      board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 &&
      board[7][1] == 0 && board[8][1] == 0)
    wing--;
  else if (board[1][1] == 0 && board[2][1] == 0 && board[3][1] == 2 &&
           board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 &&
           board[7][1] == 2 && board[8][1] == 0)
    wing--;
  return wing;
}

int score_mountain(char board[10][10]) {
  int mountain = 0;
  // 上
  // 黒
  if (board[1][1] == 0 && board[1][2] == 1 && board[1][4] == 1 &&
      board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 &&
      board[1][7] == 1 && board[1][8] == 0 && board[2][3] != 0 &&
      board[2][4] != 0 && board[2][5] != 0 && board[2][6] != 0)
    mountain--;
  else if (board[1][1] == 0 && board[1][2] == 0 && board[1][4] == 1 &&
           board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 &&
           board[1][7] == 1 && board[1][8] == 0 && board[2][3] != 0 &&
           board[2][4] != 0 && board[2][5] != 0 && board[2][6] != 0)
    mountain--;
  // 白
  if (board[1][1] == 0 && board[1][2] == 2 && board[1][4] == 2 &&
      board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 &&
      board[1][7] == 0 && board[1][8] == 0 && board[2][3] != 0 &&
      board[2][4] != 0 && board[2][5] != 0 && board[2][6] != 0)
    mountain++;
  else if (board[1][1] == 0 && board[1][2] == 0 && board[1][4] == 2 &&
           board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 &&
           board[1][7] == 2 && board[1][8] == 0 && board[2][3] != 0 &&
           board[2][4] != 0 && board[2][5] != 0 && board[2][6] != 0)
    mountain++;

  // 下
  // 黒
  if (board[8][1] == 0 && board[8][2] == 1 && board[8][4] == 1 &&
      board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 &&
      board[8][7] == 0 && board[8][8] == 0 && board[7][3] != 0 &&
      board[7][4] != 0 && board[7][5] != 0 && board[7][6] != 0)
    mountain--;
  else if (board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 1 &&
           board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 &&
           board[8][7] == 1 && board[8][8] == 0 && board[7][3] != 0 &&
           board[7][4] != 0 && board[7][5] != 0 && board[7][6] != 0)
    mountain--;
  // 白
  if (board[8][1] == 0 && board[8][2] == 2 && board[8][4] == 2 &&
      board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 &&
      board[8][7] == 0 && board[8][8] == 0 && board[7][3] != 0 &&
      board[7][4] != 0 && board[7][5] != 0 && board[7][6] != 0)
    mountain++;
  else if (board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 2 &&
           board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 &&
           board[8][7] == 2 && board[8][8] == 0 && board[7][3] != 0 &&
           board[7][4] != 0 && board[7][5] != 0 && board[7][6] != 0)
    mountain++;

  // 右
  // 黒
  if (board[1][8] == 0 && board[2][8] == 1 && board[3][8] == 1 &&
      board[4][8] == 1 && board[5][8] == 1 && board[6][8] == 1 &&
      board[7][8] == 0 && board[8][8] == 0 && board[3][7] != 0 &&
      board[4][7] != 0 && board[5][7] != 0 && board[6][7] != 0)
    mountain--;
  else if (board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 1 &&
           board[4][8] == 1 && board[5][8] == 1 && board[6][8] == 1 &&
           board[7][8] == 1 && board[8][8] == 0 && board[3][7] != 0 &&
           board[4][7] != 0 && board[5][7] != 0 && board[6][7] != 0)
    mountain--;
  // 白
  if (board[1][8] == 0 && board[2][8] == 2 && board[3][8] == 2 &&
      board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 &&
      board[7][8] == 0 && board[8][8] == 0 && board[3][7] != 0 &&
      board[4][7] != 0 && board[5][7] != 0 && board[6][7] != 0)
    mountain++;
  else if (board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 2 &&
           board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 &&
           board[7][8] == 2 && board[8][8] == 0 && board[3][7] != 0 &&
           board[4][7] != 0 && board[5][7] != 0 && board[6][7] != 0)
    mountain++;

  // 左
  // 黒
  if (board[1][1] == 0 && board[2][1] == 1 && board[3][1] == 1 &&
      board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1 &&
      board[7][1] == 0 && board[8][1] == 0 && board[3][1] != 0 &&
      board[4][1] != 0 && board[5][1] != 0 && board[6][1] != 0)
    mountain--;
  else if (board[8][1] == 0 && board[8][1] == 0 && board[8][1] == 1 &&
           board[8][1] == 1 && board[8][1] == 1 && board[8][1] == 1 &&
           board[8][1] == 1 && board[8][1] == 0 && board[3][1] != 0 &&
           board[4][1] != 0 && board[5][1] != 0 && board[6][1] != 0)
    mountain--;
  // 白
  if (board[1][1] == 0 && board[2][1] == 2 && board[3][1] == 2 &&
      board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 &&
      board[7][1] == 0 && board[8][1] == 0 && board[3][1] != 0 &&
      board[4][1] != 0 && board[5][1] != 0 && board[6][1] != 0)
    mountain++;
  else if (board[1][1] == 0 && board[2][1] == 0 && board[3][1] == 2 &&
           board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 &&
           board[7][1] == 2 && board[8][1] == 0 && board[3][1] != 0 &&
           board[4][1] != 0 && board[5][1] != 0 && board[6][1] != 0)
    mountain++;
  return mountain;
}

int score_pureblock(char board[10][10]) {
  int block = 0;
  // 上
  // 黒
  if (board[1][1] == 0 && board[1][2] == 0 && board[1][4] == 1 &&
      board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 &&
      board[1][7] == 0 && board[1][8] == 0 && board[2][3] == 1 &&
      board[2][4] == 1 && board[2][5] == 1 && board[2][6] == 1)
    block--;
  // 白
  if (board[1][1] == 0 && board[1][2] == 0 && board[1][4] == 2 &&
      board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 &&
      board[1][7] == 0 && board[1][8] == 0 && board[2][3] == 2 &&
      board[2][4] == 2 && board[2][5] == 2 && board[2][6] == 2)
    block++;

  // 下
  // 黒
  if (board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 1 &&
      board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 &&
      board[8][7] == 0 && board[8][8] == 0 && board[7][3] == 1 &&
      board[7][4] == 1 && board[7][5] == 1 && board[7][6] == 1)
    block--;
  // 白
  if (board[8][1] == 0 && board[8][2] == 0 && board[8][4] == 2 &&
      board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 &&
      board[8][7] == 0 && board[8][8] == 0 && board[7][3] == 2 &&
      board[7][4] == 2 && board[7][5] == 2 && board[7][6] == 2)
    block++;

  // 右
  // 黒
  if (board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 1 &&
      board[4][8] == 1 && board[5][8] == 1 && board[6][8] == 1 &&
      board[7][8] == 0 && board[8][8] == 0 && board[3][7] == 1 &&
      board[4][7] == 1 && board[5][7] == 1 && board[6][7] == 1)
    block--;
  // 白
  if (board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 2 &&
      board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 &&
      board[7][8] == 0 && board[8][8] == 0 && board[3][7] == 2 &&
      board[4][7] == 2 && board[5][7] == 2 && board[6][7] == 2)
    block++;

  // 左
  // 黒
  if (board[1][1] == 0 && board[2][1] == 0 && board[3][1] == 1 &&
      board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1 &&
      board[7][1] == 0 && board[8][1] == 0 && board[3][1] == 1 &&
      board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1)
    block--;
  // 白
  if (board[1][1] == 0 && board[2][1] == 0 && board[3][1] == 2 &&
      board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 &&
      board[7][1] == 0 && board[8][1] == 0 && board[3][1] == 2 &&
      board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2)
    block++;
  return -block;
}

int score_purewing(char board[10][10]) {
  int block = 0;
  // 上
  // 黒
  if (board[1][1] == 0 &&
      ((board[1][2] == 1 && board[1][7] == 0) ||
       (board[1][2] == 0 && board[1][7] == 1)) &&
      board[1][4] == 1 && board[1][4] == 1 && board[1][5] == 1 &&
      board[1][6] == 1 && board[1][8] == 0 && board[2][3] == 1 &&
      board[2][4] == 1 && board[2][5] == 1 && board[2][6] == 1)
    block--;
  // 白
  if (board[1][1] == 0 &&
      ((board[1][2] == 2 && board[1][7] == 0) ||
       (board[1][2] == 0 && board[1][7] == 2)) &&
      board[1][4] == 2 && board[1][4] == 2 && board[1][5] == 2 &&
      board[1][6] == 2 && board[1][8] == 0 && board[2][3] == 2 &&
      board[2][4] == 2 && board[2][5] == 2 && board[2][6] == 2)
    block++;

  // 下
  // 黒
  if (board[8][1] == 0 &&
      ((board[8][2] == 1 && board[8][7] == 0) ||
       (board[8][2] == 0 && board[8][7] == 1)) &&
      board[8][4] == 1 && board[8][4] == 1 && board[8][5] == 1 &&
      board[8][6] == 1 && board[8][8] == 0 && board[7][3] == 1 &&
      board[7][4] == 1 && board[7][5] == 1 && board[7][6] == 1)
    block--;
  // 白
  if (board[8][1] == 0 &&
      ((board[2][8] == 2 && board[7][8] == 0) ||
       (board[2][8] == 0 && board[7][8] == 2)) &&
      board[8][4] == 2 && board[8][4] == 2 && board[8][5] == 2 &&
      board[8][6] == 2 && board[8][8] == 0 && board[7][3] == 2 &&
      board[7][4] == 2 && board[7][5] == 2 && board[7][6] == 2)
    block++;

  // 右
  // 黒
  if (board[1][8] == 0 &&
      ((board[2][8] == 1 && board[7][8] == 0) ||
       (board[2][8] == 0 && board[7][8] == 1)) &&
      board[3][8] == 1 && board[4][8] == 1 && board[5][8] == 1 &&
      board[6][8] == 1 && board[8][8] == 0 && board[3][7] == 1 &&
      board[4][7] == 1 && board[5][7] == 1 && board[6][7] == 1)
    block--;
  // 白
  if (board[1][8] == 0 &&
      ((board[2][8] == 2 && board[7][8] == 0) ||
       (board[2][8] == 0 && board[7][8] == 2)) &&
      board[3][8] == 2 && board[4][8] == 2 && board[5][8] == 2 &&
      board[6][8] == 2 && board[8][8] == 0 && board[3][7] == 2 &&
      board[4][7] == 2 && board[5][7] == 2 && board[6][7] == 2)
    block++;

  // 左
  // 黒
  if (board[1][1] == 0 &&
      ((board[2][1] == 1 && board[7][1] == 0) ||
       (board[2][1] == 0 && board[7][1] == 1)) &&
      board[3][1] == 1 && board[4][1] == 1 && board[5][1] == 1 &&
      board[6][1] == 1 && board[8][1] == 0 && board[3][1] == 1 &&
      board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1)
    block--;
  // 白
  if (board[1][1] == 0 &&
      ((board[2][1] == 2 && board[7][1] == 0) ||
       (board[2][1] == 0 && board[7][1] == 2)) &&
      board[3][1] == 2 && board[4][1] == 2 && board[5][1] == 2 &&
      board[6][1] == 2 && board[8][1] == 0 && board[3][1] == 2 &&
      board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2)
    block++;
  return -block;
}

int score_halfblock(char board[10][10]) {
  int block = 0;
  // 上
  // 黒
  if (board[1][1] == 0 && board[1][2] == 0 && board[1][3] == 1 &&
      board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 &&
      board[1][7] == 0 && board[1][8] == 0)
    block--;
  // 白
  if (board[1][1] == 0 && board[1][2] == 0 && board[1][3] == 2 &&
      board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 &&
      board[1][7] == 0 && board[1][8] == 0)
    block++;

  // 下
  // 黒
  if (board[8][1] == 0 && board[8][2] == 0 && board[8][3] == 1 &&
      board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 &&
      board[8][7] == 0 && board[8][8] == 0)
    block--;
  // 白
  if (board[8][1] == 0 && board[8][2] == 0 && board[8][3] == 2 &&
      board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 &&
      board[8][7] == 0 && board[8][8] == 0)
    block++;

  // 右
  // 黒
  if (board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 1 &&
      board[4][8] == 1 && board[5][8] == 1 && board[6][8] == 1 &&
      board[7][8] == 0 && board[8][8] == 0)
    block--;
  // 白
  if (board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 2 &&
      board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 &&
      board[7][8] == 0 && board[8][8] == 0)
    block++;

  // 左
  // 黒
  if (board[1][1] == 0 && board[2][1] == 0 && board[3][1] == 1 &&
      board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1 &&
      board[7][1] == 0 && board[8][1] == 0)
    block--;
  // 白
  if (board[1][1] == 0 && board[2][1] == 0 && board[3][1] == 2 &&
      board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 &&
      board[7][1] == 0 && board[8][1] == 0)
    block++;
  return block;
}

int score_countstone(char board[10][10]) {
  int score = 0;
  for (char i = 1; i < 9; ++i) {
    for (char j = 1; j < 9; ++j) {
      if (board[i][j] == 1) {
        score -= 2;
      } else if (board[i][j] == 2) {
        score += 2;
      }
    }
  }
  return score;
}

int score_badcorner(char board[10][10]) {
  int block = 0;
  // 上
  // 黒
  if (board[1][1] == 1 && board[1][2] == 2 && board[1][4] == 1 &&
      board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 &&
      board[1][7] == 1 && board[1][8] == 0)
    block--;
  else if (board[1][1] == 0 && board[1][2] == 1 && board[1][4] == 1 &&
           board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 &&
           board[1][7] == 2 && board[1][8] == 1)
    block--;
  // 白
  if (board[1][1] == 2 && board[1][2] == 1 && board[1][4] == 2 &&
      board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 &&
      board[1][7] == 2 && board[1][8] == 0)
    block++;
  else if (board[1][1] == 0 && board[1][2] == 2 && board[1][4] == 2 &&
           board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 &&
           board[1][7] == 1 && board[1][8] == 2)
    block++;

  // 下
  // 黒
  if (board[8][1] == 1 && board[8][2] == 2 && board[8][4] == 1 &&
      board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 &&
      board[8][7] == 1 && board[8][8] == 0)
    block--;
  else if (board[8][1] == 0 && board[8][2] == 1 && board[8][4] == 1 &&
           board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 &&
           board[8][7] == 2 && board[8][8] == 1)
    block--;
  // 白
  if (board[8][1] == 2 && board[8][2] == 1 && board[8][4] == 2 &&
      board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 &&
      board[8][7] == 2 && board[8][8] == 0)
    block++;
  else if (board[8][1] == 0 && board[8][2] == 2 && board[8][4] == 2 &&
           board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 &&
           board[8][7] == 1 && board[8][8] == 2)
    block++;

  // 右
  // 黒
  if (board[1][8] == 1 && board[2][8] == 2 && board[3][8] == 1 &&
      board[4][8] == 1 && board[5][8] == 1 && board[6][8] == 1 &&
      board[7][8] == 1 && board[8][8] == 0)
    block--;
  else if (board[1][8] == 0 && board[2][8] == 1 && board[3][8] == 1 &&
           board[4][8] == 1 && board[5][8] == 1 && board[6][8] == 1 &&
           board[7][8] == 2 && board[8][8] == 1)
    block--;
  // 白
  if (board[1][8] == 2 && board[2][8] == 1 && board[3][8] == 2 &&
      board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 &&
      board[7][8] == 2 && board[8][8] == 0)
    block++;
  if (board[1][8] == 0 && board[2][8] == 2 && board[3][8] == 2 &&
      board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 &&
      board[7][8] == 1 && board[8][8] == 2)
    block++;

  // 左
  // 黒
  if (board[1][1] == 1 && board[2][1] == 2 && board[3][1] == 1 &&
      board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1 &&
      board[7][1] == 1 && board[8][1] == 0)
    block--;
  if (board[1][1] == 0 && board[2][1] == 1 && board[3][1] == 1 &&
      board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1 &&
      board[7][1] == 2 && board[8][1] == 1)
    block--;
  // 白
  if (board[1][1] == 2 && board[2][1] == 1 && board[3][1] == 2 &&
      board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 &&
      board[7][1] == 2 && board[8][1] == 0)
    block++;
  else if (board[1][1] == 0 && board[2][1] == 2 && board[3][1] == 2 &&
           board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 &&
           board[7][1] == 1 && board[8][1] == 2)
    block++;
  return -block;
}

int score_goodcorner(char board[10][10]) {
  int score = 0;
  // 左上
  // 白
  if (board[1][1] == 2 && board[2][1] == 2)
    score += 6;
  if (board[1][1] == 2 && board[1][2] == 2)
    score += 6;
  if (board[1][1] == 2 && board[2][2] == 2)
    score += 7;
  // 黒
  if (board[1][1] == 1 && board[2][1] == 1)
    score -= 6;
  if (board[1][1] == 1 && board[1][2] == 1)
    score -= 6;
  if (board[1][1] == 1 && board[2][2] == 1)
    score -= 7;
  // 右上
  // 白
  if (board[1][8] == 2 && board[2][8] == 2)
    score += 6;
  if (board[1][8] == 2 && board[1][7] == 2)
    score += 6;
  if (board[1][8] == 2 && board[2][7] == 2)
    score += 7;
  // 黒
  if (board[1][8] == 1 && board[2][8] == 1)
    score -= 6;
  if (board[1][8] == 1 && board[1][7] == 1)
    score -= 6;
  if (board[1][8] == 1 && board[2][7] == 1)
    score -= 7;
  // 左下
  // 白
  if (board[8][1] == 2 && board[7][1] == 2)
    score += 6;
  if (board[8][1] == 2 && board[8][2] == 2)
    score += 6;
  if (board[8][1] == 2 && board[7][2] == 2)
    score += 7;
  // 黒
  if (board[8][1] == 1 && board[7][1] == 1)
    score -= 6;
  if (board[8][1] == 1 && board[8][2] == 1)
    score -= 6;
  if (board[8][1] == 1 && board[7][2] == 1)
    score -= 7;
  // 右下
  // 白
  if (board[8][8] == 2 && board[8][7] == 2)
    score += 6;
  if (board[8][8] == 2 && board[7][8] == 2)
    score += 6;
  if (board[8][8] == 2 && board[7][7] == 2)
    score += 7;
  // 黒
  if (board[8][8] == 1 && board[8][7] == 1)
    score -= 6;
  if (board[8][8] == 1 && board[7][8] == 1)
    score -= 6;
  if (board[8][8] == 1 && board[7][7] == 1)
    score -= 7;
  // 左上
  // 白
  if (board[1][1] != 0 && board[2][1] == 2)
    score += 9;
  if (board[1][1] != 0 && board[1][2] == 2)
    score += 9;
  if (board[1][1] != 0 && board[2][2] == 2)
    score += 11;
  // 黒
  if (board[1][1] != 0 && board[2][1] == 1)
    score -= 9;
  if (board[1][1] != 0 && board[1][2] == 1)
    score -= 9;
  if (board[1][1] != 0 && board[2][2] == 1)
    score -= 11;
  // 右上
  // 白
  if (board[1][8] != 0 && board[2][8] == 2)
    score += 9;
  if (board[1][8] != 0 && board[1][7] == 2)
    score += 9;
  if (board[1][8] != 0 && board[2][7] == 2)
    score += 11;
  // 黒
  if (board[1][8] != 0 && board[2][8] == 1)
    score -= 9;
  if (board[1][8] != 0 && board[1][7] == 1)
    score -= 9;
  if (board[1][8] != 0 && board[2][7] == 1)
    score -= 11;
  // 左下
  // 白
  if (board[8][1] != 0 && board[7][1] == 2)
    score += 9;
  if (board[8][1] != 0 && board[8][2] == 2)
    score += 9;
  if (board[8][1] != 0 && board[7][2] == 2)
    score += 11;
  // 黒
  if (board[8][1] != 0 && board[7][1] == 1)
    score -= 9;
  if (board[8][1] != 1 && board[8][2] == 1)
    score -= 9;
  if (board[8][1] != 0 && board[7][2] == 1)
    score -= 11;
  // 右下
  // 白
  if (board[8][8] != 0 && board[8][7] == 2)
    score += 9;
  if (board[8][8] != 0 && board[7][8] == 2)
    score += 9;
  if (board[8][8] != 0 && board[7][7] == 2)
    score += 11;
  // 黒
  if (board[8][8] != 0 && board[8][7] == 1)
    score -= 9;
  if (board[8][8] != 0 && board[7][8] == 1)
    score -= 9;
  if (board[8][8] != 0 && board[7][7] == 1)
    score -= 11;

  return score;
}

int score_badside(char board[10][10]) {
  int block = 0;
  // 上
  // 黒
  if (board[1][1] == 0 && board[1][8] == 0 &&
      ((board[1][2] == 1 && board[1][3] == 1 && board[1][6] == 0 &&
        board[1][7] == 0) ||
       (board[1][2] == 0 && board[1][3] == 0 && board[1][6] == 1 &&
        board[1][7] == 1)) &&
      board[1][2] == 0 && board[1][3] == 1 && board[1][4] == 1 &&
      board[1][5] == 1)
    block--;
  // 白
  if (board[1][1] == 0 && board[1][8] == 0 &&
      ((board[1][2] == 2 && board[1][3] == 2 && board[1][6] == 0 &&
        board[1][7] == 0) ||
       (board[1][2] == 0 && board[1][3] == 0 && board[1][6] == 2 &&
        board[1][7] == 2)) &&
      board[1][2] == 0 && board[1][3] == 2 && board[1][4] == 2 &&
      board[1][5] == 2)
    block++;

  // 下
  // 黒
  if (board[8][1] == 0 && board[8][8] == 0 &&
      ((board[8][2] == 1 && board[8][3] == 1 && board[8][6] == 0 &&
        board[8][7] == 0) ||
       (board[8][2] == 0 && board[8][3] == 0 && board[8][6] == 1 &&
        board[8][7] == 1)) &&
      board[1][2] == 0 && board[8][3] == 1 && board[8][4] == 1 &&
      board[8][5] == 1)
    block--;
  // 白
  if (board[8][1] == 0 && board[8][8] == 0 &&
      ((board[8][2] == 2 && board[8][3] == 2 && board[8][6] == 0 &&
        board[8][7] == 0) ||
       (board[8][2] == 0 && board[8][3] == 0 && board[8][6] == 2 &&
        board[8][7] == 2)) &&
      board[8][2] == 0 && board[8][3] == 2 && board[8][4] == 2 &&
      board[8][5] == 2)
    block++;

  // 右
  // 黒
  if (board[1][8] == 0 && board[8][8] == 0 &&
      ((board[2][8] == 1 && board[3][8] == 1 && board[6][8] == 0 &&
        board[7][8] == 0) ||
       (board[2][8] == 0 && board[3][8] == 0 && board[6][8] == 1 &&
        board[7][8] == 1)) &&
      board[2][8] == 0 && board[3][8] == 1 && board[4][8] == 1 &&
      board[5][8] == 1)
    block--;
  // 白
  if (board[1][8] == 0 && board[8][8] == 0 &&
      ((board[2][8] == 2 && board[3][8] == 2 && board[6][8] == 0 &&
        board[7][8] == 0) ||
       (board[2][8] == 0 && board[3][8] == 0 && board[6][8] == 1 &&
        board[7][8] == 2)) &&
      board[2][8] == 0 && board[3][8] == 2 && board[4][8] == 2 &&
      board[5][8] == 2)
    block++;

  // 左
  // 黒
  if (board[1][1] == 0 && board[8][1] == 0 &&
      ((board[2][1] == 1 && board[3][1] == 1 && board[6][1] == 0 &&
        board[7][1] == 0) ||
       (board[2][1] == 0 && board[3][1] == 0 && board[6][1] == 1 &&
        board[7][1] == 1)) &&
      board[2][1] == 0 && board[3][1] == 1 && board[4][1] == 1 &&
      board[5][1] == 1)
    block--;
  // 白
  if (board[1][1] == 0 && board[8][1] == 0 &&
      ((board[2][1] == 2 && board[3][1] == 2 && board[6][1] == 0 &&
        board[7][1] == 0) ||
       (board[2][1] == 0 && board[3][1] == 0 && board[6][1] == 1 &&
        board[7][1] == 2)) &&
      board[2][1] == 0 && board[3][1] == 2 && board[4][1] == 2 &&
      board[5][1] == 2)
    block++;

  return -block;
}

int score_side(char board[10][10]) {
  int block = 0;
  // 上
  // 黒
  if (board[1][1] != 0 && board[1][2] == 1 && board[1][3] == 1 &&
      board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 1 &&
      board[1][7] == 1 && board[1][8] != 0)
    block--;
  // 白
  if (board[1][1] != 0 && board[1][2] == 2 && board[1][3] == 2 &&
      board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 2 &&
      board[1][7] == 2 && board[1][8] == 0)
    block++;

  // 下
  // 黒
  if (board[8][1] != 0 && board[8][2] == 1 && board[8][3] == 1 &&
      board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 1 &&
      board[8][7] == 1 && board[8][8] != 0)
    block--;
  // 白
  if (board[8][1] != 0 && board[8][2] == 2 && board[8][3] == 2 &&
      board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 2 &&
      board[8][7] == 2 && board[8][8] != 0)
    block++;

  // 右
  // 黒
  if (board[1][8] != 0 && board[2][8] == 1 && board[3][8] == 1 &&
      board[4][8] == 1 && board[5][8] == 1 && board[6][8] == 1 &&
      board[7][8] == 1 && board[8][8] != 0)
    block--;
  // 白
  if (board[1][8] != 0 && board[2][8] == 2 && board[3][8] == 2 &&
      board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 2 &&
      board[7][8] == 2 && board[8][8] != 0)
    block++;

  // 左
  // 黒
  if (board[1][1] != 0 && board[2][1] == 1 && board[3][1] == 1 &&
      board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 1 &&
      board[7][1] == 1 && board[8][1] != 0)
    block--;
  // 白
  if (board[1][1] != 0 && board[2][1] == 2 && board[3][1] == 2 &&
      board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 2 &&
      board[7][1] == 2 && board[8][1] != 0)
    block++;
  return block;
}

int score_bomb(char board[10][10]) {
  int block = 0;
  // 上
  // 黒
  if (board[1][1] == 0 && board[1][2] == 0 && board[1][3] == 1 &&
      board[1][4] == 2 && board[1][5] == 2 && board[1][6] == 1 &&
      board[1][7] == 0 && board[1][8] == 0)
    block--;
  // 白
  if (board[1][1] == 0 && board[1][2] == 0 && board[1][3] == 2 &&
      board[1][4] == 1 && board[1][5] == 1 && board[1][6] == 2 &&
      board[1][7] == 0 && board[1][8] == 0)
    block++;

  // 下
  // 黒
  if (board[8][1] == 0 && board[8][2] == 0 && board[8][3] == 1 &&
      board[8][4] == 2 && board[8][5] == 2 && board[8][6] == 1 &&
      board[8][7] == 0 && board[8][8] == 0)
    block--;
  // 白
  if (board[8][1] == 0 && board[8][2] == 0 && board[8][3] == 2 &&
      board[8][4] == 1 && board[8][5] == 1 && board[8][6] == 2 &&
      board[8][7] == 0 && board[8][8] == 0)
    block++;

  // 右
  // 黒
  if (board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 1 &&
      board[4][8] == 2 && board[5][8] == 2 && board[6][8] == 1 &&
      board[7][8] == 0 && board[8][8] == 0)
    block--;
  // 白
  if (board[1][8] == 0 && board[2][8] == 0 && board[3][8] == 2 &&
      board[4][8] == 1 && board[5][8] == 1 && board[6][8] == 2 &&
      board[7][8] == 0 && board[8][8] == 0)
    block++;

  // 左
  // 黒
  if (board[1][1] == 0 && board[2][1] == 0 && board[3][1] == 1 &&
      board[4][1] == 2 && board[5][1] == 2 && board[6][1] == 1 &&
      board[7][1] == 0 && board[8][1] == 0)
    block--;
  // 白
  if (board[1][1] == 0 && board[2][1] == 0 && board[3][1] == 2 &&
      board[4][1] == 1 && board[5][1] == 1 && board[6][1] == 2 &&
      board[7][1] == 0 && board[8][1] == 0)
    block++;
  return -4 * block;
}

int countscore(char board[10][10], int turn, bool canput[10][10],
               int *playerrn) {
  int score = 0;
  score += 2 * score_stone(board);
  score += 5 * score_fixedstone(board);
  if (*playerrn == 2)
    score += score_putable(board, canput);
  if (*playerrn == 1)
    score -= score_putable(board, canput);
  return score;
}
