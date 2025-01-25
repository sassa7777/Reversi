#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <Siv3D.hpp>
#include "../zp7/zp7.cpp"

using namespace std;
using bitboard = pair<uint64_t, uint64_t>;

#ifdef __GNUC__
#define flipVertical(x) __builtin_bswap64(x)
#else
#define flipVertical(x) _byteswap_uint64(x)
#endif

#define n_patterns 12 // 使うパターンの種類
#define pext(s, m) zp7_pext_64(s, m)
#define ppext(s, m) zp7_pext_pre_64(s, m)
constexpr int pattern_sizes[n_patterns] = {8, 7, 6, 5, 10, 8, 8, 8, 9, 10, 10, 10}; // パターンごとのマスの数

constexpr uint64_t bit_pattern[] = {0x8040201008040201, 0x4020100804020100, 0x2010080402010000, 0x1008040201000000, 0xff42000000000000, 0xff000000000000, 0xff0000000000, 0xff00000000, 0xe0e0e00000000000, 0xf8c0808080000000, 0xbd3c000000000000, 0xf0e0c08000000000};

const vector<vector<int>> bit_positions = {{0, 9, 18, 27, 36, 45, 54, 63}, {1, 10, 19, 28, 37, 46, 55}, {2, 11, 20, 29, 38, 47}, {3, 12, 21, 30, 39}, {9, 0, 1, 2, 3, 4, 5, 6, 7, 14}, {8, 9, 10, 11, 12, 13, 14, 15}, {16, 17, 18, 19, 20, 21, 22, 23}, {24, 25, 26, 27, 28, 29, 30, 31}, {0, 1, 8, 9, 2, 16, 10, 17, 18}, {32, 24, 16, 8, 0, 9, 1, 2, 3, 4}, {0, 2, 3, 10, 11, 12, 13, 4, 5, 7}, {0, 1, 2, 3, 8, 9, 10, 16, 17, 24}};

// モデルの設計パラメータ
#define n_dense0 128
#define n_dense1 128
#define n_all_input 12
#define model_count 1
#define use_book false

double final_dense[n_all_input];
double final_bias[3];

static double pattern_arr[model_count][n_patterns][4][1024][1024];
static zp7_masks_64_t maskr[n_patterns][4];

inline uint64_t delta_swap(uint64_t x, uint64_t mask, int delta) {
    uint64_t t = (x ^ (x >> delta)) & mask;
    return x ^ t ^ (t << delta);
}

// 水平反転
inline uint64_t flipHorizontal(uint64_t x) {
    x = ((x >> 1) & 0x5555555555555555) | ((x & 0x5555555555555555) << 1);
    x = ((x >> 2) & 0x3333333333333333) | ((x & 0x3333333333333333) << 2);
    x = ((x >> 4) & 0x0f0f0f0f0f0f0f0f) | ((x & 0x0f0f0f0f0f0f0f0f) << 4);
    return x;
}

// A1-H8反転
inline uint64_t flipDiagonalA1H8(uint64_t x) {
    x = delta_swap(x, 0x00AA00AA00AA00AA, 7);
    x = delta_swap(x, 0x0000CCCC0000CCCC, 14);
    return delta_swap(x, 0x00000000F0F0F0F0, 28);
}

// A8-H1反転
inline uint64_t flipDiagonalA8H1(uint64_t x) {
    x = delta_swap(x, 0x0055005500550055, 9);
    x = delta_swap(x, 0x0000333300003333, 18);
    return delta_swap(x, 0x000000000F0F0F0F, 36);
}

// 時計回りに90度回転
inline uint64_t r90(uint64_t x) {
    return flipVertical(flipDiagonalA1H8(x));
}

inline uint64_t l90(uint64_t x) {
    return flipDiagonalA1H8(flipVertical(x));
}

#ifdef __clang__
#define r180(x) __builtin_bitreverse64(x)
#else
inline uint64_t r180(uint64_t x) {
    return flipVertical(flipHorizontal(x));
}
#endif

inline int cal_pow(int x, int depth) {
    if(depth == 1) return x;
    return x * cal_pow(x, depth - 1);
}

inline double leaky_relu(double x){
    return max(0.01 * x, x);
}

