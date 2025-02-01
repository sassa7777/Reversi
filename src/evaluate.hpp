#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <Siv3D.hpp>
#include "../zp7/zp7.cpp"

using namespace std;
using bitboard = pair<uint64_t, uint64_t>;

#ifdef __GNUC__
#define flipVertical(x) __builtin_bswap64(x)
#else
#define flipVertical(x) _byteswap_uint64(x)
#endif

#define n_patterns 12
#define pext(s, m) zp7_pext_64(s, m)
#define ppext(s, m) zp7_pext_pre_64(s, m)
constexpr static int pattern_sizes[n_patterns] = {8, 7, 6, 5, 10, 8, 8, 8, 9, 10, 10, 10}; // パターンごとのマスの数

constexpr static uint64_t bit_pattern[] = {0x8040201008040201, 0x4020100804020100, 0x2010080402010000, 0x1008040201000000, 0xff42000000000000, 0xff000000000000, 0xff0000000000, 0xff00000000, 0xe0e0e00000000000, 0xf8c0808080000000, 0xbd3c000000000000, 0xf0e0c08000000000};

const static vector<vector<int>> bit_positions = {{0, 9, 18, 27, 36, 45, 54, 63}, {1, 10, 19, 28, 37, 46, 55}, {2, 11, 20, 29, 38, 47}, {3, 12, 21, 30, 39}, {9, 0, 1, 2, 3, 4, 5, 6, 7, 14}, {8, 9, 10, 11, 12, 13, 14, 15}, {16, 17, 18, 19, 20, 21, 22, 23}, {24, 25, 26, 27, 28, 29, 30, 31}, {0, 1, 8, 9, 2, 16, 10, 17, 18}, {32, 24, 16, 8, 0, 9, 1, 2, 3, 4}, {0, 2, 3, 10, 11, 12, 13, 4, 5, 7}, {0, 1, 2, 3, 8, 9, 10, 16, 17, 24}};

constexpr static uint64_t mn[12][4] = {
    // diagonal8_idx
        {0x844c1002020009a9, 0x1280219202044004, 0, 0},
        // diagonal7_idx
        {0x110142000820011, 0x4600120802380400, 0x200200c840820040, 0x809004200250510},
        // diagonal6_idx
        {0x60c080e0802, 0x4044008411800000, 0x4088400c0810300, 0xc004010281844130},
        // diagonal5_idx
        {0x120098a20451002, 0x8808409000a007, 0x3014008080900040, 0x2200331088814},
        // edge_2x_idx
        {0x4101001020000e51, 0xc01044000801008, 0x1002a080a200110, 0x80200804024001},
        // h_v_2_idx
        {0x4090000220412900, 0x31000a004080240, 0x100900001010044c, 0x1020420004108009},
        // h_v_3_idx
        {0x21c090000d810080, 0x400a00046011008, 0x2000808050008024, 0x2080201040390204},
        // h_v_4_idx
        {0x1010009004100, 0x7002084088040029, 0x100090100024c, 0x81001204002048},
        // corner_3x3_idx
        {0x12301240a4012009, 0x80408001312004, 0x2000840082001000, 0xc900042000000004},
        // edge_x_side_idx
        {0x20008a204320801, 0xc8048800104, 0xe00800240010085, 0x80040100001000},
        // edge_block_idx
        {0x80482080c3, 0x40040010800a020, 0x104002500280060, 0x8098880022001},
        // triangle_idx
        {0x820000000100809, 0x10181800200c8, 0x200200024100420, 0x81000408002200}
};

constexpr static size_t shn[12] = {56, 57, 58, 59, 54, 56, 56, 56, 55, 54, 54, 54};

#define n_dense0 128
#define n_dense1 128
#define n_add_dense0 16
#define n_add_dense1 16
#define n_add_input 1
#define n_all_input 12
#define model_count 1
#define use_book true

#define max_mobility 40

static double final_dense[n_all_input];
static int64_t final_bias[model_count];

//static double pattern_arr[model_count][n_patterns][4][1024][1024];
static vector<vector<vector<vector<vector<double>>>>> pattern_arr_pre(model_count, vector<vector<vector<vector<double>>>>(n_patterns));
static vector<vector<vector<vector<vector<int64_t>>>>> pattern_arr(model_count, vector<vector<vector<vector<int64_t>>>>(n_patterns));
//static zp7_masks_64_t maskr[n_patterns][4];
//static vector<vector<zp7_masks_64_t>> maskr(n_patterns, vector<zp7_masks_64_t>(4));

//static vector<double> add_arr(max_mobility * 2 + 1);

inline constexpr uint64_t delta_swap(uint64_t x, uint64_t mask, int delta) {
    uint64_t t = (x ^ (x >> delta)) & mask;
    return x ^ t ^ (t << delta);
}

// 水平反転
inline constexpr uint64_t flipHorizontal(uint64_t x) {
    x = ((x >> 1) & 0x5555555555555555) | ((x & 0x5555555555555555) << 1);
    x = ((x >> 2) & 0x3333333333333333) | ((x & 0x3333333333333333) << 2);
    x = ((x >> 4) & 0x0f0f0f0f0f0f0f0f) | ((x & 0x0f0f0f0f0f0f0f0f) << 4);
    return x;
}

