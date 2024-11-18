#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <ankerl/unordered_dense.h>
#include <Siv3D.hpp>

using namespace std;

#define n_patterns 11 // 使うパターンの種類
constexpr int pattern_sizes[n_patterns] = {8, 7, 6, 5, 4, 10, 8, 8, 8, 9, 10}; // パターンごとのマスの数
constexpr int pow3[11] = {3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147}; // 2の累乗

constexpr uint64_t bit_pattern[] = {0x8040201008040201, 0x4020100804020100, 0x2010080402010000, 0x1008040201000000, 0x804020100000000, 0xff42000000000000, 0xff000000000000, 0xff0000000000, 0xff00000000, 0xe0e0e00000000000, 0xf8f8000000000000};

const vector<vector<int>> bit_positions = {{0, 9, 18, 27, 36, 45, 54, 63}, {1, 10, 19, 28, 37, 46, 55}, {2, 11, 20, 29, 38, 47}, {3, 12, 21, 30, 39}, {4, 13, 22, 31}, {9, 0, 1, 2, 3, 4, 5, 6, 7, 14}, {8, 9, 10, 11, 12, 13, 14, 15}, {16, 17, 18, 19, 20, 21, 22, 23}, {24, 25, 26, 27, 28, 29, 30, 31}, {0, 1, 2, 8, 9, 10, 16, 17, 18}, {0, 1, 2, 3, 4, 8, 9, 10, 11, 12}};
// モデルの設計パラメータ
#define n_dense0 16
#define n_dense1 16
#define n_all_input 11

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
            pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)] = predict_pattern(pattern_size,
                                                                                             arr,
                                                                                             dense0,
                                                                                             bias0,
                                                                                             dense1,
                                                                                             bias1,
                                                                                             dense2,
                                                                                             bias2);
            pattern_arr[evaluate_idx][make_pair(rotateClockwise90(newpattern1), rotateClockwise90(newpattern2))] = pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)];
            pattern_arr[evaluate_idx][make_pair(rotateClockwise90(rotateClockwise90(newpattern1)), rotateClockwise90(rotateClockwise90(newpattern2)))] = pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)];
            pattern_arr[evaluate_idx][make_pair(rotateCounterclockwise90(newpattern1), rotateCounterclockwise90(newpattern2))] = pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)];
            if(pattern_idx == 10) {
                pattern_arr[evaluate_idx][make_pair(flipDiagonalA1H8(newpattern1), flipDiagonalA1H8(newpattern2))] =  pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)];
                pattern_arr[evaluate_idx][make_pair(flipDiagonalA8H1(rotateClockwise90(newpattern1)), flipDiagonalA8H1(rotateClockwise90(newpattern2)))] = pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)];
                pattern_arr[evaluate_idx][make_pair(flipDiagonalA1H8(rotateClockwise90(rotateClockwise90(newpattern1))), flipDiagonalA1H8(rotateClockwise90(rotateClockwise90(newpattern2))))] = pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)];
                pattern_arr[evaluate_idx][make_pair(flipDiagonalA8H1(rotateCounterclockwise90(newpattern1)), flipDiagonalA8H1(rotateCounterclockwise90(newpattern2)))] = pattern_arr[evaluate_idx][make_pair(newpattern1, newpattern2)];
            }
        }
    }
}

