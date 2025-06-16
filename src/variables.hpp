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

constexpr int mpc_depth[] {
    0, 0, 0, 1, 2, 1, 2, 3, 4, 3, 4, 3, 4, 5, 6, 5, 6, 5, 6
};

std::vector<std::vector<double>> mpc_deviation(20, std::vector<double>(65)), mpc_mean(20, std::vector<double>(65));
//constexpr double aa = 0.9362459622030622;
//constexpr double bb = -4.557132868696562;
//constexpr double cc = 1.9800312890471103;
//constexpr double dd = 3.75808594982661;
//constexpr double ee = -6.844843377878804;
//constexpr double ff = 7.059098357166785;
//constexpr double gg = 4.8030489141217265;

constexpr double aa = 5.372811237744748;
constexpr double bb = 18.354448593978653;
constexpr double cc = -6.145473602836057;
constexpr double dd = -11.989880888846729;
constexpr double ee = 52.906095523368684;
constexpr double ff = 163.40855945804338;
constexpr double gg = 24.888305837389666;

//constexpr double aa = 0.7308488452189136;
//constexpr double bb = -4.5708322989025865;
//constexpr double cc = 1.096319765006055;
//constexpr double dd = -0.8362251801219095;
//constexpr double ee = 4.610017383697701;
//constexpr double ff = 3.818582623595395;
//constexpr double gg = 1.8775013664098447;


constexpr uint64_t moveorder_bit[64] = {
    0x8000000000000000ULL, 0x100000000000000ULL, 0x80ULL, 0x1ULL, 0x2000000000000000ULL, 288230376151711744, 140737488355328, 35184372088832, 4398046511104, 1099511627776, 8388608, 2097152, 262144, 65536, 32, 4, 1152921504606846976, 576460752303423488, 17592186044416, 8796093022208, 549755813888, 137438953472, 68719476736, 34359738368, 17179869184, 4294967296, 2147483648, 536870912, 268435456, 134217728, 67108864, 16777216, 1048576, 524288, 16, 8, 9007199254740992, 4503599627370496, 2251799813685248, 1125899906842624, 70368744177664, 2199023255552, 274877906944, 8589934592, 1073741824, 33554432, 4194304, 131072, 8192, 4096, 2048, 1024, 4611686018427387904, 144115188075855872, 36028797018963968, 281474976710656, 32768, 256, 64, 2, 18014398509481984, 562949953421312, 16384, 512
};

#define table_size 16777216 // 2^24
#define table_mask (table_size - 1)

table_data search_table[table_size];
table_data former_search_table[table_size];
spinlock table_locks[table_size];

#endif /* variables_h */