// A1-H8反転
inline constexpr uint64_t flipDiagonalA1H8(uint64_t x) {
    x = delta_swap(x, 0x00AA00AA00AA00AA, 7);
    x = delta_swap(x, 0x0000CCCC0000CCCC, 14);
    return delta_swap(x, 0x00000000F0F0F0F0, 28);
}

// A8-H1反転
inline constexpr uint64_t flipDiagonalA8H1(uint64_t x) {
    x = delta_swap(x, 0x0055005500550055, 9);
    x = delta_swap(x, 0x0000333300003333, 18);
    return delta_swap(x, 0x000000000F0F0F0F, 36);
}

// 時計回りに90度回転
inline constexpr uint64_t r90(uint64_t x) {
    return flipVertical(flipDiagonalA1H8(x));
}

inline constexpr uint64_t l90(uint64_t x) {
    return flipDiagonalA1H8(flipVertical(x));
}

#ifdef __clang__
#define r180(x) __builtin_bitreverse64(x)
#else
inline constexpr uint64_t r180(uint64_t x) {
    return flipVertical(flipHorizontal(x));
}
#endif

constexpr array<array<zp7_masks_64_t, 4>, n_patterns> init_pext_mask() {
    array<array<zp7_masks_64_t, 4>, n_patterns> tmp;
    for (int i = 0; i < n_patterns; ++i) {
        tmp[i][0] = zp7_ppp_64(bit_pattern[i]);
        tmp[i][1] = zp7_ppp_64(r90(bit_pattern[i]));
        tmp[i][2] = zp7_ppp_64(r180(bit_pattern[i]));
        tmp[i][3] = zp7_ppp_64(l90(bit_pattern[i]));
    }
    return tmp;
}

constexpr static array<array<zp7_masks_64_t, 4>, n_patterns> maskr = init_pext_mask();

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
//    maskr[pattern_idx][0] = zp7_ppp_64(pattern1);
//    maskr[pattern_idx][1] = zp7_ppp_64(r90(pattern1));
//    maskr[pattern_idx][2] = zp7_ppp_64(r180(pattern1));
//    maskr[pattern_idx][3] = zp7_ppp_64(l90(pattern1));
    size_t popcountPattern = popcount(pattern1);
    size_t totalCombinations = 1UL << popcountPattern;
    pattern_arr_pre[ptr_num][evaluate_idx].resize(4);
    for (auto &ptar : pattern_arr_pre[ptr_num][evaluate_idx]) {
        ptar.resize(1ULL << popcountPattern);
        for (auto &ptar2 : ptar) {
            ptar2.resize(1ULL << popcountPattern);
        }
    }
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
            
//            if (e == 10) {
//                auto key1 = make_pair(ppext(newpattern1, &maskr[e][0]), ppext(newpattern2, &maskr[e][0]));
//                auto key2 = make_pair(r90(newpattern1), r90(newpattern2));
//                auto key3 = make_pair(r180(newpattern1), r180(newpattern2));
//                auto key4 = make_pair(l90(newpattern1), l90(newpattern2));
//                pattern_arr[ptr_num][evaluate_idx][0][key1.first][key1.second] = result;
//                pattern_arr[ptr_num][e][1][(key2.first * mn[e][1]) >> shn[e]][(key2.second * mn[e][1]) >> shn[e]] = result;
//                pattern_arr[ptr_num][e][2][(key3.first * mn[e][2]) >> shn[e]][(key3.second * mn[e][2]) >> shn[e]] = result;
//                pattern_arr[ptr_num][e][3][(key4.first * mn[e][3]) >> shn[e]][(key4.second * mn[e][3]) >> shn[e]] = result;
//            } else {
//                auto key1 = make_pair(newpattern1, newpattern2);
//                auto key2 = make_pair(r90(newpattern1), r90(newpattern2));
//                auto key3 = make_pair(r180(newpattern1), r180(newpattern2));
//                auto key4 = make_pair(l90(newpattern1), l90(newpattern2));
//                pattern_arr[ptr_num][e][0][(key1.first * mn[e][0]) >> shn[e]][(key1.second * mn[e][0]) >> shn[e]] = result;
//                pattern_arr[ptr_num][e][1][(key2.first * mn[e][1]) >> shn[e]][(key2.second * mn[e][1]) >> shn[e]] = result;
//                pattern_arr[ptr_num][e][2][(key3.first * mn[e][2]) >> shn[e]][(key3.second * mn[e][2]) >> shn[e]] = result;
//                pattern_arr[ptr_num][e][3][(key4.first * mn[e][3]) >> shn[e]][(key4.second * mn[e][3]) >> shn[e]] = result;
//            }
            auto key1 = make_pair(ppext(newpattern1, &maskr[e][0]), ppext(newpattern2, &maskr[e][0]));
            auto key2 = make_pair(ppext(r90(newpattern1), &maskr[e][1]), ppext(r90(newpattern2), &maskr[e][1]));
            auto key3 = make_pair(ppext(r180(newpattern1), &maskr[e][2]), ppext(r180(newpattern2), &maskr[e][2]));
            auto key4 = make_pair(ppext(l90(newpattern1), &maskr[e][3]), ppext(l90(newpattern2), &maskr[e][3]));
            