inline double predict_pattern(int pattern_size, double in_arr[], const vector<vector<double>>& dense0, const vector<double>& bias0, const vector<vector<double>>& dense1, const vector<double>& bias1, const vector<double>& dense2, double bias2){
    double hidden0[n_dense0], hidden1;
    int i, j;
    for (i = 0; i < n_dense0; ++i){
        hidden0[i] = bias0[i];
        for (j = 0; j < pattern_size * 2; ++j)
            hidden0[i] += in_arr[j] * dense0[i][j];
        hidden0[i] = leaky_relu(hidden0[i]);
    }
    double res = bias2;
    for (i = 0; i < n_dense1; ++i){
        hidden1 = bias1[i];
        for (j = 0; j < n_dense0; ++j)
            hidden1 += hidden0[j] * dense1[i][j];
        hidden1 = leaky_relu(hidden1);
        res += hidden1 * dense2[i];
    }
    res = leaky_relu(res);
    return res;
}

inline void pre_evaluation_pattern(int ptr_num, int pattern_idx, int evaluate_idx, int pattern_size, vector<vector<double>>& dense0, vector<double>& bias0, vector<vector<double>>& dense1, vector<double>& bias1, vector<double>& dense2, double bias2){
    double arr[20];
    
    vector<int> bitpositions = bit_positions[pattern_idx];
    uint64_t pattern1 = bit_pattern[pattern_idx];
    maskr[pattern_idx][0] = zp7_ppp_64(pattern1);
    maskr[pattern_idx][1] = zp7_ppp_64(r90(pattern1));
    maskr[pattern_idx][2] = zp7_ppp_64(r180(pattern1));
    maskr[pattern_idx][3] = zp7_ppp_64(l90(pattern1));
    size_t popcountPattern = popcount(pattern1);
    size_t totalCombinations = cal_pow(2, (int)popcountPattern);
//    pattern_arr[ptr_num][evaluate_idx].resize(4);
//    for (auto &ptar : pattern_arr[ptr_num][evaluate_idx]) {
//        ptar.resize(1ULL << popcountPattern);
//        for (auto &ptar2 : ptar) {
//            ptar2.resize(1ULL << popcountPattern);
//        }
//    }
    vector<uint64_t> bitShifts(bitpositions.size());
    for (size_t i = 0; i < bitpositions.size(); ++i) {
        bitShifts[i] = (0x8000000000000000ULL >> bitpositions[i]);
    }
    for (size_t comb1 = 0; comb1 < totalCombinations; ++comb1) {
        uint64_t newpattern1 = pattern1;
        for (size_t i = 0; i < bitpositions.size(); ++i) {
            if ((comb1 >> i) & 1) {
                newpattern1 ^= bitShifts[i];
            }
        }
        for (size_t comb2 = 0; comb2 < totalCombinations; ++comb2) {
            uint64_t newpattern2 = pattern1;
            for (size_t i = 0; i < bitpositions.size(); ++i) {
                if ((comb2 >> i) & 1) {
                    newpattern2 ^= bitShifts[i];
                }
            }
            if (newpattern1 & newpattern2) continue;
            for (size_t i = 0; i < popcountPattern; ++i) {
                uint64_t bitMask = bitShifts[i];
                if (newpattern1 & bitMask) {
                    arr[i] = 1.0;
                    arr[pattern_size + i] = 0.0;
                } else if (newpattern2 & bitMask) {
                    arr[i] = 0.0;
                    arr[pattern_size + i] = 1.0;
                } else {
                    arr[i] = 0.0;
                    arr[pattern_size + i] = 0.0;
                }
            }
            size_t e = evaluate_idx;
            double result = predict_pattern(pattern_size, arr, dense0, bias0, dense1, bias1, dense2, bias2);
            auto key1 = make_pair(ppext(newpattern1, &maskr[e][0]), ppext(newpattern2, &maskr[e][0]));
            auto key2 = make_pair(ppext(r90(newpattern1), &maskr[e][1]), ppext(r90(newpattern2), &maskr[e][1]));
            auto key3 = make_pair(ppext(r180(newpattern1), &maskr[e][2]), ppext(r180(newpattern2), &maskr[e][2]));
            auto key4 = make_pair(ppext(l90(newpattern1), &maskr[e][3]), ppext(l90(newpattern2), &maskr[e][3]));

            pattern_arr[ptr_num][evaluate_idx][0][key1.first][key1.second] = result;
            pattern_arr[ptr_num][evaluate_idx][1][key2.first][key2.second] = result;
            pattern_arr[ptr_num][evaluate_idx][2][key3.first][key3.second] = result;
            pattern_arr[ptr_num][evaluate_idx][3][key4.first][key4.second] = result;
        }
    }
}

