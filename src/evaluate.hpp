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
#define flipVertical(x) __builtin_bswap64(x)
#else
#define    popcountll(x) __popcnt64(x)
#define clzll(x) _lzcnt_u64(x)
#define flipVertical(x) _byteswap_uint64(x)
#endif

#define n_patterns 12 // 使うパターンの種類
constexpr int pattern_sizes[n_patterns] = {8, 7, 6, 5, 10, 8, 8, 8, 9, 10, 10, 10}; // パターンごとのマスの数

constexpr uint64_t bit_pattern[] = {0x8040201008040201, 0x4020100804020100, 0x2010080402010000, 0x1008040201000000, 0xff42000000000000, 0xff000000000000, 0xff0000000000, 0xff00000000, 0xe0e0e00000000000, 0xf8c0808080000000, 0xbd3c000000000000, 0xf0e0c08000000000};

const vector<vector<int>> bit_positions = {{0, 9, 18, 27, 36, 45, 54, 63}, {1, 10, 19, 28, 37, 46, 55}, {2, 11, 20, 29, 38, 47}, {3, 12, 21, 30, 39}, {9, 0, 1, 2, 3, 4, 5, 6, 7, 14}, {8, 9, 10, 11, 12, 13, 14, 15}, {16, 17, 18, 19, 20, 21, 22, 23}, {24, 25, 26, 27, 28, 29, 30, 31}, {0, 1, 8, 9, 2, 16, 10, 17, 18}, {32, 24, 16, 8, 0, 9, 1, 2, 3, 4}, {0, 2, 3, 10, 11, 12, 13, 4, 5, 7}, {0, 1, 2, 3, 8, 9, 10, 16, 17, 24}};
// モデルの設計パラメータ
#define n_dense0 128
#define n_dense1 128
#define n_add_input 2
#define n_add_dense0 16
#define n_all_input 12
#define max_mobility 36
#define max_surround 50

double add_arr[max_surround + 1][max_surround + 1];
double final_dense[n_all_input];
double final_bias[2];

ankerl::unordered_dense::map<pair<uint64_t, uint64_t>, double> tmp[n_patterns];
ankerl::unordered_dense::map<pair<uint64_t, uint64_t>, double> pattern_arr[n_patterns];
ankerl::unordered_dense::map<pair<uint64_t, uint64_t>, double> pattern_arr_end[n_patterns];

uint64_t delta_swap(uint64_t x, uint64_t mask, int delta) {
    uint64_t t = (x ^ (x >> delta)) & mask;
    return x ^ t ^ (t << delta);
}

// 水平反転
uint64_t flipHorizontal(uint64_t x) {
    x = delta_swap(x, 0x5555555555555555, 1);
    x = delta_swap(x, 0x3333333333333333, 2);
    return delta_swap(x, 0x0F0F0F0F0F0F0F0F, 4);
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

uint64_t rotate180(uint64_t x) {
    return rotateClockwise90(rotateClockwise90(x));
}

inline int cal_pow(int x, int depth) {
    if(depth == 1) return x;
    return x * cal_pow(x, depth - 1);
}

inline double leaky_relu(double x){
    return max(0.01 * x, x);
}

inline double predict_pattern(int pattern_size, double in_arr[], double dense0[n_dense0][20], double bias0[n_dense0], double dense1[n_dense1][n_dense0], double bias1[n_dense1], double dense2[n_dense1], double bias2){
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

inline void pre_evaluation_pattern(int pattern_idx, int evaluate_idx, int pattern_size, double dense0[n_dense0][20], double bias0[n_dense0], double dense1[n_dense1][n_dense0], double bias1[n_dense1], double dense2[n_dense1], double bias2){
    double arr[20];
    vector<int> bitpositions = bit_positions[pattern_idx];
    uint64_t pattern1 = bit_pattern[pattern_idx];
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
            tmp[evaluate_idx][make_pair(newpattern1, newpattern2)] = predict_pattern(pattern_size,
                                                                                             arr,
                                                                                             dense0,
                                                                                             bias0,
                                                                                             dense1,
                                                                                             bias1,
                                                                                             dense2,
                                                                                             bias2);
            tmp[evaluate_idx][make_pair(rotateClockwise90(newpattern1), rotateClockwise90(newpattern2))] = tmp[evaluate_idx][make_pair(newpattern1, newpattern2)];
            tmp[evaluate_idx][make_pair(rotate180(newpattern1), rotate180(newpattern2))] = tmp[evaluate_idx][make_pair(newpattern1, newpattern2)];
            tmp[evaluate_idx][make_pair(rotateCounterclockwise90(newpattern1), rotateCounterclockwise90(newpattern2))] = tmp[evaluate_idx][make_pair(newpattern1, newpattern2)];
        }
    }
}