//            auto key1 = make_pair(newpattern1, newpattern2);
//            auto key2 = make_pair(r90(newpattern1), r90(newpattern2));
//            auto key3 = make_pair(r180(newpattern1), r180(newpattern2));
//            auto key4 = make_pair(l90(newpattern1), l90(newpattern2));

            pattern_arr_pre[ptr_num][evaluate_idx][0][key1.first][key1.second] = result;
            pattern_arr_pre[ptr_num][evaluate_idx][1][key2.first][key2.second] = result;
            pattern_arr_pre[ptr_num][evaluate_idx][2][key3.first][key3.second] = result;
            pattern_arr_pre[ptr_num][evaluate_idx][3][key4.first][key4.second] = result;
//            pattern_arr[ptr_num][e][0][(key1.first * mn[e][0]) >> shn[e]][(key1.second * mn[e][0]) >> shn[e]] = result;
//            pattern_arr[ptr_num][e][1][(key2.first * mn[e][1]) >> shn[e]][(key2.second * mn[e][1]) >> shn[e]] = result;
//            pattern_arr[ptr_num][e][2][(key3.first * mn[e][2]) >> shn[e]][(key3.second * mn[e][2]) >> shn[e]] = result;
//            pattern_arr[ptr_num][e][3][(key4.first * mn[e][3]) >> shn[e]][(key4.second * mn[e][3]) >> shn[e]] = result;
        }
    }
}

//inline double predict_add(int mobility, double dense0[n_add_dense0][n_add_input], double bias0[n_add_dense0], double dense1[n_add_dense1][n_add_dense0], double bias1[n_add_dense1], double dense2[n_add_dense1], double bias2){
//    double hidden0[n_add_dense0], in_arr[n_add_input], hidden1;
//    int i, j;
//    in_arr[0] = (double)mobility / 15.0;
//    for (i = 0; i < n_add_dense0; ++i){
//        hidden0[i] = bias0[i];
//        for (j = 0; j < n_add_input; ++j)
//            hidden0[i] += in_arr[j] * dense0[i][j];
//        hidden0[i] = leaky_relu(hidden0[i]);
//    }
//    double res = bias2;
//    for (i = 0; i < n_add_dense1; ++i) {
//        hidden1 = bias1[i];
//        for (j = 0; j < n_add_dense0; ++j) {
//            hidden1 += hidden0[j] * dense1[i][j];
//        }
//        hidden1 = leaky_relu(hidden1);
//        res += hidden1 * dense2[i];
//    }
//    res = leaky_relu(res);
//    return res;
//}
//
//inline void pre_evaluation_add(double dense0[n_add_dense0][n_add_input], double bias0[n_add_dense0], double dense1[n_add_dense1][n_add_dense0], double bias1[n_add_dense1], double dense2[n_add_dense1], double bias2){
//    int mobility;
//    for (mobility = -max_mobility; mobility <= max_mobility; ++mobility){
//        add_arr[mobility + max_mobility] = predict_add(mobility, dense0, bias0, dense1, bias1, dense2, bias2);
//    }
//}

inline void evaluate_init(String model_path, int ptr_num){
    ifstream ifs(FileSystem::RelativePath(Resource(model_path)).narrow());
    if (ifs.fail()){
        cerr << "evaluation file does not exist" << endl;
        exit(1);
    }
    string line;
    int i, j, pattern_idx;

    vector<vector<double>> dense0(n_dense0, vector<double>(20));
    vector<double> bias0(n_dense0);
    vector<vector<double>> dense1(n_dense1, vector<double>(n_dense0));
    vector<double> bias1(n_dense1);
    vector<double> dense2(n_dense1);
    double bias2;
//    double add_dense0[n_add_dense0][n_add_input];
//    double add_bias0[n_add_dense0];
//    double add_dense1[n_add_dense1][n_add_dense0];
//    double add_bias1[n_add_dense1];
//    double add_dense2[n_add_dense1];
//    double add_bias2;

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
    
//    for (i = 0; i < n_add_dense0; ++i){
//        for (j = 0; j < n_add_input; ++j){
//            getline(ifs, line);
//            add_dense0[i][j] = stod(line);
//        }
//    }
//    for (i = 0; i < n_add_dense0; ++i){
//        getline(ifs, line);
//        add_bias0[i] = stod(line);
//    }
//    for (i = 0; i < n_add_dense1; ++i){
//        for (j = 0; j < n_add_dense0; ++j){
//            getline(ifs, line);
//            add_dense1[i][j] = stod(line);
//        }
//    }
//    for (i = 0; i < n_add_dense1; ++i){
//        getline(ifs, line);
//        add_bias1[i] = stod(line);
//    }
//    for (i = 0; i < n_add_dense1; ++i){
//        getline(ifs, line);
//        add_dense2[i] = stod(line);
//    }
//    getline(ifs, line);
//    add_bias2 = stof(line);
//    pre_evaluation_add(add_dense0, add_bias0, add_dense1, add_bias1, add_dense2, add_bias2);
    
    for (i = 0; i < n_all_input; ++i){
        getline(ifs, line);
        final_dense[i] = stof(line);
    }
    for (i = 0; i < n_patterns; ++i){
        for (auto &ptr : pattern_arr_pre[ptr_num][i]) {
            for (auto &ptar : ptr) {
                for (auto &ptar2 : ptar) {
                    ptar2 *= final_dense[i];
                }
            }
        }
    }
    for (int i = 0; i < n_patterns; ++i) {
        pattern_arr[ptr_num][i].resize(4);
        for (int j = 0; j < 4; ++j) {
            pattern_arr[ptr_num][i][j].resize(1 << pattern_sizes[i]);
            for (int k = 0; k < 1 << pattern_sizes[i]; ++k) {
                pattern_arr[ptr_num][i][j][k].resize(1 << pattern_sizes[i]);
                for (int l = 0; l < 1 << pattern_sizes[i]; ++l) {
                    pattern_arr[ptr_num][i][j][k][l] = pattern_arr_pre[ptr_num][i][j][k][l] * 64000000000000000;
                }
                pattern_arr_pre[ptr_num][i][j][k].clear();
            }
            pattern_arr_pre[ptr_num][i][j].clear();
        }
        pattern_arr_pre[ptr_num][i].clear();
    }
    pattern_arr_pre.clear();
//    for (auto &p : add_arr){
//        p *= final_dense[12];
//    }
    getline(ifs, line);
    final_bias[ptr_num] = stof(line) * 64000000000000000;
}