inline void evaluate_init(String model_path, int ptr_num){
    ifstream ifs(FileSystem::RelativePath(Resource(model_path)).narrow());
    if (ifs.fail()){
        cerr << "evaluation file does not exist" << endl;
        exit(1);
    }
    string line;
    int i, j, pattern_idx;

    vector<std::vector<double>> dense0(n_dense0, vector<double>(20));
    vector<double> bias0(n_dense0);
    vector<std::vector<double>> dense1(n_dense1, vector<double>(n_dense0));
    vector<double> bias1(n_dense1);
    vector<double> dense2(n_dense1);
    double bias2;

    for (pattern_idx = 0; pattern_idx < n_patterns; ++pattern_idx){
        for (i = 0; i < n_dense0; ++i){
            for (j = 0; j < pattern_sizes[pattern_idx] * 2; ++j){
                getline(ifs, line);
                dense0[i][j] = stof(line);
            }
        }
        for (i = 0; i < n_dense0; ++i){
            getline(ifs, line);
            bias0[i] = stof(line);
        }
        for (i = 0; i < n_dense1; ++i){
            for (j = 0; j < n_dense0; ++j){
                getline(ifs, line);
                dense1[i][j] = stof(line);
            }
        }
        for (i = 0; i < n_dense1; ++i){
            getline(ifs, line);
            bias1[i] = stof(line);
        }
        for (i = 0; i < n_dense1; ++i){
            getline(ifs, line);
            dense2[i] = stof(line);
        }
        getline(ifs, line);
        bias2 = stof(line);
        pre_evaluation_pattern(ptr_num, pattern_idx, pattern_idx, pattern_sizes[pattern_idx], dense0, bias0, dense1, bias1, dense2, bias2);
    }
    for (i = 0; i < n_all_input; ++i){
        getline(ifs, line);
        final_dense[i] = stof(line);
    }
    for (i = 0; i < n_patterns; ++i){
        for (auto &ptr : pattern_arr[ptr_num][i]) {
            for (auto &ptar : ptr) {
                for (auto &ptar2 : ptar) {
                    ptar2 *= final_dense[i];
                }
            }
        }
    }
    getline(ifs, line);
    final_bias[ptr_num] = stof(line);
}