// 追加パラメータの推論
inline double predict_add(int sur0, int sur1, double dense0[n_add_dense0][n_add_input], double bias0[n_add_dense0], double dense1[n_add_dense0], double bias1){
    double hidden0[n_add_dense0], in_arr[n_add_input];
    int i, j;
    in_arr[0] = ((double)sur0 - 15.0) / 15.0;
    in_arr[1] = ((double)sur1 - 15.0) / 15.0;
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
    int sur0, sur1;
    for (sur0 = 0; sur0 <= max_surround; ++sur0){
        for (sur1 = 0; sur1 <= max_surround; ++sur1)
            add_arr[sur0][sur1] = predict_add(sur0, sur1, dense0, bias0, dense1, bias1);
    }
}

inline void evaluate_init(String model_path, int ptr_num){
    for (auto &ptar : tmp) {
        ptar.clear();
    }
    ifstream ifs(FileSystem::RelativePath(Resource(model_path)).narrow());
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
    if (ptr_num == 1) {
        getline(ifs, line);
        final_bias[0] = stod(line);
        memcpy(pattern_arr, tmp, sizeof(tmp));
    } else {
        getline(ifs, line);
        final_bias[1] = stod(line);
        memcpy(pattern_arr_end, tmp, sizeof(tmp));
    }
    cout << "evaluation initialized" << endl;
}

