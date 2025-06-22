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
    
    
    a += pattern_arr[eval_num].diagonal8[b.index.indexes_1[0]];
    a += pattern_arr[eval_num].diagonal8[b.index.indexes_1[1]];
    
    a += pattern_arr[eval_num].diagonal7[b.index.indexes_1[2]];
    a += pattern_arr[eval_num].diagonal7[b.index.indexes_1[3]];
    a += pattern_arr[eval_num].diagonal7[b.index.indexes_1[4]];
    a += pattern_arr[eval_num].diagonal7[b.index.indexes_1[5]];
    
    a += pattern_arr[eval_num].diagonal6[b.index.indexes_1[6]];
    a += pattern_arr[eval_num].diagonal6[b.index.indexes_1[7]];
    a += pattern_arr[eval_num].diagonal6[b.index.indexes_1[8]];
    a += pattern_arr[eval_num].diagonal6[b.index.indexes_1[9]];
    
    a += pattern_arr[eval_num].diagonal5[b.index.indexes_1[10]];
    a += pattern_arr[eval_num].diagonal5[b.index.indexes_1[11]];
    a += pattern_arr[eval_num].diagonal5[b.index.indexes_1[12]];
    a += pattern_arr[eval_num].diagonal5[b.index.indexes_1[13]];
    
    a += pattern_arr[eval_num].edge_2x[b.index.indexes_1[14]];
    a += pattern_arr[eval_num].edge_2x[b.index.indexes_1[15]];
    a += pattern_arr[eval_num].edge_2x[b.index.indexes_1[16]];
    a += pattern_arr[eval_num].edge_2x[b.index.indexes_1[17]];
    
    a += pattern_arr[eval_num].h_v_2[b.index.indexes_1[18]];
    a += pattern_arr[eval_num].h_v_2[b.index.indexes_1[19]];
    a += pattern_arr[eval_num].h_v_2[b.index.indexes_1[20]];
    a += pattern_arr[eval_num].h_v_2[b.index.indexes_1[21]];
    
    a += pattern_arr[eval_num].h_v_3[b.index.indexes_1[22]];
    a += pattern_arr[eval_num].h_v_3[b.index.indexes_1[23]];
    a += pattern_arr[eval_num].h_v_3[b.index.indexes_1[24]];
    a += pattern_arr[eval_num].h_v_3[b.index.indexes_1[25]];
    
    a += pattern_arr[eval_num].h_v_4[b.index.indexes_1[26]];
    a += pattern_arr[eval_num].h_v_4[b.index.indexes_1[27]];
    a += pattern_arr[eval_num].h_v_4[b.index.indexes_1[28]];
    a += pattern_arr[eval_num].h_v_4[b.index.indexes_1[29]];
    
    a += pattern_arr[eval_num].corner_3x3[b.index.indexes_1[30]];
    a += pattern_arr[eval_num].corner_3x3[b.index.indexes_1[31]];
    a += pattern_arr[eval_num].corner_3x3[b.index.indexes_1[32]];
    a += pattern_arr[eval_num].corner_3x3[b.index.indexes_1[33]];
    
    a += pattern_arr[eval_num].edge_x_side[b.index.indexes_1[34]];
    a += pattern_arr[eval_num].edge_x_side[b.index.indexes_1[35]];
    a += pattern_arr[eval_num].edge_x_side[b.index.indexes_1[36]];
    a += pattern_arr[eval_num].edge_x_side[b.index.indexes_1[37]];
    
    a += pattern_arr[eval_num].edge_block[b.index.indexes_1[38]];
    a += pattern_arr[eval_num].edge_block[b.index.indexes_1[39]];
    a += pattern_arr[eval_num].edge_block[b.index.indexes_1[40]];
    a += pattern_arr[eval_num].edge_block[b.index.indexes_1[41]];
    
    a += pattern_arr[eval_num].triangle[b.index.indexes_1[42]];
    a += pattern_arr[eval_num].triangle[b.index.indexes_1[43]];
    a += pattern_arr[eval_num].triangle[b.index.indexes_1[44]];
    a += pattern_arr[eval_num].triangle[b.index.indexes_1[45]];
    
    a += pattern_arr[eval_num].corner_2x5[b.index.indexes_1[46]];
    a += pattern_arr[eval_num].corner_2x5[b.index.indexes_1[47]];
    a += pattern_arr[eval_num].corner_2x5[b.index.indexes_1[48]];
    a += pattern_arr[eval_num].corner_2x5[b.index.indexes_1[49]];
    a += pattern_arr[eval_num].corner_2x5[b.index.indexes_1[50]];
    a += pattern_arr[eval_num].corner_2x5[b.index.indexes_1[51]];
    a += pattern_arr[eval_num].corner_2x5[b.index.indexes_1[52]];
    a += pattern_arr[eval_num].corner_2x5[b.index.indexes_1[53]];
    
    a = b.player ? -a : a;
    
    a += mobility_arr[eval_num][plegal * 36 + olegal];
    
    //    a += stone_arr[eval_num][P_cnt * 65 + O_cnt];
    
    return a;
}
