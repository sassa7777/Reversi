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

#elif defined(__aarch64__)

#include <arm_acle.h>
#define popcnt_u64(x) (int)__builtin_popcountll(x)
#define clz_u64(x) (int)__builtin_arm_clz64(x)

#else

#define popcnt_u64(x) std::poppcount(x)
#define clz_u64(x) std::countl_zero(x)

#endif

#ifdef __GNUC__
#define flipVertical(x) __builtin_bswap64(x)
#else
#define flipVertical(x) _byteswap_uint64(x)
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
    return flipVertical(flipDiagonalA1H8(x));
}

inline constexpr uint64_t r90(uint64_t x) {
    return flipDiagonalA1H8(flipVertical(x));
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

#endif /* bit_h */
