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
struct Pattern_Eval {
    int16_t diagonal8[2][256][256];
    int16_t diagonal7[4][128][128];
    int16_t diagonal6[4][64][64];
    int16_t diagonal5[4][32][32];
    int16_t edge_2x[4][1024][1024];
    int16_t h_v_2[4][256][256];
    int16_t h_v_3[4][256][256];
    int16_t h_v_4[4][256][256];
    int16_t corner_3x3[4][512][512];
    int16_t edge_x_side[4][1024][1024];
    int16_t edge_block_1[2048][2048];
    int16_t edge_block_2[3][1024][1024];
    int16_t triangle[4][1024][1024];
    int16_t edge_2x5[4][1024][1024];
    int16_t edge_5x2[4][1024][1024];
};
static Pattern_Eval pattern_arr;
static int16_t mobility_arr[36][36];
static int16_t stone_arr[65][65];

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
    
    for (auto &x : pattern_arr.diagonal8) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
//    for (int i = 0; i < 131072; ++i) {
//        fast_next_int(p);
//    }
    p += 131072 * 2;
//    for (auto &x : pattern_arr.diagonal8) {
//        for (auto &y : x) {
//            for (auto &z : y) {
//                fast_next_int(p);
//            }
//        }
//    }
    for (auto &x : pattern_arr.diagonal7) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.diagonal6) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.diagonal5) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.edge_2x) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.h_v_2) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.h_v_3) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.h_v_4) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.corner_3x3) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.edge_x_side) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.edge_block_1) {
        for (auto &y : x) {
            y = fast_next_int(p);
        }
    }
    for (auto &x : pattern_arr.edge_block_2) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.triangle) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.edge_2x5) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
            }
        }
    }
    for (auto &x : pattern_arr.edge_5x2) {
        for (auto &y : x) {
            for (auto &z : y) {
                z = fast_next_int(p);
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


inline int evaluate(const board &b) noexcept {
    
    if (b.p == 0) [[unlikely]] return -32768;
    if (b.o == 0) [[unlikely]] return 32768;
    int plegal = popcnt_u64(makelegalboard(b));
    board b2 = b.flipped();
    int olegal = popcnt_u64(makelegalboard(b2));
    if (plegal == 0 && olegal == 0) return 256 * (popcnt_u64(b.p) - popcnt_u64(b.o));
    int32_t a = 0;
    
    a += (pattern_arr.diagonal8[0][((b.p & 0x8040201008040201ULL) * mn[0][0]) >> 56]
          [((b.o & 0x8040201008040201ULL) * mn[0][0]) >> 56] +
          pattern_arr.diagonal8[1][((b.p & 0x0102040810204080ULL) * mn[0][1]) >> 56]
          [((b.o & 0x0102040810204080ULL) * mn[0][1]) >> 56]);
    
    a += (pattern_arr.diagonal7[0][((b.p & 0x4020100804020100ULL) * mn[1][0]) >> 57]
          [((b.o & 0x4020100804020100ULL) * mn[1][0]) >> 57] +
          pattern_arr.diagonal7[1][((b.p & 0x0001020408102040ULL) * mn[1][1]) >> 57]
          [((b.o & 0x0001020408102040ULL) * mn[1][1]) >> 57] +
          pattern_arr.diagonal7[2][((b.p & 0x0080402010080402ULL) * mn[1][2]) >> 57]
          [((b.o & 0x0080402010080402ULL) * mn[1][2]) >> 57] +
          pattern_arr.diagonal7[3][((b.p & 0x0204081020408000ULL) * mn[1][3]) >> 57]
          [((b.o & 0x0204081020408000ULL) * mn[1][3]) >> 57]);
    
    a += (pattern_arr.diagonal6[0][((b.p & 0x2010080402010000ULL) * mn[2][0]) >> 58]
          [((b.o & 0x2010080402010000ULL) * mn[2][0]) >> 58] +
          pattern_arr.diagonal6[1][((b.p & 0x0000010204081020ULL) * mn[2][1]) >> 58]
          [((b.o & 0x0000010204081020ULL) * mn[2][1]) >> 58] +
          pattern_arr.diagonal6[2][((b.p & 0x0000804020100804ULL) * mn[2][2]) >> 58]
          [((b.o & 0x0000804020100804ULL) * mn[2][2]) >> 58] +
          pattern_arr.diagonal6[3][((b.p & 0x0408102040800000ULL) * mn[2][3]) >> 58]
          [((b.o & 0x0408102040800000ULL) * mn[2][3]) >> 58]);
    
    a += (pattern_arr.diagonal5[0][((b.p & 0x1008040201000000ULL) * mn[3][0]) >> 59]
          [((b.o & 0x1008040201000000ULL) * mn[3][0]) >> 59] +
          pattern_arr.diagonal5[1][((b.p & 0x0000000102040810ULL) * mn[3][1]) >> 59]
          [((b.o & 0x0000000102040810ULL) * mn[3][1]) >> 59] +
          pattern_arr.diagonal5[2][((b.p & 0x0000008040201008ULL) * mn[3][2]) >> 59]
          [((b.o & 0x0000008040201008ULL) * mn[3][2]) >> 59] +
          pattern_arr.diagonal5[3][((b.p & 0x0810204080000000ULL) * mn[3][3]) >> 59]
          [((b.o & 0x0810204080000000ULL) * mn[3][3]) >> 59]);
    
    a += (pattern_arr.edge_2x[0][((b.p & 0xff42000000000000ULL) * mn[4][0]) >> 54]
          [((b.o & 0xff42000000000000ULL) * mn[4][0]) >> 54] +
          pattern_arr.edge_2x[1][((b.p & 0x0103010101010301ULL) * mn[4][1]) >> 54]
          [((b.o & 0x0103010101010301ULL) * mn[4][1]) >> 54] +
          pattern_arr.edge_2x[2][((b.p & 0x00000000000042ffULL) * mn[4][2]) >> 54]
          [((b.o & 0x00000000000042ffULL) * mn[4][2]) >> 54] +
          pattern_arr.edge_2x[3][((b.p & 0x80c080808080c080ULL) * mn[4][3]) >> 54]
          [((b.o & 0x80c080808080c080ULL) * mn[4][3]) >> 54]);
    
    a += (pattern_arr.h_v_2[0][((b.p & 0x00ff000000000000ULL) * mn[5][0]) >> 56]
          [((b.o & 0x00ff000000000000ULL) * mn[5][0]) >> 56] +
          pattern_arr.h_v_2[1][((b.p & 0x0202020202020202ULL) * mn[5][1]) >> 56]
          [((b.o & 0x0202020202020202ULL) * mn[5][1]) >> 56] +
          pattern_arr.h_v_2[2][((b.p & 0x000000000000ff00ULL) * mn[5][2]) >> 56]
          [((b.o & 0x000000000000ff00ULL) * mn[5][2]) >> 56] +
          pattern_arr.h_v_2[3][((b.p & 0x4040404040404040ULL) * mn[5][3]) >> 56]
          [((b.o & 0x4040404040404040ULL) * mn[5][3]) >> 56]);
    
    a += (pattern_arr.h_v_3[0][((b.p & 0x0000ff0000000000ULL) * mn[6][0]) >> 56]
          [((b.o & 0x0000ff0000000000ULL) * mn[6][0]) >> 56] +
          pattern_arr.h_v_3[1][((b.p & 0x0404040404040404ULL) * mn[6][1]) >> 56]
          [((b.o & 0x0404040404040404ULL) * mn[6][1]) >> 56] +
          pattern_arr.h_v_3[2][((b.p & 0x0000000000ff0000ULL) * mn[6][2]) >> 56]
          [((b.o & 0x0000000000ff0000ULL) * mn[6][2]) >> 56] +
          pattern_arr.h_v_3[3][((b.p & 0x2020202020202020ULL) * mn[6][3]) >> 56]
          [((b.o & 0x2020202020202020ULL) * mn[6][3]) >> 56]);
    
    a += (pattern_arr.h_v_4[0][((b.p & 0x000000ff00000000ULL) * mn[7][0]) >> 56]
          [((b.o & 0x000000ff00000000ULL) * mn[7][0]) >> 56] +
          pattern_arr.h_v_4[1][((b.p & 0x0808080808080808ULL) * mn[7][1]) >> 56]
          [((b.o & 0x0808080808080808ULL) * mn[7][1]) >> 56] +
          pattern_arr.h_v_4[2][((b.p & 0x00000000ff000000ULL) * mn[7][2]) >> 56]
          [((b.o & 0x00000000ff000000ULL) * mn[7][2]) >> 56] +
          pattern_arr.h_v_4[3][((b.p & 0x1010101010101010ULL) * mn[7][3]) >> 56]
          [((b.o & 0x1010101010101010ULL) * mn[7][3]) >> 56]);
    
    a += (pattern_arr.corner_3x3[0][((b.p & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55]
          [((b.o & 0xe0e0e00000000000ULL) * mn[8][0]) >> 55] +
          pattern_arr.corner_3x3[1][((b.p & 0x0707070000000000ULL) * mn[8][1]) >> 55]
          [((b.o & 0x0707070000000000ULL) * mn[8][1]) >> 55] +
          pattern_arr.corner_3x3[2][((b.p & 0x0000000000070707ULL) * mn[8][2]) >> 55]
          [((b.o & 0x0000000000070707ULL) * mn[8][2]) >> 55] +
          pattern_arr.corner_3x3[3][((b.p & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55]
          [((b.o & 0x0000000000e0e0e0ULL) * mn[8][3]) >> 55]);
    
    a += (pattern_arr.edge_x_side[0][((b.p & 0xf8c0808080000000ULL) * mn[9][0]) >> 54]
          [((b.o & 0xf8c0808080000000ULL) * mn[9][0]) >> 54] +
          pattern_arr.edge_x_side[1][((b.p & 0x1f03010101000000ULL) * mn[9][1]) >> 54]
          [((b.o & 0x1f03010101000000ULL) * mn[9][1]) >> 54] +
          pattern_arr.edge_x_side[2][((b.p & 0x000000010101031fULL) * mn[9][2]) >> 54]
          [((b.o & 0x000000010101031fULL) * mn[9][2]) >> 54] +
          pattern_arr.edge_x_side[3][((b.p & 0x000000808080c0f8ULL) * mn[9][3]) >> 54]
          [((b.o & 0x000000808080c0f8ULL) * mn[9][3]) >> 54]);
    
    a += (pattern_arr.edge_block_1[((b.p & 0xbd3c000000000000ULL) * mn[10][0]) >> 53]
          [((b.o & 0xbd3c000000000000ULL) * mn[10][0]) >> 53] +
          pattern_arr.edge_block_2[0][((b.p & 0x0100030303030001ULL) * mn[10][1]) >> 54]
          [((b.o & 0x0100030303030001ULL) * mn[10][1]) >> 54] +
          pattern_arr.edge_block_2[1][((b.p & 0x0000000000003cbdULL) * mn[10][2]) >> 54]
          [((b.o & 0x0000000000003cbdULL) * mn[10][2]) >> 54] +
          pattern_arr.edge_block_2[2][((b.p & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54]
          [((b.o & 0x8000c0c0c0c00080ULL) * mn[10][3]) >> 54]);
    
    a += (pattern_arr.triangle[0][((b.p & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54]
          [((b.o & 0xf0e0c08000000000ULL) * mn[11][0]) >> 54] +
          pattern_arr.triangle[1][((b.p & 0x0f07030100000000ULL) * mn[11][1]) >> 54]
          [((b.o & 0x0f07030100000000ULL) * mn[11][1]) >> 54] +
          pattern_arr.triangle[2][((b.p & 0x000000000103070fULL) * mn[11][2]) >> 54]
          [((b.o & 0x000000000103070fULL) * mn[11][2]) >> 54] +
          pattern_arr.triangle[3][((b.p & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54]
          [((b.o & 0x0000000080c0e0f0ULL) * mn[11][3]) >> 54]);
    
    a += (pattern_arr.edge_2x5[0][((b.p & 0xF8F8000000000000ULL) * mn[12][0]) >> 54]
          [((b.o & 0xF8F8000000000000ULL) * mn[12][0]) >> 54] +
          pattern_arr.edge_2x5[1][((b.p & 0x0303030303000000ULL) * mn[12][1]) >> 54]
          [((b.o & 0x0303030303000000ULL) * mn[12][1]) >> 54] +
          pattern_arr.edge_2x5[2][((b.p & 0x0000000000001f1fULL) * mn[12][2]) >> 54]
          [((b.o & 0x0000000000001f1fULL) * mn[12][2]) >> 54] +
          pattern_arr.edge_2x5[3][((b.p & 0x000000c0c0c0c0c0ULL) * mn[12][3]) >> 54]
          [((b.o & 0x000000c0c0c0c0c0ULL) * mn[12][3]) >> 54]);
    
    a += (pattern_arr.edge_5x2[0][((b.p & 0xC0C0C0C0C0000000ULL) * mn[13][0]) >> 54]
          [((b.o & 0xC0C0C0C0C0000000ULL) * mn[13][0]) >> 54] +
          pattern_arr.edge_5x2[1][((b.p & 0x1f1f000000000000ULL) * mn[13][1]) >> 54]
          [((b.o & 0x1f1f000000000000ULL) * mn[13][1]) >> 54] +
          pattern_arr.edge_5x2[2][((b.p & 0x0000000303030303ULL) * mn[13][2]) >> 54]
          [((b.o & 0x0000000303030303ULL) * mn[13][2]) >> 54] +
          pattern_arr.edge_5x2[3][((b.p & 0x000000000000f8f8ULL) * mn[13][3]) >> 54]
          [((b.o & 0x000000000000f8f8ULL) * mn[13][3]) >> 54]);
    
    a += mobility_arr[plegal][olegal];
    
    a += stone_arr[popcnt_u64(b.p)][popcnt_u64(b.o)];
    
//    return max(-16383, min(16383, a));
    return a;
}