inline int64_t evaluate_moveorder(uint64_t playerboard, uint64_t opponentboard) noexcept {
    
//    int mobility = min(max_mobility * 2, max(0, max_mobility + popcountll(makelegalboard(playerboard, opponentboard))));
    
//    int sur0 = min(max_surround, score_surround(playerboard, opponentboard));
//    int sur1 = min(max_surround, score_surround(opponentboard, playerboard));
    
    double a =
        (pattern_arr[0].at({playerboard & 0x8040201008040201, opponentboard & 0x8040201008040201}) +
         pattern_arr[0].at({playerboard & 0x0102040810204080, opponentboard & 0x0102040810204080}));

    a += (pattern_arr[1].at({playerboard & 0x4020100804020100, opponentboard & 0x4020100804020100}) +
          pattern_arr[1].at({playerboard & 0x0001020408102040, opponentboard & 0x0001020408102040}) +
          pattern_arr[1].at({playerboard & 0x0080402010080402, opponentboard & 0x0080402010080402}) +
          pattern_arr[1].at({playerboard & 0x0204081020408000, opponentboard & 0x0204081020408000}));

//    a += (pattern_arr[2].at({playerboard & 0x2010080402010000, opponentboard & 0x2010080402010000}) +
//          pattern_arr[2].at({playerboard & 0x0000010204081020, opponentboard & 0x0000010204081020}) +
//          pattern_arr[2].at({playerboard & 0x0000804020100804, opponentboard & 0x0000804020100804}) +
//          pattern_arr[2].at({playerboard & 0x0408102040800000, opponentboard & 0x0408102040800000}));

//    a += (pattern_arr[3].at({playerboard & 0x1008040201000000, opponentboard & 0x1008040201000000}) +
//          pattern_arr[3].at({playerboard & 0x0000000102040810, opponentboard & 0x0000000102040810}) +
//          pattern_arr[3].at({playerboard & 0x0000008040201008, opponentboard & 0x0000008040201008}) +
//          pattern_arr[3].at({playerboard & 0x0810204080000000, opponentboard & 0x0810204080000000}));

    a += (pattern_arr[4].at({playerboard & 0x42FF, opponentboard & 0x42FF}) +
          pattern_arr[4].at({playerboard & 0x80c080808080c080, opponentboard & 0x80c080808080c080}) +
          pattern_arr[4].at({playerboard & 0xff42000000000000, opponentboard & 0xff42000000000000}) +
          pattern_arr[4].at({playerboard & 0x0103010101010301, opponentboard & 0x0103010101010301}));

//    a += (pattern_arr[5].at({playerboard & 0xff000000000000, opponentboard & 0xff000000000000}) +
//          pattern_arr[5].at({playerboard & 0x0202020202020202, opponentboard & 0x0202020202020202}) +
//          pattern_arr[5].at({playerboard & 0x000000000000ff00, opponentboard & 0x000000000000ff00}) +
//          pattern_arr[5].at({playerboard & 0x4040404040404040, opponentboard & 0x4040404040404040}));

//    a += (pattern_arr[6].at({playerboard & 0xff0000000000, opponentboard & 0xff0000000000}) +
//          pattern_arr[6].at({playerboard & 0x0404040404040404, opponentboard & 0x0404040404040404}) +
//          pattern_arr[6].at({playerboard & 0x0000000000ff0000, opponentboard & 0x0000000000ff0000}) +
//          pattern_arr[6].at({playerboard & 0x2020202020202020, opponentboard & 0x2020202020202020}));

//    a += (pattern_arr[7].at({playerboard & 0xff00000000, opponentboard & 0xff00000000}) +
//          pattern_arr[7].at({playerboard & 0x0808080808080808, opponentboard & 0x0808080808080808}) +
//          pattern_arr[7].at({playerboard & 0x00000000ff000000, opponentboard & 0x00000000ff000000}) +
//          pattern_arr[7].at({playerboard & 0x1010101010101010, opponentboard & 0x1010101010101010}));

//    a += (pattern_arr[8].at({playerboard & 0xe0e0e00000000000, opponentboard & 0xe0e0e00000000000}) +
//          pattern_arr[8].at({playerboard & 0x0707070000000000, opponentboard & 0x0707070000000000}) +
//          pattern_arr[8].at({playerboard & 0x0000000000070707, opponentboard & 0x0000000000070707}) +
//          pattern_arr[8].at({playerboard & 0x0000000000e0e0e0, opponentboard & 0x0000000000e0e0e0}));

//    a += (pattern_arr[9].at({playerboard & 0xf8c0808080000000, opponentboard & 0xf8c0808080000000}) +
//          pattern_arr[9].at({playerboard & 0x1f03010101000000, opponentboard & 0x1f03010101000000}) +
//          pattern_arr[9].at({playerboard & 0x000000010101031f, opponentboard & 0x000000010101031f}) +
//          pattern_arr[9].at({playerboard & 0x000000808080c0f8, opponentboard & 0x000000808080c0f8}));

//    a += (pattern_arr[10].at({playerboard & 0xbd3c000000000000, opponentboard & 0xbd3c000000000000}) +
//          pattern_arr[10].at({playerboard & 0x0100030303030001, opponentboard & 0x0100030303030001}) +
//          pattern_arr[10].at({playerboard & 0x0000000000003cbd, opponentboard & 0x0000000000003cbd}) +
//          pattern_arr[10].at({playerboard & 0x8000c0c0c0c00080, opponentboard & 0x8000c0c0c0c00080}));

    a += (pattern_arr[11].at({playerboard & 0xf0e0c08000000000, opponentboard & 0xf0e0c08000000000}) +
          pattern_arr[11].at({playerboard & 0x0f07030100000000, opponentboard & 0x0f07030100000000}) +
          pattern_arr[11].at({playerboard & 0x000000000103070f, opponentboard & 0x000000000103070f}) +
          pattern_arr[11].at({playerboard & 0x0000000080c0e0f0, opponentboard & 0x0000000080c0e0f0}));

    double b = final_bias[0] + a;

    return llround(max(-1.0, min(1.0, b)) * 640000000);
}