inline int64_t evaluate_moveorder(uint64_t playerboard, uint64_t opponentboard) noexcept {
    
    double a = 0;
    a += (pattern_arr[evaluate_ptr_num][0][0][ppext(playerboard, &maskr[0][0])][ppext(opponentboard, &maskr[0][0])] +
          pattern_arr[evaluate_ptr_num][0][1][ppext(playerboard, &maskr[0][1])][ppext(opponentboard, &maskr[0][1])]);

    a += (pattern_arr[evaluate_ptr_num][1][0][ppext(playerboard, &maskr[1][0])][ppext(opponentboard, &maskr[1][0])] +
          pattern_arr[evaluate_ptr_num][1][1][ppext(playerboard, &maskr[1][1])][ppext(opponentboard, &maskr[1][1])] +
          pattern_arr[evaluate_ptr_num][1][2][ppext(playerboard, &maskr[1][2])][ppext(opponentboard, &maskr[1][2])] +
          pattern_arr[evaluate_ptr_num][1][3][ppext(playerboard, &maskr[1][3])][ppext(opponentboard, &maskr[1][3])]);

    a += (pattern_arr[evaluate_ptr_num][2][0][ppext(playerboard, &maskr[2][0])][ppext(opponentboard, &maskr[2][0])] +
          pattern_arr[evaluate_ptr_num][2][1][ppext(playerboard, &maskr[2][1])][ppext(opponentboard, &maskr[2][1])] +
          pattern_arr[evaluate_ptr_num][2][2][ppext(playerboard, &maskr[2][2])][ppext(opponentboard, &maskr[2][2])] +
          pattern_arr[evaluate_ptr_num][2][3][ppext(playerboard, &maskr[2][3])][ppext(opponentboard, &maskr[2][3])]);

    a += (pattern_arr[evaluate_ptr_num][3][0][ppext(playerboard, &maskr[3][0])][ppext(opponentboard, &maskr[3][0])] +
          pattern_arr[evaluate_ptr_num][3][1][ppext(playerboard, &maskr[3][1])][ppext(opponentboard, &maskr[3][1])] +
          pattern_arr[evaluate_ptr_num][3][2][ppext(playerboard, &maskr[3][2])][ppext(opponentboard, &maskr[3][2])] +
          pattern_arr[evaluate_ptr_num][3][3][ppext(playerboard, &maskr[3][3])][ppext(opponentboard, &maskr[3][3])]);

    a += (pattern_arr[evaluate_ptr_num][4][0][ppext(playerboard, &maskr[4][0])][ppext(opponentboard, &maskr[4][0])] +
          pattern_arr[evaluate_ptr_num][4][1][ppext(playerboard, &maskr[4][1])][ppext(opponentboard, &maskr[4][1])] +
          pattern_arr[evaluate_ptr_num][4][2][ppext(playerboard, &maskr[4][2])][ppext(opponentboard, &maskr[4][2])] +
          pattern_arr[evaluate_ptr_num][4][3][ppext(playerboard, &maskr[4][3])][ppext(opponentboard, &maskr[4][3])]);

    a += (pattern_arr[evaluate_ptr_num][5][0][ppext(playerboard, &maskr[5][0])][ppext(opponentboard, &maskr[5][0])] +
          pattern_arr[evaluate_ptr_num][5][1][ppext(playerboard, &maskr[5][1])][ppext(opponentboard, &maskr[5][1])] +
          pattern_arr[evaluate_ptr_num][5][2][ppext(playerboard, &maskr[5][2])][ppext(opponentboard, &maskr[5][2])] +
          pattern_arr[evaluate_ptr_num][5][3][ppext(playerboard, &maskr[5][3])][ppext(opponentboard, &maskr[5][3])]);

    a += (pattern_arr[evaluate_ptr_num][6][0][ppext(playerboard, &maskr[6][0])][ppext(opponentboard, &maskr[6][0])] +
          pattern_arr[evaluate_ptr_num][6][1][ppext(playerboard, &maskr[6][1])][ppext(opponentboard, &maskr[6][1])] +
          pattern_arr[evaluate_ptr_num][6][2][ppext(playerboard, &maskr[6][2])][ppext(opponentboard, &maskr[6][2])] +
          pattern_arr[evaluate_ptr_num][6][3][ppext(playerboard, &maskr[6][3])][ppext(opponentboard, &maskr[6][3])]);

    a += (pattern_arr[evaluate_ptr_num][7][0][ppext(playerboard, &maskr[7][0])][ppext(opponentboard, &maskr[7][0])] +
          pattern_arr[evaluate_ptr_num][7][1][ppext(playerboard, &maskr[7][1])][ppext(opponentboard, &maskr[7][1])] +
          pattern_arr[evaluate_ptr_num][7][2][ppext(playerboard, &maskr[7][2])][ppext(opponentboard, &maskr[7][2])] +
          pattern_arr[evaluate_ptr_num][7][3][ppext(playerboard, &maskr[7][3])][ppext(opponentboard, &maskr[7][3])]);

    a += (pattern_arr[evaluate_ptr_num][8][0][ppext(playerboard, &maskr[8][0])][ppext(opponentboard, &maskr[8][0])] +
          pattern_arr[evaluate_ptr_num][8][1][ppext(playerboard, &maskr[8][1])][ppext(opponentboard, &maskr[8][1])] +
          pattern_arr[evaluate_ptr_num][8][2][ppext(playerboard, &maskr[8][2])][ppext(opponentboard, &maskr[8][2])] +
          pattern_arr[evaluate_ptr_num][8][3][ppext(playerboard, &maskr[8][3])][ppext(opponentboard, &maskr[8][3])]);

    a += (pattern_arr[evaluate_ptr_num][9][0][ppext(playerboard, &maskr[9][0])][ppext(opponentboard, &maskr[9][0])] +
          pattern_arr[evaluate_ptr_num][9][1][ppext(playerboard, &maskr[9][1])][ppext(opponentboard, &maskr[9][1])] +
          pattern_arr[evaluate_ptr_num][9][2][ppext(playerboard, &maskr[9][2])][ppext(opponentboard, &maskr[9][2])] +
          pattern_arr[evaluate_ptr_num][9][3][ppext(playerboard, &maskr[9][3])][ppext(opponentboard, &maskr[9][3])]);

    a += (pattern_arr[evaluate_ptr_num][10][0][ppext(playerboard, &maskr[10][0])][ppext(opponentboard, &maskr[10][0])] +
          pattern_arr[evaluate_ptr_num][10][1][ppext(playerboard, &maskr[10][1])][ppext(opponentboard, &maskr[10][1])] +
          pattern_arr[evaluate_ptr_num][10][2][ppext(playerboard, &maskr[10][2])][ppext(opponentboard, &maskr[10][2])] +
          pattern_arr[evaluate_ptr_num][10][3][ppext(playerboard, &maskr[10][3])][ppext(opponentboard, &maskr[10][3])]);

    a += (pattern_arr[evaluate_ptr_num][11][0][ppext(playerboard, &maskr[11][0])][ppext(opponentboard, &maskr[11][0])] +
          pattern_arr[evaluate_ptr_num][11][1][ppext(playerboard, &maskr[11][1])][ppext(opponentboard, &maskr[11][1])] +
          pattern_arr[evaluate_ptr_num][11][2][ppext(playerboard, &maskr[11][2])][ppext(opponentboard, &maskr[11][2])] +
          pattern_arr[evaluate_ptr_num][11][3][ppext(playerboard, &maskr[11][3])][ppext(opponentboard, &maskr[11][3])]);
    
    a += final_bias[evaluate_ptr_num];

    return llround(a * 640000000);
}

