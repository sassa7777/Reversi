#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <Siv3D.hpp>

using namespace std;

#ifdef __GNUC__
#define popcountll(x) __builtin_popcountll(x)
#define clzll(x) __builtin_clzll(x)
#else
#define    popcountll(x) __popcnt64(x)
#define clzll(x) _lzcnt_u64(x)
#endif

#define n_patterns 13 // 使うパターンの種類
constexpr int pattern_sizes[n_patterns] = {8, 7, 6, 5, 4, 10, 8, 8, 8, 9, 10, 10, 10}; // パターンごとのマスの数

vector<vector<uint64_t>> bit_pattern = {{0x8040201008040201, 0x0102040810204080}, {0x4020100804020100, 0x0080402010080402, 0x0204081020408000, 0x0001020408102040}, {0x2010080402010000, 0x0000804020100804, 0x0408102040800000, 0x0000010204081020}, {0x1008040201000000, 0x0000008040201008, 0x0810204080000000, 0x0000000102040810}, {0x0804020100000000, 0x0000000080402010, 0x1020408000000000, 0x0000000001020408}, {0xff42000000000000, 0x80c080808080c080, 0x00000000000042ff, 0x0103010101010301}, {0x00ff000000000000, 0x000000000000ff00, 0x4040404040404040, 0x0202020202020202}, {0x0000ff0000000000, 0x0000000000ff0000, 0x2020202020202020, 0x0404040404040404}, {0x000000ff00000000, 0x00000000ff000000, 0x1010101010101010, 0x0808080808080808}, {0xe0e0e00000000000, 0x0707070000000000, 0x0000000000e0e0e0, 0x0000000000070707}, {0xf8c0808080000000, 0x1f03010101000000, 0x000000808080c0f8, 0x000000010101031f}, {0xbd3c000000000000, 0x0100030303030001, 0x0000000000003cbd, 0x8000c0c0c0c00080}, {0xf0e0c08000000000, 0x0f07030100000000, 0x000000000103070f, 0x0000000080c0e0f0}};

const vector<vector<vector<int>>> bit_positions = {
    {{0, 9, 18, 27, 36, 45, 54, 63}, {7, 14, 21, 28, 35, 42, 49, 56}},
    {{1, 10, 19, 28, 37, 46, 55}, {8, 17, 26, 35, 44, 53, 62}, {6, 13, 20, 27, 34, 41, 48}, {15, 22, 29, 36, 43, 50, 57}},
    {{2, 11, 20, 29, 38, 47}, {16, 25, 34, 43, 52, 61}, {5, 12, 19, 26, 33, 40}, {23, 30, 37, 44, 51, 58}},
    {{3, 12, 21, 30, 39}, {24, 33, 42, 51, 60}, {4, 11, 18, 25, 32}, {31, 38, 45, 52, 59}},
    {{4, 13, 22, 31}, {32, 41, 50, 59}, {3, 10, 17, 24}, {39, 46, 53, 60}},
    {{9, 0, 1, 2, 3, 4, 5, 6, 7, 14}, {9, 0, 8, 16, 24, 32, 40, 48, 56, 49}, {49, 56, 57, 58, 59, 60, 61, 62, 63, 54}, {54, 63, 55, 47, 39, 31, 23, 15, 7, 14}},
    {{8, 9, 10, 11, 12, 13, 14, 15}, {48, 49, 50, 51, 52, 53, 54, 55}, {1, 9, 17, 25, 33, 41, 49, 57}, {6, 14, 22, 30, 38, 46, 54, 62}},
    {{16, 17, 18, 19, 20, 21, 22, 23}, {40, 41, 42, 43, 44, 45, 46, 47}, {2, 10, 18, 26, 34, 42, 50, 58}, {5, 13, 21, 29, 37, 45, 53, 61}},
    {{24, 25, 26, 27, 28, 29, 30, 31}, {32, 33, 34, 35, 36, 37, 38, 39}, {3, 11, 19, 27, 35, 43, 51, 59}, {4, 12, 20, 28, 36, 44, 52, 60}},
    {{0, 1, 8, 9, 2, 16, 10, 17, 18}, {7, 6, 15, 14, 5, 23, 13, 22, 21}, {56, 48, 57, 49, 40, 58, 41, 50, 42}, {63, 62, 55, 54, 61, 47, 53, 46, 45}},
    {{32, 24, 16, 8, 0, 9, 1, 2, 3, 4}, {39, 31, 23, 15, 7, 14, 6, 5, 4, 3}, {24, 32, 40, 48, 56, 49, 57, 58, 59, 60}, {59, 60, 61, 62, 63, 54, 55, 47, 39, 31}},
    {{0, 2, 3, 10, 11, 12, 13, 4, 5, 7}, {7, 23, 31, 22, 30, 38, 46, 39, 47, 63}, {56, 58, 59, 50, 51, 52, 53, 60, 61, 63}, {0, 16, 24, 17, 25, 33, 41, 32, 40, 56}},
    {{0, 1, 2, 3, 8, 9, 10, 16, 17, 24}, {7, 6, 5, 4, 15, 14, 13, 23, 22, 31}, {63, 62, 61, 60, 55, 54, 53, 47, 46, 39}, {56, 57, 58, 59, 48, 49, 50, 40, 41, 32}}
};
// モデルの設計パラメータ
#define n_dense0 16
#define n_dense1 16
#define n_add_input 1
#define n_add_dense0 8
#define n_all_input 14
#define max_mobility 36

