//
//  bit.h
//  Reversi
//
//  Created by sasa on 2025/03/05.
//

#ifndef bit_h
#define bit_h

#ifdef __x86_64__

#include <x86intrin.h>
#define popcnt_u64(x) (int)_popcnt64(x)
#define clz_u64(x) (int)_lzcnt_u64(x)
#define crc32c_u64(crc,d) _mm_crc32_u64((crc),(d))

#elif defined(__aarch64__)

#include <arm_acle.h>
#define popcnt_u64(x) (int)__builtin_popcountll(x)
#define clz_u64(x) (int)__builtin_arm_clz64(x)
#define crc32c_u64(crc,d) __crc32cd((crc),(d))

#else

#define popcnt_u64(x) std::poppcount(x)
#define clz_u64(x) std::countl_zero(x)

#endif

#ifdef __GNUC__
#define VerticalMirror(x) __builtin_bswap64(x)
#else
#define VerticalMirror(x) _byteswap_uint64(x)
#endif

inline constexpr uint64_t delta_swap(uint64_t x, uint64_t mask, int delta) {
    uint64_t t = (x ^ (x >> delta)) & mask;
    return x ^ t ^ (t << delta);
}

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
    return VerticalMirror(flipDiagonalA1H8(x));
}

inline constexpr uint64_t r90(uint64_t x) {
    return flipDiagonalA1H8(VerticalMirror(x));
}
#ifdef __aarch64__
#define r180(x) __builtin_arm_rbit64(x)
#elif defined __clang__
#define r180(x) __builtin_bitreverse64(x)
#else
inline constexpr uint64_t r180(uint64_t x) {
    return flipVertical(flipHorizontal(x));
}
#endif

#ifdef __BMI__
#define blsr_u64(x) _blsr_u64(x)
#define blsi_u64(x) _blsi_u64(x)
#else
#define blsr_u64(x) (x & (x - 1))
#define blsi_u64(x) (x & -x)
#endif

constexpr uint64_t INDEX_A1 = 1ULL << 63;
constexpr uint64_t INDEX_B1 = 1ULL << 62;
constexpr uint64_t INDEX_C1 = 1ULL << 61;
constexpr uint64_t INDEX_D1 = 1ULL << 60;
constexpr uint64_t INDEX_E1 = 1ULL << 59;
constexpr uint64_t INDEX_F1 = 1ULL << 58;
constexpr uint64_t INDEX_G1 = 1ULL << 57;
constexpr uint64_t INDEX_H1 = 1ULL << 56;

constexpr uint64_t INDEX_A2 = 1ULL << 55;
constexpr uint64_t INDEX_B2 = 1ULL << 54;
constexpr uint64_t INDEX_C2 = 1ULL << 53;
constexpr uint64_t INDEX_D2 = 1ULL << 52;
constexpr uint64_t INDEX_E2 = 1ULL << 51;
constexpr uint64_t INDEX_F2 = 1ULL << 50;
constexpr uint64_t INDEX_G2 = 1ULL << 49;
constexpr uint64_t INDEX_H2 = 1ULL << 48;

constexpr uint64_t INDEX_A3 = 1ULL << 47;
constexpr uint64_t INDEX_B3 = 1ULL << 46;
constexpr uint64_t INDEX_C3 = 1ULL << 45;
constexpr uint64_t INDEX_D3 = 1ULL << 44;
constexpr uint64_t INDEX_E3 = 1ULL << 43;
constexpr uint64_t INDEX_F3 = 1ULL << 42;
constexpr uint64_t INDEX_G3 = 1ULL << 41;
constexpr uint64_t INDEX_H3 = 1ULL << 40;

constexpr uint64_t INDEX_A4 = 1ULL << 39;
constexpr uint64_t INDEX_B4 = 1ULL << 38;
constexpr uint64_t INDEX_C4 = 1ULL << 37;
constexpr uint64_t INDEX_D4 = 1ULL << 36;
constexpr uint64_t INDEX_E4 = 1ULL << 35;
constexpr uint64_t INDEX_F4 = 1ULL << 34;
constexpr uint64_t INDEX_G4 = 1ULL << 33;
constexpr uint64_t INDEX_H4 = 1ULL << 32;

constexpr uint64_t INDEX_A5 = 1ULL << 31;
constexpr uint64_t INDEX_B5 = 1ULL << 30;
constexpr uint64_t INDEX_C5 = 1ULL << 29;
constexpr uint64_t INDEX_D5 = 1ULL << 28;
constexpr uint64_t INDEX_E5 = 1ULL << 27;
constexpr uint64_t INDEX_F5 = 1ULL << 26;
constexpr uint64_t INDEX_G5 = 1ULL << 25;
constexpr uint64_t INDEX_H5 = 1ULL << 24;

constexpr uint64_t INDEX_A6 = 1ULL << 23;
constexpr uint64_t INDEX_B6 = 1ULL << 22;
constexpr uint64_t INDEX_C6 = 1ULL << 21;
constexpr uint64_t INDEX_D6 = 1ULL << 20;
constexpr uint64_t INDEX_E6 = 1ULL << 19;
constexpr uint64_t INDEX_F6 = 1ULL << 18;
constexpr uint64_t INDEX_G6 = 1ULL << 17;
constexpr uint64_t INDEX_H6 = 1ULL << 16;

constexpr uint64_t INDEX_A7 = 1ULL << 15;
constexpr uint64_t INDEX_B7 = 1ULL << 14;
constexpr uint64_t INDEX_C7 = 1ULL << 13;
constexpr uint64_t INDEX_D7 = 1ULL << 12;
constexpr uint64_t INDEX_E7 = 1ULL << 11;
constexpr uint64_t INDEX_F7 = 1ULL << 10;
constexpr uint64_t INDEX_G7 = 1ULL <<  9;
constexpr uint64_t INDEX_H7 = 1ULL <<  8;

constexpr uint64_t INDEX_A8 = 1ULL <<  7;
constexpr uint64_t INDEX_B8 = 1ULL <<  6;
constexpr uint64_t INDEX_C8 = 1ULL <<  5;
constexpr uint64_t INDEX_D8 = 1ULL <<  4;
constexpr uint64_t INDEX_E8 = 1ULL <<  3;
constexpr uint64_t INDEX_F8 = 1ULL <<  2;
constexpr uint64_t INDEX_G8 = 1ULL <<  1;
constexpr uint64_t INDEX_H8 = 1ULL <<  0;

#endif /* bit_h */