inline int64_t evaluate(uint64_t playerboard, uint64_t opponentboard) noexcept {

    if (!playerboard) return -64000000000;
    if (!opponentboard) return 64000000000;
    if (afterIndex >= 64) return (popcount(playerboard)-popcount(opponentboard))*1000000000;
    
    double a = 0;
    a += (pattern_arr[evaluate_ptr_num][0][0][ppext(playerboard, &maskr[0][0])][ppext(opponentboard, &maskr[0][0])] +
          pattern_arr[evaluate_ptr_num][0][1][ppext(playerboard, &maskr[0][1])][ppext(opponentboard, &maskr[0][1])]);

    a += (pattern_arr[evaluate_ptr_num][1][0][ppext(playerboard, &maskr[1][0])][ppext(opponentboard, &maskr[1][0])] +
          pattern_arr[evaluate_ptr_num][1][1][ppext(playerboard, &maskr[1][1])][ppext(opponentboard, &maskr[1][1])] +
          pattern_arr[evaluate_ptr_num][1][2][ppext(playerboard, &maskr[1][2])][ppext(opponentboard, &maskr[1][2])] +
          pattern_arr[evaluate_ptr_num][1][3][ppext(playerboard, &maskr[1][3])][ppext(opponentboard, &maskr[1][3])]);

    a += (pattern_arr[evaluate_ptr_num][2][0][ppext(playerboard, &maskr[2][0])][ppext(opponentboard, &maskr[2][0])] +
          pattern_arr[evaluate_ptr_num][2][1][ppext(playerboard, &maskr[2][1])][ppext(opponentboard, &maskr[2][1])] +
          pattern_arr[evaluate_ptr_num][2][2][ppext(playerboard, &maskr[2][2])][ppext(opponentboard, &maskr[2][2])] +
          pattern_arr[evaluate_ptr_num][2][3][ppext(playerboard, &maskr[2][3])][ppext(opponentboard, &maskr[2][3])]);

    a += (pattern_arr[evaluate_ptr_num][3][0][ppext(playerboard, &maskr[3][0])][ppext(opponentboard, &maskr[3][0])] +
          pattern_arr[evaluate_ptr_num][3][1][ppext(playerboard, &maskr[3][1])][ppext(opponentboard, &maskr[3][1])] +
          pattern_arr[evaluate_ptr_num][3][2][ppext(playerboard, &maskr[3][2])][ppext(opponentboard, &maskr[3][2])] +
          pattern_arr[evaluate_ptr_num][3][3][ppext(playerboard, &maskr[3][3])][ppext(opponentboard, &maskr[3][3])]);

    a += (pattern_arr[evaluate_ptr_num][4][0][ppext(playerboard, &maskr[4][0])][ppext(opponentboard, &maskr[4][0])] +
          pattern_arr[evaluate_ptr_num][4][1][ppext(playerboard, &maskr[4][1])][ppext(opponentboard, &maskr[4][1])] +
          pattern_arr[evaluate_ptr_num][4][2][ppext(playerboard, &maskr[4][2])][ppext(opponentboard, &maskr[4][2])] +
          pattern_arr[evaluate_ptr_num][4][3][ppext(playerboard, &maskr[4][3])][ppext(opponentboard, &maskr[4][3])]);

    a += (pattern_arr[evaluate_ptr_num][5][0][ppext(playerboard, &maskr[5][0])][ppext(opponentboard, &maskr[5][0])] +
          pattern_arr[evaluate_ptr_num][5][1][ppext(playerboard, &maskr[5][1])][ppext(opponentboard, &maskr[5][1])] +
          pattern_arr[evaluate_ptr_num][5][2][ppext(playerboard, &maskr[5][2])][ppext(opponentboard, &maskr[5][2])] +
          pattern_arr[evaluate_ptr_num][5][3][ppext(playerboard, &maskr[5][3])][ppext(opponentboard, &maskr[5][3])]);

    a += (pattern_arr[evaluate_ptr_num][6][0][ppext(playerboard, &maskr[6][0])][ppext(opponentboard, &maskr[6][0])] +
          pattern_arr[evaluate_ptr_num][6][1][ppext(playerboard, &maskr[6][1])][ppext(opponentboard, &maskr[6][1])] +
          pattern_arr[evaluate_ptr_num][6][2][ppext(playerboard, &maskr[6][2])][ppext(opponentboard, &maskr[6][2])] +
          pattern_arr[evaluate_ptr_num][6][3][ppext(playerboard, &maskr[6][3])][ppext(opponentboard, &maskr[6][3])]);

    a += (pattern_arr[evaluate_ptr_num][7][0][ppext(playerboard, &maskr[7][0])][ppext(opponentboard, &maskr[7][0])] +
          pattern_arr[evaluate_ptr_num][7][1][ppext(playerboard, &maskr[7][1])][ppext(opponentboard, &maskr[7][1])] +
          pattern_arr[evaluate_ptr_num][7][2][ppext(playerboard, &maskr[7][2])][ppext(opponentboard, &maskr[7][2])] +
          pattern_arr[evaluate_ptr_num][7][3][ppext(playerboard, &maskr[7][3])][ppext(opponentboard, &maskr[7][3])]);

    a += (pattern_arr[evaluate_ptr_num][8][0][ppext(playerboard, &maskr[8][0])][ppext(opponentboard, &maskr[8][0])] +
          pattern_arr[evaluate_ptr_num][8][1][ppext(playerboard, &maskr[8][1])][ppext(opponentboard, &maskr[8][1])] +
          pattern_arr[evaluate_ptr_num][8][2][ppext(playerboard, &maskr[8][2])][ppext(opponentboard, &maskr[8][2])] +
          pattern_arr[evaluate_ptr_num][8][3][ppext(playerboard, &maskr[8][3])][ppext(opponentboard, &maskr[8][3])]);

    a += (pattern_arr[evaluate_ptr_num][9][0][ppext(playerboard, &maskr[9][0])][ppext(opponentboard, &maskr[9][0])] +
          pattern_arr[evaluate_ptr_num][9][1][ppext(playerboard, &maskr[9][1])][ppext(opponentboard, &maskr[9][1])] +
          pattern_arr[evaluate_ptr_num][9][2][ppext(playerboard, &maskr[9][2])][ppext(opponentboard, &maskr[9][2])] +
          pattern_arr[evaluate_ptr_num][9][3][ppext(playerboard, &maskr[9][3])][ppext(opponentboard, &maskr[9][3])]);

    a += (pattern_arr[evaluate_ptr_num][10][0][ppext(playerboard, &maskr[10][0])][ppext(opponentboard, &maskr[10][0])] +
          pattern_arr[evaluate_ptr_num][10][1][ppext(playerboard, &maskr[10][1])][ppext(opponentboard, &maskr[10][1])] +
          pattern_arr[evaluate_ptr_num][10][2][ppext(playerboard, &maskr[10][2])][ppext(opponentboard, &maskr[10][2])] +
          pattern_arr[evaluate_ptr_num][10][3][ppext(playerboard, &maskr[10][3])][ppext(opponentboard, &maskr[10][3])]);

    a += (pattern_arr[evaluate_ptr_num][11][0][ppext(playerboard, &maskr[11][0])][ppext(opponentboard, &maskr[11][0])] +
          pattern_arr[evaluate_ptr_num][11][1][ppext(playerboard, &maskr[11][1])][ppext(opponentboard, &maskr[11][1])] +
          pattern_arr[evaluate_ptr_num][11][2][ppext(playerboard, &maskr[11][2])][ppext(opponentboard, &maskr[11][2])] +
          pattern_arr[evaluate_ptr_num][11][3][ppext(playerboard, &maskr[11][3])][ppext(opponentboard, &maskr[11][3])]);
  
    a += final_bias[evaluate_ptr_num];
    

    return llround(a * 64000000000);
}
