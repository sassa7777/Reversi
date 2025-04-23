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
//    for (auto &x : mobility_arr[eval_num]) {
//        x = fast_next_int(p);
//    }
//    for (auto &x : stone_arr[eval_num]) {
//        x = fast_next_int(p);
//    }
}

void mpc_init() {
    ifstream ifs(FileSystem::RelativePath(Resource(U"mpc.txt")).narrow());
    string line;
    for (auto &x : mpc_deviation) {
        for (auto &y : x) {
            getline(ifs, line);
            y = stod(line);
        }
    }
    for (auto &x : mpc_mean) {
        for (auto &y : x) {
            getline(ifs, line);
            y = stod(line);
        }
    }
}

#define evaluate_moveorder(b) evaluate(b)


//inline int evaluate(const board &b) noexcept {
//    
//    if (b.p == 0) [[unlikely]] return -32768;
//    if (b.o == 0) [[unlikely]] return 32768;
//    int plegal = popcnt_u64(makelegalboard(b));
//    board b2 = b.flipped();
//    int olegal = popcnt_u64(makelegalboard(b2));
//    if (plegal == 0 && olegal == 0) return 256 * (popcnt_u64(b.p) - popcnt_u64(b.o));
//    int P_cnt = popcnt_u64(b.p);
//    int O_cnt = popcnt_u64(b.o);
//    int eval_num = (P_cnt + O_cnt - 5) / 4;
////    int eval_num = (b.p_cnt + b.o_cnt - 4);
//    int32_t a = 0;
//    
//
//    a += pattern_arr[eval_num].diagonal8[CALC_INDEX(b.p, INDEX_A1, INDEX_B2, INDEX_C3, INDEX_D4, INDEX_E5, INDEX_F6, INDEX_G7, INDEX_H8) + CALC_INDEX(b.o, INDEX_A1, INDEX_B2, INDEX_C3, INDEX_D4, INDEX_E5, INDEX_F6, INDEX_G7, INDEX_H8) * 2];
//    a += pattern_arr[eval_num].diagonal8[CALC_INDEX(b.p, INDEX_H1, INDEX_G2, INDEX_F3, INDEX_E4, INDEX_D5, INDEX_C6, INDEX_B7, INDEX_A8) + CALC_INDEX(b.o, INDEX_H1, INDEX_G2, INDEX_F3, INDEX_E4, INDEX_D5, INDEX_C6, INDEX_B7, INDEX_A8) * 2];
//
//    a += pattern_arr[eval_num].diagonal7[CALC_INDEX(b.p, INDEX_B1, INDEX_C2, INDEX_D3, INDEX_E4, INDEX_F5, INDEX_G6, INDEX_H7) +
//                               CALC_INDEX(b.o, INDEX_B1, INDEX_C2, INDEX_D3, INDEX_E4, INDEX_F5, INDEX_G6, INDEX_H7) * 2];
//    a += pattern_arr[eval_num].diagonal7[CALC_INDEX(b.p, INDEX_H2, INDEX_G3, INDEX_F4, INDEX_E5, INDEX_D6, INDEX_C7, INDEX_B8) +
//                               CALC_INDEX(b.o, INDEX_H2, INDEX_G3, INDEX_F4, INDEX_E5, INDEX_D6, INDEX_C7, INDEX_B8) * 2];
//    a += pattern_arr[eval_num].diagonal7[CALC_INDEX(b.p, INDEX_A2, INDEX_B3, INDEX_C4, INDEX_D5, INDEX_E6, INDEX_F7, INDEX_G8) +
//                               CALC_INDEX(b.o, INDEX_A2, INDEX_B3, INDEX_C4, INDEX_D5, INDEX_E6, INDEX_F7, INDEX_G8) * 2];
//    a += pattern_arr[eval_num].diagonal7[CALC_INDEX(b.p, INDEX_A7, INDEX_B6, INDEX_C5, INDEX_D4, INDEX_E3, INDEX_F2, INDEX_G1) +
//                               CALC_INDEX(b.o, INDEX_A7, INDEX_B6, INDEX_C5, INDEX_D4, INDEX_E3, INDEX_F2, INDEX_G1) * 2];
//
//    a += pattern_arr[eval_num].diagonal6[CALC_INDEX(b.p, INDEX_C1, INDEX_D2, INDEX_E3, INDEX_F4, INDEX_G5, INDEX_H6) +
//                               CALC_INDEX(b.o, INDEX_C1, INDEX_D2, INDEX_E3, INDEX_F4, INDEX_G5, INDEX_H6) * 2];
//    a += pattern_arr[eval_num].diagonal6[CALC_INDEX(b.p, INDEX_C8, INDEX_D7, INDEX_E6, INDEX_F5, INDEX_G4, INDEX_H3) +
//                               CALC_INDEX(b.o, INDEX_C8, INDEX_D7, INDEX_E6, INDEX_F5, INDEX_G4, INDEX_H3)* 2];
//    a += pattern_arr[eval_num].diagonal6[CALC_INDEX(b.p, INDEX_A3, INDEX_B4, INDEX_C5, INDEX_D6, INDEX_E7, INDEX_F8) +
//                               CALC_INDEX(b.o, INDEX_A3, INDEX_B4, INDEX_C5, INDEX_D6, INDEX_E7, INDEX_F8)* 2];
//    a += pattern_arr[eval_num].diagonal6[CALC_INDEX(b.p, INDEX_A6, INDEX_B5, INDEX_C4, INDEX_D3, INDEX_E2, INDEX_F1) +
//                               CALC_INDEX(b.o, INDEX_A6, INDEX_B5, INDEX_C4, INDEX_D3, INDEX_E2, INDEX_F1) * 2];
//
//    a += pattern_arr[eval_num].diagonal5[CALC_INDEX(b.p, INDEX_D1, INDEX_E2, INDEX_F3, INDEX_G4, INDEX_H5) +
//                               CALC_INDEX(b.o, INDEX_D1, INDEX_E2, INDEX_F3, INDEX_G4, INDEX_H5) * 2];
//    a += pattern_arr[eval_num].diagonal5[CALC_INDEX(b.p, INDEX_D8, INDEX_E7, INDEX_F6, INDEX_G5, INDEX_H4) +
//                               CALC_INDEX(b.o, INDEX_D8, INDEX_E7, INDEX_F6, INDEX_G5, INDEX_H4) * 2];
//    a += pattern_arr[eval_num].diagonal5[CALC_INDEX(b.p, INDEX_A4, INDEX_B5, INDEX_C6, INDEX_D7, INDEX_E8) +
//                               CALC_INDEX(b.o, INDEX_A4, INDEX_B5, INDEX_C6, INDEX_D7, INDEX_E8) * 2];
//    a += pattern_arr[eval_num].diagonal5[CALC_INDEX(b.p, INDEX_A5, INDEX_B4, INDEX_C3, INDEX_D2, INDEX_E1) +
//                               CALC_INDEX(b.o, INDEX_A5, INDEX_B4, INDEX_C3, INDEX_D2, INDEX_E1) * 2];
//
//    a += pattern_arr[eval_num].edge_2x[CALC_INDEX(b.p, INDEX_A1, INDEX_B1, INDEX_C1, INDEX_D1, INDEX_E1, INDEX_F1, INDEX_G1, INDEX_H1, INDEX_B2, INDEX_G2) + CALC_INDEX(b.o, INDEX_A1, INDEX_B1, INDEX_C1, INDEX_D1, INDEX_E1, INDEX_F1, INDEX_G1, INDEX_H1, INDEX_B2, INDEX_G2) * 2];
//    a += pattern_arr[eval_num].edge_2x[CALC_INDEX(b.p, INDEX_H1, INDEX_H2, INDEX_H3, INDEX_H4, INDEX_H5, INDEX_H6, INDEX_H7, INDEX_H8, INDEX_G2, INDEX_G7) + CALC_INDEX(b.o, INDEX_H1, INDEX_H2, INDEX_H3, INDEX_H4, INDEX_H5, INDEX_H6, INDEX_H7, INDEX_H8, INDEX_G2, INDEX_G7) * 2];
//    a += pattern_arr[eval_num].edge_2x[CALC_INDEX(b.p, INDEX_H8, INDEX_G8, INDEX_F8, INDEX_E8, INDEX_D8, INDEX_C8, INDEX_B8, INDEX_A8, INDEX_G7, INDEX_B7) + CALC_INDEX(b.o, INDEX_H8, INDEX_G8, INDEX_F8, INDEX_E8, INDEX_D8, INDEX_C8, INDEX_B8, INDEX_A8, INDEX_G7, INDEX_B7) * 2];
//    a += pattern_arr[eval_num].edge_2x[CALC_INDEX(b.p, INDEX_A8, INDEX_A7, INDEX_A6, INDEX_A5, INDEX_A4, INDEX_A3, INDEX_A2, INDEX_A1, INDEX_B7, INDEX_B2) + CALC_INDEX(b.o, INDEX_A8, INDEX_A7, INDEX_A6, INDEX_A5, INDEX_A4, INDEX_A3, INDEX_A2, INDEX_A1, INDEX_B7, INDEX_B2) * 2];
//
//    a += pattern_arr[eval_num].h_v_2[CALC_INDEX(b.p, INDEX_A2, INDEX_B2, INDEX_C2, INDEX_D2, INDEX_E2, INDEX_F2, INDEX_G2, INDEX_H2) +                        CALC_INDEX(b.o, INDEX_A2, INDEX_B2, INDEX_C2, INDEX_D2, INDEX_E2, INDEX_F2, INDEX_G2, INDEX_H2) * 2];
//    a += pattern_arr[eval_num].h_v_2[CALC_INDEX(b.p, INDEX_G1, INDEX_G2, INDEX_G3, INDEX_G4, INDEX_G5, INDEX_G6, INDEX_G7, INDEX_G8) + CALC_INDEX(b.o, INDEX_G1, INDEX_G2, INDEX_G3, INDEX_G4, INDEX_G5, INDEX_G6, INDEX_G7, INDEX_G8) * 2];
//    a += pattern_arr[eval_num].h_v_2[CALC_INDEX(b.p, INDEX_A7, INDEX_B7, INDEX_C7, INDEX_D7, INDEX_E7, INDEX_F7, INDEX_G7, INDEX_H7) + CALC_INDEX(b.o, INDEX_A7, INDEX_B7, INDEX_C7, INDEX_D7, INDEX_E7, INDEX_F7, INDEX_G7, INDEX_H7) * 2];
//    a += pattern_arr[eval_num].h_v_2[CALC_INDEX(b.p, INDEX_B1, INDEX_B2, INDEX_B3, INDEX_B4, INDEX_B5, INDEX_B6, INDEX_B7, INDEX_B8) + CALC_INDEX(b.o, INDEX_B1, INDEX_B2, INDEX_B3, INDEX_B4, INDEX_B5, INDEX_B6, INDEX_B7, INDEX_B8) * 2];
//
//    a += pattern_arr[eval_num].h_v_3[CALC_INDEX(b.p, INDEX_A3, INDEX_B3, INDEX_C3, INDEX_D3, INDEX_E3, INDEX_F3, INDEX_G3, INDEX_H3) + CALC_INDEX(b.o, INDEX_A3, INDEX_B3, INDEX_C3, INDEX_D3, INDEX_E3, INDEX_F3, INDEX_G3, INDEX_H3) * 2];
//    a += pattern_arr[eval_num].h_v_3[CALC_INDEX(b.p, INDEX_F1, INDEX_F2, INDEX_F3, INDEX_F4, INDEX_F5, INDEX_F6, INDEX_F7, INDEX_F8) + CALC_INDEX(b.o, INDEX_F1, INDEX_F2, INDEX_F3, INDEX_F4, INDEX_F5, INDEX_F6, INDEX_F7, INDEX_F8) * 2];
//    a += pattern_arr[eval_num].h_v_3[CALC_INDEX(b.p, INDEX_A6, INDEX_B6, INDEX_C6, INDEX_D6, INDEX_E6, INDEX_F6, INDEX_G6, INDEX_H6) + CALC_INDEX(b.o, INDEX_A6, INDEX_B6, INDEX_C6, INDEX_D6, INDEX_E6, INDEX_F6, INDEX_G6, INDEX_H6) * 2];
//    a += pattern_arr[eval_num].h_v_3[CALC_INDEX(b.p, INDEX_C1, INDEX_C2, INDEX_C3, INDEX_C4, INDEX_C5, INDEX_C6, INDEX_C7, INDEX_C8) + CALC_INDEX(b.o, INDEX_C1, INDEX_C2, INDEX_C3, INDEX_C4, INDEX_C5, INDEX_C6, INDEX_C7, INDEX_C8) * 2];
//
//    a += pattern_arr[eval_num].h_v_4[CALC_INDEX(b.p, INDEX_A4, INDEX_B4, INDEX_C4, INDEX_D4, INDEX_E4, INDEX_F4, INDEX_G4, INDEX_H4) + CALC_INDEX(b.o, INDEX_A4, INDEX_B4, INDEX_C4, INDEX_D4, INDEX_E4, INDEX_F4, INDEX_G4, INDEX_H4) * 2];
//    a += pattern_arr[eval_num].h_v_4[CALC_INDEX(b.p, INDEX_E1, INDEX_E2, INDEX_E3, INDEX_E4, INDEX_E5, INDEX_E6, INDEX_E7, INDEX_E8) + CALC_INDEX(b.o, INDEX_E1, INDEX_E2, INDEX_E3, INDEX_E4, INDEX_E5, INDEX_E6, INDEX_E7, INDEX_E8) * 2];
//    a += pattern_arr[eval_num].h_v_4[CALC_INDEX(b.p, INDEX_A5, INDEX_B5, INDEX_C5, INDEX_D5, INDEX_E5, INDEX_F5, INDEX_G5, INDEX_H5) + CALC_INDEX(b.o, INDEX_A5, INDEX_B5, INDEX_C5, INDEX_D5, INDEX_E5, INDEX_F5, INDEX_G5, INDEX_H5) * 2];
//    a += pattern_arr[eval_num].h_v_4[CALC_INDEX(b.p, INDEX_D1, INDEX_D2, INDEX_D3, INDEX_D4, INDEX_D5, INDEX_D6, INDEX_D7, INDEX_D8) + CALC_INDEX(b.o, INDEX_D1, INDEX_D2, INDEX_D3, INDEX_D4, INDEX_D5, INDEX_D6, INDEX_D7, INDEX_D8) * 2];
//
//    a += pattern_arr[eval_num].corner_3x3[CALC_INDEX(b.p, INDEX_A1, INDEX_B1, INDEX_C1, INDEX_A2, INDEX_B2, INDEX_C2, INDEX_A3, INDEX_B2, INDEX_C3) + CALC_INDEX(b.o, INDEX_A1, INDEX_B1, INDEX_C1, INDEX_A2, INDEX_B2, INDEX_C2, INDEX_A3, INDEX_B2, INDEX_C3) * 2];
//    a += pattern_arr[eval_num].corner_3x3[CALC_INDEX(b.p, INDEX_H1, INDEX_H2, INDEX_H3, INDEX_G1, INDEX_G2, INDEX_G3, INDEX_F1, INDEX_F2, INDEX_F3) + CALC_INDEX(b.o, INDEX_H1, INDEX_H2, INDEX_H3, INDEX_G1, INDEX_G2, INDEX_G3, INDEX_F1, INDEX_F2, INDEX_F3) * 2];
//    a += pattern_arr[eval_num].corner_3x3[CALC_INDEX(b.p, INDEX_H8, INDEX_G8, INDEX_F8, INDEX_H7, INDEX_G7, INDEX_F7, INDEX_H6, INDEX_G6, INDEX_F6) + CALC_INDEX(b.o, INDEX_H8, INDEX_G8, INDEX_F8, INDEX_H7, INDEX_G7, INDEX_F7, INDEX_H6, INDEX_G6, INDEX_F6) * 2];
//    a += pattern_arr[eval_num].corner_3x3[CALC_INDEX(b.p, INDEX_A8, INDEX_A7, INDEX_A6, INDEX_B8, INDEX_B7, INDEX_B6, INDEX_C8, INDEX_C7, INDEX_C6) + CALC_INDEX(b.o, INDEX_A8, INDEX_A7, INDEX_A6, INDEX_B8, INDEX_B7, INDEX_B6, INDEX_C8, INDEX_C7, INDEX_C6) * 2];
//
//    a += pattern_arr[eval_num].edge_x_side[CALC_INDEX(b.p, INDEX_A1, INDEX_B1, INDEX_C1, INDEX_D1, INDEX_E1, INDEX_A2, INDEX_B2, INDEX_A3, INDEX_A4, INDEX_A5) + CALC_INDEX(b.o, INDEX_A1, INDEX_B1, INDEX_C1, INDEX_D1, INDEX_E1, INDEX_A2, INDEX_B2, INDEX_A3, INDEX_A4, INDEX_A5) * 2];
//    a += pattern_arr[eval_num].edge_x_side[CALC_INDEX(b.p, INDEX_H1, INDEX_H2, INDEX_H3, INDEX_H4, INDEX_H5, INDEX_G1, INDEX_G2, INDEX_F1    , INDEX_E1, INDEX_D1) + CALC_INDEX(b.o, INDEX_H1, INDEX_H2, INDEX_H3, INDEX_H4, INDEX_H5, INDEX_G1, INDEX_G2, INDEX_F1    , INDEX_E1, INDEX_D1) * 2];
//    a += pattern_arr[eval_num].edge_x_side[CALC_INDEX(b.p, INDEX_H8, INDEX_G8, INDEX_F8, INDEX_E8, INDEX_D8, INDEX_H7, INDEX_G7, INDEX_H6, INDEX_H5, INDEX_H4) + CALC_INDEX(b.o, INDEX_H8, INDEX_G8, INDEX_F8, INDEX_E8, INDEX_D8, INDEX_H7, INDEX_G7, INDEX_H6, INDEX_H5, INDEX_H4) * 2];
//    a += pattern_arr[eval_num].edge_x_side[CALC_INDEX(b.p, INDEX_A8, INDEX_A7, INDEX_A6, INDEX_A5, INDEX_A4, INDEX_B8, INDEX_B7, INDEX_C8, INDEX_D8, INDEX_E8) + CALC_INDEX(b.o, INDEX_A8, INDEX_A7, INDEX_A6, INDEX_A5, INDEX_A4, INDEX_B8, INDEX_B7, INDEX_C8, INDEX_D8, INDEX_E8) * 2];
//
//    a += pattern_arr[eval_num].edge_block[CALC_INDEX(b.p, INDEX_A1, INDEX_C1, INDEX_D1, INDEX_E1, INDEX_F1, INDEX_H1, INDEX_C2, INDEX_D2, INDEX_E2, INDEX_F2) + CALC_INDEX(b.o, INDEX_A1, INDEX_C1, INDEX_D1, INDEX_E1, INDEX_F1, INDEX_H1, INDEX_C2, INDEX_D2, INDEX_E2, INDEX_F2) * 2];
//    a += pattern_arr[eval_num].edge_block[CALC_INDEX(b.p, INDEX_H1, INDEX_H3, INDEX_H4, INDEX_H5, INDEX_H6, INDEX_H8, INDEX_G3, INDEX_G4, INDEX_G5, INDEX_G6) + CALC_INDEX(b.o, INDEX_H1, INDEX_H3, INDEX_H4, INDEX_H5, INDEX_H6, INDEX_H8, INDEX_G3, INDEX_G4, INDEX_G5, INDEX_G6) * 2];
//    a += pattern_arr[eval_num].edge_block[CALC_INDEX(b.p, INDEX_H8, INDEX_F8, INDEX_E8, INDEX_D8, INDEX_C8, INDEX_A8, INDEX_F7, INDEX_E7, INDEX_D7, INDEX_C7) + CALC_INDEX(b.o, INDEX_H8, INDEX_F8, INDEX_E8, INDEX_D8, INDEX_C8, INDEX_A8, INDEX_F7, INDEX_E7, INDEX_D7, INDEX_C7) * 2];
//    a += pattern_arr[eval_num].edge_block[CALC_INDEX(b.p, INDEX_A8, INDEX_A6, INDEX_A5, INDEX_A4, INDEX_A3, INDEX_A1, INDEX_B6, INDEX_B5, INDEX_B4, INDEX_B3) + CALC_INDEX(b.o, INDEX_A8, INDEX_A6, INDEX_A5, INDEX_A4, INDEX_A3, INDEX_A1, INDEX_B6, INDEX_B5, INDEX_B4, INDEX_B3) * 2];
//
//    a += pattern_arr[eval_num].triangle[CALC_INDEX(b.p, INDEX_A1, INDEX_B1, INDEX_C1, INDEX_D1, INDEX_A2, INDEX_B2, INDEX_C2, INDEX_A3, INDEX_B3, INDEX_A4) + CALC_INDEX(b.o, INDEX_A1, INDEX_B1, INDEX_C1, INDEX_D1, INDEX_A2, INDEX_B2, INDEX_C2, INDEX_A3, INDEX_B3, INDEX_A4) * 2];
//    a += pattern_arr[eval_num].triangle[CALC_INDEX(b.p, INDEX_H1, INDEX_H2, INDEX_H3, INDEX_H4, INDEX_G1, INDEX_G2, INDEX_G3, INDEX_F1, INDEX_F2, INDEX_E1) + CALC_INDEX(b.o, INDEX_H1, INDEX_H2, INDEX_H3, INDEX_H4, INDEX_G1, INDEX_G2, INDEX_G3, INDEX_F1, INDEX_F2, INDEX_E1) * 2];
//    a += pattern_arr[eval_num].triangle[CALC_INDEX(b.p, INDEX_H8, INDEX_G8, INDEX_F8, INDEX_E8, INDEX_H7, INDEX_G7, INDEX_F7, INDEX_H6, INDEX_G6, INDEX_H5) + CALC_INDEX(b.o, INDEX_H8, INDEX_G8, INDEX_F8, INDEX_E8, INDEX_H7, INDEX_G7, INDEX_F7, INDEX_H6, INDEX_G6, INDEX_H5) * 2];
//    a += pattern_arr[eval_num].triangle[CALC_INDEX(b.p, INDEX_A8, INDEX_A7, INDEX_A6, INDEX_A5, INDEX_B8, INDEX_B7, INDEX_B6, INDEX_C8, INDEX_C7, INDEX_D8) + CALC_INDEX(b.o, INDEX_A8, INDEX_A7, INDEX_A6, INDEX_A5, INDEX_B8, INDEX_B7, INDEX_B6, INDEX_C8, INDEX_C7, INDEX_D8) * 2];
//
//    a += pattern_arr[eval_num].corner_2x5[CALC_INDEX(b.p, INDEX_A1, INDEX_B1, INDEX_C1, INDEX_D1, INDEX_E1, INDEX_A2, INDEX_B2, INDEX_C2, INDEX_D2, INDEX_E2) + CALC_INDEX(b.o, INDEX_A1, INDEX_B1, INDEX_C1, INDEX_D1, INDEX_E1, INDEX_A2, INDEX_B2, INDEX_C2, INDEX_D2, INDEX_E2) * 2];
//    a += pattern_arr[eval_num].corner_2x5[CALC_INDEX(b.p, INDEX_H1, INDEX_H2, INDEX_H3, INDEX_H4, INDEX_H5, INDEX_G1, INDEX_G2, INDEX_G3, INDEX_G4, INDEX_G5) + CALC_INDEX(b.o, INDEX_H1, INDEX_H2, INDEX_H3, INDEX_H4, INDEX_H5, INDEX_G1, INDEX_G2, INDEX_G3, INDEX_G4, INDEX_G5) * 2];
//    a += pattern_arr[eval_num].corner_2x5[CALC_INDEX(b.p, INDEX_H8, INDEX_G8, INDEX_F8, INDEX_E8, INDEX_D8, INDEX_H7, INDEX_G7, INDEX_F7, INDEX_E7, INDEX_D7) + CALC_INDEX(b.o, INDEX_H8, INDEX_G8, INDEX_F8, INDEX_E8, INDEX_D8, INDEX_H7, INDEX_G7, INDEX_F7, INDEX_E7, INDEX_D7) * 2];
//    a += pattern_arr[eval_num].corner_2x5[CALC_INDEX(b.p, INDEX_A8, INDEX_A7, INDEX_A6, INDEX_A5, INDEX_A4, INDEX_B8, INDEX_B7, INDEX_B6, INDEX_B5, INDEX_B4) + CALC_INDEX(b.o, INDEX_A8, INDEX_A7, INDEX_A6, INDEX_A5, INDEX_A4, INDEX_B8, INDEX_B7, INDEX_B6, INDEX_B5, INDEX_B4) * 2];
//
//    a += pattern_arr[eval_num].corner_2x5[CALC_INDEX(b.p, INDEX_A1, INDEX_A2, INDEX_A3, INDEX_A4, INDEX_A5, INDEX_B1, INDEX_B2, INDEX_B3, INDEX_B4, INDEX_B5) + CALC_INDEX(b.o, INDEX_A1, INDEX_A2, INDEX_A3, INDEX_A4, INDEX_A5, INDEX_B1, INDEX_B2, INDEX_B3, INDEX_B4, INDEX_B5) * 2];
//    a += pattern_arr[eval_num].corner_2x5[CALC_INDEX(b.p, INDEX_H1, INDEX_G1, INDEX_F1, INDEX_E1, INDEX_D1, INDEX_H2, INDEX_G2, INDEX_F2, INDEX_E2, INDEX_D2) + CALC_INDEX(b.o, INDEX_H1, INDEX_G1, INDEX_F1, INDEX_E1, INDEX_D1, INDEX_H2, INDEX_G2, INDEX_F2, INDEX_E2, INDEX_D2) * 2];
//    a += pattern_arr[eval_num].corner_2x5[CALC_INDEX(b.p, INDEX_H8, INDEX_H7, INDEX_H6, INDEX_H5, INDEX_H4, INDEX_G8, INDEX_G7, INDEX_G6, INDEX_G5, INDEX_G4) + CALC_INDEX(b.o, INDEX_H8, INDEX_H7, INDEX_H6, INDEX_H5, INDEX_H4, INDEX_G8, INDEX_G7, INDEX_G6, INDEX_G5, INDEX_G4) * 2];
//    a += pattern_arr[eval_num].corner_2x5[CALC_INDEX(b.p, INDEX_A8, INDEX_B8, INDEX_C8, INDEX_D8, INDEX_E8, INDEX_A7, INDEX_B7, INDEX_C7, INDEX_D7, INDEX_E7) + CALC_INDEX(b.o, INDEX_A8, INDEX_B8, INDEX_C8, INDEX_D8, INDEX_E8, INDEX_A7, INDEX_B7, INDEX_C7, INDEX_D7, INDEX_E7) * 2];
//    
//    a += mobility_arr[eval_num][plegal * 36 + olegal];
//    
//    a += stone_arr[eval_num][P_cnt * 65 + O_cnt];
//    
//    return a;
//}

inline int evaluate(const board &b) noexcept {
    
    if (b.p == 0) [[unlikely]] return -32768;
    if (b.o == 0) [[unlikely]] return 32768;
//    int plegal = popcnt_u64(makelegalboard(b));
//    board b2 = b.flipped();
//    int olegal = popcnt_u64(makelegalboard(b2));
//    if (plegal == 0 && olegal == 0) return 256 * (popcnt_u64(b.p) - popcnt_u64(b.o));
    int P_cnt = popcnt_u64(b.p);
    int O_cnt = popcnt_u64(b.o);
    int eval_num = (P_cnt + O_cnt - 5) / 4;
//    int eval_num = (b.p_cnt + b.o_cnt - 4);
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

//    
//    a += mobility_arr[eval_num][plegal * 36 + olegal];
//    
//    a += stone_arr[eval_num][P_cnt * 65 + O_cnt];
    
    return a;
}
