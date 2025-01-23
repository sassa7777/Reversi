#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <Siv3D.hpp>

using namespace std;
using bitboard = pair<uint64_t, uint64_t>;

#ifdef __GNUC__
#define flipVertical(x) __builtin_bswap64(x)
#else
#define flipVertical(x) _byteswap_uint64(x)
#endif

#define n_patterns 12 // 使うパターンの種類
constexpr int pattern_sizes[n_patterns] = {8, 7, 6, 5, 10, 8, 8, 8, 9, 10, 10, 10}; // パターンごとのマスの数

constexpr uint64_t bit_pattern[] = {0x8040201008040201, 0x4020100804020100, 0x2010080402010000, 0x1008040201000000, 0xff42000000000000, 0xff000000000000, 0xff0000000000, 0xff00000000, 0xe0e0e00000000000, 0xf8c0808080000000, 0xbd3c000000000000, 0xf0e0c08000000000};

const vector<vector<int>> bit_positions = {{0, 9, 18, 27, 36, 45, 54, 63}, {1, 10, 19, 28, 37, 46, 55}, {2, 11, 20, 29, 38, 47}, {3, 12, 21, 30, 39}, {9, 0, 1, 2, 3, 4, 5, 6, 7, 14}, {8, 9, 10, 11, 12, 13, 14, 15}, {16, 17, 18, 19, 20, 21, 22, 23}, {24, 25, 26, 27, 28, 29, 30, 31}, {0, 1, 8, 9, 2, 16, 10, 17, 18}, {32, 24, 16, 8, 0, 9, 1, 2, 3, 4}, {0, 2, 3, 10, 11, 12, 13, 4, 5, 7}, {0, 1, 2, 3, 8, 9, 10, 16, 17, 24}};

constexpr uint64_t mn[11][4] = {{0x513de207d3f3037d, 0x014fe47f65da7454, 0x0, 0x0}, {0x0058211811e33407, 0x0038385079e568ac, 0x35dfe3487e16bea8, 0x0000437ea663be08}, {0x0000780f605dab1a, 0x0096280cc026ba33, 0x32892c0479f5bf02, 0x0000009c840421c2}, {0x0000009dbeafc4b9, 0x09b7c9bf900046c6, 0x1c87f1fbace32e75, 0x00000001c4e67750}, {0x00000000000026fd, 0x6d7e128964970ce4, 0x657fa299405c5d12, 0x016d8e3e8d0ec8cf}, {0x14432327482504ce, 0x1379c9ffc0076bf1, 0x00ccfd6665e66667, 0x01bd45d2f475a716}, {0x14432327480b0000, 0x0977fb411a695d0e, 0x00003f8987e8a517, 0x037fe7dc0050ffaa}, {0x1443232743767e38, 0x1bf78ffc3a34d4e7, 0x1443232100000000, 0x0019bc0721dbc558}, {0x000000000003e7fd, 0x14432327483ed99b, 0x44f0aa0055758901, 0x123af351dfe9423a}, {0x000000018ed7c80d, 0x000000cf890057d1, 0x56f8bd8dc5ed044f, 0x034d0fcf0a62e000}, {0x0000000000001f35, 0x2a7e9d809b390195, 0x14c2563f0c4ee85d, 0x007e31f1178d5f15}};

constexpr size_t comp[11] = {1, 1, 0, 0, 3, 1, 1, 1, 1, 3, 3};
constexpr size_t shn[11] = {55, 56, 58, 59, 51, 55, 55, 55, 54, 51, 51};

// モデルの設計パラメータ
#define n_dense0 128
#define n_dense1 128
#define n_all_input 12
#define use_book true

double final_dense[n_all_input];
double final_bias[3];

