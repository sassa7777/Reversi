#include <bits/stdc++.h>
#include <arm_neon.h>

using namespace std;

struct Adam {
    double w = 0;
    int64_t delta = 0;
    double m = 0.0, v = 0.0;
    unsigned int t = 0;

    Adam() = default;
};

struct Mobility {
    double w = 0;
    int64_t delta = 0;
    unsigned int t = 0;
    Mobility() = default;
};

constexpr int shn[14][4] = {{56, 56, 56, 56}, {57, 57, 57, 57}, {58, 58, 58, 58}, {59, 59, 59, 59}, {54, 54, 54, 54}, {56, 56, 56, 56}, {56, 56, 56, 56}, {56, 56, 56, 56}, {55, 55, 55, 55}, {54, 54, 54, 54}, {53, 54, 54, 54}, {54, 54, 54, 54}, {54, 54, 54, 54}, {54, 54, 54, 54}};

constexpr int comp[14][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
constexpr int pattern_sizes[] = {8, 7, 6, 5, 10, 8, 8, 8, 9, 10, 10, 10, 10, 10};

constexpr uint64_t bit_pattern[] = {0x8040201008040201ULL, 0x4020100804020100ULL, 0x2010080402010000ULL, 0x1008040201000000ULL, 0xff42000000000000ULL, 0xff000000000000ULL, 0xff0000000000ULL, 0xff00000000ULL, 0xe0e0e00000000000ULL, 0xf8c0808080000000ULL, 0xbd3c000000000000ULL, 0xf0e0c08000000000ULL, 0xF8F8000000000000, 0xC0C0C0C0C0000000};


constexpr uint64_t mn[14][4] = {
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
    {0x820000000100809ULL, 0x10181800200c8ULL, 0xc00c00a01424200ULL, 0x81000408002200ULL},
    //2x5_idx
    {0x8005011084008061, 0x1000401401, 0x8800401400001100, 0x804010040100002},
    {0x10000040100403, 0xa004100920410108, 0x1001004004000000, 0x100082200001000}
};

vector<vector<vector<vector<vector<Adam>>>>> pattern_arr(1, vector<vector<vector<vector<Adam>>>>(14));
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

// code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm
inline uint64_t makelegalboard(uint64_t p, uint64_t o) noexcept {
    uint64_t moves, hb, flip1, flip7, flip9, flip8, pre1, pre7, pre9, pre8;
    
    hb = o & 0x7e7e7e7e7e7e7e7eULL;
    flip1  = hb & (p << 1);    flip7  = hb & (p << 7);        flip9  = hb & (p << 9);        flip8  = o & (p << 8);
    flip1 |= hb & (flip1 << 1);    flip7 |= hb & (flip7 << 7);    flip9 |= hb & (flip9 << 9);    flip8 |= o & (flip8 << 8);
    pre1 = hb & (hb << 1);         pre7 = hb & (hb << 7);        pre9 = hb & (hb << 9);        pre8 = o & (o << 8);
    flip1 |= pre1 & (flip1 << 2);    flip7 |= pre7 & (flip7 << 14);    flip9 |= pre9 & (flip9 << 18);    flip8 |= pre8 & (flip8 << 16);
    flip1 |= pre1 & (flip1 << 2);    flip7 |= pre7 & (flip7 << 14);    flip9 |= pre9 & (flip9 << 18);    flip8 |= pre8 & (flip8 << 16);
    moves = flip1 << 1;        moves |= flip7 << 7;        moves |= flip9 << 9;        moves |= flip8 << 8;
    flip1  = hb & (p >> 1);        flip7  = hb & (p >> 7);        flip9  = hb & (p >> 9);        flip8  = o & (p >> 8);
    flip1 |= hb & (flip1 >> 1);    flip7 |= hb & (flip7 >> 7);    flip9 |= hb & (flip9 >> 9);    flip8 |= o & (flip8 >> 8);
    pre1 >>= 1;            pre7 >>= 7;            pre9 >>= 9;            pre8 >>= 8;
    flip1 |= pre1 & (flip1 >> 2);    flip7 |= pre7 & (flip7 >> 14);    flip9 |= pre9 & (flip9 >> 18);    flip8 |= pre8 & (flip8 >> 16);
    flip1 |= pre1 & (flip1 >> 2);    flip7 |= pre7 & (flip7 >> 14);    flip9 |= pre9 & (flip9 >> 18);    flip8 |= pre8 & (flip8 >> 16);
    moves |= flip1 >> 1;        moves |= flip7 >> 7;        moves |= flip9 >> 9;        moves |= flip8 >> 8;
    
    return moves & ~(p|o);
}

inline double evaluate(uint64_t playerboard, uint64_t opponentboard) noexcept {
    double a = 0;

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
    
    a += (pattern_arr[0][12][0][((playerboard & 0xF8F8000000000000) * mn[12][0]) >> 54]
          [((opponentboard & 0xF8F8000000000000) * mn[12][0]) >> 54].w +
          pattern_arr[0][12][1][((playerboard & 0x0303030303000000) * mn[12][1]) >> 54]
          [((opponentboard & 0x0303030303000000) * mn[12][1]) >> 54].w +
          pattern_arr[0][12][2][((playerboard & 0x0000000000001f1f) * mn[12][2]) >> 54]
          [((opponentboard & 0x0000000000001f1f) * mn[12][2]) >> 54].w +
          pattern_arr[0][12][3][((playerboard & 0x000000c0c0c0c0c0) * mn[12][3]) >> 54]
          [((opponentboard & 0x000000c0c0c0c0c0) * mn[12][3]) >> 54].w);
    
    a += (pattern_arr[0][13][0][((playerboard & 0xC0C0C0C0C0000000) * mn[13][0]) >> 54]
          [((opponentboard & 0xC0C0C0C0C0000000) * mn[13][0]) >> 54].w +
          pattern_arr[0][13][1][((playerboard & 0x1f1f000000000000) * mn[13][1]) >> 54]
          [((opponentboard & 0x1f1f000000000000) * mn[13][1]) >> 54].w +
          pattern_arr[0][13][2][((playerboard & 0x0000000303030303) * mn[13][2]) >> 54]
          [((opponentboard & 0x0000000303030303) * mn[13][2]) >> 54].w +
          pattern_arr[0][13][3][((playerboard & 0x000000000000f8f8) * mn[13][3]) >> 54]
          [((opponentboard & 0x000000000000f8f8) * mn[13][3]) >> 54].w);
        
    return a;
}

inline void init_appear(uint64_t playerboard, uint64_t opponentboard) noexcept {
    pattern_arr[0][0][0][((playerboard & 0x8040201008040201ULL) * mn[0][0]) >> 56]
    [((opponentboard & 0x8040201008040201ULL) * mn[0][0]) >> 56].t++;
    pattern_arr[0][0][1][((playerboard & 0x0102040810204080ULL) * mn[0][1]) >> 56]
    [((opponentboard & 0x0102040810204080ULL) * mn[0][1]) >> 56].t++;
    
    pattern_arr[0][1][0][((playerboard & 0x4020100804020100ULL) * mn[1][0]) >> 57]
    [((opponentboard & 0x4020100804020100ULL) * mn[1][0]) >> 57].t++;
    pattern_arr[0][1][1][((playerboard & 0x0001020408102040ULL) * mn[1][1]) >> 57]
    [((opponentboard & 0x0001020408102040ULL) * mn[1][1]) >> 57].t++;
    pattern_arr[0][1][2][((playerboard & 0x0080402010080402ULL) * mn[1][2]) >> 57]
    [((opponentboard & 0x0080402010080402ULL) * mn[1][2]) >> 57].t++;
    pattern_arr[0][1][3][((playerboard & 0x0204081020408000ULL) * mn[1][3]) >> 57]
    [((opponentboard & 0x0204081020408000ULL) * mn[1][3]) >> 57].t++;
    
    pattern_arr[0][2][0][((playerboard & 0x2010080402010000ULL) * mn[2][0]) >> 58]
    [((opponentboard & 0x2010080402010000ULL) * mn[2][0]) >> 58].t++;
    pattern_arr[0][2][1][((playerboard & 0x0000010204081020ULL) * mn[2][1]) >> 58]
    [((opponentboard & 0x0000010204081020ULL) * mn[2][1]) >> 58].t++;
    pattern_arr[0][2][2][((playerboard & 0x0000804020100804ULL) * mn[2][2]) >> 58]
    [((opponentboard & 0x0000804020100804ULL) * mn[2][2]) >> 58].t++;
    pattern_arr[0][2][3][((playerboard & 0x0408102040800000ULL) * mn[2][3]) >> 58]
    [((opponentboard & 0x0408102040800000ULL) * mn[2][3]) >> 58].t++;
    
    pattern_arr[0][3][0][((playerboard & 0x1008040201000000ULL) * mn[3][0]) >> 59]
    [((opponentboard & 0x1008040201000000ULL) * mn[3][0]) >> 59].t++;
    pattern_arr[0][3][1][((playerboard & 0x0000000102040810ULL) * mn[3][1]) >> 59]
    [((opponentboard & 0x0000000102040810ULL) * mn[3][1]) >> 59].t++;
    pattern_arr[0][3][2][((playerboard & 0x0000008040201008ULL) * mn[3][2]) >> 59]
    [((opponentboard & 0x0000008040201008ULL) * mn[3][2]) >> 59].t++;
    pattern_arr[0][3][3][((playerboard & 0x0810204080000000ULL) * mn[3][3]) >> 59]
    [((opponentboard & 0x0810204080000000ULL) * mn[3][3]) >> 59].t++;
    
    pattern_arr[0][4][0][((playerboard & 0xff42000000000000ULL) * mn[4][0]) >> 54]
    [((opponentboard & 0xff42000000000000ULL) * mn[4][0]) >> 54].t++;
    pattern_arr[0][4][1][((playerboard & 0x0103010101010301ULL) * mn[4][1]) >> 54]
    [((opponentboard & 0x0103010101010301ULL) * mn[4][1]) >> 54].t++;
    pattern_arr[0][4][2][((playerboard & 0x00000000000042ffULL) * mn[4][2]) >> 54]
    [((opponentboard & 0x00000000000042ffULL) * mn[4][2]) >> 54].t++;
    pattern_arr[0][4][3][((playerboard & 0x80c080808080c080ULL) * mn[4][3]) >> 54]
    [((opponentboard & 0x80c080808080c080ULL) * mn[4][3]) >> 54].t++;
    
    pattern_arr[0][5][0][((playerboard & 0x00ff000000000000ULL) * mn[5][0]) >> 56]
    [((opponentboard & 0x00ff000000000000ULL) * mn[5][0]) >> 56].t++;
    pattern_arr[0][5][1][((playerboard & 0x0202020202020202ULL) * mn[5][1]) >> 56]
    [((opponentboard & 0x0202020202020202ULL) * mn[5][1]) >> 56].t++;
    pattern_arr[0][5][2][((playerboard & 0x000000000000ff00ULL) * mn[5][2]) >> 56]
    [((opponentboard & 0x000000000000ff00ULL) * mn[5][2]) >> 56].t++;
    pattern_arr[0][5][3][((playerboard & 0x4040404040404040ULL) * mn[5][3]) >> 56]
    [((opponentboard & 0x4040404040404040ULL) * mn[5][3]) >> 56].t++;
    
    pattern_arr[0][6][0][((playerboard & 0x0000ff0000000000ULL) * mn[6][0]) >> 56]
    [((opponentboard & 0x0000ff0000000000ULL) * mn[6][0]) >> 56].t++;
    pattern_arr[0][6][1][((playerboard & 0x0404040404040404ULL) * mn[6][1]) >> 56]
    [((opponentboard & 0x0404040404040404ULL) * mn[6][1]) >> 56].t++;
    pattern_arr[0][6][2][((playerboard & 0x0000000000ff0000ULL) * mn[6][2]) >> 56]
    [((opponentboard & 0x0000000000ff0000ULL) * mn[6][2]) >> 56].t++;
    pattern_arr[0][6][3][((playerboard & 0x2020202020202020ULL) * mn[6][3]) >> 56]
    [((opponentboard & 0x2020202020202020ULL) * mn[6][3]) >> 56].t++;
    
    pattern_arr[0][7][0][((playerboard & 0x000000ff00000000ULL) * mn[7][0]) >> 56]
    [((opponentboard & 0x000000ff00000000ULL) * mn[7][0]) >> 56].t++;
    pattern_arr[0][7][1][((playerboard & 0x0808080808080808ULL) * mn[7][1]) >> 56]
    [((opponentboard & 0x0808080808080808ULL) * mn[7][1]) >> 56].t++;
    pattern_arr[0][7][2][((playerboard & 0x00000000ff000000ULL) * mn[7][2]) >> 56]
    [((opponentboard & 0x00000000ff000000ULL) * mn[7][2]) >> 56].t++;
    pattern_arr[0][7][3][((playerboard & 0x1010101010101010ULL) * mn[7][3]) >> 56]
    [((opponentboard & 0x1010101010101010ULL) * mn[7][3]) >> 56].t++;
    
    pattern_arr[0][8][0][((playerboard & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55]
    [((opponentboard & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55].t++;
    pattern_arr[0][8][1][((playerboard & 0x0707070000000000ULL) * mn[8][1]) >> 55]
    [((opponentboard & 0x0707070000000000ULL) * mn[8][1]) >> 55].t++;
    pattern_arr[0][8][2][((playerboard & 0x0000000000070707ULL) * mn[8][2]) >> 55]
    [((opponentboard & 0x0000000000070707ULL) * mn[8][2]) >> 55].t++;
    pattern_arr[0][8][3][((playerboard & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55]
    [((opponentboard & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55].t++;
    
    pattern_arr[0][9][0][((playerboard & 0xf8c0808080000000ULL) * mn[9][0]) >> 54]
    [((opponentboard & 0xf8c0808080000000ULL) * mn[9][0]) >> 54].t++;
    pattern_arr[0][9][1][((playerboard & 0x1f03010101000000ULL) * mn[9][1]) >> 54]
    [((opponentboard & 0x1f03010101000000ULL) * mn[9][1]) >> 54].t++;
    pattern_arr[0][9][2][((playerboard & 0x000000010101031fULL) * mn[9][2]) >> 54]
    [((opponentboard & 0x000000010101031fULL) * mn[9][2]) >> 54].t++;
    pattern_arr[0][9][3][((playerboard & 0x000000808080c0f8ULL) * mn[9][3]) >> 54]
    [((opponentboard & 0x000000808080c0f8ULL) * mn[9][3]) >> 54].t++;
    
    pattern_arr[0][10][0][((playerboard & 0xbd3c000000000000ULL) * mn[10][0]) >> 53]
    [((opponentboard & 0xbd3c000000000000ULL) * mn[10][0]) >> 53].t++;
    pattern_arr[0][10][1][((playerboard & 0x0100030303030001ULL) * mn[10][1]) >> 54]
    [((opponentboard & 0x0100030303030001ULL) * mn[10][1]) >> 54].t++;
    pattern_arr[0][10][2][((playerboard & 0x0000000000003cbdULL) * mn[10][2]) >> 54]
    [((opponentboard & 0x0000000000003cbdULL) * mn[10][2]) >> 54].t++;
    pattern_arr[0][10][3][((playerboard & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54]
    [((opponentboard & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54].t++;
    
    pattern_arr[0][11][0][((playerboard & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54]
    [((opponentboard & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54].t++;
    pattern_arr[0][11][1][((playerboard & 0x0f07030100000000ULL) * mn[11][1]) >> 54]
    [((opponentboard & 0x0f07030100000000ULL) * mn[11][1]) >> 54].t++;
    pattern_arr[0][11][2][((playerboard & 0x000000000103070fULL) * mn[11][2]) >> 54]
    [((opponentboard & 0x000000000103070fULL) * mn[11][2]) >> 54].t++;
    pattern_arr[0][11][3][((playerboard & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54]
    [((opponentboard & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54].t++;
    
    pattern_arr[0][12][0][((playerboard & 0xF8F8000000000000) * mn[12][0]) >> 54]
    [((opponentboard & 0xF8F8000000000000) * mn[12][0]) >> 54].t++;
    pattern_arr[0][12][1][((playerboard & 0x0303030303000000) * mn[12][1]) >> 54]
    [((opponentboard & 0x0303030303000000) * mn[12][1]) >> 54].t++;
    pattern_arr[0][12][2][((playerboard & 0x0000000000001f1f) * mn[12][2]) >> 54]
    [((opponentboard & 0x0000000000001f1f) * mn[12][2]) >> 54].t++;
    pattern_arr[0][12][3][((playerboard & 0x000000c0c0c0c0c0) * mn[12][3]) >> 54]
    [((opponentboard & 0x000000c0c0c0c0c0) * mn[12][3]) >> 54].t++;
    
    pattern_arr[0][13][0][((playerboard & 0xC0C0C0C0C0000000) * mn[13][0]) >> 54]
    [((opponentboard & 0xC0C0C0C0C0000000) * mn[13][0]) >> 54].t++;
    pattern_arr[0][13][1][((playerboard & 0x1f1f000000000000) * mn[13][1]) >> 54]
    [((opponentboard & 0x1f1f000000000000) * mn[13][1]) >> 54].t++;
    pattern_arr[0][13][2][((playerboard & 0x0000000303030303) * mn[13][2]) >> 54]
    [((opponentboard & 0x0000000303030303) * mn[13][2]) >> 54].t++;
    pattern_arr[0][13][3][((playerboard & 0x000000000000f8f8) * mn[13][3]) >> 54]
    [((opponentboard & 0x000000000000f8f8) * mn[13][3]) >> 54].t++;
    
    return;
}

constexpr int game_phase_start = 40 + 4;
constexpr int game_phase_end = 60 + 4;
double alpha = 0.05;
//double alpha_t = alpha / 5.0;
double alpha_t = alpha;
int phase = 0;

inline void adam(Adam &a) {
    constexpr double beta1 = 0.9, beta2 = 0.999, epsilon = 1e-8;
    if (a.t == 0) return;
    double lr = alpha_t / a.t;
    double grad = (a.delta * 2);
    double lrt = lr * sqrt(1.0 - pow(beta2, phase)) / (1.0 - pow(beta1, phase));
    a.m += (1.0 - beta1) * (grad - a.m);
    a.v += (1.0 - beta2) * (grad * grad - a.v);

    a.w += lrt * a.m / (sqrt(a.v) + epsilon);
    a.delta = 0;
}

template <typename T>
inline void gd(T &a) {
    if (a.t == 0) return;
    constexpr unsigned int M = 50;
    double lr = alpha_t / std::max(M, a.t);
    double grad = (a.delta * 2);

    a.w += lr * grad;
    a.delta = 0;
}

inline void momentum(Adam &a) {
    if (a.t == 0) return;
    constexpr unsigned int M = 50;
    double lr = alpha_t / max(M, a.t);
    double grad = (a.delta * 2);
    constexpr double beta1 = 0.9;
    a.m = beta1 * a.m + lr * grad;
    a.w += a.m;
    a.delta = 0;
}

inline void cal_diff(Adam &a, int grad) {
    a.delta += grad;
}

inline void cal_diff(Mobility &a, int grad, uint64_t playerboard, uint64_t opponentboard) {
    int legal_moves = popcount(makelegalboard(playerboard, opponentboard));
    if (legal_moves > 0) {
        a.delta += grad / legal_moves;
    }
}


inline void update_eval_param(uint64_t playerboard, uint64_t opponentboard, int ans) noexcept {
    int grad = (ans - evaluate(playerboard, opponentboard));
    
    cal_diff(pattern_arr[0][0][0][((playerboard & 0x8040201008040201ULL) * mn[0][0]) >> 56][((opponentboard & 0x8040201008040201ULL) * mn[0][0]) >> 56], grad);

    cal_diff(pattern_arr[0][0][1][((playerboard & 0x0102040810204080ULL) * mn[0][1]) >> 56][((opponentboard & 0x0102040810204080ULL) * mn[0][1]) >> 56], grad);

    cal_diff(pattern_arr[0][1][0][((playerboard & 0x4020100804020100ULL) * mn[1][0]) >> 57][((opponentboard & 0x4020100804020100ULL) * mn[1][0]) >> 57], grad);

    cal_diff(pattern_arr[0][1][1][((playerboard & 0x0001020408102040ULL) * mn[1][1]) >> 57][((opponentboard & 0x0001020408102040ULL) * mn[1][1]) >> 57], grad);

    cal_diff(pattern_arr[0][1][2][((playerboard & 0x0080402010080402ULL) * mn[1][2]) >> 57][((opponentboard & 0x0080402010080402ULL) * mn[1][2]) >> 57], grad);

    cal_diff(pattern_arr[0][1][3][((playerboard & 0x0204081020408000ULL) * mn[1][3]) >> 57][((opponentboard & 0x0204081020408000ULL) * mn[1][3]) >> 57], grad);

    cal_diff(pattern_arr[0][2][0][((playerboard & 0x2010080402010000ULL) * mn[2][0]) >> 58][((opponentboard & 0x2010080402010000ULL) * mn[2][0]) >> 58], grad);

    cal_diff(pattern_arr[0][2][1][((playerboard & 0x0000010204081020ULL) * mn[2][1]) >> 58][((opponentboard & 0x0000010204081020ULL) * mn[2][1]) >> 58], grad);

    cal_diff(pattern_arr[0][2][2][((playerboard & 0x0000804020100804ULL) * mn[2][2]) >> 58][((opponentboard & 0x0000804020100804ULL) * mn[2][2]) >> 58], grad);

    cal_diff(pattern_arr[0][2][3][((playerboard & 0x0408102040800000ULL) * mn[2][3]) >> 58][((opponentboard & 0x0408102040800000ULL) * mn[2][3]) >> 58], grad);

    cal_diff(pattern_arr[0][3][0][((playerboard & 0x1008040201000000ULL) * mn[3][0]) >> 59][((opponentboard & 0x1008040201000000ULL) * mn[3][0]) >> 59], grad);

    cal_diff(pattern_arr[0][3][1][((playerboard & 0x0000000102040810ULL) * mn[3][1]) >> 59][((opponentboard & 0x0000000102040810ULL) * mn[3][1]) >> 59], grad);

    cal_diff(pattern_arr[0][3][2][((playerboard & 0x0000008040201008ULL) * mn[3][2]) >> 59][((opponentboard & 0x0000008040201008ULL) * mn[3][2]) >> 59], grad);

    cal_diff(pattern_arr[0][3][3][((playerboard & 0x0810204080000000ULL) * mn[3][3]) >> 59][((opponentboard & 0x0810204080000000ULL) * mn[3][3]) >> 59], grad);

    cal_diff(pattern_arr[0][4][0][((playerboard & 0xff42000000000000ULL) * mn[4][0]) >> 54][((opponentboard & 0xff42000000000000ULL) * mn[4][0]) >> 54], grad);

    cal_diff(pattern_arr[0][4][1][((playerboard & 0x0103010101010301ULL) * mn[4][1]) >> 54][((opponentboard & 0x0103010101010301ULL) * mn[4][1]) >> 54], grad);

    cal_diff(pattern_arr[0][4][2][((playerboard & 0x00000000000042ffULL) * mn[4][2]) >> 54][((opponentboard & 0x00000000000042ffULL) * mn[4][2]) >> 54], grad);

    cal_diff(pattern_arr[0][4][3][((playerboard & 0x80c080808080c080ULL) * mn[4][3]) >> 54][((opponentboard & 0x80c080808080c080ULL) * mn[4][3]) >> 54], grad);

    cal_diff(pattern_arr[0][5][0][((playerboard & 0x00ff000000000000ULL) * mn[5][0]) >> 56][((opponentboard & 0x00ff000000000000ULL) * mn[5][0]) >> 56], grad);

    cal_diff(pattern_arr[0][5][1][((playerboard & 0x0202020202020202ULL) * mn[5][1]) >> 56][((opponentboard & 0x0202020202020202ULL) * mn[5][1]) >> 56], grad);

    cal_diff(pattern_arr[0][5][2][((playerboard & 0x000000000000ff00ULL) * mn[5][2]) >> 56][((opponentboard & 0x000000000000ff00ULL) * mn[5][2]) >> 56], grad);

    cal_diff(pattern_arr[0][5][3][((playerboard & 0x4040404040404040ULL) * mn[5][3]) >> 56][((opponentboard & 0x4040404040404040ULL) * mn[5][3]) >> 56], grad);

    cal_diff(pattern_arr[0][6][0][((playerboard & 0x0000ff0000000000ULL) * mn[6][0]) >> 56][((opponentboard & 0x0000ff0000000000ULL) * mn[6][0]) >> 56], grad);

    cal_diff(pattern_arr[0][6][1][((playerboard & 0x0404040404040404ULL) * mn[6][1]) >> 56][((opponentboard & 0x0404040404040404ULL) * mn[6][1]) >> 56], grad);

    cal_diff(pattern_arr[0][6][2][((playerboard & 0x0000000000ff0000ULL) * mn[6][2]) >> 56][((opponentboard & 0x0000000000ff0000ULL) * mn[6][2]) >> 56], grad);

    cal_diff(pattern_arr[0][6][3][((playerboard & 0x2020202020202020ULL) * mn[6][3]) >> 56][((opponentboard & 0x2020202020202020ULL) * mn[6][3]) >> 56], grad);

    cal_diff(pattern_arr[0][7][0][((playerboard & 0x000000ff00000000ULL) * mn[7][0]) >> 56][((opponentboard & 0x000000ff00000000ULL) * mn[7][0]) >> 56], grad);

    cal_diff(pattern_arr[0][7][1][((playerboard & 0x0808080808080808ULL) * mn[7][1]) >> 56][((opponentboard & 0x0808080808080808ULL) * mn[7][1]) >> 56], grad);

    cal_diff(pattern_arr[0][7][2][((playerboard & 0x00000000ff000000ULL) * mn[7][2]) >> 56][((opponentboard & 0x00000000ff000000ULL) * mn[7][2]) >> 56], grad);

    cal_diff(pattern_arr[0][7][3][((playerboard & 0x1010101010101010ULL) * mn[7][3]) >> 56][((opponentboard & 0x1010101010101010ULL) * mn[7][3]) >> 56], grad);

    cal_diff(pattern_arr[0][8][0][((playerboard & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55][((opponentboard & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55], grad);

    cal_diff(pattern_arr[0][8][1][((playerboard & 0x0707070000000000ULL) * mn[8][1]) >> 55][((opponentboard & 0x0707070000000000ULL) * mn[8][1]) >> 55], grad);

    cal_diff(pattern_arr[0][8][2][((playerboard & 0x0000000000070707ULL) * mn[8][2]) >> 55][((opponentboard & 0x0000000000070707ULL) * mn[8][2]) >> 55], grad);

    cal_diff(pattern_arr[0][8][3][((playerboard & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55][((opponentboard & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55], grad);

    cal_diff(pattern_arr[0][9][0][((playerboard & 0xf8c0808080000000ULL) * mn[9][0]) >> 54][((opponentboard & 0xf8c0808080000000ULL) * mn[9][0]) >> 54], grad);

    cal_diff(pattern_arr[0][9][1][((playerboard & 0x1f03010101000000ULL) * mn[9][1]) >> 54][((opponentboard & 0x1f03010101000000ULL) * mn[9][1]) >> 54], grad);

    cal_diff(pattern_arr[0][9][2][((playerboard & 0x000000010101031fULL) * mn[9][2]) >> 54][((opponentboard & 0x000000010101031fULL) * mn[9][2]) >> 54], grad);

    cal_diff(pattern_arr[0][9][3][((playerboard & 0x000000808080c0f8ULL) * mn[9][3]) >> 54][((opponentboard & 0x000000808080c0f8ULL) * mn[9][3]) >> 54], grad);

    cal_diff(pattern_arr[0][10][0][((playerboard & 0xbd3c000000000000ULL) * mn[10][0]) >> 53][((opponentboard & 0xbd3c000000000000ULL) * mn[10][0]) >> 53], grad);

    cal_diff(pattern_arr[0][10][1][((playerboard & 0x0100030303030001ULL) * mn[10][1]) >> 54][((opponentboard & 0x0100030303030001ULL) * mn[10][1]) >> 54], grad);

    cal_diff(pattern_arr[0][10][2][((playerboard & 0x0000000000003cbdULL) * mn[10][2]) >> 54][((opponentboard & 0x0000000000003cbdULL) * mn[10][2]) >> 54], grad);

    cal_diff(pattern_arr[0][10][3][((playerboard & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54][((opponentboard & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54], grad);

    cal_diff(pattern_arr[0][11][0][((playerboard & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54][((opponentboard & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54], grad);

    cal_diff(pattern_arr[0][11][1][((playerboard & 0x0f07030100000000ULL) * mn[11][1]) >> 54][((opponentboard & 0x0f07030100000000ULL) * mn[11][1]) >> 54], grad);

    cal_diff(pattern_arr[0][11][2][((playerboard & 0x000000000103070fULL) * mn[11][2]) >> 54][((opponentboard & 0x000000000103070fULL) * mn[11][2]) >> 54], grad);

    cal_diff(pattern_arr[0][11][3][((playerboard & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54][((opponentboard & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54], grad);
    
    cal_diff(pattern_arr[0][12][0][((playerboard & 0xF8F8000000000000) * mn[12][0]) >> 54]
                      [((opponentboard & 0xF8F8000000000000) * mn[12][0]) >> 54], grad);

    cal_diff(pattern_arr[0][12][1][((playerboard & 0x0303030303000000) * mn[12][1]) >> 54]
                      [((opponentboard & 0x0303030303000000) * mn[12][1]) >> 54], grad);

    cal_diff(pattern_arr[0][12][2][((playerboard & 0x0000000000001f1f) * mn[12][2]) >> 54]
                      [((opponentboard & 0x0000000000001f1f) * mn[12][2]) >> 54], grad);

    cal_diff(pattern_arr[0][12][3][((playerboard & 0x000000c0c0c0c0c0) * mn[12][3]) >> 54]
                      [((opponentboard & 0x000000c0c0c0c0c0) * mn[12][3]) >> 54], grad);

    cal_diff(pattern_arr[0][13][0][((playerboard & 0xC0C0C0C0C0000000) * mn[13][0]) >> 54]
                      [((opponentboard & 0xC0C0C0C0C0000000) * mn[13][0]) >> 54], grad);

    cal_diff(pattern_arr[0][13][1][((playerboard & 0x1f1f000000000000) * mn[13][1]) >> 54]
                      [((opponentboard & 0x1f1f000000000000) * mn[13][1]) >> 54], grad);

    cal_diff(pattern_arr[0][13][2][((playerboard & 0x0000000303030303) * mn[13][2]) >> 54]
                      [((opponentboard & 0x0000000303030303) * mn[13][2]) >> 54], grad);

    cal_diff(pattern_arr[0][13][3][((playerboard & 0x000000000000f8f8) * mn[13][3]) >> 54]
                      [((opponentboard & 0x000000000000f8f8) * mn[13][3]) >> 54], grad);
}

void load_model(string str) {
    int ptr_num = 0;
    for (int evaluate_idx = 0; evaluate_idx < 14; ++evaluate_idx) {
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
    ifstream model(str);
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
//    for (auto& value : mobility_arr) {
//        getline(model, line);
//        value.w = stoi(line);
//    }
}

inline void fastParseLine(const std::string &line, uint64_t &p, uint64_t &o, int &ans) {
    const char *s = line.c_str();
    p = 0;
    o = 0;
    ans = 0;
    
    // p のパース
    while (*s >= '0' && *s <= '9') {
        p = p * 10 + (*s - '0');
        s++;
    }
    while (*s == ' ') s++;  // 空白をスキップ

    // o のパース
    while (*s >= '0' && *s <= '9') {
        o = o * 10 + (*s - '0');
        s++;
    }
    while (*s == ' ') s++;  // 空白をスキップ

    // ans のパース（符号付き）
    bool neg = false;
    if (*s == '-') {
        neg = true;
        s++;
    }
    while (*s >= '0' && *s <= '9') {
        ans = ans * 10 + (*s - '0');
        s++;
    }
    if (neg) ans = -ans;
}

int main() {
    int ptr_num = 0;
    for (int evaluate_idx = 0; evaluate_idx < 14; ++evaluate_idx) {
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
//    load_model("eval.bin");
    ifstream test_file("test_data.txt");
//    ifstream test_file("train_data_first.txt");
    string line;
    uint64_t idx = 0;
    double MSE = 0, MAE = 0, MSE_b = 0, MAE_b = 0;
    double tmp = 0;
    double abs_error_sum = 0;
    double squared_error_sum = 0;
    ifstream train_file("train_data.txt");
//    ifstream train_file("train_data_first.txt");
    while (getline(train_file, line)) {
        uint64_t p, o;
        int ans;
        fastParseLine(line, p, o, ans);
        if (!(popcount(p | o) >= game_phase_start && popcount(p | o) <= game_phase_end)) continue;
        init_appear(p, o);
        init_appear(r90(p), r90(o));
        init_appear(r180(p), r180(o));
        init_appear(l90(p), l90(o));
        init_appear(flipVertical(p), flipVertical(o));
        init_appear(flipVertical(r90(p)), flipVertical(r90(o)));
        init_appear(flipVertical(r180(p)), flipVertical(r180(o)));
        init_appear(flipVertical(l90(p)), flipVertical(l90(o)));
        init_appear(o, p);
        init_appear(r90(o), r90(p));
        init_appear(r180(o), r180(p));
        init_appear(l90(o), l90(p));
        init_appear(flipVertical(o), flipVertical(p));
        init_appear(flipVertical(r90(o)), flipVertical(r90(p)));
        init_appear(flipVertical(r180(o)), flipVertical(r180(p)));
        init_appear(flipVertical(l90(o)), flipVertical(l90(p)));
        idx += 16;
        cout << "\rinited: " << idx << flush;
    }
//    for (auto& v1 : pattern_arr) {
//        for (auto& v2 : v1) {
//            for (auto& v3 : v2) {
//                for (auto& v4 : v3) {
//                    for (auto& value : v4) {
//                        value.t = min(50, value.t);
//                    }
//                }
//            }
//        }
//    }
    cout << endl;
    idx = 0;
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
        if (!(popcount(p | o) >= game_phase_start && popcount(p | o) <= game_phase_end)) continue;
        idx += 16;
        double errors[16] = {
            (double)abs(ans - evaluate(p, o)),
            (double)abs(ans - evaluate(r90(p), r90(o))),
            (double)abs(ans - evaluate(r180(p), r180(o))),
            (double)abs(ans - evaluate(l90(p), l90(o))),
            (double)abs(ans - evaluate(flipVertical(p), flipVertical(o))),
            (double)abs(ans - evaluate(flipVertical(r90(p)), flipVertical(r90(o)))),
            (double)abs(ans - evaluate(flipVertical(r180(p)), flipVertical(r180(o)))),
            (double)abs(ans - evaluate(flipVertical(l90(p)), flipVertical(l90(o)))),

            (double)abs(-ans - evaluate(o, p)),
            (double)abs(-ans - evaluate(r90(o), r90(p))),
            (double)abs(-ans - evaluate(r180(o), r180(p))),
            (double)abs(-ans - evaluate(l90(o), l90(p))),
            (double)abs(-ans - evaluate(flipVertical(o), flipVertical(p))),
            (double)abs(-ans - evaluate(flipVertical(r90(o)), flipVertical(r90(p)))),
            (double)abs(-ans - evaluate(flipVertical(r180(o)), flipVertical(r180(p)))),
            (double)abs(-ans - evaluate(flipVertical(l90(o)), flipVertical(l90(p))))
        };
        for (int i = 0; i < 16; ++i) {
            abs_error_sum += errors[i]/256;      // MAE
            squared_error_sum += errors[i]/256 * errors[i]/256;  // MSE
        }
        MAE = abs_error_sum / idx;
        MSE = squared_error_sum / idx;
        std::cout << "\rstep: " << idx
                  << " TEST_MAE: " << std::setw(10) << std::left << MAE
                  << " TEST_MSE: " << std::setw(10) << std::left << MSE
                  << std::flush;
    }
    cout << endl;
    for (phase = 1; phase <= 70; ++phase) {
        MSE_b = MSE;
        MAE_b = MAE;
        cout << "phase: " << phase << endl;
//        lr -= 0.1;
        idx = 0;
        MSE = 0, MAE = 0;
        tmp = 0;
        abs_error_sum = 0;
        squared_error_sum = 0;
        ifstream train_file("train_data.txt");
//        ifstream train_file("train_data_first.txt");
        while (getline(train_file, line)) {
            uint64_t p, o;
            int ans;
            fastParseLine(line, p, o, ans);
            if (!(popcount(p | o) >= game_phase_start && popcount(p | o) <= game_phase_end)) continue;
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
            std::cout << "\rstep: " << idx << std::flush;
        }
        cout << endl;
        for (auto& v1 : pattern_arr) {
            for (auto& v2 : v1) {
                for (auto& v3 : v2) {
                    for (auto& v4 : v3) {
                        for (auto& value : v4) {
                            gd(value);
                        }
                    }
                }
            }
        }
        idx = 0;
        MSE = 0, MAE = 0;
        tmp = 0;
        abs_error_sum = 0;
        squared_error_sum = 0;
        ifstream test_file("test_data.txt");
//        ifstream test_file("train_data_first.txt");
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
            if (!(popcount(p | o) >= game_phase_start && popcount(p | o) <= game_phase_end)) continue;
            idx += 16;
            double errors[16] = {
                (double)abs(ans - evaluate(p, o)),
                (double)abs(ans - evaluate(r90(p), r90(o))),
                (double)abs(ans - evaluate(r180(p), r180(o))),
                (double)abs(ans - evaluate(l90(p), l90(o))),
                (double)abs(ans - evaluate(flipVertical(p), flipVertical(o))),
                (double)abs(ans - evaluate(flipVertical(r90(p)), flipVertical(r90(o)))),
                (double)abs(ans - evaluate(flipVertical(r180(p)), flipVertical(r180(o)))),
                (double)abs(ans - evaluate(flipVertical(l90(p)), flipVertical(l90(o)))),

                (double)abs(-ans - evaluate(o, p)),
                (double)abs(-ans - evaluate(r90(o), r90(p))),
                (double)abs(-ans - evaluate(r180(o), r180(p))),
                (double)abs(-ans - evaluate(l90(o), l90(p))),
                (double)abs(-ans - evaluate(flipVertical(o), flipVertical(p))),
                (double)abs(-ans - evaluate(flipVertical(r90(o)), flipVertical(r90(p)))),
                (double)abs(-ans - evaluate(flipVertical(r180(o)), flipVertical(r180(p)))),
                (double)abs(-ans - evaluate(flipVertical(l90(o)), flipVertical(l90(p))))
            };
            for (int i = 0; i < 16; ++i) {
                abs_error_sum += errors[i]/256;      // MAE
                squared_error_sum += errors[i]/256 * errors[i]/256;  // MSE
            }
            MAE = abs_error_sum / idx;
            MSE = squared_error_sum / idx;
            std::cout << "\rstep: " << idx
                      << " TEST_MAE: " << std::setw(10) << std::left << MAE
                      << " TEST_MSE: " << std::setw(10) << std::left << MSE
                      << std::flush;
        }
        cout << endl;
        if (alpha_t < alpha) {
            alpha_t += alpha / 50.0;
        }
        if (MAE_b <= MAE) {
            alpha *= 0.7;
        }
        if (alpha_t > alpha) {
            alpha_t = alpha;
        }
        string txt = "out_adam_first" + to_string(phase) + ".txt";
        ofstream ofs(txt);
        // 5次元ベクターをファイルに出力
        for (const auto& v1 : pattern_arr) {
            for (const auto& v2 : v1) {
                for (const auto& v3 : v2) {
                    for (const auto& v4 : v3) {
                        for (const auto& value : v4) {
                            ofs << round(value.w) << "\n";
                        }
                    }
                }
            }
        }
    }
}