array<double, max_mobility * 2 + 1> add_arr;
double final_dense[n_all_input];
double final_bias;

vector<ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, double>> pattern_arr(n_patterns);

uint64_t delta_swap(uint64_t& x, uint64_t mask, int delta) {
    uint64_t t = (x ^ (x >> delta)) & mask;
    return x ^ t ^ (t << delta);
}

// 水平反転
uint64_t flipHorizontal(uint64_t x) {
    x = delta_swap(x, 0x5555555555555555, 1);
    x = delta_swap(x, 0x3333333333333333, 2);
    return delta_swap(x, 0x0F0F0F0F0F0F0F0F, 4);
}

// 垂直反転
uint64_t flipVertical(uint64_t x) {
    x = delta_swap(x, 0x00FF00FF00FF00FF, 8);
    x = delta_swap(x, 0x0000FFFF0000FFFF, 16);
    return delta_swap(x, 0x00000000FFFFFFFF, 32);
}

// A1-H8反転
uint64_t flipDiagonalA1H8(uint64_t x) {
    x = delta_swap(x, 0x00AA00AA00AA00AA, 7);
    x = delta_swap(x, 0x0000CCCC0000CCCC, 14);
    return delta_swap(x, 0x00000000F0F0F0F0, 28);
}

// A8-H1反転
uint64_t flipDiagonalA8H1(uint64_t x) {
    x = delta_swap(x, 0x0055005500550055, 9);
    x = delta_swap(x, 0x0000333300003333, 18);
    return delta_swap(x, 0x000000000F0F0F0F, 36);
}

// 時計回りに90度回転
uint64_t rotateClockwise90(uint64_t x) {
    return flipHorizontal(flipDiagonalA1H8(x));
}

uint64_t rotateCounterclockwise90(uint64_t x) {
    return flipVertical(flipDiagonalA1H8(x));
}

inline int cal_pow(int x, int depth) {
    if(depth == 1) return x;
    return x * cal_pow(x, depth - 1);
}

inline double leaky_relu(double x){
    return max(0.01 * x, x);
}