ankerl::unordered_dense::map<bitboard, double> pattern_map[3][n_patterns];
vector<vector<vector<vector<vector<double>>>>> pattern_arr(3, vector<vector<vector<vector<double>>>>(n_patterns));

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
    size_t popcountPattern = popcount(pattern1);
    pattern_arr[ptr_num][evaluate_idx].resize(4);
    for (auto &p :pattern_arr[ptr_num][evaluate_idx]) {
        p.resize(cal_pow(2, (int)(popcountPattern+comp[evaluate_idx])));
        for (auto &pp : p) {
            pp.resize(cal_pow(2, (int)(popcountPattern+comp[evaluate_idx])));
        }
    }
    size_t totalCombinations = cal_pow(2, (int)popcountPattern);
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
            double result = predict_pattern(pattern_size, arr, dense0, bias0, dense1, bias1, dense2, bias2);
            auto key1 = make_pair(newpattern1, newpattern2);
            auto key2 = make_pair(r90(newpattern1), r90(newpattern2));
            auto key3 = make_pair(r180(newpattern1), r180(newpattern2));
            auto key4 = make_pair(l90(newpattern1), l90(newpattern2));
            if (pattern_idx == 11) {
                pattern_map[ptr_num][evaluate_idx][key1] = result;
                pattern_map[ptr_num][evaluate_idx][key2] = result;
                pattern_map[ptr_num][evaluate_idx][key3] = result;
                pattern_map[ptr_num][evaluate_idx][key4] = result;
            } else {
                size_t e = evaluate_idx;
                pattern_arr[ptr_num][e][0][(key1.first * mn[e][0]) >> shn[e]][(key1.second * mn[e][0]) >> shn[e]] = result;
                pattern_arr[ptr_num][e][1][(key2.first * mn[e][1]) >> shn[e]][(key2.second * mn[e][1]) >> shn[e]] = result;
                pattern_arr[ptr_num][e][2][(key3.first * mn[e][2]) >> shn[e]][(key3.second * mn[e][2]) >> shn[e]] = result;
                pattern_arr[ptr_num][e][3][(key4.first * mn[e][3]) >> shn[e]][(key4.second * mn[e][3]) >> shn[e]] = result;
            }
        }
    }
}

inline void evaluate_init(String model_path, int ptr_num){
    for (auto &ptar : pattern_arr[ptr_num]) {
        ptar.clear();
    }
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
                dense0[i][j] = stod(line);
            }
        }
        for (i = 0; i < n_dense0; ++i){
            getline(ifs, line);
            bias0[i] = stod(line);
        }
        for (i = 0; i < n_dense1; ++i){
            for (j = 0; j < n_dense0; ++j){
                getline(ifs, line);
                dense1[i][j] = stod(line);
            }
        }
        for (i = 0; i < n_dense1; ++i){
            getline(ifs, line);
            bias1[i] = stod(line);
        }
        for (i = 0; i < n_dense1; ++i){
            getline(ifs, line);
            dense2[i] = stod(line);
        }
        getline(ifs, line);
        bias2 = stod(line);
        pre_evaluation_pattern(ptr_num, pattern_idx, pattern_idx, pattern_sizes[pattern_idx], dense0, bias0, dense1, bias1, dense2, bias2);
    }
    for (i = 0; i < n_all_input; ++i){
        getline(ifs, line);
        final_dense[i] = stod(line);
    }
    for (i = 0; i < 11; ++i){
        for (auto &ptr : pattern_arr[ptr_num][i]) {
            for (auto &pt : ptr) {
                for (auto &ptt : pt) {
                    ptt *= final_dense[i];
                }
            }
        }
    }
    for (auto &ptr : pattern_map[ptr_num][11]) {
        ptr.second *= final_dense[11];
    }
    getline(ifs, line);
    final_bias[ptr_num] = stod(line);
}

