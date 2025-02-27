#include <bits/stdc++.h>

using namespace std;

struct Adam {
    int16_t w;
    double m, v;
    int t;
};

constexpr int shn[12][4] = {{56, 56, 56, 56}, {57, 57, 57, 57}, {58, 58, 58, 58}, {59, 59, 59, 59}, {54, 54, 54, 54}, {56, 56, 56, 56}, {56, 56, 56, 56}, {56, 56, 56, 56}, {55, 55, 55, 55}, {54, 54, 54, 54}, {53, 54, 54, 54}, {54, 54, 54, 54}};
//constexpr int comp[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
constexpr int comp[12][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}};
constexpr int pattern_sizes[] = {8, 7, 6, 5, 10, 8, 8, 8, 9, 10, 10, 10};

constexpr uint64_t bit_pattern[] = {0x8040201008040201ULL, 0x4020100804020100ULL, 0x2010080402010000ULL, 0x1008040201000000ULL, 0xff42000000000000ULL, 0xff000000000000ULL, 0xff0000000000ULL, 0xff00000000ULL, 0xe0e0e00000000000ULL, 0xf8c0808080000000ULL, 0xbd3c000000000000ULL, 0xf0e0c08000000000ULL};

const vector<vector<int>> bit_positions = {{0, 9, 18, 27, 36, 45, 54, 63}, {1, 10, 19, 28, 37, 46, 55}, {2, 11, 20, 29, 38, 47}, {3, 12, 21, 30, 39}, {9, 0, 1, 2, 3, 4, 5, 6, 7, 14}, {8, 9, 10, 11, 12, 13, 14, 15}, {16, 17, 18, 19, 20, 21, 22, 23}, {24, 25, 26, 27, 28, 29, 30, 31}, {0, 1, 8, 9, 2, 16, 10, 17, 18}, {32, 24, 16, 8, 0, 9, 1, 2, 3, 4}, {0, 2, 3, 10, 11, 12, 13, 4, 5, 7}, {0, 1, 2, 3, 8, 9, 10, 16, 17, 24}};

constexpr uint64_t mn[12][4] = {
    // diagonal8_idx
    {0x844c1002020009a9ULL, 0x1280219202044004ULL, 0x844c1002020009a9ULL, 0x1280219202044004ULL},
    // diagonal7_idx
    {0x110142000820011ULL, 0x4600120802380400ULL, 0x200200c840820040ULL, 0x809004200250510ULL},
    // diagonal6_idx
    {0x60c080e0802ULL, 0x4044008411800000ULL, 0x4088400c0810300ULL, 0xc004010281844130ULL},
    // diagonal5_idx
    {0x120098a20451002ULL, 0x8808409000a007ULL, 0x3014008080900040ULL, 0x2200331088814ULL},
    // edge_2x_idx
    {0x4101001020000e51ULL, 0xc01044000801008ULL, 0x1002a080a200110ULL, 0x80200804024001ULL},
    // h_v_2_idx
    {0x4090000220412900ULL, 0x31000a004080240ULL, 0x100900001010044cULL, 0x1020420004108009ULL},
    // h_v_3_idx
    {0x21c090000d810080ULL, 0x400a00046011008ULL, 0x2000808050008024ULL, 0x2080201040390204ULL},
    // h_v_4_idx
    {0x1010009004100ULL, 0x7002084088040029ULL, 0x100090100024cULL, 0x81001204002048ULL},
    // corner_3x3_idx
    {0x12301240a4012009ULL, 0x80408001312004ULL, 0x2000840082001000ULL, 0xc900042000000004ULL},
    // edge_x_side_idx
    {0x20008a204320801ULL, 0xc8048800104ULL, 0xe00800240010085ULL, 0xc980110022040020ULL},
    // edge_block_idx
    {0x4000200000404919ULL, 0x4100004004100016ULL, 0x9104000008090640ULL, 0x5482000820068001ULL},
    // triangle_idx
    {0x820000000100809ULL, 0x10181800200c8ULL, 0xc00c00a01424200ULL, 0x81000408002200ULL}
};

static vector<vector<vector<vector<vector<Adam>>>>> pattern_arr(1, vector<vector<vector<vector<Adam>>>>(12));
//static vector<vector<vector<vector<vector<double>>>>> m(1, vector<vector<vector<vector<double>>>>(12));

inline constexpr uint64_t delta_swap(uint64_t x, uint64_t mask, int delta) {
    uint64_t t = (x ^ (x >> delta)) & mask;
    return x ^ t ^ (t << delta);
}
#ifdef __GNUC__
#define flipVertical(x) __builtin_bswap64(x)
#else
#define flipVertical(x) _byteswap_uint64(x)
#endif