inline double predict_pattern(int pattern_size, double in_arr[], double dense0[n_dense0][20], double bias0[n_dense0], double dense1[n_dense1][n_dense0], double bias1[n_dense1], double dense2[n_dense1], double bias2){
    double hidden0[16], hidden1;
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

inline void pre_evaluation_pattern(int pattern_idx, int evaluate_idx, int pattern_size, double dense0[n_dense0][20], double bias0[n_dense0], double dense1[n_dense1][n_dense0], double bias1[n_dense1], double dense2[n_dense1], double bias2){
    double arr[20];
    for (size_t i = 0; i < bit_positions[pattern_idx].size(); ++i) {
        vector<int> bitpositions = bit_positions[pattern_idx][i];
        uint64_t pattern1 = bit_pattern[pattern_idx][i];
        size_t totalCombinations = cal_pow(2, popcount(pattern1));
        for (size_t comb = 0; comb < totalCombinations; ++comb) {
            uint64_t newpattern1 = pattern1;
            for (size_t i = 0; i < bitpositions.size(); ++i) {
                if ((comb >> i) & 1) {
                    newpattern1 ^= (0x8000000000000000ULL >> bitpositions[i]);
                }
            }
            for (size_t comb = 0; comb < totalCombinations; ++comb) {
                uint64_t newpattern2 = pattern1;
                for (size_t i = 0; i < bitpositions.size(); ++i) {
                    if ((comb >> i) & 1) {
                        newpattern2 ^= (0x8000000000000000ULL >> bitpositions[i]);
                    }
                }
                if(newpattern1 & newpattern2) continue;
                for (int i = 0; i < popcount(pattern1); ++i) {
                    if(newpattern1 & (0x8000000000000000ULL >> bitpositions[i])) {
                        arr[i] = 1.0;
                        arr[pattern_size + i] = 0.0;
                    }
                    else if(newpattern2 & (0x8000000000000000ULL >> bitpositions[i])) {
                        arr[i] = 0.0;
                        arr[pattern_size + i] = 1.0;
                    }
                    else {
                        arr[i] = 0;
                        arr[pattern_size + i] = 0.0;
                    }
                }
                pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)] = predict_pattern(pattern_size,
                                                                                                 arr,
                                                                                                 dense0,
                                                                                                 bias0,
                                                                                                 dense1,
                                                                                                 bias1,
                                                                                                 dense2,
                                                                                                 bias2);
//                pattern_arr[evaluate_idx][make_pair(rotateClockwise90(newpattern1), rotateClockwise90(newpattern2))] = pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)];
//                pattern_arr[evaluate_idx][make_pair(rotateClockwise90(rotateClockwise90(newpattern1)), rotateClockwise90(rotateClockwise90(newpattern2)))] = pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)];
//                pattern_arr[evaluate_idx][make_pair(rotateCounterclockwise90(newpattern1), rotateCounterclockwise90(newpattern2))] = pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)];
            }
        }
    }
}

// 追加パラメータの推論
inline double predict_add(int mobility, double dense0[n_add_dense0][n_add_input], double bias0[n_add_dense0], double dense1[n_add_dense0], double bias1){
    double hidden0[n_add_dense0], in_arr[n_add_input];
    int i, j;
    in_arr[0] = (double)mobility / 30.0;
    for (i = 0; i < n_add_dense0; ++i){
        hidden0[i] = bias0[i];
        for (j = 0; j < n_add_input; ++j)
            hidden0[i] += in_arr[j] * dense0[i][j];
        hidden0[i] = leaky_relu(hidden0[i]);
    }
    double res = bias1;
    for (j = 0; j < n_add_dense0; ++j)
        res += hidden0[j] * dense1[j];
    res = leaky_relu(res);
    return res;
}

// 追加パラメータの前計算
inline void pre_evaluation_add(double dense0[n_add_dense0][n_add_input], double bias0[n_add_dense0], double dense1[n_add_dense0], double bias1){
    int mobility;
    for (mobility = -max_mobility; mobility <= max_mobility; ++mobility){
        add_arr[mobility + max_mobility] = predict_add(mobility, dense0, bias0, dense1, bias1);
    }
}

