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

#endif /* bit_h */