inline int64_t evaluate_moveorder(uint64_t playerboard, uint64_t opponentboard) noexcept {
    
    double a = 0;
    
    a += (pattern_arr[evaluate_ptr_num][0][0][((playerboard & 0x8040201008040201) * mn[0][0]) >> shn[0]]
              [((opponentboard & 0x8040201008040201) * mn[0][0]) >> shn[0]] +
          pattern_arr[evaluate_ptr_num][0][1][((playerboard & 0x0102040810204080) * mn[0][1]) >> shn[0]]
              [((opponentboard & 0x0102040810204080) * mn[0][1]) >> shn[0]]);

    a += (pattern_arr[evaluate_ptr_num][1][0][((playerboard & 0x4020100804020100) * mn[1][0]) >> shn[1]]
              [((opponentboard & 0x4020100804020100) * mn[1][0]) >> shn[1]] +
          pattern_arr[evaluate_ptr_num][1][1][((playerboard & 0x0001020408102040) * mn[1][1]) >> shn[1]]
              [((opponentboard & 0x0001020408102040) * mn[1][1]) >> shn[1]] +
          pattern_arr[evaluate_ptr_num][1][2][((playerboard & 0x0080402010080402) * mn[1][2]) >> shn[1]]
              [((opponentboard & 0x0080402010080402) * mn[1][2]) >> shn[1]] +
          pattern_arr[evaluate_ptr_num][1][3][((playerboard & 0x0204081020408000) * mn[1][3]) >> shn[1]]
              [((opponentboard & 0x0204081020408000) * mn[1][3]) >> shn[1]]);

    a += (pattern_arr[evaluate_ptr_num][5][0][((playerboard & 0xff000000000000) * mn[5][0]) >> shn[5]]
              [((opponentboard & 0xff000000000000) * mn[5][0]) >> shn[5]] +
          pattern_arr[evaluate_ptr_num][5][1][((playerboard & 0x0202020202020202) * mn[5][1]) >> shn[5]]
              [((opponentboard & 0x0202020202020202) * mn[5][1]) >> shn[5]] +
          pattern_arr[evaluate_ptr_num][5][2][((playerboard & 0x000000000000ff00) * mn[5][2]) >> shn[5]]
              [((opponentboard & 0x000000000000ff00) * mn[5][2]) >> shn[5]] +
          pattern_arr[evaluate_ptr_num][5][3][((playerboard & 0x4040404040404040) * mn[5][3]) >> shn[5]]
              [((opponentboard & 0x4040404040404040) * mn[5][3]) >> shn[5]]);

    a += (pattern_map[evaluate_ptr_num][11].at({playerboard & 0xf0e0c08000000000, opponentboard & 0xf0e0c08000000000}) +
          pattern_map[evaluate_ptr_num][11].at({playerboard & 0x0f07030100000000, opponentboard & 0x0f07030100000000}) +
          pattern_map[evaluate_ptr_num][11].at({playerboard & 0x000000000103070f, opponentboard & 0x000000000103070f}) +
          pattern_map[evaluate_ptr_num][11].at({playerboard & 0x0000000080c0e0f0, opponentboard & 0x0000000080c0e0f0}));
    
    a += final_bias[evaluate_ptr_num];

    return llround(a * 640000000);
}