inline int64_t evaluate(uint64_t playerboard, uint64_t opponentboard) noexcept {
    
//    int mobility = min(max_mobility * 2, max(0, max_mobility + popcountll(makelegalboard(playerboard, opponentboard))));
//    int sur0 = min(max_surround, score_surround(playerboard, opponentboard));
//    int sur1 = min(max_surround, score_surround(opponentboard, playerboard));
//    cout << mobility << endl;
//
    double a = 0;
    if (afterIndex <= 40) {
        a += (pattern_arr[0].at({playerboard & 0x8040201008040201, opponentboard & 0x8040201008040201}) +
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

        a += (pattern_arr[4].at({playerboard & 0x42FF, opponentboard & 0x42FF}) +
              pattern_arr[4].at({playerboard & 0x80c080808080c080, opponentboard & 0x80c080808080c080}) +
              pattern_arr[4].at({playerboard & 0xff42000000000000, opponentboard & 0xff42000000000000}) +
              pattern_arr[4].at({playerboard & 0x0103010101010301, opponentboard & 0x0103010101010301}));

        a += (pattern_arr[5].at({playerboard & 0xff000000000000, opponentboard & 0xff000000000000}) +
              pattern_arr[5].at({playerboard & 0x0202020202020202, opponentboard & 0x0202020202020202}) +
              pattern_arr[5].at({playerboard & 0x000000000000ff00, opponentboard & 0x000000000000ff00}) +
              pattern_arr[5].at({playerboard & 0x4040404040404040, opponentboard & 0x4040404040404040}));

        a += (pattern_arr[6].at({playerboard & 0xff0000000000, opponentboard & 0xff0000000000}) +
              pattern_arr[6].at({playerboard & 0x0404040404040404, opponentboard & 0x0404040404040404}) +
              pattern_arr[6].at({playerboard & 0x0000000000ff0000, opponentboard & 0x0000000000ff0000}) +
              pattern_arr[6].at({playerboard & 0x2020202020202020, opponentboard & 0x2020202020202020}));

        a += (pattern_arr[7].at({playerboard & 0xff00000000, opponentboard & 0xff00000000}) +
              pattern_arr[7].at({playerboard & 0x0808080808080808, opponentboard & 0x0808080808080808}) +
              pattern_arr[7].at({playerboard & 0x00000000ff000000, opponentboard & 0x00000000ff000000}) +
              pattern_arr[7].at({playerboard & 0x1010101010101010, opponentboard & 0x1010101010101010}));

        a += (pattern_arr[8].at({playerboard & 0xe0e0e00000000000, opponentboard & 0xe0e0e00000000000}) +
              pattern_arr[8].at({playerboard & 0x0707070000000000, opponentboard & 0x0707070000000000}) +
              pattern_arr[8].at({playerboard & 0x0000000000070707, opponentboard & 0x0000000000070707}) +
              pattern_arr[8].at({playerboard & 0x0000000000e0e0e0, opponentboard & 0x0000000000e0e0e0}));

        a += (pattern_arr[9].at({playerboard & 0xf8c0808080000000, opponentboard & 0xf8c0808080000000}) +
              pattern_arr[9].at({playerboard & 0x1f03010101000000, opponentboard & 0x1f03010101000000}) +
              pattern_arr[9].at({playerboard & 0x000000010101031f, opponentboard & 0x000000010101031f}) +
              pattern_arr[9].at({playerboard & 0x000000808080c0f8, opponentboard & 0x000000808080c0f8}));

        a += (pattern_arr[10].at({playerboard & 0xbd3c000000000000, opponentboard & 0xbd3c000000000000}) +
              pattern_arr[10].at({playerboard & 0x0100030303030001, opponentboard & 0x0100030303030001}) +
              pattern_arr[10].at({playerboard & 0x0000000000003cbd, opponentboard & 0x0000000000003cbd}) +
              pattern_arr[10].at({playerboard & 0x8000c0c0c0c00080, opponentboard & 0x8000c0c0c0c00080}));

        a += (pattern_arr[11].at({playerboard & 0xf0e0c08000000000, opponentboard & 0xf0e0c08000000000}) +
              pattern_arr[11].at({playerboard & 0x0f07030100000000, opponentboard & 0x0f07030100000000}) +
              pattern_arr[11].at({playerboard & 0x000000000103070f, opponentboard & 0x000000000103070f}) +
              pattern_arr[11].at({playerboard & 0x0000000080c0e0f0, opponentboard & 0x0000000080c0e0f0}));

        a += final_bias[0];
    } else {
        
        a += (pattern_arr_end[0].at({playerboard & 0x8040201008040201, opponentboard & 0x8040201008040201}) +
             pattern_arr_end[0].at({playerboard & 0x0102040810204080, opponentboard & 0x0102040810204080}));

        a += (pattern_arr_end[1].at({playerboard & 0x4020100804020100, opponentboard & 0x4020100804020100}) +
              pattern_arr_end[1].at({playerboard & 0x0001020408102040, opponentboard & 0x0001020408102040}) +
              pattern_arr_end[1].at({playerboard & 0x0080402010080402, opponentboard & 0x0080402010080402}) +
              pattern_arr_end[1].at({playerboard & 0x0204081020408000, opponentboard & 0x0204081020408000}));

        a += (pattern_arr_end[2].at({playerboard & 0x2010080402010000, opponentboard & 0x2010080402010000}) +
              pattern_arr_end[2].at({playerboard & 0x0000010204081020, opponentboard & 0x0000010204081020}) +
              pattern_arr_end[2].at({playerboard & 0x0000804020100804, opponentboard & 0x0000804020100804}) +
              pattern_arr_end[2].at({playerboard & 0x0408102040800000, opponentboard & 0x0408102040800000}));

        a += (pattern_arr_end[3].at({playerboard & 0x1008040201000000, opponentboard & 0x1008040201000000}) +
              pattern_arr_end[3].at({playerboard & 0x0000000102040810, opponentboard & 0x0000000102040810}) +
              pattern_arr_end[3].at({playerboard & 0x0000008040201008, opponentboard & 0x0000008040201008}) +
              pattern_arr_end[3].at({playerboard & 0x0810204080000000, opponentboard & 0x0810204080000000}));

        a += (pattern_arr_end[4].at({playerboard & 0x42FF, opponentboard & 0x42FF}) +
              pattern_arr_end[4].at({playerboard & 0x80c080808080c080, opponentboard & 0x80c080808080c080}) +
              pattern_arr_end[4].at({playerboard & 0xff42000000000000, opponentboard & 0xff42000000000000}) +
              pattern_arr_end[4].at({playerboard & 0x0103010101010301, opponentboard & 0x0103010101010301}));

        a += (pattern_arr_end[5].at({playerboard & 0xff000000000000, opponentboard & 0xff000000000000}) +
              pattern_arr_end[5].at({playerboard & 0x0202020202020202, opponentboard & 0x0202020202020202}) +
              pattern_arr_end[5].at({playerboard & 0x000000000000ff00, opponentboard & 0x000000000000ff00}) +
              pattern_arr_end[5].at({playerboard & 0x4040404040404040, opponentboard & 0x4040404040404040}));

        a += (pattern_arr_end[6].at({playerboard & 0xff0000000000, opponentboard & 0xff0000000000}) +
              pattern_arr_end[6].at({playerboard & 0x0404040404040404, opponentboard & 0x0404040404040404}) +
              pattern_arr_end[6].at({playerboard & 0x0000000000ff0000, opponentboard & 0x0000000000ff0000}) +
              pattern_arr_end[6].at({playerboard & 0x2020202020202020, opponentboard & 0x2020202020202020}));

        a += (pattern_arr_end[7].at({playerboard & 0xff00000000, opponentboard & 0xff00000000}) +
              pattern_arr_end[7].at({playerboard & 0x0808080808080808, opponentboard & 0x0808080808080808}) +
              pattern_arr_end[7].at({playerboard & 0x00000000ff000000, opponentboard & 0x00000000ff000000}) +
              pattern_arr_end[7].at({playerboard & 0x1010101010101010, opponentboard & 0x1010101010101010}));

        a += (pattern_arr_end[8].at({playerboard & 0xe0e0e00000000000, opponentboard & 0xe0e0e00000000000}) +
              pattern_arr_end[8].at({playerboard & 0x0707070000000000, opponentboard & 0x0707070000000000}) +
              pattern_arr_end[8].at({playerboard & 0x0000000000070707, opponentboard & 0x0000000000070707}) +
              pattern_arr_end[8].at({playerboard & 0x0000000000e0e0e0, opponentboard & 0x0000000000e0e0e0}));

        a += (pattern_arr_end[9].at({playerboard & 0xf8c0808080000000, opponentboard & 0xf8c0808080000000}) +
              pattern_arr_end[9].at({playerboard & 0x1f03010101000000, opponentboard & 0x1f03010101000000}) +
              pattern_arr_end[9].at({playerboard & 0x000000010101031f, opponentboard & 0x000000010101031f}) +
              pattern_arr_end[9].at({playerboard & 0x000000808080c0f8, opponentboard & 0x000000808080c0f8}));

        a += (pattern_arr_end[10].at({playerboard & 0xbd3c000000000000, opponentboard & 0xbd3c000000000000}) +
              pattern_arr_end[10].at({playerboard & 0x0100030303030001, opponentboard & 0x0100030303030001}) +
              pattern_arr_end[10].at({playerboard & 0x0000000000003cbd, opponentboard & 0x0000000000003cbd}) +
              pattern_arr_end[10].at({playerboard & 0x8000c0c0c0c00080, opponentboard & 0x8000c0c0c0c00080}));

        a += (pattern_arr_end[11].at({playerboard & 0xf0e0c08000000000, opponentboard & 0xf0e0c08000000000}) +
              pattern_arr_end[11].at({playerboard & 0x0f07030100000000, opponentboard & 0x0f07030100000000}) +
              pattern_arr_end[11].at({playerboard & 0x000000000103070f, opponentboard & 0x000000000103070f}) +
              pattern_arr_end[11].at({playerboard & 0x0000000080c0e0f0, opponentboard & 0x0000000080c0e0f0}));

        a += final_bias[1];
    }
    

    return llround(max(-1.0, min(1.0, a)) * 64000000000);
}
