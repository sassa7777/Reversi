#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <Siv3D.hpp>
#include <zstd.h>
#include "bit.hpp"

using namespace std;
using bitboard = pair<uint64_t, uint64_t>;

#define n_patterns 14
#define use_book false

Pattern_Eval pattern_arr[15];
static int16_t mobility_arr[15][36 * 36];
static int16_t stone_arr[15][65 * 65];

inline int fast_next_int(char*& p) {
    int num = 0;
    int sign = 1;
    while (*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t')
        ++p;
    if (*p == '-') {
        sign = -1;
        ++p;
    }
    while (*p >= '0' && *p <= '9') {
        num = num * 10 + (*p - '0');
        ++p;
    }
    return num * sign;
}

inline void evaluate_init(String model_path, int eval_num) {
    FILE* fp = fopen(FileSystem::RelativePath(Resource(model_path)).narrow().c_str(), "rb");
    fseek(fp, 0, SEEK_END);
    size_t compressed_size = ftell(fp);
    rewind(fp);
    vector<char> compressed_data(compressed_size);
    fread(compressed_data.data(), 1, compressed_size, fp);
    fclose(fp);
    
    size_t decompressed_size = ZSTD_getFrameContentSize(compressed_data.data(), compressed_size);
    vector<char> buffer(decompressed_size);
    ZSTD_decompress(buffer.data(), decompressed_size, compressed_data.data(), compressed_size);
    
    char* p = buffer.data();
    for (auto &x : pattern_arr[eval_num].diagonal8) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].diagonal7) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].diagonal6) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].diagonal5) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].edge_2x) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].h_v_2) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].h_v_3) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].h_v_4) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].corner_3x3) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].edge_x_side) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].edge_block) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].triangle) {
        x = fast_next_int(p);
    }
    for (auto &x : pattern_arr[eval_num].corner_2x5) {
        x = fast_next_int(p);
    }
    for (auto &x : mobility_arr[eval_num]) {
        x = fast_next_int(p);
    }
//    for (auto &x : stone_arr[eval_num]) {
//        x = fast_next_int(p);
//    }
}

#define evaluate_moveorder(b) evaluate(b)