inline void evaluate_init() {
    for (auto &ptar : pattern_arr) {
        ptar.clear();
    }
    ifstream ifs(FileSystem::RelativePath(Resource(U"model.txt")).narrow());
    if (ifs.fail()){
        cerr << "evaluation file does not exist" << endl;
        exit(1);
    }
    string line;
    int i, j, pattern_idx;

    // モデルのパラメータを格納する
    double dense0[n_dense0][20];
    double bias0[n_dense0];
    double dense1[n_dense1][n_dense0];
    double bias1[n_dense1];
    double dense2[n_dense1];
    double bias2;
    double add_dense0[n_add_dense0][n_add_input];
    double add_bias0[n_add_dense0];
    double add_dense1[n_add_dense0];
    double add_bias1;

    // パターンのパラメータを得て前計算をする
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
        pre_evaluation_pattern(pattern_idx, pattern_idx, pattern_sizes[pattern_idx], dense0, bias0, dense1, bias1, dense2, bias2);
    }
    // 追加入力のパラメータを得て前計算をする
    for (i = 0; i < n_add_dense0; ++i){
        for (j = 0; j < n_add_input; ++j){
            getline(ifs, line);
            add_dense0[i][j] = stod(line);
        }
    }
    for (i = 0; i < n_add_dense0; ++i){
        getline(ifs, line);
        add_bias0[i] = stod(line);
    }
    for (i = 0; i < n_add_dense0; ++i){
        getline(ifs, line);
        add_dense1[i] = stod(line);
    }
    getline(ifs, line);
    add_bias1 = stod(line);
    pre_evaluation_add(add_dense0, add_bias0, add_dense1, add_bias1);
    // 最後の層のパラメータを得る
    for (i = 0; i < n_all_input; ++i){
        getline(ifs, line);
        final_dense[i] = stod(line);
    }
    for (i = 0; i < n_patterns; ++i){
        for (auto &ptr : pattern_arr[i]) {
            ptr.second *= final_dense[i];
        }
    }
    cout << "evaluation initialized" << endl;
}

