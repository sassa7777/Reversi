//
//  tiger.h
//  Reversi
//
//  Created by sasa on 8/5/24.
//

#ifndef tiger_h
#define tiger_h

#include <ankerl/unordered_dense.h>
#include <cstdint>

const ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, uint64_t> book_tiger = {
    {std::make_pair(0x1000000000, 0x818080000), 0x400000000},
    {std::make_pair(0x18080000, 0x1C00000000), 0x200000000000},
    {std::make_pair(0xC00000000, 0x201018080000), 0x2000000000},
    {std::make_pair(0x200018080000, 0x3C00000000), 0x100000000000}
};

const ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, uint64_t> book_stevenson = {
    {std::make_pair(0x2C00000000, 0x301018080000), 0x100000},
    {std::make_pair(0x301010080000, 0x2C08100000), 0x40000}
};

const ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, uint64_t> book_conpos = {
    {std::make_pair(0x2C00100000, 0x3010180C0000), 0x200000},
    {std::make_pair(0x3010080C0000, 0x2C10300000), 0x4000000},
    {std::make_pair(0x2410300000, 0x30180C0C0000), 0x2000000},
    {std::make_pair(0x3018000C0000, 0x241E300000), 0x200000000}
};

const ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, uint64_t> book_FATDraw = {
    {std::make_pair(0x201A300000, 0x301E040C0000), 0x400},
    {std::make_pair(0x301E04040000, 0x201A380400), 0x10000},
    {std::make_pair(0x2018380400, 0x301E06050000), 0x20000},
    {std::make_pair(0x301E06010000, 0x20183E0400), 0x800},
    {std::make_pair(0x2010320400, 0x301E0E0D0800), 0x4},
    {std::make_pair(0x301E0E0D0000, 0x2010320C04), 0x10},
};

#endif /* tiger_h */
