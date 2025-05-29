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

constexpr inline uint64_t CALC_INDEX(const uint64_t B, const uint64_t t0, const uint64_t t1, const uint64_t t2, const uint64_t t3, const uint64_t t4, const uint64_t t5, const uint64_t t6, const uint64_t t7, const uint64_t t8, const uint64_t t9) {
    return ((B & t0) ? 1 : 0) * pow3[9] + ((B & t1) ? 1 : 0) * pow3[8] + ((B & t2) ? 1 : 0) * pow3[7] + ((B & t3) ? 1 : 0) * pow3[6] +
           ((B & t4) ? 1 : 0) * pow3[5] + ((B & t5) ? 1 : 0) * pow3[4] + ((B & t6) ? 1 : 0) * pow3[3] + ((B & t7) ? 1 : 0) * pow3[2] +
           ((B & t8) ? 1 : 0) * pow3[1] + ((B & t9) ? 1 : 0) * pow3[0];
}

constexpr inline uint64_t CALC_INDEX(const uint64_t B, const uint64_t t1, const uint64_t t2, const uint64_t t3, const uint64_t t4, const uint64_t t5, const uint64_t t6, const uint64_t t7, const uint64_t t8, const uint64_t t9) {
    return ((B & t1) ? 1 : 0) * pow3[8] + ((B & t2) ? 1 : 0) * pow3[7] + ((B & t3) ? 1 : 0) * pow3[6] + ((B & t4) ? 1 : 0) * pow3[5] +
           ((B & t5) ? 1 : 0) * pow3[4] + ((B & t6) ? 1 : 0) * pow3[3] + ((B & t7) ? 1 : 0) * pow3[2] + ((B & t8) ? 1 : 0) * pow3[1] +
           ((B & t9) ? 1 : 0) * pow3[0];
}

constexpr inline uint64_t CALC_INDEX(const uint64_t B, const uint64_t t2, const uint64_t t3, const uint64_t t4, const uint64_t t5, const uint64_t t6, const uint64_t t7, const uint64_t t8, const uint64_t t9) {
    return ((B & t2) ? 1 : 0) * pow3[7] + ((B & t3) ? 1 : 0) * pow3[6] + ((B & t4) ? 1 : 0) * pow3[5] + ((B & t5) ? 1 : 0) * pow3[4] +
           ((B & t6) ? 1 : 0) * pow3[3] + ((B & t7) ? 1 : 0) * pow3[2] + ((B & t8) ? 1 : 0) * pow3[1] + ((B & t9) ? 1 : 0) * pow3[0];
}

constexpr inline uint64_t CALC_INDEX(const uint64_t B, const uint64_t t3, const uint64_t t4, const uint64_t t5, const uint64_t t6, const uint64_t t7, const uint64_t t8, const uint64_t t9) {
    return ((B & t3) ? 1 : 0) * pow3[6] + ((B & t4) ? 1 : 0) * pow3[5] + ((B & t5) ? 1 : 0) * pow3[4] + ((B & t6) ? 1 : 0) * pow3[3] +
           ((B & t7) ? 1 : 0) * pow3[2] + ((B & t8) ? 1 : 0) * pow3[1] + ((B & t9) ? 1 : 0) * pow3[0];
}

constexpr inline uint64_t CALC_INDEX(const uint64_t B, const uint64_t t4, const uint64_t t5, const uint64_t t6, const uint64_t t7, const uint64_t t8, const uint64_t t9) {
    return ((B & t4) ? 1 : 0) * pow3[5] + ((B & t5) ? 1 : 0) * pow3[4] + ((B & t6) ? 1 : 0) * pow3[3] + ((B & t7) ? 1 : 0) * pow3[2] +
           ((B & t8) ? 1 : 0) * pow3[1] + ((B & t9) ? 1 : 0) * pow3[0];
}