inline int64_t evaluate(uint64_t playerboard, uint64_t opponentboard) noexcept {

    if (!playerboard) return -64000000000;
    if (!opponentboard) return 64000000000;
    if (afterIndex >= 64) return (popcount(playerboard)-popcount(opponentboard))*1000000000;
    
    double a = 0;
    a += (pattern_arr[evaluate_ptr_num][0][0][((playerboard & 0x8040201008040201) * mn[0][0]) >> shn[0]]
          [((opponentboard & 0x8040201008040201) * mn[0][0]) >> shn[0]] +
          pattern_arr[evaluate_ptr_num][0][1][((playerboard & 0x0102040810204080) * mn[0][1]) >> shn[0]]
          [((opponentboard & 0x0102040810204080) * mn[0][1]) >> shn[0]]);

    a += (pattern_arr[evaluate_ptr_num][1][0][((playerboard & 0x4020100804020100) * mn[1][0]) >> shn[1]]
          [((opponentboard & 0x4020100804020100) * mn[1][0]) >> shn[1]] +
          pattern_arr[evaluate_ptr_num][1][1][((playerboard & 0x0001020408102040) * mn[1][1]) >> shn[1]]
          [((opponentboard & 0x0001020408102040) * mn[1][1]) >> shn[1]] +
          pattern_arr[evaluate_ptr_num][1][2][((playerboard & 0x0080402010080402) * mn[1][2]) >> shn[1]]
          [((opponentboard & 0x0080402010080402) * mn[1][2]) >> shn[1]] +
          pattern_arr[evaluate_ptr_num][1][3][((playerboard & 0x0204081020408000) * mn[1][3]) >> shn[1]]
          [((opponentboard & 0x0204081020408000) * mn[1][3]) >> shn[1]]);

    a += (pattern_arr[evaluate_ptr_num][2][0][((playerboard & 0x2010080402010000) * mn[2][0]) >> shn[2]]
          [((opponentboard & 0x2010080402010000) * mn[2][0]) >> shn[2]] +
          pattern_arr[evaluate_ptr_num][2][1][((playerboard & 0x0000010204081020) * mn[2][1]) >> shn[2]]
          [((opponentboard & 0x0000010204081020) * mn[2][1]) >> shn[2]] +
          pattern_arr[evaluate_ptr_num][2][2][((playerboard & 0x0000804020100804) * mn[2][2]) >> shn[2]]
          [((opponentboard & 0x0000804020100804) * mn[2][2]) >> shn[2]] +
          pattern_arr[evaluate_ptr_num][2][3][((playerboard & 0x0408102040800000) * mn[2][3]) >> shn[2]]
          [((opponentboard & 0x0408102040800000) * mn[2][3]) >> shn[2]]);

    a += (pattern_arr[evaluate_ptr_num][3][0][((playerboard & 0x1008040201000000) * mn[3][0]) >> shn[3]]
          [((opponentboard & 0x1008040201000000) * mn[3][0]) >> shn[3]] +
          pattern_arr[evaluate_ptr_num][3][1][((playerboard & 0x0000000102040810) * mn[3][1]) >> shn[3]]
          [((opponentboard & 0x0000000102040810) * mn[3][1]) >> shn[3]] +
          pattern_arr[evaluate_ptr_num][3][2][((playerboard & 0x0000008040201008) * mn[3][2]) >> shn[3]]
          [((opponentboard & 0x0000008040201008) * mn[3][2]) >> shn[3]] +
          pattern_arr[evaluate_ptr_num][3][3][((playerboard & 0x0810204080000000) * mn[3][3]) >> shn[3]]
          [((opponentboard & 0x0810204080000000) * mn[3][3]) >> shn[3]]);

    a += (pattern_arr[evaluate_ptr_num][4][0][((playerboard & 0x42FF) * mn[4][0]) >> shn[4]]
          [((opponentboard & 0x42FF) * mn[4][0]) >> shn[4]] +
          pattern_arr[evaluate_ptr_num][4][1][((playerboard & 0x80c080808080c080) * mn[4][1]) >> shn[4]]
          [((opponentboard & 0x80c080808080c080) * mn[4][1]) >> shn[4]] +
          pattern_arr[evaluate_ptr_num][4][2][((playerboard & 0xff42000000000000) * mn[4][2]) >> shn[4]]
          [((opponentboard & 0xff42000000000000) * mn[4][2]) >> shn[4]] +
          pattern_arr[evaluate_ptr_num][4][3][((playerboard & 0x0103010101010301) * mn[4][3]) >> shn[4]]
          [((opponentboard & 0x0103010101010301) * mn[4][3]) >> shn[4]]);

    a += (pattern_arr[evaluate_ptr_num][5][0][((playerboard & 0xff000000000000) * mn[5][0]) >> shn[5]]
          [((opponentboard & 0xff000000000000) * mn[5][0]) >> shn[5]] +
          pattern_arr[evaluate_ptr_num][5][1][((playerboard & 0x0202020202020202) * mn[5][1]) >> shn[5]]
          [((opponentboard & 0x0202020202020202) * mn[5][1]) >> shn[5]] +
          pattern_arr[evaluate_ptr_num][5][2][((playerboard & 0x000000000000ff00) * mn[5][2]) >> shn[5]]
          [((opponentboard & 0x000000000000ff00) * mn[5][2]) >> shn[5]] +
          pattern_arr[evaluate_ptr_num][5][3][((playerboard & 0x4040404040404040) * mn[5][3]) >> shn[5]]
          [((opponentboard & 0x4040404040404040) * mn[5][3]) >> shn[5]]);

    a += (pattern_arr[evaluate_ptr_num][6][0][((playerboard & 0xff0000000000) * mn[6][0]) >> shn[6]]
          [((opponentboard & 0xff0000000000) * mn[6][0]) >> shn[6]] +
          pattern_arr[evaluate_ptr_num][6][1][((playerboard & 0x0404040404040404) * mn[6][1]) >> shn[6]]
          [((opponentboard & 0x0404040404040404) * mn[6][1]) >> shn[6]] +
          pattern_arr[evaluate_ptr_num][6][2][((playerboard & 0x0000000000ff0000) * mn[6][2]) >> shn[6]]
          [((opponentboard & 0x0000000000ff0000) * mn[6][2]) >> shn[6]] +
          pattern_arr[evaluate_ptr_num][6][3][((playerboard & 0x2020202020202020) * mn[6][3]) >> shn[6]]
          [((opponentboard & 0x2020202020202020) * mn[6][3]) >> shn[6]]);

    a += (pattern_arr[evaluate_ptr_num][7][0][((playerboard & 0xff00000000) * mn[7][0]) >> shn[7]]
          [((opponentboard & 0xff00000000) * mn[7][0]) >> shn[7]] +
          pattern_arr[evaluate_ptr_num][7][1][((playerboard & 0x0808080808080808) * mn[7][1]) >> shn[7]]
          [((opponentboard & 0x0808080808080808) * mn[7][1]) >> shn[7]] +
          pattern_arr[evaluate_ptr_num][7][2][((playerboard & 0x00000000ff000000) * mn[7][2]) >> shn[7]]
          [((opponentboard & 0x00000000ff000000) * mn[7][2]) >> shn[7]] +
          pattern_arr[evaluate_ptr_num][7][3][((playerboard & 0x1010101010101010) * mn[7][3]) >> shn[7]]
          [((opponentboard & 0x1010101010101010) * mn[7][3]) >> shn[7]]);

    a += (pattern_arr[evaluate_ptr_num][8][0][((playerboard & 0xe0e0e00000000000) * mn[8][0]) >> shn[8]]
          [((opponentboard & 0xe0e0e00000000000) * mn[8][0]) >> shn[8]] +
          pattern_arr[evaluate_ptr_num][8][1][((playerboard & 0x0707070000000000) * mn[8][1]) >> shn[8]]
          [((opponentboard & 0x0707070000000000) * mn[8][1]) >> shn[8]] +
          pattern_arr[evaluate_ptr_num][8][2][((playerboard & 0x0000000000070707) * mn[8][2]) >> shn[8]]
          [((opponentboard & 0x0000000000070707) * mn[8][2]) >> shn[8]] +
          pattern_arr[evaluate_ptr_num][8][3][((playerboard & 0x0000000000e0e0e0) * mn[8][3]) >> shn[8]]
          [((opponentboard & 0x0000000000e0e0e0) * mn[8][3]) >> shn[8]]);

    a += (pattern_arr[evaluate_ptr_num][9][0][((playerboard & 0xf8c0808080000000) * mn[9][0]) >> shn[9]]
          [((opponentboard & 0xf8c0808080000000) * mn[9][0]) >> shn[9]] +
          pattern_arr[evaluate_ptr_num][9][1][((playerboard & 0x1f03010101000000) * mn[9][1]) >> shn[9]]
          [((opponentboard & 0x1f03010101000000) * mn[9][1]) >> shn[9]] +
          pattern_arr[evaluate_ptr_num][9][2][((playerboard & 0x000000010101031f) * mn[9][2]) >> shn[9]]
          [((opponentboard & 0x000000010101031f) * mn[9][2]) >> shn[9]] +
          pattern_arr[evaluate_ptr_num][9][3][((playerboard & 0x000000808080c0f8) * mn[9][3]) >> shn[9]]
          [((opponentboard & 0x000000808080c0f8) * mn[9][3]) >> shn[9]]);

    a += (pattern_arr[evaluate_ptr_num][10][0][((playerboard & 0xbd3c000000000000) * mn[10][0]) >> shn[10]]
          [((opponentboard & 0xbd3c000000000000) * mn[10][0]) >> shn[10]] +
          pattern_arr[evaluate_ptr_num][10][1][((playerboard & 0x0100030303030001) * mn[10][1]) >> shn[10]]
          [((opponentboard & 0x0100030303030001) * mn[10][1]) >> shn[10]] +
          pattern_arr[evaluate_ptr_num][10][2][((playerboard & 0x0000000000003cbd) * mn[10][2]) >> shn[10]]
          [((opponentboard & 0x0000000000003cbd) * mn[10][2]) >> shn[10]] +
          pattern_arr[evaluate_ptr_num][10][3][((playerboard & 0x0000010103010300) * mn[10][3]) >> shn[10]]
          [((opponentboard & 0x0000010103010300) * mn[10][3]) >> shn[10]]);

    a += (pattern_map[evaluate_ptr_num][11].at({playerboard & 0xf0e0c08000000000, opponentboard & 0xf0e0c08000000000}) +
          pattern_map[evaluate_ptr_num][11].at({playerboard & 0x0f07030100000000, opponentboard & 0x0f07030100000000}) +
          pattern_map[evaluate_ptr_num][11].at({playerboard & 0x000000000103070f, opponentboard & 0x000000000103070f}) +
          pattern_map[evaluate_ptr_num][11].at({playerboard & 0x0000000080c0e0f0, opponentboard & 0x0000000080c0e0f0}));
    a += final_bias[evaluate_ptr_num];
    

    return llround(a * 64000000000);
}