inline void evaluate_init2() {
    for (auto &ptar : pattern_arr) {
        ptar.clear();
    }
    ifstream ifs(FileSystem::RelativePath(Resource(U"model.txt")).narrow());
    if (ifs.fail()){
        cerr << "evaluation file not exist" << endl;
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
}

inline double evaluate(const uint64_t &playerboard, const uint64_t &opponentboard) {
    double a = final_dense[0] * (pattern_arr[0].at(make_pair(playerboard & bit_pattern[0], opponentboard & bit_pattern[0])) + pattern_arr[0].at(make_pair(playerboard & 0x0102040810204080, opponentboard & 0x0102040810204080)));
    
    a += final_dense[1] * (pattern_arr[1].at(make_pair(playerboard & 0x4020100804020100, opponentboard & 0x4020100804020100)) + pattern_arr[1].at(make_pair(playerboard & 0x0001020408102040, opponentboard & 0x0001020408102040)) + pattern_arr[1].at(make_pair(playerboard & 0x0080402010080402, opponentboard & 0x0080402010080402)) + pattern_arr[1].at(make_pair(playerboard & 0x0204081020408000, opponentboard & 0x0204081020408000)));
    
    a += final_dense[2] * (pattern_arr[2].at(make_pair(playerboard & 0x2010080402010000, opponentboard & 0x2010080402010000)) + pattern_arr[2].at(make_pair(playerboard & 0x0000010204081020, opponentboard & 0x0000010204081020)) + pattern_arr[2].at(make_pair(playerboard & 0x0000804020100804, opponentboard & 0x0000804020100804)) + pattern_arr[2].at(make_pair(playerboard & 0x0408102040800000, opponentboard & 0x0408102040800000)));
    
    a += final_dense[3] * (pattern_arr[3].at(make_pair(playerboard & 0x1008040201000000, opponentboard & 0x1008040201000000)) + pattern_arr[3].at(make_pair(playerboard & 0x0000000102040810, opponentboard & 0x0000000102040810)) + pattern_arr[3].at(make_pair(playerboard & 0x0000008040201008, opponentboard & 0x0000008040201008)) + pattern_arr[3].at(make_pair(playerboard & 0x0810204080000000, opponentboard & 0x0810204080000000)));
    
    a += final_dense[5] * (pattern_arr[5].at(make_pair(playerboard & 0x42FF, opponentboard & 0x42FF)) + pattern_arr[5].at(make_pair(playerboard & 0x80c080808080c080, opponentboard & 0x80c080808080c080)) + pattern_arr[5].at(make_pair(playerboard & 0xff42000000000000, opponentboard & 0xff42000000000000)) + pattern_arr[5].at(make_pair(playerboard & 0x0103010101010301, opponentboard & 0x0103010101010301)));
    
    a += final_dense[6] * (pattern_arr[6].at(make_pair(playerboard & 0xff000000000000, opponentboard & 0xff000000000000)) + pattern_arr[6].at(make_pair(playerboard & 0x0202020202020202, opponentboard & 0x0202020202020202)) + pattern_arr[6].at(make_pair(playerboard & 0x000000000000ff00, opponentboard & 0x000000000000ff00)) + pattern_arr[6].at(make_pair(playerboard & 0x4040404040404040, opponentboard & 0x4040404040404040)));
    
    a += final_dense[7] * (pattern_arr[7].at(make_pair(playerboard & 0xff0000000000, opponentboard & 0xff0000000000)) + pattern_arr[7].at(make_pair(playerboard & 0x0404040404040404, opponentboard & 0x0404040404040404)) + pattern_arr[7].at(make_pair(playerboard & 0x0000000000ff0000, opponentboard & 0x0000000000ff0000)) + pattern_arr[7].at(make_pair(playerboard & 0x2020202020202020, opponentboard & 0x2020202020202020)));
    
    a += final_dense[8] * (pattern_arr[8].at(make_pair(playerboard & 0xff00000000, opponentboard & 0xff00000000)) + pattern_arr[8].at(make_pair(playerboard & 0x0808080808080808, opponentboard & 0x0808080808080808)) + pattern_arr[8].at(make_pair(playerboard & 0x00000000ff000000, opponentboard & 0x00000000ff000000)) + pattern_arr[8].at(make_pair(playerboard & 0x1010101010101010, opponentboard & 0x1010101010101010)));
    
    a += final_dense[9] * (pattern_arr[9].at(make_pair(playerboard & 0xe0e0e00000000000, opponentboard & 0xe0e0e00000000000)) + pattern_arr[9].at(make_pair(playerboard & 0x0707070000000000, opponentboard & 0x0707070000000000)) + pattern_arr[9].at(make_pair(playerboard & 0x0000000000070707, opponentboard & 0x0000000000070707)) + pattern_arr[9].at(make_pair(playerboard & 0x0000000000e0e0e0, opponentboard & 0x0000000000e0e0e0)));
    
    a += final_dense[10] * (pattern_arr[10].at(make_pair(playerboard & 0xf8f8000000000000, opponentboard & 0xf8f8000000000000)) + pattern_arr[10].at(make_pair(playerboard & 0x0303030303000000, opponentboard & 0x0303030303000000)) + pattern_arr[10].at(make_pair(playerboard & 0x0000000000001f1f, opponentboard & 0x0000000000001f1f)) + pattern_arr[10].at(make_pair(playerboard & 0x000000c0c0c0c0c0, opponentboard & 0x000000c0c0c0c0c0)) + pattern_arr[10].at(make_pair(playerboard & 0x1f1f000000000000, opponentboard & 0x1f1f000000000000)) + pattern_arr[10].at(make_pair(playerboard & 0x0000000303030303, opponentboard & 0x0000000303030303)) + pattern_arr[10].at(make_pair(playerboard & 0x000000000000f8f8, opponentboard & 0x000000000000f8f8)) + pattern_arr[10].at(make_pair(playerboard & 0xc0c0c0c0c0000000, opponentboard & 0xc0c0c0c0c0000000)));
    
    if (a > 0) a+=1;
    else a-=1;
//    a += score_fixedstone_table(playerboard, opponentboard)/10;
    a *= 1000;
    
//    a/=64;
//    cout << a << endl;
//    a += score_null_place(playerboard, opponentboard);
    
//    a += score_putable(playerboard, opponentboard)*5;
    return min(max((int)round(a), MIN_INF), MAX_INF);
}