inline constexpr uint64_t flipHorizontal(uint64_t x) {
    x = ((x >> 1) & 0x5555555555555555) | ((x & 0x5555555555555555) << 1);
    x = ((x >> 2) & 0x3333333333333333) | ((x & 0x3333333333333333) << 2);
    x = ((x >> 4) & 0x0f0f0f0f0f0f0f0f) | ((x & 0x0f0f0f0f0f0f0f0f) << 4);
    return x;
}

inline constexpr uint64_t flipDiagonalA1H8(uint64_t x) {
    x = delta_swap(x, 0x00AA00AA00AA00AA, 7);
    x = delta_swap(x, 0x0000CCCC0000CCCC, 14);
    return delta_swap(x, 0x00000000F0F0F0F0, 28);
}

inline constexpr uint64_t flipDiagonalA8H1(uint64_t x) {
    x = delta_swap(x, 0x0055005500550055, 9);
    x = delta_swap(x, 0x0000333300003333, 18);
    return delta_swap(x, 0x000000000F0F0F0F, 36);
}

inline constexpr uint64_t l90(uint64_t x) {
    return flipVertical(flipDiagonalA1H8(x));
}

inline constexpr uint64_t r90(uint64_t x) {
    return flipDiagonalA1H8(flipVertical(x));
}

#ifdef __clang__
#define r180(x) __builtin_bitreverse64(x)
#else
inline constexpr uint64_t r180(uint64_t x) {
    return flipVertical(flipHorizontal(x));
}
#endif