constexpr inline uint64_t CALC_INDEX(const uint64_t B, const uint64_t t5, const uint64_t t6, const uint64_t t7, const uint64_t t8, const uint64_t t9) {
    return ((B & t5) ? 1 : 0) * pow3[4] + ((B & t6) ? 1 : 0) * pow3[3] + ((B & t7) ? 1 : 0) * pow3[2] + ((B & t8) ? 1 : 0) * pow3[1] +
           ((B & t9) ? 1 : 0) * pow3[0];
}

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
    

    a += pattern_arr[eval_num].diagonal8[b.index_p.diagonal8_0 + b.index_o.diagonal8_0 * 2];
    a += pattern_arr[eval_num].diagonal8[b.index_p.diagonal8_1 + b.index_o.diagonal8_1 * 2];

    a += pattern_arr[eval_num].diagonal7[b.index_p.diagonal7_0 + b.index_o.diagonal7_0 * 2];
    a += pattern_arr[eval_num].diagonal7[b.index_p.diagonal7_1 + b.index_o.diagonal7_1 * 2];
    a += pattern_arr[eval_num].diagonal7[b.index_p.diagonal7_2 + b.index_o.diagonal7_2 * 2];
    a += pattern_arr[eval_num].diagonal7[b.index_p.diagonal7_3 + b.index_o.diagonal7_3 * 2];

    a += pattern_arr[eval_num].diagonal6[b.index_p.diagonal6_0 + b.index_o.diagonal6_0 * 2];
    a += pattern_arr[eval_num].diagonal6[b.index_p.diagonal6_1 + b.index_o.diagonal6_1 * 2];
    a += pattern_arr[eval_num].diagonal6[b.index_p.diagonal6_2 + b.index_o.diagonal6_2 * 2];
    a += pattern_arr[eval_num].diagonal6[b.index_p.diagonal6_3 + b.index_o.diagonal6_3 * 2];

    a += pattern_arr[eval_num].diagonal5[b.index_p.diagonal5_0 + b.index_o.diagonal5_0 * 2];
    a += pattern_arr[eval_num].diagonal5[b.index_p.diagonal5_1 + b.index_o.diagonal5_1 * 2];
    a += pattern_arr[eval_num].diagonal5[b.index_p.diagonal5_2 + b.index_o.diagonal5_2 * 2];
    a += pattern_arr[eval_num].diagonal5[b.index_p.diagonal5_3 + b.index_o.diagonal5_3 * 2];

    a += pattern_arr[eval_num].edge_2x[b.index_p.edge_2x_0 + b.index_o.edge_2x_0 * 2];
    a += pattern_arr[eval_num].edge_2x[b.index_p.edge_2x_1 + b.index_o.edge_2x_1 * 2];
    a += pattern_arr[eval_num].edge_2x[b.index_p.edge_2x_2 + b.index_o.edge_2x_2 * 2];
    a += pattern_arr[eval_num].edge_2x[b.index_p.edge_2x_3 + b.index_o.edge_2x_3 * 2];

    a += pattern_arr[eval_num].h_v_2[b.index_p.h_v_2_0 + b.index_o.h_v_2_0 * 2];
    a += pattern_arr[eval_num].h_v_2[b.index_p.h_v_2_1 + b.index_o.h_v_2_1 * 2];
    a += pattern_arr[eval_num].h_v_2[b.index_p.h_v_2_2 + b.index_o.h_v_2_2 * 2];
    a += pattern_arr[eval_num].h_v_2[b.index_p.h_v_2_3 + b.index_o.h_v_2_3 * 2];

    a += pattern_arr[eval_num].h_v_3[b.index_p.h_v_3_0 + b.index_o.h_v_3_0 * 2];
    a += pattern_arr[eval_num].h_v_3[b.index_p.h_v_3_1 + b.index_o.h_v_3_1 * 2];
    a += pattern_arr[eval_num].h_v_3[b.index_p.h_v_3_2 + b.index_o.h_v_3_2 * 2];
    a += pattern_arr[eval_num].h_v_3[b.index_p.h_v_3_3 + b.index_o.h_v_3_3 * 2];

    a += pattern_arr[eval_num].h_v_4[b.index_p.h_v_4_0 + b.index_o.h_v_4_0 * 2];
    a += pattern_arr[eval_num].h_v_4[b.index_p.h_v_4_1 + b.index_o.h_v_4_1 * 2];
    a += pattern_arr[eval_num].h_v_4[b.index_p.h_v_4_2 + b.index_o.h_v_4_2 * 2];
    a += pattern_arr[eval_num].h_v_4[b.index_p.h_v_4_3 + b.index_o.h_v_4_3 * 2];

    a += pattern_arr[eval_num].corner_3x3[b.index_p.corner_3x3_0 + b.index_o.corner_3x3_0 * 2];
    a += pattern_arr[eval_num].corner_3x3[b.index_p.corner_3x3_1 + b.index_o.corner_3x3_1 * 2];
    a += pattern_arr[eval_num].corner_3x3[b.index_p.corner_3x3_2 + b.index_o.corner_3x3_2 * 2];
    a += pattern_arr[eval_num].corner_3x3[b.index_p.corner_3x3_3 + b.index_o.corner_3x3_3 * 2];

    a += pattern_arr[eval_num].edge_x_side[b.index_p.edge_x_side_0 + b.index_o.edge_x_side_0 * 2];
    a += pattern_arr[eval_num].edge_x_side[b.index_p.edge_x_side_1 + b.index_o.edge_x_side_1 * 2];
    a += pattern_arr[eval_num].edge_x_side[b.index_p.edge_x_side_2 + b.index_o.edge_x_side_2 * 2];
    a += pattern_arr[eval_num].edge_x_side[b.index_p.edge_x_side_3 + b.index_o.edge_x_side_3 * 2];

    a += pattern_arr[eval_num].edge_block[b.index_p.edge_block_0 + b.index_o.edge_block_0 * 2];
    a += pattern_arr[eval_num].edge_block[b.index_p.edge_block_1 + b.index_o.edge_block_1 * 2];
    a += pattern_arr[eval_num].edge_block[b.index_p.edge_block_2 + b.index_o.edge_block_2 * 2];
    a += pattern_arr[eval_num].edge_block[b.index_p.edge_block_3 + b.index_o.edge_block_3 * 2];

    a += pattern_arr[eval_num].triangle[b.index_p.triangle_0 + b.index_o.triangle_0 * 2];
    a += pattern_arr[eval_num].triangle[b.index_p.triangle_1 + b.index_o.triangle_1 * 2];
    a += pattern_arr[eval_num].triangle[b.index_p.triangle_2 + b.index_o.triangle_2 * 2];
    a += pattern_arr[eval_num].triangle[b.index_p.triangle_3 + b.index_o.triangle_3 * 2];

    a += pattern_arr[eval_num].corner_2x5[b.index_p.corner_2x5_0 + b.index_o.corner_2x5_0 * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.corner_2x5_1 + b.index_o.corner_2x5_1 * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.corner_2x5_2 + b.index_o.corner_2x5_2 * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.corner_2x5_3 + b.index_o.corner_2x5_3 * 2];

    a += pattern_arr[eval_num].corner_2x5[b.index_p.corner_2x5_4 + b.index_o.corner_2x5_4 * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.corner_2x5_5 + b.index_o.corner_2x5_5 * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.corner_2x5_6 + b.index_o.corner_2x5_6 * 2];
    a += pattern_arr[eval_num].corner_2x5[b.index_p.corner_2x5_7 + b.index_o.corner_2x5_7 * 2];

    
    a += mobility_arr[eval_num][plegal * 36 + olegal];
//    
//    a += stone_arr[eval_num][P_cnt * 65 + O_cnt];
    
    return a;
}