inline int64_t evaluate_moveorder(const uint64_t &playerboard, const uint64_t &opponentboard) {
    double a = (pattern_arr[0][make_pair(playerboard & 0x8040201008040201, opponentboard & 0x8040201008040201)] + pattern_arr[0][make_pair(playerboard & 0x0102040810204080, opponentboard & 0x0102040810204080)]);
    
    a += (pattern_arr[1][make_pair(playerboard & 0x4020100804020100, opponentboard & 0x4020100804020100)] + pattern_arr[1][make_pair(playerboard & 0x0001020408102040, opponentboard & 0x0001020408102040)] + pattern_arr[1][make_pair(playerboard & 0x0080402010080402, opponentboard & 0x0080402010080402)] + pattern_arr[1][make_pair(playerboard & 0x0204081020408000, opponentboard & 0x0204081020408000)]);
    
//    a += (pattern_arr[2][make_pair(playerboard & 0x2010080402010000, opponentboard & 0x2010080402010000)] + pattern_arr[2][make_pair(playerboard & 0x0000010204081020, opponentboard & 0x0000010204081020)] + pattern_arr[2][make_pair(playerboard & 0x0000804020100804, opponentboard & 0x0000804020100804)] + pattern_arr[2][make_pair(playerboard & 0x0408102040800000, opponentboard & 0x0408102040800000)]);
    
//    a += (pattern_arr[3][make_pair(playerboard & 0x1008040201000000, opponentboard & 0x1008040201000000)] + pattern_arr[3][make_pair(playerboard & 0x0000000102040810, opponentboard & 0x0000000102040810)] + pattern_arr[3][make_pair(playerboard & 0x0000008040201008, opponentboard & 0x0000008040201008)] + pattern_arr[3][make_pair(playerboard & 0x0810204080000000, opponentboard & 0x0810204080000000)]);
    
    a += (pattern_arr[5][make_pair(playerboard & 0x42FF, opponentboard & 0x42FF)] +
          pattern_arr[5][make_pair(playerboard & 0x80c080808080c080, opponentboard & 0x80c080808080c080)] + pattern_arr[5][make_pair(playerboard & 0xff42000000000000, opponentboard & 0xff42000000000000)] + pattern_arr[5][make_pair(playerboard & 0x0103010101010301, opponentboard & 0x0103010101010301)]);
    
//    a += (pattern_arr[6][make_pair(playerboard & 0xff000000000000, opponentboard & 0xff000000000000)] + pattern_arr[6][make_pair(playerboard & 0x0202020202020202, opponentboard & 0x0202020202020202)] + pattern_arr[6][make_pair(playerboard & 0x000000000000ff00, opponentboard & 0x000000000000ff00)] + pattern_arr[6][make_pair(playerboard & 0x4040404040404040, opponentboard & 0x4040404040404040)]);
    
//    a += (pattern_arr[7][make_pair(playerboard & 0xff0000000000, opponentboard & 0xff0000000000)] + pattern_arr[7][make_pair(playerboard & 0x0404040404040404, opponentboard & 0x0404040404040404)] + pattern_arr[7][make_pair(playerboard & 0x0000000000ff0000, opponentboard & 0x0000000000ff0000)] + pattern_arr[7][make_pair(playerboard & 0x2020202020202020, opponentboard & 0x2020202020202020)]);
    
//    a += (pattern_arr[8][make_pair(playerboard & 0xff00000000, opponentboard & 0xff00000000)] +
//          pattern_arr[8][make_pair(playerboard & 0x0808080808080808, opponentboard & 0x0808080808080808)] + pattern_arr[8][make_pair(playerboard & 0x00000000ff000000, opponentboard & 0x00000000ff000000)] + pattern_arr[8][make_pair(playerboard & 0x1010101010101010, opponentboard & 0x1010101010101010)]);
    
//    a += (pattern_arr[9][make_pair(playerboard & 0xe0e0e00000000000, opponentboard & 0xe0e0e00000000000)] + pattern_arr[9][make_pair(playerboard & 0x0707070000000000, opponentboard & 0x0707070000000000)] + pattern_arr[9][make_pair(playerboard & 0x0000000000070707, opponentboard & 0x0000000000070707)] + pattern_arr[9][make_pair(playerboard & 0x0000000000e0e0e0, opponentboard & 0x0000000000e0e0e0)]);
    
//    a += (pattern_arr[10][make_pair(playerboard & 0xf8c0808080000000, opponentboard & 0xf8c0808080000000)] + pattern_arr[10][make_pair(playerboard & 0x1f03010101000000, opponentboard & 0x1f03010101000000)] + pattern_arr[10][make_pair(playerboard & 0x000000010101031f, opponentboard & 0x000000010101031f)] + pattern_arr[10][make_pair(playerboard & 0x000000808080c0f8, opponentboard & 0x000000808080c0f8)]);
    
//    a += (pattern_arr[11][make_pair(playerboard & 0xbd3c000000000000, opponentboard & 0xbd3c000000000000)] + pattern_arr[11][make_pair(playerboard & 0x0100030303030001, opponentboard & 0x0100030303030001)] + pattern_arr[11][make_pair(playerboard & 0x0000000000003cbd, opponentboard & 0x0000000000003cbd)] + pattern_arr[11][make_pair(playerboard & 0x8000c0c0c0c00080, opponentboard & 0x8000c0c0c0c00080)]);
    
    a += (pattern_arr[12][make_pair(playerboard & 0xf0e0c08000000000, opponentboard & 0xf0e0c08000000000)] + pattern_arr[12][make_pair(playerboard & 0x0f07030100000000, opponentboard & 0x0f07030100000000)] + pattern_arr[12][make_pair(playerboard & 0x000000000103070f, opponentboard & 0x000000000103070f)] + pattern_arr[12][make_pair(playerboard & 0x0000000080c0e0f0, opponentboard & 0x0000000080c0e0f0)]);
    
//    if (a > 0) a+=1;
//    else a-=1;
//    a += score_fixedstone_table(playerboard, opponentboard)/10;
    a *= 10000000000;
//    a/=64;
//    cout << a << endl;
//    a += score_null_place(playerboard, opponentboard);
    
//    a += (double)score_putable(playerboard, opponentboard);
//    constexpr int64_t MIN_INF = -9223372036854775807;
//    constexpr int64_t MAX_INF = 9223372036854775807;
    return (int64_t)llround(a);
    
}

