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
#define ctz_u64(x) (int)_tzcnt_u64(x)
#define crc32c_u64(crc,d) _mm_crc32_u64((crc),(d))

#elif defined(__aarch64__)

#include <arm_acle.h>
#define popcnt_u64(x) (int)__builtin_popcountll(x)
#define clz_u64(x) (int)__builtin_arm_clz64(x)
#define ctz_u64(x) (int)__builtin_ctzll(x)
#define crc32c_u64(crc,d) __crc32cd((crc),(d))

#else

#define popcnt_u64(x) std::poppcount(x)
#define clz_u64(x) std::countl_zero(x)
#define ctz_u64(x) std::countr_zero(x)

#endif

#ifdef __GNUC__
#define VerticalMirror(x) __builtin_bswap64(x)
#else
#define VerticalMirror(x) _byteswap_uint64(x)
#endif

inline uint64_t delta_swap(uint64_t x, uint64_t mask, int delta) {
    uint64_t t = (x ^ (x >> delta)) & mask;
    return x ^ t ^ (t << delta);
}

inline uint64_t flipHorizontal(uint64_t x) {
    x = ((x >> 1) & 0x5555555555555555) | ((x & 0x5555555555555555) << 1);
    x = ((x >> 2) & 0x3333333333333333) | ((x & 0x3333333333333333) << 2);
    x = ((x >> 4) & 0x0f0f0f0f0f0f0f0f) | ((x & 0x0f0f0f0f0f0f0f0f) << 4);
    return x;
}

inline uint64_t flipDiagonalA1H8(uint64_t x) {
    x = delta_swap(x, 0x00AA00AA00AA00AA, 7);
    x = delta_swap(x, 0x0000CCCC0000CCCC, 14);
    return delta_swap(x, 0x00000000F0F0F0F0, 28);
}

inline uint64_t flipDiagonalA8H1(uint64_t x) {
    x = delta_swap(x, 0x0055005500550055, 9);
    x = delta_swap(x, 0x0000333300003333, 18);
    return delta_swap(x, 0x000000000F0F0F0F, 36);
}

inline uint64_t l90(uint64_t x) {
    return VerticalMirror(flipDiagonalA1H8(x));
}

inline uint64_t r90(uint64_t x) {
    return flipDiagonalA1H8(VerticalMirror(x));
}
#ifdef __aarch64__
#define r180(x) __builtin_arm_rbit64(x)
#elif defined __clang__
#define r180(x) __builtin_bitreverse64(x)
#else
inline uint64_t r180(uint64_t x) {
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

namespace INDEX {
constexpr uint64_t A1 = 1ULL << 63;
constexpr uint64_t B1 = 1ULL << 62;
constexpr uint64_t C1 = 1ULL << 61;
constexpr uint64_t D1 = 1ULL << 60;
constexpr uint64_t E1 = 1ULL << 59;
constexpr uint64_t F1 = 1ULL << 58;
constexpr uint64_t G1 = 1ULL << 57;
constexpr uint64_t H1 = 1ULL << 56;

constexpr uint64_t A2 = 1ULL << 55;
constexpr uint64_t B2 = 1ULL << 54;
constexpr uint64_t C2 = 1ULL << 53;
constexpr uint64_t D2 = 1ULL << 52;
constexpr uint64_t E2 = 1ULL << 51;
constexpr uint64_t F2 = 1ULL << 50;
constexpr uint64_t G2 = 1ULL << 49;
constexpr uint64_t H2 = 1ULL << 48;

constexpr uint64_t A3 = 1ULL << 47;
constexpr uint64_t B3 = 1ULL << 46;
constexpr uint64_t C3 = 1ULL << 45;
constexpr uint64_t D3 = 1ULL << 44;
constexpr uint64_t E3 = 1ULL << 43;
constexpr uint64_t F3 = 1ULL << 42;
constexpr uint64_t G3 = 1ULL << 41;
constexpr uint64_t H3 = 1ULL << 40;

constexpr uint64_t A4 = 1ULL << 39;
constexpr uint64_t B4 = 1ULL << 38;
constexpr uint64_t C4 = 1ULL << 37;
constexpr uint64_t D4 = 1ULL << 36;
constexpr uint64_t E4 = 1ULL << 35;
constexpr uint64_t F4 = 1ULL << 34;
constexpr uint64_t G4 = 1ULL << 33;
constexpr uint64_t H4 = 1ULL << 32;

constexpr uint64_t A5 = 1ULL << 31;
constexpr uint64_t B5 = 1ULL << 30;
constexpr uint64_t C5 = 1ULL << 29;
constexpr uint64_t D5 = 1ULL << 28;
constexpr uint64_t E5 = 1ULL << 27;
constexpr uint64_t F5 = 1ULL << 26;
constexpr uint64_t G5 = 1ULL << 25;
constexpr uint64_t H5 = 1ULL << 24;

constexpr uint64_t A6 = 1ULL << 23;
constexpr uint64_t B6 = 1ULL << 22;
constexpr uint64_t C6 = 1ULL << 21;
constexpr uint64_t D6 = 1ULL << 20;
constexpr uint64_t E6 = 1ULL << 19;
constexpr uint64_t F6 = 1ULL << 18;
constexpr uint64_t G6 = 1ULL << 17;
constexpr uint64_t H6 = 1ULL << 16;

constexpr uint64_t A7 = 1ULL << 15;
constexpr uint64_t B7 = 1ULL << 14;
constexpr uint64_t C7 = 1ULL << 13;
constexpr uint64_t D7 = 1ULL << 12;
constexpr uint64_t E7 = 1ULL << 11;
constexpr uint64_t F7 = 1ULL << 10;
constexpr uint64_t G7 = 1ULL <<  9;
constexpr uint64_t H7 = 1ULL <<  8;

constexpr uint64_t A8 = 1ULL <<  7;
constexpr uint64_t B8 = 1ULL <<  6;
constexpr uint64_t C8 = 1ULL <<  5;
constexpr uint64_t D8 = 1ULL <<  4;
constexpr uint64_t E8 = 1ULL <<  3;
constexpr uint64_t F8 = 1ULL <<  2;
constexpr uint64_t G8 = 1ULL <<  1;
constexpr uint64_t H8 = 1ULL <<  0;
};

#endif /* bit_h */