inline int64_t evaluate_moveorder(uint64_t playerboard, uint64_t opponentboard) noexcept {
    
    if (playerboard == 0) [[unlikely]] return -64000000000000000;
    if (opponentboard == 0) [[unlikely]] return 64000000000000000;
    
    int64_t a = 0;
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
    
//    a += (pattern_arr[evaluate_ptr_num][0][0][((playerboard & 0x8040201008040201) * mn[0][0]) >> shn[0]]
//          [((opponentboard & 0x8040201008040201) * mn[0][0]) >> shn[0]] +
//          pattern_arr[evaluate_ptr_num][0][1][((playerboard & 0x0102040810204080) * mn[0][1]) >> shn[0]]
//          [((opponentboard & 0x0102040810204080) * mn[0][1]) >> shn[0]]);
//
//    a += (pattern_arr[evaluate_ptr_num][1][0][((playerboard & 0x4020100804020100) * mn[1][0]) >> shn[1]]
//          [((opponentboard & 0x4020100804020100) * mn[1][0]) >> shn[1]] +
//          pattern_arr[evaluate_ptr_num][1][1][((playerboard & 0x0001020408102040) * mn[1][1]) >> shn[1]]
//          [((opponentboard & 0x0001020408102040) * mn[1][1]) >> shn[1]] +
//          pattern_arr[evaluate_ptr_num][1][2][((playerboard & 0x0080402010080402) * mn[1][2]) >> shn[1]]
//          [((opponentboard & 0x0080402010080402) * mn[1][2]) >> shn[1]] +
//          pattern_arr[evaluate_ptr_num][1][3][((playerboard & 0x0204081020408000) * mn[1][3]) >> shn[1]]
//          [((opponentboard & 0x0204081020408000) * mn[1][3]) >> shn[1]]);
//
//    a += (pattern_arr[evaluate_ptr_num][2][0][((playerboard & 0x2010080402010000) * mn[2][0]) >> shn[2]]
//          [((opponentboard & 0x2010080402010000) * mn[2][0]) >> shn[2]] +
//          pattern_arr[evaluate_ptr_num][2][1][((playerboard & 0x0000010204081020) * mn[2][1]) >> shn[2]]
//          [((opponentboard & 0x0000010204081020) * mn[2][1]) >> shn[2]] +
//          pattern_arr[evaluate_ptr_num][2][2][((playerboard & 0x0000804020100804) * mn[2][2]) >> shn[2]]
//          [((opponentboard & 0x0000804020100804) * mn[2][2]) >> shn[2]] +
//          pattern_arr[evaluate_ptr_num][2][3][((playerboard & 0x0408102040800000) * mn[2][3]) >> shn[2]]
//          [((opponentboard & 0x0408102040800000) * mn[2][3]) >> shn[2]]);
//
//    a += (pattern_arr[evaluate_ptr_num][3][0][((playerboard & 0x1008040201000000) * mn[3][0]) >> shn[3]]
//          [((opponentboard & 0x1008040201000000) * mn[3][0]) >> shn[3]] +
//          pattern_arr[evaluate_ptr_num][3][1][((playerboard & 0x0000000102040810) * mn[3][1]) >> shn[3]]
//          [((opponentboard & 0x0000000102040810) * mn[3][1]) >> shn[3]] +
//          pattern_arr[evaluate_ptr_num][3][2][((playerboard & 0x0000008040201008) * mn[3][2]) >> shn[3]]
//          [((opponentboard & 0x0000008040201008) * mn[3][2]) >> shn[3]] +
//          pattern_arr[evaluate_ptr_num][3][3][((playerboard & 0x0810204080000000) * mn[3][3]) >> shn[3]]
//          [((opponentboard & 0x0810204080000000) * mn[3][3]) >> shn[3]]);
//
//    a += (pattern_arr[evaluate_ptr_num][4][0][((playerboard & 0xff42000000000000) * mn[4][0]) >> shn[4]]
//          [((opponentboard & 0xff42000000000000) * mn[4][0]) >> shn[4]] +
//          pattern_arr[evaluate_ptr_num][4][1][((playerboard & 0x0103010101010301) * mn[4][1]) >> shn[4]]
//          [((opponentboard & 0x0103010101010301) * mn[4][1]) >> shn[4]] +
//          pattern_arr[evaluate_ptr_num][4][2][((playerboard & 0x00000000000042ff) * mn[4][2]) >> shn[4]]
//          [((opponentboard & 0x00000000000042ff) * mn[4][2]) >> shn[4]] +
//          pattern_arr[evaluate_ptr_num][4][3][((playerboard & 0x80c080808080c080) * mn[4][3]) >> shn[4]]
//          [((opponentboard & 0x80c080808080c080) * mn[4][3]) >> shn[4]]);
//
//    a += (pattern_arr[evaluate_ptr_num][5][0][((playerboard & 0xff000000000000) * mn[5][0]) >> shn[5]]
//          [((opponentboard & 0xff000000000000) * mn[5][0]) >> shn[5]] +
//          pattern_arr[evaluate_ptr_num][5][1][((playerboard & 0x0202020202020202) * mn[5][1]) >> shn[5]]
//          [((opponentboard & 0x0202020202020202) * mn[5][1]) >> shn[5]] +
//          pattern_arr[evaluate_ptr_num][5][2][((playerboard & 0x000000000000ff00) * mn[5][2]) >> shn[5]]
//          [((opponentboard & 0x000000000000ff00) * mn[5][2]) >> shn[5]] +
//          pattern_arr[evaluate_ptr_num][5][3][((playerboard & 0x4040404040404040) * mn[5][3]) >> shn[5]]
//          [((opponentboard & 0x4040404040404040) * mn[5][3]) >> shn[5]]);
//
//    a += (pattern_arr[evaluate_ptr_num][6][0][((playerboard & 0xff0000000000) * mn[6][0]) >> shn[6]]
//          [((opponentboard & 0xff0000000000) * mn[6][0]) >> shn[6]] +
//          pattern_arr[evaluate_ptr_num][6][1][((playerboard & 0x0404040404040404) * mn[6][1]) >> shn[6]]
//          [((opponentboard & 0x0404040404040404) * mn[6][1]) >> shn[6]] +
//          pattern_arr[evaluate_ptr_num][6][2][((playerboard & 0x0000000000ff0000) * mn[6][2]) >> shn[6]]
//          [((opponentboard & 0x0000000000ff0000) * mn[6][2]) >> shn[6]] +
//          pattern_arr[evaluate_ptr_num][6][3][((playerboard & 0x2020202020202020) * mn[6][3]) >> shn[6]]
//          [((opponentboard & 0x2020202020202020) * mn[6][3]) >> shn[6]]);
//
//    a += (pattern_arr[evaluate_ptr_num][7][0][((playerboard & 0xff00000000) * mn[7][0]) >> shn[7]]
//          [((opponentboard & 0xff00000000) * mn[7][0]) >> shn[7]] +
//          pattern_arr[evaluate_ptr_num][7][1][((playerboard & 0x0808080808080808) * mn[7][1]) >> shn[7]]
//          [((opponentboard & 0x0808080808080808) * mn[7][1]) >> shn[7]] +
//          pattern_arr[evaluate_ptr_num][7][2][((playerboard & 0x00000000ff000000) * mn[7][2]) >> shn[7]]
//          [((opponentboard & 0x00000000ff000000) * mn[7][2]) >> shn[7]] +
//          pattern_arr[evaluate_ptr_num][7][3][((playerboard & 0x1010101010101010) * mn[7][3]) >> shn[7]]
//          [((opponentboard & 0x1010101010101010) * mn[7][3]) >> shn[7]]);
//
//    a += (pattern_arr[evaluate_ptr_num][8][0][((playerboard & 0xe0e0e00000000000) * mn[8][0]) >> shn[8]]
//          [((opponentboard & 0xe0e0e00000000000) * mn[8][0]) >> shn[8]] +
//          pattern_arr[evaluate_ptr_num][8][1][((playerboard & 0x0707070000000000) * mn[8][1]) >> shn[8]]
//          [((opponentboard & 0x0707070000000000) * mn[8][1]) >> shn[8]] +
//          pattern_arr[evaluate_ptr_num][8][2][((playerboard & 0x0000000000070707) * mn[8][2]) >> shn[8]]
//          [((opponentboard & 0x0000000000070707) * mn[8][2]) >> shn[8]] +
//          pattern_arr[evaluate_ptr_num][8][3][((playerboard & 0x0000000000e0e0e0) * mn[8][3]) >> shn[8]]
//          [((opponentboard & 0x0000000000e0e0e0) * mn[8][3]) >> shn[8]]);
//
//    a += (pattern_arr[evaluate_ptr_num][9][0][((playerboard & 0xf8c0808080000000) * mn[9][0]) >> shn[9]]
//          [((opponentboard & 0xf8c0808080000000) * mn[9][0]) >> shn[9]] +
//          pattern_arr[evaluate_ptr_num][9][1][((playerboard & 0x1f03010101000000) * mn[9][1]) >> shn[9]]
//          [((opponentboard & 0x1f03010101000000) * mn[9][1]) >> shn[9]] +
//          pattern_arr[evaluate_ptr_num][9][2][((playerboard & 0x000000010101031f) * mn[9][2]) >> shn[9]]
//          [((opponentboard & 0x000000010101031f) * mn[9][2]) >> shn[9]] +
//          pattern_arr[evaluate_ptr_num][9][3][((playerboard & 0x000000808080c0f8) * mn[9][3]) >> shn[9]]
//          [((opponentboard & 0x000000808080c0f8) * mn[9][3]) >> shn[9]]);
//
//    a += ((pattern_arr[evaluate_ptr_num][10][0][ppext(playerboard, &maskr[10][0])][ppext(opponentboard, &maskr[10][0])] +
//          pattern_arr[evaluate_ptr_num][10][1][((playerboard & 0x0100030303030001) * mn[10][1]) >> shn[10]]
//          [((opponentboard & 0x0100030303030001) * mn[10][1]) >> shn[10]] +
//          pattern_arr[evaluate_ptr_num][10][2][((playerboard & 0x0000000000003cbd) * mn[10][2]) >> shn[10]]
//          [((opponentboard & 0x0000000000003cbd) * mn[10][2]) >> shn[10]] +
//          pattern_arr[evaluate_ptr_num][10][3][((playerboard & 0x0000010103010300) * mn[10][3]) >> shn[10]]
//          [((opponentboard & 0x0000010103010300) * mn[10][3]) >> shn[10]]));
//
//    a += (pattern_arr[evaluate_ptr_num][11][0][((playerboard & 0xf0e0c08000000000) * mn[11][0]) >> shn[11]]
//          [((opponentboard & 0xf0e0c08000000000) * mn[11][0]) >> shn[11]] +
//          pattern_arr[evaluate_ptr_num][11][1][((playerboard & 0x0f07030100000000) * mn[11][1]) >> shn[11]]
//          [((opponentboard & 0x0f07030100000000) * mn[11][1]) >> shn[11]] +
//          pattern_arr[evaluate_ptr_num][11][2][((playerboard & 0x000000000103070f) * mn[11][2]) >> shn[11]]
//          [((opponentboard & 0x000000000103070f) * mn[11][2]) >> shn[11]] +
//          pattern_arr[evaluate_ptr_num][11][3][((playerboard & 0x0000000080c0e0f0) * mn[11][3]) >> shn[11]]
//          [((opponentboard & 0x0000000080c0e0f0) * mn[11][3]) >> shn[11]]);
    a += final_bias[evaluate_ptr_num];

    return a;
}

