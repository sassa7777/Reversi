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

int score_stone(uint64_t playerboard, uint64_t oppenentboard) {
    int score = 0;
    for (char i = 0; i<8; ++i) {
        for (char j = 0; j<8; ++j) {
            uint64_t mask = 0x8000000000000000;
            if(playerboard & mask) score += scoreboard[i][j];
            else if(oppenentboard & mask) score -= scoreboard[i][j];
            mask = mask >> 1;
        }
    }
    return score;
}

uint64_t makelegalboard(uint64_t oppenentboard, uint64_t playerboard) {
    const uint64_t horizontalboard = (oppenentboard & 0x7e7e7e7e7e7e7e7e);
    const uint64_t verticalboard = (oppenentboard & 0x00FFFFFFFFFFFF00);
    const uint64_t allsideboard = (oppenentboard & 0x007e7e7e7e7e7e00);
    const uint64_t blankboard = ~(playerboard | oppenentboard);
    
    uint64_t tmp;
    uint64_t legalboard;
    
    //左
    tmp = horizontalboard & (playerboard << 1);
    tmp |= horizontalboard & (tmp << 1);
    tmp |= horizontalboard & (tmp << 1);
    tmp |= horizontalboard & (tmp << 1);
    tmp |= horizontalboard & (tmp << 1);
    tmp |= horizontalboard & (tmp << 1);
    legalboard = blankboard & (tmp << 1);
    
    //右
    tmp = horizontalboard & (playerboard >> 1);
    tmp |= horizontalboard & (tmp >> 1);
    tmp |= horizontalboard & (tmp >> 1);
    tmp |= horizontalboard & (tmp >> 1);
    tmp |= horizontalboard & (tmp >> 1);
    tmp |= horizontalboard & (tmp >> 1);
    legalboard |= blankboard & (tmp >> 1);

    //上
    tmp = verticalboard & (playerboard << 8);
    tmp |= verticalboard & (tmp << 8);
    tmp |= verticalboard & (tmp << 8);
    tmp |= verticalboard & (tmp << 8);
    tmp |= verticalboard & (tmp << 8);
    tmp |= verticalboard & (tmp << 8);
    legalboard |= blankboard & (tmp << 8);
    
    //下
    tmp = verticalboard & (playerboard >> 8);
    tmp |= verticalboard & (tmp >> 8);
    tmp |= verticalboard & (tmp >> 8);
    tmp |= verticalboard & (tmp >> 8);
    tmp |= verticalboard & (tmp >> 8);
    tmp |= verticalboard & (tmp >> 8);
    legalboard |= blankboard & (tmp >> 8);
    
    //右斜め上
    tmp = allsideboard & (playerboard << 7);
    tmp |= allsideboard & (tmp << 7);
    tmp |= allsideboard & (tmp << 7);
    tmp |= allsideboard & (tmp << 7);
    tmp |= allsideboard & (tmp << 7);
    tmp |= allsideboard & (tmp << 7);
    legalboard |= blankboard & (tmp << 7);
    
    //左斜め上
    tmp = allsideboard & (playerboard << 9);
    tmp |= allsideboard & (tmp << 9);
    tmp |= allsideboard & (tmp << 9);
    tmp |= allsideboard & (tmp << 9);
    tmp |= allsideboard & (tmp << 9);
    tmp |= allsideboard & (tmp << 9);
    legalboard |= blankboard & (tmp << 9);
    
    //右斜め下
    tmp = allsideboard & (playerboard >> 9);
    tmp |= allsideboard & (tmp >> 9);
    tmp |= allsideboard & (tmp >> 9);
    tmp |= allsideboard & (tmp >> 9);
    tmp |= allsideboard & (tmp >> 9);
    tmp |= allsideboard & (tmp >> 9);
    legalboard |= blankboard & (tmp >> 9);
    
    //左斜め下
    tmp = allsideboard & (playerboard >> 7);
    tmp |= allsideboard & (tmp >> 7);
    tmp |= allsideboard & (tmp >> 7);
    tmp |= allsideboard & (tmp >> 7);
    tmp |= allsideboard & (tmp >> 7);
    tmp |= allsideboard & (tmp >> 7);
    legalboard |= blankboard & (tmp >> 7);
    
    return legalboard;
}

int score_putable(uint64_t playerboard, uint64_t oppenentboard) {
    int score = 0;
    uint64_t legalboard;
    legalboard = makelegalboard(oppenentboard, playerboard);
    score += bitcount(legalboard);
    legalboard = makelegalboard(playerboard, oppenentboard);
    score -= bitcount(legalboard);
    return score;
}

int score_fixedstone(uint64_t playerboard) {
    int fixedstone = 0;

    //左上
    uint64_t mask = 0x8000000000000000;
    if(playerboard & mask) {
        while(playerboard & mask) {
            fixedstone++;
            mask = mask >> 1;
        }
        mask = 0x8000000000000000;
        while(playerboard & mask) {
            fixedstone++;
            mask = mask >> 8;
        }
    }
    //右上
    mask = 0x8000000000000000 >> 7;
    if(playerboard & mask) {
        while(playerboard & mask) {
            fixedstone++;
            mask = mask << 1;
        }
        mask = 0x8000000000000000;
        while(playerboard & mask) {
            fixedstone++;
            mask = mask >> 8;
        }
    }
    //左下
    mask = 0x8000000000000000 >> 56;
    if(playerboard & mask) {
        while(playerboard & mask) {
            fixedstone++;
            mask = mask >> 1;
        }
        mask = 0x8000000000000000;
        while(playerboard & mask) {
            fixedstone++;
            mask = mask << 8;
        }
    }
    //右下
    mask = 0x8000000000000000 >> 63;
    if(playerboard & mask) {
        while(playerboard & mask) {
            fixedstone++;
            mask = mask << 1;
        }
        mask = 0x8000000000000000;
        while(playerboard & mask) {
            fixedstone++;
            mask = mask << 8;
        }
    }
    //上一列
    if(playerboard & 0xFF00000000000000) fixedstone -= 8;
    //下一列
    if(playerboard & 0x00000000000000FF) fixedstone -= 8;
    //左一列
    if(playerboard & 0x8080808080808080) fixedstone -= 8;
    //右一列
    if(playerboard & 0x0101010101010101) fixedstone -= 8;
    return fixedstone;
}

bool is_all_oppenent(uint64_t playerboard) {
    return (playerboard == 0x0000000000000000);
}

int countscore(uint64_t playerboard, uint64_t oppenentboard) {
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
//    if(*playerrn == ) {
//        return bitcount(*oppenentboard)-bitcount(*playerboard);
//    } else {
//        return bitcount(*playerboard)-bitcount(*oppenentboard);
//    }
    if(is_all_oppenent(playerboard)) return -9999;
    if(nowIndex+DEPTH >= 60) return bitcount(playerboard)-bitcount(oppenentboard);
    if(nowIndex+DEPTH >= 44) return (3*score_stone(playerboard, oppenentboard))+(55*(score_fixedstone(playerboard)-score_fixedstone(oppenentboard)));
    return (3*score_stone(playerboard, oppenentboard))+(2*score_putable(playerboard, oppenentboard))+(55*(score_fixedstone(playerboard)-score_fixedstone(oppenentboard)));
    return bitcount(playerboard)-bitcount(oppenentboard);
}