inline int evaluate(uint64_t playerboard, uint64_t opponentboard) noexcept {
    int a = 0;

    a += (pattern_arr[0][0][0][((playerboard & 0x8040201008040201ULL) * mn[0][0]) >> 56]
          [((opponentboard & 0x8040201008040201ULL) * mn[0][0]) >> 56].w +
          pattern_arr[0][0][1][((playerboard & 0x0102040810204080ULL) * mn[0][1]) >> 56]
          [((opponentboard & 0x0102040810204080ULL) * mn[0][1]) >> 56].w);

    a += (pattern_arr[0][1][0][((playerboard & 0x4020100804020100ULL) * mn[1][0]) >> 57]
          [((opponentboard & 0x4020100804020100ULL) * mn[1][0]) >> 57].w +
          pattern_arr[0][1][1][((playerboard & 0x0001020408102040ULL) * mn[1][1]) >> 57]
          [((opponentboard & 0x0001020408102040ULL) * mn[1][1]) >> 57].w +
          pattern_arr[0][1][2][((playerboard & 0x0080402010080402ULL) * mn[1][2]) >> 57]
          [((opponentboard & 0x0080402010080402ULL) * mn[1][2]) >> 57].w +
          pattern_arr[0][1][3][((playerboard & 0x0204081020408000ULL) * mn[1][3]) >> 57]
          [((opponentboard & 0x0204081020408000ULL) * mn[1][3]) >> 57].w);

    a += (pattern_arr[0][2][0][((playerboard & 0x2010080402010000ULL) * mn[2][0]) >> 58]
          [((opponentboard & 0x2010080402010000ULL) * mn[2][0]) >> 58].w +
          pattern_arr[0][2][1][((playerboard & 0x0000010204081020ULL) * mn[2][1]) >> 58]
          [((opponentboard & 0x0000010204081020ULL) * mn[2][1]) >> 58].w +
          pattern_arr[0][2][2][((playerboard & 0x0000804020100804ULL) * mn[2][2]) >> 58]
          [((opponentboard & 0x0000804020100804ULL) * mn[2][2]) >> 58].w +
          pattern_arr[0][2][3][((playerboard & 0x0408102040800000ULL) * mn[2][3]) >> 58]
          [((opponentboard & 0x0408102040800000ULL) * mn[2][3]) >> 58].w);

    a += (pattern_arr[0][3][0][((playerboard & 0x1008040201000000ULL) * mn[3][0]) >> 59]
          [((opponentboard & 0x1008040201000000ULL) * mn[3][0]) >> 59].w +
          pattern_arr[0][3][1][((playerboard & 0x0000000102040810ULL) * mn[3][1]) >> 59]
          [((opponentboard & 0x0000000102040810ULL) * mn[3][1]) >> 59].w +
          pattern_arr[0][3][2][((playerboard & 0x0000008040201008ULL) * mn[3][2]) >> 59]
          [((opponentboard & 0x0000008040201008ULL) * mn[3][2]) >> 59].w +
          pattern_arr[0][3][3][((playerboard & 0x0810204080000000ULL) * mn[3][3]) >> 59]
          [((opponentboard & 0x0810204080000000ULL) * mn[3][3]) >> 59].w);

    a += (pattern_arr[0][4][0][((playerboard & 0xff42000000000000ULL) * mn[4][0]) >> 54]
          [((opponentboard & 0xff42000000000000ULL) * mn[4][0]) >> 54].w +
          pattern_arr[0][4][1][((playerboard & 0x0103010101010301ULL) * mn[4][1]) >> 54]
          [((opponentboard & 0x0103010101010301ULL) * mn[4][1]) >> 54].w +
          pattern_arr[0][4][2][((playerboard & 0x00000000000042ffULL) * mn[4][2]) >> 54]
          [((opponentboard & 0x00000000000042ffULL) * mn[4][2]) >> 54].w +
          pattern_arr[0][4][3][((playerboard & 0x80c080808080c080ULL) * mn[4][3]) >> 54]
          [((opponentboard & 0x80c080808080c080ULL) * mn[4][3]) >> 54].w);

    a += (pattern_arr[0][5][0][((playerboard & 0x00ff000000000000ULL) * mn[5][0]) >> 56]
          [((opponentboard & 0x00ff000000000000ULL) * mn[5][0]) >> 56].w +
          pattern_arr[0][5][1][((playerboard & 0x0202020202020202ULL) * mn[5][1]) >> 56]
          [((opponentboard & 0x0202020202020202ULL) * mn[5][1]) >> 56].w +
          pattern_arr[0][5][2][((playerboard & 0x000000000000ff00ULL) * mn[5][2]) >> 56]
          [((opponentboard & 0x000000000000ff00ULL) * mn[5][2]) >> 56].w +
          pattern_arr[0][5][3][((playerboard & 0x4040404040404040ULL) * mn[5][3]) >> 56]
          [((opponentboard & 0x4040404040404040ULL) * mn[5][3]) >> 56].w);

    a += (pattern_arr[0][6][0][((playerboard & 0x0000ff0000000000ULL) * mn[6][0]) >> 56]
          [((opponentboard & 0x0000ff0000000000ULL) * mn[6][0]) >> 56].w +
          pattern_arr[0][6][1][((playerboard & 0x0404040404040404ULL) * mn[6][1]) >> 56]
          [((opponentboard & 0x0404040404040404ULL) * mn[6][1]) >> 56].w +
          pattern_arr[0][6][2][((playerboard & 0x0000000000ff0000ULL) * mn[6][2]) >> 56]
          [((opponentboard & 0x0000000000ff0000ULL) * mn[6][2]) >> 56].w +
          pattern_arr[0][6][3][((playerboard & 0x2020202020202020ULL) * mn[6][3]) >> 56]
          [((opponentboard & 0x2020202020202020ULL) * mn[6][3]) >> 56].w);

    a += (pattern_arr[0][7][0][((playerboard & 0x000000ff00000000ULL) * mn[7][0]) >> 56]
          [((opponentboard & 0x000000ff00000000ULL) * mn[7][0]) >> 56].w +
          pattern_arr[0][7][1][((playerboard & 0x0808080808080808ULL) * mn[7][1]) >> 56]
          [((opponentboard & 0x0808080808080808ULL) * mn[7][1]) >> 56].w +
          pattern_arr[0][7][2][((playerboard & 0x00000000ff000000ULL) * mn[7][2]) >> 56]
          [((opponentboard & 0x00000000ff000000ULL) * mn[7][2]) >> 56].w +
          pattern_arr[0][7][3][((playerboard & 0x1010101010101010ULL) * mn[7][3]) >> 56]
          [((opponentboard & 0x1010101010101010ULL) * mn[7][3]) >> 56].w);

    a += (pattern_arr[0][8][0][((playerboard & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55]
          [((opponentboard & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55].w +
          pattern_arr[0][8][1][((playerboard & 0x0707070000000000ULL) * mn[8][1]) >> 55]
          [((opponentboard & 0x0707070000000000ULL) * mn[8][1]) >> 55].w +
          pattern_arr[0][8][2][((playerboard & 0x0000000000070707ULL) * mn[8][2]) >> 55]
          [((opponentboard & 0x0000000000070707ULL) * mn[8][2]) >> 55].w +
          pattern_arr[0][8][3][((playerboard & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55]
          [((opponentboard & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55].w);

    a += (pattern_arr[0][9][0][((playerboard & 0xf8c0808080000000ULL) * mn[9][0]) >> 54]
          [((opponentboard & 0xf8c0808080000000ULL) * mn[9][0]) >> 54].w +
          pattern_arr[0][9][1][((playerboard & 0x1f03010101000000ULL) * mn[9][1]) >> 54]
          [((opponentboard & 0x1f03010101000000ULL) * mn[9][1]) >> 54].w +
          pattern_arr[0][9][2][((playerboard & 0x000000010101031fULL) * mn[9][2]) >> 54]
          [((opponentboard & 0x000000010101031fULL) * mn[9][2]) >> 54].w +
          pattern_arr[0][9][3][((playerboard & 0x000000808080c0f8ULL) * mn[9][3]) >> 54]
          [((opponentboard & 0x000000808080c0f8ULL) * mn[9][3]) >> 54].w);

    a += (pattern_arr[0][10][0][((playerboard & 0xbd3c000000000000ULL) * mn[10][0]) >> 53]
          [((opponentboard & 0xbd3c000000000000ULL) * mn[10][0]) >> 53].w +
          pattern_arr[0][10][1][((playerboard & 0x0100030303030001ULL) * mn[10][1]) >> 54]
          [((opponentboard & 0x0100030303030001ULL) * mn[10][1]) >> 54].w +
          pattern_arr[0][10][2][((playerboard & 0x0000000000003cbdULL) * mn[10][2]) >> 54]
          [((opponentboard & 0x0000000000003cbdULL) * mn[10][2]) >> 54].w +
          pattern_arr[0][10][3][((playerboard & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54]
          [((opponentboard & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54].w);

    a += (pattern_arr[0][11][0][((playerboard & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54]
          [((opponentboard & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54].w +
          pattern_arr[0][11][1][((playerboard & 0x0f07030100000000ULL) * mn[11][1]) >> 54]
          [((opponentboard & 0x0f07030100000000ULL) * mn[11][1]) >> 54].w +
          pattern_arr[0][11][2][((playerboard & 0x000000000103070fULL) * mn[11][2]) >> 54]
          [((opponentboard & 0x000000000103070fULL) * mn[11][2]) >> 54].w +
          pattern_arr[0][11][3][((playerboard & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54]
          [((opponentboard & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54].w);
    return a;
}

void adam(Adam &a, double grad) {
    constexpr int MAX_NUM = 2048;
    constexpr double beta1 = 0.9, beta2 = 0.999;
    constexpr double epsilon = 1e-8;
    constexpr double lr = 2.0;
    a.t++;
    double lrt = lr * sqrt(1.0 - pow(beta2, a.t)) / (1.0 - pow(beta1, a.t));
    a.m += (1.0 - beta1) * (grad - a.m);
    a.v += (1.0 - beta2) * (grad * grad - a.v);
    a.w += max(min((int)(lrt * a.m / (sqrt(a.v) + epsilon)), MAX_NUM), -MAX_NUM);
}

void update_eval_param(uint64_t playerboard, uint64_t opponentboard, int ans) noexcept {
    double grad = (ans - evaluate(playerboard, opponentboard));
//    cout << diff << endl;
    
    adam(pattern_arr[0][0][0][((playerboard & 0x8040201008040201ULL) * mn[0][0]) >> 56][((opponentboard & 0x8040201008040201ULL) * mn[0][0]) >> 56], grad);

    adam(pattern_arr[0][0][1][((playerboard & 0x0102040810204080ULL) * mn[0][1]) >> 56][((opponentboard & 0x0102040810204080ULL) * mn[0][1]) >> 56], grad);

    adam(pattern_arr[0][1][0][((playerboard & 0x4020100804020100ULL) * mn[1][0]) >> 57][((opponentboard & 0x4020100804020100ULL) * mn[1][0]) >> 57], grad);

    adam(pattern_arr[0][1][1][((playerboard & 0x0001020408102040ULL) * mn[1][1]) >> 57][((opponentboard & 0x0001020408102040ULL) * mn[1][1]) >> 57], grad);

    adam(pattern_arr[0][1][2][((playerboard & 0x0080402010080402ULL) * mn[1][2]) >> 57][((opponentboard & 0x0080402010080402ULL) * mn[1][2]) >> 57], grad);

    adam(pattern_arr[0][1][3][((playerboard & 0x0204081020408000ULL) * mn[1][3]) >> 57][((opponentboard & 0x0204081020408000ULL) * mn[1][3]) >> 57], grad);

    adam(pattern_arr[0][2][0][((playerboard & 0x2010080402010000ULL) * mn[2][0]) >> 58][((opponentboard & 0x2010080402010000ULL) * mn[2][0]) >> 58], grad);

    adam(pattern_arr[0][2][1][((playerboard & 0x0000010204081020ULL) * mn[2][1]) >> 58][((opponentboard & 0x0000010204081020ULL) * mn[2][1]) >> 58], grad);

    adam(pattern_arr[0][2][2][((playerboard & 0x0000804020100804ULL) * mn[2][2]) >> 58][((opponentboard & 0x0000804020100804ULL) * mn[2][2]) >> 58], grad);

    adam(pattern_arr[0][2][3][((playerboard & 0x0408102040800000ULL) * mn[2][3]) >> 58][((opponentboard & 0x0408102040800000ULL) * mn[2][3]) >> 58], grad);

    adam(pattern_arr[0][3][0][((playerboard & 0x1008040201000000ULL) * mn[3][0]) >> 59][((opponentboard & 0x1008040201000000ULL) * mn[3][0]) >> 59], grad);

    adam(pattern_arr[0][3][1][((playerboard & 0x0000000102040810ULL) * mn[3][1]) >> 59][((opponentboard & 0x0000000102040810ULL) * mn[3][1]) >> 59], grad);

    adam(pattern_arr[0][3][2][((playerboard & 0x0000008040201008ULL) * mn[3][2]) >> 59][((opponentboard & 0x0000008040201008ULL) * mn[3][2]) >> 59], grad);

    adam(pattern_arr[0][3][3][((playerboard & 0x0810204080000000ULL) * mn[3][3]) >> 59][((opponentboard & 0x0810204080000000ULL) * mn[3][3]) >> 59], grad);

    adam(pattern_arr[0][4][0][((playerboard & 0xff42000000000000ULL) * mn[4][0]) >> 54][((opponentboard & 0xff42000000000000ULL) * mn[4][0]) >> 54], grad);

    adam(pattern_arr[0][4][1][((playerboard & 0x0103010101010301ULL) * mn[4][1]) >> 54][((opponentboard & 0x0103010101010301ULL) * mn[4][1]) >> 54], grad);

    adam(pattern_arr[0][4][2][((playerboard & 0x00000000000042ffULL) * mn[4][2]) >> 54][((opponentboard & 0x00000000000042ffULL) * mn[4][2]) >> 54], grad);

    adam(pattern_arr[0][4][3][((playerboard & 0x80c080808080c080ULL) * mn[4][3]) >> 54][((opponentboard & 0x80c080808080c080ULL) * mn[4][3]) >> 54], grad);

    adam(pattern_arr[0][5][0][((playerboard & 0x00ff000000000000ULL) * mn[5][0]) >> 56][((opponentboard & 0x00ff000000000000ULL) * mn[5][0]) >> 56], grad);

    adam(pattern_arr[0][5][1][((playerboard & 0x0202020202020202ULL) * mn[5][1]) >> 56][((opponentboard & 0x0202020202020202ULL) * mn[5][1]) >> 56], grad);

    adam(pattern_arr[0][5][2][((playerboard & 0x000000000000ff00ULL) * mn[5][2]) >> 56][((opponentboard & 0x000000000000ff00ULL) * mn[5][2]) >> 56], grad);

    adam(pattern_arr[0][5][3][((playerboard & 0x4040404040404040ULL) * mn[5][3]) >> 56][((opponentboard & 0x4040404040404040ULL) * mn[5][3]) >> 56], grad);

    adam(pattern_arr[0][6][0][((playerboard & 0x0000ff0000000000ULL) * mn[6][0]) >> 56][((opponentboard & 0x0000ff0000000000ULL) * mn[6][0]) >> 56], grad);

    adam(pattern_arr[0][6][1][((playerboard & 0x0404040404040404ULL) * mn[6][1]) >> 56][((opponentboard & 0x0404040404040404ULL) * mn[6][1]) >> 56], grad);

    adam(pattern_arr[0][6][2][((playerboard & 0x0000000000ff0000ULL) * mn[6][2]) >> 56][((opponentboard & 0x0000000000ff0000ULL) * mn[6][2]) >> 56], grad);

    adam(pattern_arr[0][6][3][((playerboard & 0x2020202020202020ULL) * mn[6][3]) >> 56][((opponentboard & 0x2020202020202020ULL) * mn[6][3]) >> 56], grad);

    adam(pattern_arr[0][7][0][((playerboard & 0x000000ff00000000ULL) * mn[7][0]) >> 56][((opponentboard & 0x000000ff00000000ULL) * mn[7][0]) >> 56], grad);

    adam(pattern_arr[0][7][1][((playerboard & 0x0808080808080808ULL) * mn[7][1]) >> 56][((opponentboard & 0x0808080808080808ULL) * mn[7][1]) >> 56], grad);

    adam(pattern_arr[0][7][2][((playerboard & 0x00000000ff000000ULL) * mn[7][2]) >> 56][((opponentboard & 0x00000000ff000000ULL) * mn[7][2]) >> 56], grad);

    adam(pattern_arr[0][7][3][((playerboard & 0x1010101010101010ULL) * mn[7][3]) >> 56][((opponentboard & 0x1010101010101010ULL) * mn[7][3]) >> 56], grad);

    adam(pattern_arr[0][8][0][((playerboard & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55][((opponentboard & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55], grad);

    adam(pattern_arr[0][8][1][((playerboard & 0x0707070000000000ULL) * mn[8][1]) >> 55][((opponentboard & 0x0707070000000000ULL) * mn[8][1]) >> 55], grad);

    adam(pattern_arr[0][8][2][((playerboard & 0x0000000000070707ULL) * mn[8][2]) >> 55][((opponentboard & 0x0000000000070707ULL) * mn[8][2]) >> 55], grad);

    adam(pattern_arr[0][8][3][((playerboard & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55][((opponentboard & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55], grad);

    adam(pattern_arr[0][9][0][((playerboard & 0xf8c0808080000000ULL) * mn[9][0]) >> 54][((opponentboard & 0xf8c0808080000000ULL) * mn[9][0]) >> 54], grad);

    adam(pattern_arr[0][9][1][((playerboard & 0x1f03010101000000ULL) * mn[9][1]) >> 54][((opponentboard & 0x1f03010101000000ULL) * mn[9][1]) >> 54], grad);

    adam(pattern_arr[0][9][2][((playerboard & 0x000000010101031fULL) * mn[9][2]) >> 54][((opponentboard & 0x000000010101031fULL) * mn[9][2]) >> 54], grad);

    adam(pattern_arr[0][9][3][((playerboard & 0x000000808080c0f8ULL) * mn[9][3]) >> 54][((opponentboard & 0x000000808080c0f8ULL) * mn[9][3]) >> 54], grad);

    adam(pattern_arr[0][10][0][((playerboard & 0xbd3c000000000000ULL) * mn[10][0]) >> 53][((opponentboard & 0xbd3c000000000000ULL) * mn[10][0]) >> 53], grad);

    adam(pattern_arr[0][10][1][((playerboard & 0x0100030303030001ULL) * mn[10][1]) >> 54][((opponentboard & 0x0100030303030001ULL) * mn[10][1]) >> 54], grad);

    adam(pattern_arr[0][10][2][((playerboard & 0x0000000000003cbdULL) * mn[10][2]) >> 54][((opponentboard & 0x0000000000003cbdULL) * mn[10][2]) >> 54], grad);

    adam(pattern_arr[0][10][3][((playerboard & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54][((opponentboard & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54], grad);

    adam(pattern_arr[0][11][0][((playerboard & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54][((opponentboard & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54], grad);

    adam(pattern_arr[0][11][1][((playerboard & 0x0f07030100000000ULL) * mn[11][1]) >> 54][((opponentboard & 0x0f07030100000000ULL) * mn[11][1]) >> 54], grad);

    adam(pattern_arr[0][11][2][((playerboard & 0x000000000103070fULL) * mn[11][2]) >> 54][((opponentboard & 0x000000000103070fULL) * mn[11][2]) >> 54], grad);

    adam(pattern_arr[0][11][3][((playerboard & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54][((opponentboard & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54], grad);
}

void load_model() {
    int ptr_num = 0;
    for (int evaluate_idx = 0; evaluate_idx < 12; ++evaluate_idx) {
        int pattern_idx = evaluate_idx;
        uint64_t pattern1 = bit_pattern[pattern_idx];
        size_t popcountPattern = popcount(pattern1);
        size_t totalCombinations = (1UL << popcountPattern);
        pattern_arr[ptr_num][evaluate_idx].resize(4);
        for (int i = 0; i < 4; ++i) {
            pattern_arr[ptr_num][evaluate_idx][i].resize(1ULL << (popcountPattern+comp[pattern_idx][i]));
            for (int j = 0; j < (1 << (popcountPattern+comp[pattern_idx][i])); ++j) {
                pattern_arr[ptr_num][evaluate_idx][i][j].resize(1ULL << (popcountPattern+comp[pattern_idx][i]));
            }
        }
    }
    ifstream model("out1.txt");
    string line;
    for (auto& v1 : pattern_arr) {
        for (auto& v2 : v1) {
            for (auto& v3 : v2) {
                for (auto& v4 : v3) {
                    for (auto& value : v4) {
                        getline(model, line);
                        value.w = stoi(line);
                    }
                }
            }
        }
    }
}

int main() {
    int ptr_num = 0;
    for (int evaluate_idx = 0; evaluate_idx < 12; ++evaluate_idx) {
        int pattern_idx = evaluate_idx;
        uint64_t pattern1 = bit_pattern[pattern_idx];
        size_t popcountPattern = popcount(pattern1);
        size_t totalCombinations = (1UL << popcountPattern);
        pattern_arr[ptr_num][evaluate_idx].resize(4);
        for (int i = 0; i < 4; ++i) {
            pattern_arr[ptr_num][evaluate_idx][i].resize(1ULL << (popcountPattern+comp[pattern_idx][i]));
            for (int j = 0; j < (1 << (popcountPattern+comp[pattern_idx][i])); ++j) {
                pattern_arr[ptr_num][evaluate_idx][i][j].resize(1ULL << (popcountPattern+comp[pattern_idx][i]));
            }
        }
    }
//    load_model("out_adam1.txt");
    ifstream test_file("test_data.txt");
    string line;
    uint64_t idx = 0;
    int64_t MSE = 0, MAE = 0;
    int64_t tmp = 0;
    int64_t abs_error_sum = 0;
    int64_t squared_error_sum = 0;
    while (getline(test_file, line)) {
        istringstream stream(line);
        string token;
        uint64_t p, o;
        int ans;
        stream >> token;
        p = stoull(token);
        stream >> token;
        o = stoull(token);
        stream >> token;
        ans = stoi(token);
        idx += 16;
        int64_t errors[16] = {
            (int64_t)abs(ans - evaluate(p, o)),
            (int64_t)abs(ans - evaluate(r90(p), r90(o))),
            (int64_t)abs(ans - evaluate(r180(p), r180(o))),
            (int64_t)abs(ans - evaluate(l90(p), l90(o))),
            (int64_t)abs(ans - evaluate(flipVertical(p), flipVertical(o))),
            (int64_t)abs(ans - evaluate(flipVertical(r90(p)), flipVertical(r90(o)))),
            (int64_t)abs(ans - evaluate(flipVertical(r180(p)), flipVertical(r180(o)))),
            (int64_t)abs(ans - evaluate(flipVertical(l90(p)), flipVertical(l90(o)))),

            (int64_t)abs(-ans - evaluate(o, p)),
            (int64_t)abs(-ans - evaluate(r90(o), r90(p))),
            (int64_t)abs(-ans - evaluate(r180(o), r180(p))),
            (int64_t)abs(-ans - evaluate(l90(o), l90(p))),
            (int64_t)abs(-ans - evaluate(flipVertical(o), flipVertical(p))),
            (int64_t)abs(-ans - evaluate(flipVertical(r90(o)), flipVertical(r90(p)))),
            (int64_t)abs(-ans - evaluate(flipVertical(r180(o)), flipVertical(r180(p)))),
            (int64_t)abs(-ans - evaluate(flipVertical(l90(o)), flipVertical(l90(p))))
        };
        for (int i = 0; i < 16; ++i) {
            abs_error_sum += errors[i];      // MAE
            squared_error_sum += errors[i] * errors[i];  // MSE
        }
        MAE = abs_error_sum / idx;
        MSE = squared_error_sum / idx;
        cout << "\ridx: " << idx << " TEST_MAE: " << MAE << " TEST_MSE: " << MSE << flush;
    }
    cout << endl;
    for (int i = 1; i <= 20; ++i) {
        cout << "EPOCH: " << i << endl;
        idx = 0;
        MSE = 0, MAE = 0;
        tmp = 0;
        abs_error_sum = 0;
        squared_error_sum = 0;
        ifstream train_file("train_data.txt");
        while (getline(train_file, line)) {
            istringstream stream(line);
            string token;
            uint64_t p, o;
            int ans;
            stream >> token;
            p = stoull(token);
            stream >> token;
            o = stoull(token);
            stream >> token;
            ans = stoi(token);
            update_eval_param(p, o, ans);
            update_eval_param(r90(p), r90(o), ans);
            update_eval_param(r180(p), r180(o), ans);
            update_eval_param(l90(p), l90(o), ans);
            update_eval_param(flipVertical(p), flipVertical(o), ans);
            update_eval_param(flipVertical(r90(p)), flipVertical(r90(o)), ans);
            update_eval_param(flipVertical(r180(p)), flipVertical(r180(o)), ans);
            update_eval_param(flipVertical(l90(p)), flipVertical(l90(o)), ans);
            update_eval_param(o, p, -ans);
            update_eval_param(r90(o), r90(p), -ans);
            update_eval_param(r180(o), r180(p), -ans);
            update_eval_param(l90(o), l90(p), -ans);
            update_eval_param(flipVertical(o), flipVertical(p), -ans);
            update_eval_param(flipVertical(r90(o)), flipVertical(r90(p)), -ans);
            update_eval_param(flipVertical(r180(o)), flipVertical(r180(p)), -ans);
            update_eval_param(flipVertical(l90(o)), flipVertical(l90(p)), -ans);
            idx += 16;
            int64_t errors[16] = {
                (int64_t)abs(ans - evaluate(p, o)),
                (int64_t)abs(ans - evaluate(r90(p), r90(o))),
                (int64_t)abs(ans - evaluate(r180(p), r180(o))),
                (int64_t)abs(ans - evaluate(l90(p), l90(o))),
                (int64_t)abs(ans - evaluate(flipVertical(p), flipVertical(o))),
                (int64_t)abs(ans - evaluate(flipVertical(r90(p)), flipVertical(r90(o)))),
                (int64_t)abs(ans - evaluate(flipVertical(r180(p)), flipVertical(r180(o)))),
                (int64_t)abs(ans - evaluate(flipVertical(l90(p)), flipVertical(l90(o)))),

                (int64_t)abs(-ans - evaluate(o, p)),
                (int64_t)abs(-ans - evaluate(r90(o), r90(p))),
                (int64_t)abs(-ans - evaluate(r180(o), r180(p))),
                (int64_t)abs(-ans - evaluate(l90(o), l90(p))),
                (int64_t)abs(-ans - evaluate(flipVertical(o), flipVertical(p))),
                (int64_t)abs(-ans - evaluate(flipVertical(r90(o)), flipVertical(r90(p)))),
                (int64_t)abs(-ans - evaluate(flipVertical(r180(o)), flipVertical(r180(p)))),
                (int64_t)abs(-ans - evaluate(flipVertical(l90(o)), flipVertical(l90(p))))
            };
            for (int i = 0; i < 16; ++i) {
                abs_error_sum += errors[i];      // MAE
                squared_error_sum += errors[i] * errors[i];  // MSE
            }
            MAE = abs_error_sum / idx;
            MSE = squared_error_sum / idx;
            cout << "\ridx: " << idx << " MAE: " << MAE << " MSE: " << MSE << flush;
        }
        cout << endl;
        idx = 0;
        MSE = 0, MAE = 0;
        tmp = 0;
        abs_error_sum = 0;
        squared_error_sum = 0;
        ifstream test_file("test_data.txt");
        while (getline(test_file, line)) {
            istringstream stream(line);
            string token;
            uint64_t p, o;
            int ans;
            stream >> token;
            p = stoull(token);
            stream >> token;
            o = stoull(token);
            stream >> token;
            ans = stoi(token);
            idx += 16;
            int64_t errors[16] = {
                (int64_t)abs(ans - evaluate(p, o)),
                (int64_t)abs(ans - evaluate(r90(p), r90(o))),
                (int64_t)abs(ans - evaluate(r180(p), r180(o))),
                (int64_t)abs(ans - evaluate(l90(p), l90(o))),
                (int64_t)abs(ans - evaluate(flipVertical(p), flipVertical(o))),
                (int64_t)abs(ans - evaluate(flipVertical(r90(p)), flipVertical(r90(o)))),
                (int64_t)abs(ans - evaluate(flipVertical(r180(p)), flipVertical(r180(o)))),
                (int64_t)abs(ans - evaluate(flipVertical(l90(p)), flipVertical(l90(o)))),

                (int64_t)abs(-ans - evaluate(o, p)),
                (int64_t)abs(-ans - evaluate(r90(o), r90(p))),
                (int64_t)abs(-ans - evaluate(r180(o), r180(p))),
                (int64_t)abs(-ans - evaluate(l90(o), l90(p))),
                (int64_t)abs(-ans - evaluate(flipVertical(o), flipVertical(p))),
                (int64_t)abs(-ans - evaluate(flipVertical(r90(o)), flipVertical(r90(p)))),
                (int64_t)abs(-ans - evaluate(flipVertical(r180(o)), flipVertical(r180(p)))),
                (int64_t)abs(-ans - evaluate(flipVertical(l90(o)), flipVertical(l90(p))))
            };
            for (int i = 0; i < 16; ++i) {
                abs_error_sum += errors[i];      // MAE
                squared_error_sum += errors[i] * errors[i];  // MSE
            }
            MAE = abs_error_sum / idx;
            MSE = squared_error_sum / idx;
            cout << "\ridx: " << idx << " TEST_MAE: " << MAE << " TEST_MSE: " << MSE << flush;
        }
        cout << endl;
        string txt = "out_adam" + to_string(i) + ".txt";
        std::ofstream ofs(txt);
        // 5次元ベクターをファイルに出力
        for (const auto& v1 : pattern_arr) {
            for (const auto& v2 : v1) {
                for (const auto& v3 : v2) {
                    for (const auto& v4 : v3) {
                        for (const auto& value : v4) {
                            ofs << value.w << "\n"; // 数値をスペース区切りで出力
                        }
                    }
                }
            }
        }
    }
}