inline int evaluate(const board &b) noexcept {
    
    if (b.p == 0) [[unlikely]] return -32768;
    if (b.o == 0) [[unlikely]] return 32768;
    int plegal = popcnt_u64(makelegalboard(b));
    board b2 = b.flipped();
    int olegal = popcnt_u64(makelegalboard(b2));
    if (plegal == 0 && olegal == 0) return 256 * (popcnt_u64(b.p) - popcnt_u64(b.o));
    int P_cnt = popcnt_u64(b.p);
    int O_cnt = popcnt_u64(b.o);
    int eval_num = (P_cnt + O_cnt - 5) / 4;
    int32_t a = 0;
    

    a += pattern_arr[eval_num].diagonal8[b.index_p.indexes_1[0] + b.index_o.indexes_1[0] * 2];
    a += pattern_arr[eval_num].diagonal8[b.index_p.indexes_1[1] + b.index_o.indexes_1[1] * 2];

    a += pattern_arr[eval_num].diagonal7[b.index_p.indexes_1[2] + b.index_o.indexes_1[2] * 2];
    a += pattern_arr[eval_num].diagonal7[b.index_p.indexes_1[3] + b.index_o.indexes_1[3] * 2];
    a += pattern_arr[eval_num].diagonal7[b.index_p.indexes_1[4] + b.index_o.indexes_1[4] * 2];
    a += pattern_arr[eval_num].diagonal7[b.index_p.indexes_1[5] + b.index_o.indexes_1[5] * 2];

    a += pattern_arr[eval_num].diagonal6[b.index_p.indexes_1[6] + b.index_o.indexes_1[6] * 2];
    a += pattern_arr[eval_num].diagonal6[b.index_p.indexes_1[7] + b.index_o.indexes_1[7] * 2];
    a += pattern_arr[eval_num].diagonal6[b.index_p.indexes_1[8] + b.index_o.indexes_1[8] * 2];
    a += pattern_arr[eval_num].diagonal6[b.index_p.indexes_1[9] + b.index_o.indexes_1[9] * 2];

    a += pattern_arr[eval_num].diagonal5[b.index_p.indexes_1[10] + b.index_o.indexes_1[10] * 2];
    a += pattern_arr[eval_num].diagonal5[b.index_p.indexes_1[11] + b.index_o.indexes_1[11] * 2];
    a += pattern_arr[eval_num].diagonal5[b.index_p.indexes_1[12] + b.index_o.indexes_1[12] * 2];
    a += pattern_arr[eval_num].diagonal5[b.index_p.indexes_1[13] + b.index_o.indexes_1[13] * 2];

    a += pattern_arr[eval_num].edge_2x[b.index_p.indexes_1[14] + b.index_o.indexes_1[14] * 2];
    a += pattern_arr[eval_num].edge_2x[b.index_p.indexes_1[15] + b.index_o.indexes_1[15] * 2];
    a += pattern_arr[eval_num].edge_2x[b.index_p.indexes_1[16] + b.index_o.indexes_1[16] * 2];
    a += pattern_arr[eval_num].edge_2x[b.index_p.indexes_1[17] + b.index_o.indexes_1[17] * 2];

    a += pattern_arr[eval_num].h_v_2[b.index_p.indexes_1[18] + b.index_o.indexes_1[18] * 2];
    a += pattern_arr[eval_num].h_v_2[b.index_p.indexes_1[19] + b.index_o.indexes_1[19] * 2];
    a += pattern_arr[eval_num].h_v_2[b.index_p.indexes_1[20] + b.index_o.indexes_1[20] * 2];
    a += pattern_arr[eval_num].h_v_2[b.index_p.indexes_1[21] + b.index_o.indexes_1[21] * 2];

    a += pattern_arr[eval_num].h_v_3[b.index_p.indexes_1[22] + b.index_o.indexes_1[22] * 2];
    a += pattern_arr[eval_num].h_v_3[b.index_p.indexes_1[23] + b.index_o.indexes_1[23] * 2];
    a += pattern_arr[eval_num].h_v_3[b.index_p.indexes_1[24] + b.index_o.indexes_1[24] * 2];
    a += pattern_arr[eval_num].h_v_3[b.index_p.indexes_1[25] + b.index_o.indexes_1[25] * 2];

    a += pattern_arr[eval_num].h_v_4[b.index_p.indexes_1[26] + b.index_o.indexes_1[26] * 2];
    a += pattern_arr[eval_num].h_v_4[b.index_p.indexes_1[27] + b.index_o.indexes_1[27] * 2];
    a += pattern_arr[eval_num].h_v_4[b.index_p.indexes_1[28] + b.index_o.indexes_1[28] * 2];
    a += pattern_arr[eval_num].h_v_4[b.index_p.indexes_1[29] + b.index_o.indexes_1[29] * 2];

    a += pattern_arr[eval_num].corner_3x3[b.index_p.indexes_1[30] + b.index_o.indexes_1[30] * 2];
    a += pattern_arr[eval_num].corner_3x3[b.index_p.indexes_1[31] + b.index_o.indexes_1[31] * 2];
    a += pattern_arr[eval_num].corner_3x3[b.index_p.indexes_1[32] + b.index_o.indexes_1[32] * 2];
    a += pattern_arr[eval_num].corner_3x3[b.index_p.indexes_1[33] + b.index_o.indexes_1[33] * 2];

    a += pattern_arr[eval_num].edge_x_side[b.index_p.indexes_1[34] + b.index_o.indexes_1[34] * 2];
    a += pattern_arr[eval_num].edge_x_side[b.index_p.indexes_1[35] + b.index_o.indexes_1[35] * 2];
    a += pattern_arr[eval_num].edge_x_side[b.index_p.indexes_1[36] + b.index_o.indexes_1[36] * 2];
    a += pattern_arr[eval_num].edge_x_side[b.index_p.indexes_1[37] + b.index_o.indexes_1[37] * 2];

    a += pattern_arr[eval_num].edge_block[b.index_p.indexes_1[38] + b.index_o.indexes_1[38] * 2];
    a += pattern_arr[eval_num].edge_block[b.index_p.indexes_1[39] + b.index_o.indexes_1[39] * 2];
    a += pattern_arr[eval_num].edge_block[b.index_p.indexes_1[40] + b.index_o.indexes_1[40] * 2];
    a += pattern_arr[eval_num].edge_block[b.index_p.indexes_1[41] + b.index_o.indexes_1[41] * 2];

    a += pattern_arr[eval_num].triangle[b.index_p.indexes_1[42] + b.index_o.indexes_1[42] * 2];
    a += pattern_arr[eval_num].triangle[b.index_p.indexes_1[43] + b.index_o.indexes_1[43] * 2];
    a += pattern_arr[eval_num].triangle[b.index_p.indexes_1[44] + b.index_o.indexes_1[44] * 2];
    a += pattern_arr[eval_num].triangle[b.index_p.indexes_1[45] + b.index_o.indexes_1[45] * 2];

    a += pattern_arr[eval_num].corner_2x5[b.index_p.indexes_1[46] + b.index_o.indexes_1[46] * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.indexes_1[47] + b.index_o.indexes_1[47] * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.indexes_1[48] + b.index_o.indexes_1[48] * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.indexes_1[49] + b.index_o.indexes_1[49] * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.indexes_1[50] + b.index_o.indexes_1[50] * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.indexes_1[51] + b.index_o.indexes_1[51] * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.indexes_1[52] + b.index_o.indexes_1[52] * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.indexes_1[53] + b.index_o.indexes_1[53] * 2];

    
    a += mobility_arr[eval_num][plegal * 36 + olegal];
    
//    a += stone_arr[eval_num][P_cnt * 65 + O_cnt];
    
    return a;
}
