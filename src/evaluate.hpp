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
#define use_book true

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
static vector<vector<vector<vector<int16_t>>>> pattern_arr(n_patterns);
static vector<vector<int16_t>> mobility_arr(36, vector<int16_t>(36));
static vector<vector<int16_t>> stone_arr(65, vector<int16_t>(65));

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

inline void evaluate_init(String model_path) {
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

    for (int i = 0; i < n_patterns; ++i) {
        pattern_arr[i].resize(4);
        for (int j = 0; j < 4; ++j) {
            int dim = 1 << (pattern_sizes[i] + comp[i][j]);
            pattern_arr[i][j].resize(dim);
            for (int k = 0; k < dim; ++k) {
                pattern_arr[i][j][k].resize(dim);
                for (int l = 0; l < dim; ++l) {
                    pattern_arr[i][j][k][l] = fast_next_int(p);
                }
            }
        }
    }
    for (auto &v1 : mobility_arr) {
        for (auto &v2 : v1) {
            v2 = fast_next_int(p);
        }
    }
    for (auto &v1 : stone_arr) {
        for (auto &v2 : v1) {
            v2 = fast_next_int(p);
        }
    }
}

#define evaluate_moveorder(p, o) evaluate(p, o)

inline int evaluate(const uint64_t playerboard, const uint64_t opponentboard) noexcept {

    if (playerboard == 0) [[unlikely]] return -32768;
    if (opponentboard == 0) [[unlikely]] return 32768;
    
    int a = 0;

    a += (pattern_arr[0][0][((playerboard & 0x8040201008040201ULL) * mn[0][0]) >> 56]
          [((opponentboard & 0x8040201008040201ULL) * mn[0][0]) >> 56] +
          pattern_arr[0][1][((playerboard & 0x0102040810204080ULL) * mn[0][1]) >> 56]
          [((opponentboard & 0x0102040810204080ULL) * mn[0][1]) >> 56]);
    
    a += (pattern_arr[1][0][((playerboard & 0x4020100804020100ULL) * mn[1][0]) >> 57]
          [((opponentboard & 0x4020100804020100ULL) * mn[1][0]) >> 57] +
          pattern_arr[1][1][((playerboard & 0x0001020408102040ULL) * mn[1][1]) >> 57]
          [((opponentboard & 0x0001020408102040ULL) * mn[1][1]) >> 57] +
          pattern_arr[1][2][((playerboard & 0x0080402010080402ULL) * mn[1][2]) >> 57]
          [((opponentboard & 0x0080402010080402ULL) * mn[1][2]) >> 57] +
          pattern_arr[1][3][((playerboard & 0x0204081020408000ULL) * mn[1][3]) >> 57]
          [((opponentboard & 0x0204081020408000ULL) * mn[1][3]) >> 57]);
    
    a += (pattern_arr[2][0][((playerboard & 0x2010080402010000ULL) * mn[2][0]) >> 58]
          [((opponentboard & 0x2010080402010000ULL) * mn[2][0]) >> 58] +
          pattern_arr[2][1][((playerboard & 0x0000010204081020ULL) * mn[2][1]) >> 58]
          [((opponentboard & 0x0000010204081020ULL) * mn[2][1]) >> 58] +
          pattern_arr[2][2][((playerboard & 0x0000804020100804ULL) * mn[2][2]) >> 58]
          [((opponentboard & 0x0000804020100804ULL) * mn[2][2]) >> 58] +
          pattern_arr[2][3][((playerboard & 0x0408102040800000ULL) * mn[2][3]) >> 58]
          [((opponentboard & 0x0408102040800000ULL) * mn[2][3]) >> 58]);
    
    a += (pattern_arr[3][0][((playerboard & 0x1008040201000000ULL) * mn[3][0]) >> 59]
          [((opponentboard & 0x1008040201000000ULL) * mn[3][0]) >> 59] +
          pattern_arr[3][1][((playerboard & 0x0000000102040810ULL) * mn[3][1]) >> 59]
          [((opponentboard & 0x0000000102040810ULL) * mn[3][1]) >> 59] +
          pattern_arr[3][2][((playerboard & 0x0000008040201008ULL) * mn[3][2]) >> 59]
          [((opponentboard & 0x0000008040201008ULL) * mn[3][2]) >> 59] +
          pattern_arr[3][3][((playerboard & 0x0810204080000000ULL) * mn[3][3]) >> 59]
          [((opponentboard & 0x0810204080000000ULL) * mn[3][3]) >> 59]);
    
    a += (pattern_arr[4][0][((playerboard & 0xff42000000000000ULL) * mn[4][0]) >> 54]
          [((opponentboard & 0xff42000000000000ULL) * mn[4][0]) >> 54] +
          pattern_arr[4][1][((playerboard & 0x0103010101010301ULL) * mn[4][1]) >> 54]
          [((opponentboard & 0x0103010101010301ULL) * mn[4][1]) >> 54] +
          pattern_arr[4][2][((playerboard & 0x00000000000042ffULL) * mn[4][2]) >> 54]
          [((opponentboard & 0x00000000000042ffULL) * mn[4][2]) >> 54] +
          pattern_arr[4][3][((playerboard & 0x80c080808080c080ULL) * mn[4][3]) >> 54]
          [((opponentboard & 0x80c080808080c080ULL) * mn[4][3]) >> 54]);
    
    a += (pattern_arr[5][0][((playerboard & 0x00ff000000000000ULL) * mn[5][0]) >> 56]
          [((opponentboard & 0x00ff000000000000ULL) * mn[5][0]) >> 56] +
          pattern_arr[5][1][((playerboard & 0x0202020202020202ULL) * mn[5][1]) >> 56]
          [((opponentboard & 0x0202020202020202ULL) * mn[5][1]) >> 56] +
          pattern_arr[5][2][((playerboard & 0x000000000000ff00ULL) * mn[5][2]) >> 56]
          [((opponentboard & 0x000000000000ff00ULL) * mn[5][2]) >> 56] +
          pattern_arr[5][3][((playerboard & 0x4040404040404040ULL) * mn[5][3]) >> 56]
          [((opponentboard & 0x4040404040404040ULL) * mn[5][3]) >> 56]);
    
    a += (pattern_arr[6][0][((playerboard & 0x0000ff0000000000ULL) * mn[6][0]) >> 56]
          [((opponentboard & 0x0000ff0000000000ULL) * mn[6][0]) >> 56] +
          pattern_arr[6][1][((playerboard & 0x0404040404040404ULL) * mn[6][1]) >> 56]
          [((opponentboard & 0x0404040404040404ULL) * mn[6][1]) >> 56] +
          pattern_arr[6][2][((playerboard & 0x0000000000ff0000ULL) * mn[6][2]) >> 56]
          [((opponentboard & 0x0000000000ff0000ULL) * mn[6][2]) >> 56] +
          pattern_arr[6][3][((playerboard & 0x2020202020202020ULL) * mn[6][3]) >> 56]
          [((opponentboard & 0x2020202020202020ULL) * mn[6][3]) >> 56]);
    
    a += (pattern_arr[7][0][((playerboard & 0x000000ff00000000ULL) * mn[7][0]) >> 56]
          [((opponentboard & 0x000000ff00000000ULL) * mn[7][0]) >> 56] +
          pattern_arr[7][1][((playerboard & 0x0808080808080808ULL) * mn[7][1]) >> 56]
          [((opponentboard & 0x0808080808080808ULL) * mn[7][1]) >> 56] +
          pattern_arr[7][2][((playerboard & 0x00000000ff000000ULL) * mn[7][2]) >> 56]
          [((opponentboard & 0x00000000ff000000ULL) * mn[7][2]) >> 56] +
          pattern_arr[7][3][((playerboard & 0x1010101010101010ULL) * mn[7][3]) >> 56]
          [((opponentboard & 0x1010101010101010ULL) * mn[7][3]) >> 56]);
    
    a += (pattern_arr[8][0][((playerboard & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55]
          [((opponentboard & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55] +
          pattern_arr[8][1][((playerboard & 0x0707070000000000ULL) * mn[8][1]) >> 55]
          [((opponentboard & 0x0707070000000000ULL) * mn[8][1]) >> 55] +
          pattern_arr[8][2][((playerboard & 0x0000000000070707ULL) * mn[8][2]) >> 55]
          [((opponentboard & 0x0000000000070707ULL) * mn[8][2]) >> 55] +
          pattern_arr[8][3][((playerboard & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55]
          [((opponentboard & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55]);
    
    a += (pattern_arr[9][0][((playerboard & 0xf8c0808080000000ULL) * mn[9][0]) >> 54]
          [((opponentboard & 0xf8c0808080000000ULL) * mn[9][0]) >> 54] +
          pattern_arr[9][1][((playerboard & 0x1f03010101000000ULL) * mn[9][1]) >> 54]
          [((opponentboard & 0x1f03010101000000ULL) * mn[9][1]) >> 54] +
          pattern_arr[9][2][((playerboard & 0x000000010101031fULL) * mn[9][2]) >> 54]
          [((opponentboard & 0x000000010101031fULL) * mn[9][2]) >> 54] +
          pattern_arr[9][3][((playerboard & 0x000000808080c0f8ULL) * mn[9][3]) >> 54]
          [((opponentboard & 0x000000808080c0f8ULL) * mn[9][3]) >> 54]);
    
    a += (pattern_arr[10][0][((playerboard & 0xbd3c000000000000ULL) * mn[10][0]) >> 53]
          [((opponentboard & 0xbd3c000000000000ULL) * mn[10][0]) >> 53] +
          pattern_arr[10][1][((playerboard & 0x0100030303030001ULL) * mn[10][1]) >> 54]
          [((opponentboard & 0x0100030303030001ULL) * mn[10][1]) >> 54] +
          pattern_arr[10][2][((playerboard & 0x0000000000003cbdULL) * mn[10][2]) >> 54]
          [((opponentboard & 0x0000000000003cbdULL) * mn[10][2]) >> 54] +
          pattern_arr[10][3][((playerboard & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54]
          [((opponentboard & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54]);
    
    a += (pattern_arr[11][0][((playerboard & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54]
          [((opponentboard & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54] +
          pattern_arr[11][1][((playerboard & 0x0f07030100000000ULL) * mn[11][1]) >> 54]
          [((opponentboard & 0x0f07030100000000ULL) * mn[11][1]) >> 54] +
          pattern_arr[11][2][((playerboard & 0x000000000103070fULL) * mn[11][2]) >> 54]
          [((opponentboard & 0x000000000103070fULL) * mn[11][2]) >> 54] +
          pattern_arr[11][3][((playerboard & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54]
          [((opponentboard & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54]);
    
    a += (pattern_arr[12][0][((playerboard & 0xF8F8000000000000ULL) * mn[12][0]) >> 54]
          [((opponentboard & 0xF8F8000000000000ULL) * mn[12][0]) >> 54] +
          pattern_arr[12][1][((playerboard & 0x0303030303000000ULL) * mn[12][1]) >> 54]
          [((opponentboard & 0x0303030303000000ULL) * mn[12][1]) >> 54] +
          pattern_arr[12][2][((playerboard & 0x0000000000001f1fULL) * mn[12][2]) >> 54]
          [((opponentboard & 0x0000000000001f1fULL) * mn[12][2]) >> 54] +
          pattern_arr[12][3][((playerboard & 0x000000c0c0c0c0c0ULL) * mn[12][3]) >> 54]
          [((opponentboard & 0x000000c0c0c0c0c0ULL) * mn[12][3]) >> 54]);
    
    a += (pattern_arr[13][0][((playerboard & 0xC0C0C0C0C0000000ULL) * mn[13][0]) >> 54]
          [((opponentboard & 0xC0C0C0C0C0000000ULL) * mn[13][0]) >> 54] +
          pattern_arr[13][1][((playerboard & 0x1f1f000000000000ULL) * mn[13][1]) >> 54]
          [((opponentboard & 0x1f1f000000000000ULL) * mn[13][1]) >> 54] +
          pattern_arr[13][2][((playerboard & 0x0000000303030303ULL) * mn[13][2]) >> 54]
          [((opponentboard & 0x0000000303030303ULL) * mn[13][2]) >> 54] +
          pattern_arr[13][3][((playerboard & 0x000000000000f8f8ULL) * mn[13][3]) >> 54]
          [((opponentboard & 0x000000000000f8f8ULL) * mn[13][3]) >> 54]);
            
    a += mobility_arr[popcnt_u64(makelegalboard(playerboard, opponentboard))][popcnt_u64(makelegalboard(opponentboard, playerboard))];
    
    a += stone_arr[popcnt_u64(playerboard)][popcnt_u64(opponentboard)];
    
    return a;
}