inline int64_t evaluate(const uint64_t &playerboard, const uint64_t &opponentboard) {
    
    int mobility = min(max_mobility * 2, max(0, max_mobility + popcountll(makelegalboard(playerboard, opponentboard))));
//    cout << mobility << endl;
    
//    double a = (pattern_arr[0][make_pair(playerboard & 0x8040201008040201, opponentboard & 0x8040201008040201)] + pattern_arr[0][make_pair(playerboard & 0x0102040810204080, opponentboard & 0x0102040810204080)]);
//    
//    a += (pattern_arr[1][make_pair(playerboard & 0x4020100804020100, opponentboard & 0x4020100804020100)] + pattern_arr[1][make_pair(playerboard & 0x0001020408102040, opponentboard & 0x0001020408102040)] + pattern_arr[1][make_pair(playerboard & 0x0080402010080402, opponentboard & 0x0080402010080402)] + pattern_arr[1][make_pair(playerboard & 0x0204081020408000, opponentboard & 0x0204081020408000)]);
//    
//    a += (pattern_arr[2][make_pair(playerboard & 0x2010080402010000, opponentboard & 0x2010080402010000)] + pattern_arr[2][make_pair(playerboard & 0x0000010204081020, opponentboard & 0x0000010204081020)] + pattern_arr[2][make_pair(playerboard & 0x0000804020100804, opponentboard & 0x0000804020100804)] + pattern_arr[2][make_pair(playerboard & 0x0408102040800000, opponentboard & 0x0408102040800000)]);
//    
//    a += (pattern_arr[3][make_pair(playerboard & 0x1008040201000000, opponentboard & 0x1008040201000000)] + pattern_arr[3][make_pair(playerboard & 0x0000000102040810, opponentboard & 0x0000000102040810)] + pattern_arr[3][make_pair(playerboard & 0x0000008040201008, opponentboard & 0x0000008040201008)] + pattern_arr[3][make_pair(playerboard & 0x0810204080000000, opponentboard & 0x0810204080000000)]);
//    
//    a += (pattern_arr[5][make_pair(playerboard & 0x42FF, opponentboard & 0x42FF)] +
//          pattern_arr[5][make_pair(playerboard & 0x80c080808080c080, opponentboard & 0x80c080808080c080)] + pattern_arr[5][make_pair(playerboard & 0xff42000000000000, opponentboard & 0xff42000000000000)] + pattern_arr[5][make_pair(playerboard & 0x0103010101010301, opponentboard & 0x0103010101010301)]);
//    
//    a += (pattern_arr[6][make_pair(playerboard & 0xff000000000000, opponentboard & 0xff000000000000)] + pattern_arr[6][make_pair(playerboard & 0x0202020202020202, opponentboard & 0x0202020202020202)] + pattern_arr[6][make_pair(playerboard & 0x000000000000ff00, opponentboard & 0x000000000000ff00)] + pattern_arr[6][make_pair(playerboard & 0x4040404040404040, opponentboard & 0x4040404040404040)]);
//    
//    a += (pattern_arr[7][make_pair(playerboard & 0xff0000000000, opponentboard & 0xff0000000000)] + pattern_arr[7][make_pair(playerboard & 0x0404040404040404, opponentboard & 0x0404040404040404)] + pattern_arr[7][make_pair(playerboard & 0x0000000000ff0000, opponentboard & 0x0000000000ff0000)] + pattern_arr[7][make_pair(playerboard & 0x2020202020202020, opponentboard & 0x2020202020202020)]);
//    
//    a += (pattern_arr[8][make_pair(playerboard & 0xff00000000, opponentboard & 0xff00000000)] +
//          pattern_arr[8][make_pair(playerboard & 0x0808080808080808, opponentboard & 0x0808080808080808)] + pattern_arr[8][make_pair(playerboard & 0x00000000ff000000, opponentboard & 0x00000000ff000000)] + pattern_arr[8][make_pair(playerboard & 0x1010101010101010, opponentboard & 0x1010101010101010)]);
//    
//    a += (pattern_arr[9][make_pair(playerboard & 0xe0e0e00000000000, opponentboard & 0xe0e0e00000000000)] + pattern_arr[9][make_pair(playerboard & 0x0707070000000000, opponentboard & 0x0707070000000000)] + pattern_arr[9][make_pair(playerboard & 0x0000000000070707, opponentboard & 0x0000000000070707)] + pattern_arr[9][make_pair(playerboard & 0x0000000000e0e0e0, opponentboard & 0x0000000000e0e0e0)]);
//    
//    a += (pattern_arr[10][make_pair(playerboard & 0xf8c0808080000000, opponentboard & 0xf8c0808080000000)] + pattern_arr[10][make_pair(playerboard & 0x1f03010101000000, opponentboard & 0x1f03010101000000)] + pattern_arr[10][make_pair(playerboard & 0x000000010101031f, opponentboard & 0x000000010101031f)] + pattern_arr[10][make_pair(playerboard & 0x000000808080c0f8, opponentboard & 0x000000808080c0f8)]);
//    
//    a += (pattern_arr[11][make_pair(playerboard & 0xbd3c000000000000, opponentboard & 0xbd3c000000000000)] + pattern_arr[11][make_pair(playerboard & 0x0100030303030001, opponentboard & 0x0100030303030001)] + pattern_arr[11][make_pair(playerboard & 0x0000000000003cbd, opponentboard & 0x0000000000003cbd)] + pattern_arr[11][make_pair(playerboard & 0x8000c0c0c0c00080, opponentboard & 0x8000c0c0c0c00080)]);
//    
//    a += (pattern_arr[12][make_pair(playerboard & 0xf0e0c08000000000, opponentboard & 0xf0e0c08000000000)] + pattern_arr[12][make_pair(playerboard & 0x0f07030100000000, opponentboard & 0x0f07030100000000)] + pattern_arr[12][make_pair(playerboard & 0x000000000103070f, opponentboard & 0x000000000103070f)] + pattern_arr[12][make_pair(playerboard & 0x0000000080c0e0f0, opponentboard & 0x0000000080c0e0f0)]);
//    
//    if (a > 0) a+=1;
//    else a-=1;
//    a += score_fixedstone_table(playerboard, opponentboard)/10;
//    a *= 10000000000;
//    a/=64;
//    cout << a << endl;
//    a += score_null_place(playerboard, opponentboard);
    double a =
        (pattern_arr[0].at({playerboard & 0x8040201008040201, opponentboard & 0x8040201008040201}) +
         pattern_arr[0].at({playerboard & 0x0102040810204080, opponentboard & 0x0102040810204080}));

    a += (pattern_arr[1].at({playerboard & 0x4020100804020100, opponentboard & 0x4020100804020100}) +
          pattern_arr[1].at({playerboard & 0x0001020408102040, opponentboard & 0x0001020408102040}) +
          pattern_arr[1].at({playerboard & 0x0080402010080402, opponentboard & 0x0080402010080402}) +
          pattern_arr[1].at({playerboard & 0x0204081020408000, opponentboard & 0x0204081020408000}));

    a += (pattern_arr[2].at({playerboard & 0x2010080402010000, opponentboard & 0x2010080402010000}) +
          pattern_arr[2].at({playerboard & 0x0000010204081020, opponentboard & 0x0000010204081020}) +
          pattern_arr[2].at({playerboard & 0x0000804020100804, opponentboard & 0x0000804020100804}) +
          pattern_arr[2].at({playerboard & 0x0408102040800000, opponentboard & 0x0408102040800000}));

    a += (pattern_arr[3].at({playerboard & 0x1008040201000000, opponentboard & 0x1008040201000000}) +
          pattern_arr[3].at({playerboard & 0x0000000102040810, opponentboard & 0x0000000102040810}) +
          pattern_arr[3].at({playerboard & 0x0000008040201008, opponentboard & 0x0000008040201008}) +
          pattern_arr[3].at({playerboard & 0x0810204080000000, opponentboard & 0x0810204080000000}));

    a += (pattern_arr[5].at({playerboard & 0x42FF, opponentboard & 0x42FF}) +
          pattern_arr[5].at({playerboard & 0x80c080808080c080, opponentboard & 0x80c080808080c080}) +
          pattern_arr[5].at({playerboard & 0xff42000000000000, opponentboard & 0xff42000000000000}) +
          pattern_arr[5].at({playerboard & 0x0103010101010301, opponentboard & 0x0103010101010301}));

    a += (pattern_arr[6].at({playerboard & 0xff000000000000, opponentboard & 0xff000000000000}) +
          pattern_arr[6].at({playerboard & 0x0202020202020202, opponentboard & 0x0202020202020202}) +
          pattern_arr[6].at({playerboard & 0x000000000000ff00, opponentboard & 0x000000000000ff00}) +
          pattern_arr[6].at({playerboard & 0x4040404040404040, opponentboard & 0x4040404040404040}));

    a += (pattern_arr[7].at({playerboard & 0xff0000000000, opponentboard & 0xff0000000000}) +
          pattern_arr[7].at({playerboard & 0x0404040404040404, opponentboard & 0x0404040404040404}) +
          pattern_arr[7].at({playerboard & 0x0000000000ff0000, opponentboard & 0x0000000000ff0000}) +
          pattern_arr[7].at({playerboard & 0x2020202020202020, opponentboard & 0x2020202020202020}));

    a += (pattern_arr[8].at({playerboard & 0xff00000000, opponentboard & 0xff00000000}) +
          pattern_arr[8].at({playerboard & 0x0808080808080808, opponentboard & 0x0808080808080808}) +
          pattern_arr[8].at({playerboard & 0x00000000ff000000, opponentboard & 0x00000000ff000000}) +
          pattern_arr[8].at({playerboard & 0x1010101010101010, opponentboard & 0x1010101010101010}));

    a += (pattern_arr[9].at({playerboard & 0xe0e0e00000000000, opponentboard & 0xe0e0e00000000000}) +
          pattern_arr[9].at({playerboard & 0x0707070000000000, opponentboard & 0x0707070000000000}) +
          pattern_arr[9].at({playerboard & 0x0000000000070707, opponentboard & 0x0000000000070707}) +
          pattern_arr[9].at({playerboard & 0x0000000000e0e0e0, opponentboard & 0x0000000000e0e0e0}));

    a += (pattern_arr[10].at({playerboard & 0xf8c0808080000000, opponentboard & 0xf8c0808080000000}) +
          pattern_arr[10].at({playerboard & 0x1f03010101000000, opponentboard & 0x1f03010101000000}) +
          pattern_arr[10].at({playerboard & 0x000000010101031f, opponentboard & 0x000000010101031f}) +
          pattern_arr[10].at({playerboard & 0x000000808080c0f8, opponentboard & 0x000000808080c0f8}));

    a += (pattern_arr[11].at({playerboard & 0xbd3c000000000000, opponentboard & 0xbd3c000000000000}) +
          pattern_arr[11].at({playerboard & 0x0100030303030001, opponentboard & 0x0100030303030001}) +
          pattern_arr[11].at({playerboard & 0x0000000000003cbd, opponentboard & 0x0000000000003cbd}) +
          pattern_arr[11].at({playerboard & 0x8000c0c0c0c00080, opponentboard & 0x8000c0c0c0c00080}));

    a += (pattern_arr[12].at({playerboard & 0xf0e0c08000000000, opponentboard & 0xf0e0c08000000000}) +
          pattern_arr[12].at({playerboard & 0x0f07030100000000, opponentboard & 0x0f07030100000000}) +
          pattern_arr[12].at({playerboard & 0x000000000103070f, opponentboard & 0x000000000103070f}) +
          pattern_arr[12].at({playerboard & 0x0000000080c0e0f0, opponentboard & 0x0000000080c0e0f0}));

    double b = final_bias + a + ((afterIndex <= 44) ? final_dense[3] * add_arr[mobility] : 0);

    return llround(max(-1.0, min(1.0, b)) * 640000000);
}


