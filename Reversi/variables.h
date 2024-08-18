//
//  variables.h
//  Reversi
//
//  Created by sasa on 2024/05/19.
//

#ifndef variables_h
#define variables_h

#include "reversi.h"
#include "book/cow.h"
#include "book/tiger.h"

char DEPTH;
char search_depth;
char Level;
char px, py;
char whitec;
char blackc;
char tmpx, tmpy;
uint64_t tmpbit;
char think_percent;
char think_count;
char botplayer;
char nowTurn;
char nowIndex;
char firstDEPTH;
char afterIndex;
int visited_nodes;
uint64_t legalboard;
uint64_t rev;

board b;

constexpr char moveorder[64][2] = {
    {0,0}, {0,7}, {7,0}, {7,7}, {0,2}, {0,5}, {2,0}, {2,2}, {2,5}, {2,7}, {5,0}, {5,2}, {5,5}, {5,7}, {7,2}, {7,5}, {0,3}, {0,4}, {2,3}, {2,4}, {3,0}, {3,2}, {3,3}, {3,4}, {3,5}, {3,7}, {4,0}, {4,2}, {4,3}, {4,4}, {4,5}, {4,7}, {5,3}, {5,4}, {7,3}, {7,4}, {1,2}, {1,3}, {1,4}, {1,5}, {2,1}, {2,6}, {3,1}, {3,6}, {4,1}, {4,6}, {5,1}, {5,6}, {6,2}, {6,3}, {6,4}, {6,5}, {0,1}, {0,6}, {1,0}, {1,7}, {6,0}, {6,7}, {7,1}, {7,6}, {1,1}, {1,6}, {6,1}, {6,6}
};

constexpr uint64_t moveorder_bit[64] = {
    0x8000000000000000ULL, 0x100000000000000ULL, 0x80ULL, 0x1ULL, 0x2000000000000000ULL, 288230376151711744, 140737488355328, 35184372088832, 4398046511104, 1099511627776, 8388608, 2097152, 262144, 65536, 32, 4, 1152921504606846976, 576460752303423488, 17592186044416, 8796093022208, 549755813888, 137438953472, 68719476736, 34359738368, 17179869184, 4294967296, 2147483648, 536870912, 268435456, 134217728, 67108864, 16777216, 1048576, 524288, 16, 8, 9007199254740992, 4503599627370496, 2251799813685248, 1125899906842624, 70368744177664, 2199023255552, 274877906944, 8589934592, 1073741824, 33554432, 4194304, 131072, 8192, 4096, 2048, 1024, 4611686018427387904, 144115188075855872, 36028797018963968, 281474976710656, 32768, 256, 64, 2, 18014398509481984, 562949953421312, 16384, 512
};

//constexpr int scoreboard_score[5] = {
//    30, -1, -3, -12, -15
//};
//
//constexpr int scoreboard_score2[4] = {
//    30, -3, -12, -15
//};
//
//constexpr uint64_t scoreboard_weight[5] = {
//    0x8100000000000081ULL,
//    0x180018BDBD180018ULL,
//    0x003C424242423C00ULL,
//    0x4281000000008142ULL,
//    0x0042000000004200ULL,
//};
//
//constexpr uint64_t scoreboard_weight2[4] = {
//    0x8100000000000081ULL,
//    0x003C424242423C00ULL,
//    0x4281000000008142ULL,
//    0x0042000000004200ULL,
//};

ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, int> transpose_table_up;
ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, int> transpose_table_low;
ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, int> former_transpose_table_up;
ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, int> former_transpose_table_low;

#endif /* variables_h */