inline int64_t evaluate(uint64_t playerboard, uint64_t opponentboard) noexcept {

    if (playerboard == 0) [[unlikely]] return -64000000000000000;
    if (opponentboard == 0) [[unlikely]] return 64000000000000000;
    if (afterIndex >= 64) return (popcount(playerboard)-popcount(opponentboard))*1000000000000000;
    
    int64_t a = 0;
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

//    a += (pattern_arr[evaluate_ptr_num][0][0][((playerboard & 0x8040201008040201) * mn[0][0]) >> shn[0]]
//          [((opponentboard & 0x8040201008040201) * mn[0][0]) >> shn[0]] +
//          pattern_arr[evaluate_ptr_num][0][1][((playerboard & 0x0102040810204080) * mn[0][1]) >> shn[0]]
//          [((opponentboard & 0x0102040810204080) * mn[0][1]) >> shn[0]]);
//
//    a += (pattern_arr[evaluate_ptr_num][1][0][((playerboard & 0x4020100804020100) * mn[1][0]) >> shn[1]]
//          [((opponentboard & 0x4020100804020100) * mn[1][0]) >> shn[1]] +
//          pattern_arr[evaluate_ptr_num][1][1][((playerboard & 0x0001020408102040) * mn[1][1]) >> shn[1]]
//          [((opponentboard & 0x0001020408102040) * mn[1][1]) >> shn[1]] +
//          pattern_arr[evaluate_ptr_num][1][2][((playerboard & 0x0080402010080402) * mn[1][2]) >> shn[1]]
//          [((opponentboard & 0x0080402010080402) * mn[1][2]) >> shn[1]] +
//          pattern_arr[evaluate_ptr_num][1][3][((playerboard & 0x0204081020408000) * mn[1][3]) >> shn[1]]
//          [((opponentboard & 0x0204081020408000) * mn[1][3]) >> shn[1]]);
//
//    a += (pattern_arr[evaluate_ptr_num][2][0][((playerboard & 0x2010080402010000) * mn[2][0]) >> shn[2]]
//          [((opponentboard & 0x2010080402010000) * mn[2][0]) >> shn[2]] +
//          pattern_arr[evaluate_ptr_num][2][1][((playerboard & 0x0000010204081020) * mn[2][1]) >> shn[2]]
//          [((opponentboard & 0x0000010204081020) * mn[2][1]) >> shn[2]] +
//          pattern_arr[evaluate_ptr_num][2][2][((playerboard & 0x0000804020100804) * mn[2][2]) >> shn[2]]
//          [((opponentboard & 0x0000804020100804) * mn[2][2]) >> shn[2]] +
//          pattern_arr[evaluate_ptr_num][2][3][((playerboard & 0x0408102040800000) * mn[2][3]) >> shn[2]]
//          [((opponentboard & 0x0408102040800000) * mn[2][3]) >> shn[2]]);
//
//    a += (pattern_arr[evaluate_ptr_num][3][0][((playerboard & 0x1008040201000000) * mn[3][0]) >> shn[3]]
//          [((opponentboard & 0x1008040201000000) * mn[3][0]) >> shn[3]] +
//          pattern_arr[evaluate_ptr_num][3][1][((playerboard & 0x0000000102040810) * mn[3][1]) >> shn[3]]
//          [((opponentboard & 0x0000000102040810) * mn[3][1]) >> shn[3]] +
//          pattern_arr[evaluate_ptr_num][3][2][((playerboard & 0x0000008040201008) * mn[3][2]) >> shn[3]]
//          [((opponentboard & 0x0000008040201008) * mn[3][2]) >> shn[3]] +
//          pattern_arr[evaluate_ptr_num][3][3][((playerboard & 0x0810204080000000) * mn[3][3]) >> shn[3]]
//          [((opponentboard & 0x0810204080000000) * mn[3][3]) >> shn[3]]);
//
//    a += (pattern_arr[evaluate_ptr_num][4][0][((playerboard & 0xff42000000000000) * mn[4][0]) >> shn[4]]
//          [((opponentboard & 0xff42000000000000) * mn[4][0]) >> shn[4]] +
//          pattern_arr[evaluate_ptr_num][4][1][((playerboard & 0x0103010101010301) * mn[4][1]) >> shn[4]]
//          [((opponentboard & 0x0103010101010301) * mn[4][1]) >> shn[4]] +
//          pattern_arr[evaluate_ptr_num][4][2][((playerboard & 0x00000000000042ff) * mn[4][2]) >> shn[4]]
//          [((opponentboard & 0x00000000000042ff) * mn[4][2]) >> shn[4]] +
//          pattern_arr[evaluate_ptr_num][4][3][((playerboard & 0x80c080808080c080) * mn[4][3]) >> shn[4]]
//          [((opponentboard & 0x80c080808080c080) * mn[4][3]) >> shn[4]]);
//
//    a += (pattern_arr[evaluate_ptr_num][5][0][((playerboard & 0xff000000000000) * mn[5][0]) >> shn[5]]
//          [((opponentboard & 0xff000000000000) * mn[5][0]) >> shn[5]] +
//          pattern_arr[evaluate_ptr_num][5][1][((playerboard & 0x0202020202020202) * mn[5][1]) >> shn[5]]
//          [((opponentboard & 0x0202020202020202) * mn[5][1]) >> shn[5]] +
//          pattern_arr[evaluate_ptr_num][5][2][((playerboard & 0x000000000000ff00) * mn[5][2]) >> shn[5]]
//          [((opponentboard & 0x000000000000ff00) * mn[5][2]) >> shn[5]] +
//          pattern_arr[evaluate_ptr_num][5][3][((playerboard & 0x4040404040404040) * mn[5][3]) >> shn[5]]
//          [((opponentboard & 0x4040404040404040) * mn[5][3]) >> shn[5]]);
//
//    a += (pattern_arr[evaluate_ptr_num][6][0][((playerboard & 0xff0000000000) * mn[6][0]) >> shn[6]]
//          [((opponentboard & 0xff0000000000) * mn[6][0]) >> shn[6]] +
//          pattern_arr[evaluate_ptr_num][6][1][((playerboard & 0x0404040404040404) * mn[6][1]) >> shn[6]]
//          [((opponentboard & 0x0404040404040404) * mn[6][1]) >> shn[6]] +
//          pattern_arr[evaluate_ptr_num][6][2][((playerboard & 0x0000000000ff0000) * mn[6][2]) >> shn[6]]
//          [((opponentboard & 0x0000000000ff0000) * mn[6][2]) >> shn[6]] +
//          pattern_arr[evaluate_ptr_num][6][3][((playerboard & 0x2020202020202020) * mn[6][3]) >> shn[6]]
//          [((opponentboard & 0x2020202020202020) * mn[6][3]) >> shn[6]]);
//
//    a += (pattern_arr[evaluate_ptr_num][7][0][((playerboard & 0xff00000000) * mn[7][0]) >> shn[7]]
//          [((opponentboard & 0xff00000000) * mn[7][0]) >> shn[7]] +
//          pattern_arr[evaluate_ptr_num][7][1][((playerboard & 0x0808080808080808) * mn[7][1]) >> shn[7]]
//          [((opponentboard & 0x0808080808080808) * mn[7][1]) >> shn[7]] +
//          pattern_arr[evaluate_ptr_num][7][2][((playerboard & 0x00000000ff000000) * mn[7][2]) >> shn[7]]
//          [((opponentboard & 0x00000000ff000000) * mn[7][2]) >> shn[7]] +
//          pattern_arr[evaluate_ptr_num][7][3][((playerboard & 0x1010101010101010) * mn[7][3]) >> shn[7]]
//          [((opponentboard & 0x1010101010101010) * mn[7][3]) >> shn[7]]);
//
//    a += (pattern_arr[evaluate_ptr_num][8][0][((playerboard & 0xe0e0e00000000000) * mn[8][0]) >> shn[8]]
//          [((opponentboard & 0xe0e0e00000000000) * mn[8][0]) >> shn[8]] +
//          pattern_arr[evaluate_ptr_num][8][1][((playerboard & 0x0707070000000000) * mn[8][1]) >> shn[8]]
//          [((opponentboard & 0x0707070000000000) * mn[8][1]) >> shn[8]] +
//          pattern_arr[evaluate_ptr_num][8][2][((playerboard & 0x0000000000070707) * mn[8][2]) >> shn[8]]
//          [((opponentboard & 0x0000000000070707) * mn[8][2]) >> shn[8]] +
//          pattern_arr[evaluate_ptr_num][8][3][((playerboard & 0x0000000000e0e0e0) * mn[8][3]) >> shn[8]]
//          [((opponentboard & 0x0000000000e0e0e0) * mn[8][3]) >> shn[8]]);
//
//    a += (pattern_arr[evaluate_ptr_num][9][0][((playerboard & 0xf8c0808080000000) * mn[9][0]) >> shn[9]]
//          [((opponentboard & 0xf8c0808080000000) * mn[9][0]) >> shn[9]] +
//          pattern_arr[evaluate_ptr_num][9][1][((playerboard & 0x1f03010101000000) * mn[9][1]) >> shn[9]]
//          [((opponentboard & 0x1f03010101000000) * mn[9][1]) >> shn[9]] +
//          pattern_arr[evaluate_ptr_num][9][2][((playerboard & 0x000000010101031f) * mn[9][2]) >> shn[9]]
//          [((opponentboard & 0x000000010101031f) * mn[9][2]) >> shn[9]] +
//          pattern_arr[evaluate_ptr_num][9][3][((playerboard & 0x000000808080c0f8) * mn[9][3]) >> shn[9]]
//          [((opponentboard & 0x000000808080c0f8) * mn[9][3]) >> shn[9]]);
//
//    a += ((pattern_arr[evaluate_ptr_num][10][0][ppext(playerboard, &maskr[10][0])][ppext(opponentboard, &maskr[10][0])] +
//          pattern_arr[evaluate_ptr_num][10][1][((playerboard & 0x0100030303030001) * mn[10][1]) >> shn[10]]
//          [((opponentboard & 0x0100030303030001) * mn[10][1]) >> shn[10]] +
//          pattern_arr[evaluate_ptr_num][10][2][((playerboard & 0x0000000000003cbd) * mn[10][2]) >> shn[10]]
//          [((opponentboard & 0x0000000000003cbd) * mn[10][2]) >> shn[10]] +
//          pattern_arr[evaluate_ptr_num][10][3][((playerboard & 0x0000010103010300) * mn[10][3]) >> shn[10]]
//          [((opponentboard & 0x0000010103010300) * mn[10][3]) >> shn[10]]));
//
//    a += (pattern_arr[evaluate_ptr_num][11][0][((playerboard & 0xf0e0c08000000000) * mn[11][0]) >> shn[11]]
//          [((opponentboard & 0xf0e0c08000000000) * mn[11][0]) >> shn[11]] +
//          pattern_arr[evaluate_ptr_num][11][1][((playerboard & 0x0f07030100000000) * mn[11][1]) >> shn[11]]
//          [((opponentboard & 0x0f07030100000000) * mn[11][1]) >> shn[11]] +
//          pattern_arr[evaluate_ptr_num][11][2][((playerboard & 0x000000000103070f) * mn[11][2]) >> shn[11]]
//          [((opponentboard & 0x000000000103070f) * mn[11][2]) >> shn[11]] +
//          pattern_arr[evaluate_ptr_num][11][3][((playerboard & 0x0000000080c0e0f0) * mn[11][3]) >> shn[11]]
//          [((opponentboard & 0x0000000080c0e0f0) * mn[11][3]) >> shn[11]]);
    a += final_bias[evaluate_ptr_num];
    
//    if (afterIndex <= 36) a += add_arr[popcount(makelegalboard(playerboard, opponentboard))-popcount(makelegalboard(opponentboard, playerboard)) + max_mobility];
    return a;
}
