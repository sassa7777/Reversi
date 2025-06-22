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
bool use_mpc;
double mpc_p;
uint64_t legalboard;
uint64_t rev;
std::string play_record;
board b;
board_back b_back;

//multi prob cut based on egaroucid https://github.com/Nyanyan/Egaroucid
constexpr double aa_mid = 3.6498594135461473;
constexpr double bb_mid = 16.328790201074472;
constexpr double cc_mid = -4.104748049569666;
constexpr double dd_mid = -1.0933906581427852;
constexpr double ee_mid = 53.35363110305511;
constexpr double ff_mid = 170.13035450582603;
constexpr double gg_mid = 172.49384089638207;

constexpr double aa_end = 9.864501175177745;
constexpr double bb_end = -24.22689316517559;
constexpr double cc_end = 36.860730860309445;
constexpr double dd_end = -338.68521250573633;
constexpr double ee_end = 1136.182272092143;
constexpr double ff_end = 9.803237207924484;

constexpr int mpc_depth[] {
    0, 0, 0, 1, 2, 1, 2, 3, 4, 3, 4, 3, 4, 5, 6, 5, 6, 5, 6, 7, 8, 7, 8, 7, 8, 9, 10, 9, 10, 9, 10, 11
};

constexpr std::array<std::array<double, 21>, 65> init_mpc_mid_data() {
    std::array<std::array<double, 21>, 65> tmp;
    for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 65; ++j) {
            double A = aa_mid*(double)j/64.0 + bb_mid*(double)mpc_depth[i]/60.0 + (double)cc_mid*i/60.0;
            double dev = dd_mid*A*A*A + ee_mid*A*A + ff_mid*A + gg_mid;
            tmp[j][i] = dev;
        }
    }
    return tmp;
}

constexpr std::array<std::array<double, 31>, 65> init_mpc_end_data() {
    std::array<std::array<double, 31>, 65> tmp;
    for (int i = 0; i < 31; ++i) {
        for (int j = 0; j < 65; ++j) {
            double A = aa_end*(double)j/64.0 + (double)bb_end*i/60.0;
            double dev = cc_end*A*A*A + dd_end*A*A + ee_end*A + ff_end;
            tmp[j][i] = dev;
        }
    }
    return tmp;
}

constexpr std::array<std::array<double, 21>, 65> mpc_data_mid = init_mpc_mid_data();
constexpr std::array<std::array<double, 31>, 65> mpc_data_end = init_mpc_end_data();

constexpr uint64_t moveorder_bit[64] = {
    0x8000000000000000ULL, 0x100000000000000ULL, 0x80ULL, 0x1ULL, 0x2000000000000000ULL, 288230376151711744, 140737488355328, 35184372088832, 4398046511104, 1099511627776, 8388608, 2097152, 262144, 65536, 32, 4, 1152921504606846976, 576460752303423488, 17592186044416, 8796093022208, 549755813888, 137438953472, 68719476736, 34359738368, 17179869184, 4294967296, 2147483648, 536870912, 268435456, 134217728, 67108864, 16777216, 1048576, 524288, 16, 8, 9007199254740992, 4503599627370496, 2251799813685248, 1125899906842624, 70368744177664, 2199023255552, 274877906944, 8589934592, 1073741824, 33554432, 4194304, 131072, 8192, 4096, 2048, 1024, 4611686018427387904, 144115188075855872, 36028797018963968, 281474976710656, 32768, 256, 64, 2, 18014398509481984, 562949953421312, 16384, 512
};

constexpr size_t table_size = 1UL << 24;
constexpr size_t table_mask = table_size - 1;

table_data search_table[table_size];
table_data former_search_table[table_size];
spinlock table_locks[table_size];

#endif /* variables_h */
