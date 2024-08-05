//
//  cow.h
//  Reversi
//
//  Created by sasa on 8/5/24.
//

#ifndef cow_h
#define cow_h

#include <ankerl/unordered_dense.h>
#include <cstdint>

const ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, uint64_t> book_usi = {
    {std::make_pair(0x1000000000, 0x818080000), 0x40000},
    {std::make_pair(0x810080000, 0x1008040000), 0x4000000},
    {std::make_pair(0x1000040000, 0x81C080000), 0x100000},
    {std::make_pair(0x80C000000, 0x1011C00000), 0x20000000}
};

const ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, uint64_t> book_kaisoku_senn = {
    {std::make_pair(0x10001C0000, 0x83C000000), 0x80000000000},
    {std::make_pair(0x34000000, 0x81881C00000), 0x100000000000},
    {std::make_pair(0x34000000, 0x81881C00000), 0x100000000000},
};

const ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, uint64_t> book_sennsya = {
    {std::make_pair(0x34000000, 0x81881C00000), 0x40000000000},
    {std::make_pair(0x810081C0000, 0x40834000000), 0x400000000},
    {std::make_pair(0x30000000, 0x1C0C1C0000), 0x40000000000},
    {std::make_pair(0x140C1C0000, 0x40830000000), 0x80000000000},
    {std::make_pair(0x40030000000, 0x81C0C1C0000), 0x2000000},
    {std::make_pair(0x81C001C0000, 0x4003E000000), 0x20000}
};

const ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, uint64_t> book_ura_koumori = {
    {std::make_pair(0x1010140000, 0x80C040800), 0x2000000},
    {std::make_pair(0x800080800, 0x101E140000), 0x20000000}
};

const ankerl::unordered_dense::map<std::pair<uint64_t, uint64_t>, uint64_t> book_ura_yotto = {
    {std::make_pair(0x800080800, 0x101E140000), 0x20000},
    {std::make_pair(0x101A100000, 0x8040E0800), 0x400000000}
};


#endif /* cow_h */
