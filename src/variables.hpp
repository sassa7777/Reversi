//
//  variables.h
//  Reversi
//
//  Created by sasa on 2024/05/19.
//

#ifndef variables_h
#define variables_h

#include "reversi.hpp"

int end_search_stone_count;
bool first_reset;
int DEPTH;
int search_depth;
int Level;
int px, py;
int whitec;
int blackc;
int tmpx, tmpy;
int hint_x, hint_y;
uint64_t tmpbit;
int think_percent;
int think_count;
int AIplayer;
int nowTurn;
int nowIndex;
int firstDEPTH;
int afterIndex;
int now_model_idx;
int visited_nodes;
std::vector<bool> eval_num(15);
uint64_t legalboard;
uint64_t rev;
std::string play_record;
board b;
board_back b_back;

constexpr int mpc_depth[] {
    0, 0, 0, 1, 2, 2, 2, 3, 4, 4, 4, 4, 4, 5, 6, 5, 6
};

constexpr int MAX_MPC_DEPTH = 12;
std::vector<std::vector<double>> mpc_deviation(20, std::vector<double>(65)), mpc_mean(20, std::vector<double>(65));
constexpr double aa = 0.9390386342771441;
constexpr double bb = -3.150216720967751;
constexpr double cc = 8.376261451733438;
constexpr double dd = 0.9534455399047843;
constexpr double ee = -6.309886761153341;
constexpr double ff = 10.961360294000041;
constexpr double gg = -2.3863205426037464;

//constexpr double aa = 1.126330616982283;
//constexpr double bb = -5.963587268979202;
//constexpr double cc = 2.7907656975166057;
//constexpr double dd = 2.228763499230016;
//constexpr double ee = -3.276227419557784;
//constexpr double ff = 2.994283839182209;
//constexpr double gg = 1.8772336326411698;


//constexpr double mpc_deviation[] = {
//    0,
//    0,
//    0,
//    762.478,
//    691.448,
//    1001.63,
//    990.041,
//    967.152,
//    889.399,
//    1114,
//    1068.45,
//    1212.08,
//    1163.04,
//};

//constexpr double mpc_mean[] = {
//    0,
//    0,
//    0,
//    -18.6924,
//    -49.5777,
//    -773.413,
//    -106.465,
//    41.9353,
//    -60.3942,
//    -639.905,
//    -91.8874,
//    -598.626,
//    -112.366,
//};


//constexpr int moveorder[64][2] = {
//    {0,0}, {0,7}, {7,0}, {7,7}, {0,2}, {0,5}, {2,0}, {2,2}, {2,5}, {2,7}, {5,0}, {5,2}, {5,5}, {5,7}, {7,2}, {7,5}, {0,3}, {0,4}, {2,3}, {2,4}, {3,0}, {3,2}, {3,3}, {3,4}, {3,5}, {3,7}, {4,0}, {4,2}, {4,3}, {4,4}, {4,5}, {4,7}, {5,3}, {5,4}, {7,3}, {7,4}, {1,2}, {1,3}, {1,4}, {1,5}, {2,1}, {2,6}, {3,1}, {3,6}, {4,1}, {4,6}, {5,1}, {5,6}, {6,2}, {6,3}, {6,4}, {6,5}, {0,1}, {0,6}, {1,0}, {1,7}, {6,0}, {6,7}, {7,1}, {7,6}, {1,1}, {1,6}, {6,1}, {6,6}
//};

constexpr uint64_t moveorder_bit[64] = {
    0x8000000000000000ULL, 0x100000000000000ULL, 0x80ULL, 0x1ULL, 0x2000000000000000ULL, 288230376151711744, 140737488355328, 35184372088832, 4398046511104, 1099511627776, 8388608, 2097152, 262144, 65536, 32, 4, 1152921504606846976, 576460752303423488, 17592186044416, 8796093022208, 549755813888, 137438953472, 68719476736, 34359738368, 17179869184, 4294967296, 2147483648, 536870912, 268435456, 134217728, 67108864, 16777216, 1048576, 524288, 16, 8, 9007199254740992, 4503599627370496, 2251799813685248, 1125899906842624, 70368744177664, 2199023255552, 274877906944, 8589934592, 1073741824, 33554432, 4194304, 131072, 8192, 4096, 2048, 1024, 4611686018427387904, 144115188075855872, 36028797018963968, 281474976710656, 32768, 256, 64, 2, 18014398509481984, 562949953421312, 16384, 512
};

MAP transpose_table;
MAP former_transpose_table;

#endif /* variables_h */
