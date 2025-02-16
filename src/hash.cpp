//
//  hash.cpp
//  Reversi
//
//  Created by sasa on 2025/02/16.
//
#include "hash.hpp"
#include "reversi.hpp"

constexpr uint64_t hash_num[64] = {
    0x903fe0b841020fd6,
    0x4e1917773cc701b5,
    0xebd3b7a50bc3488,
    0x357d9c5c90bd2814,
    0xc74744cc1823e148,
    0x71bc181e0b74d12f,
    0xa10ec6b6c541441d,
    0xc11c2d9b11953cfc,
    0x974c59f95ae912f3,
    0x4271335fd9890407,
    0xb4712c4a81aa030c,
    0x58118bc2433ed67f,
    0xcbbb18cc8c007b35,
    0xa01d1ef58b23c366,
    0xede9a942cb0a22cd,
    0xeeb9542212458dc9,
    0x99baa09a34eacfa1,
    0xad1d89b70b6c1c4,
    0xe62e4c877e0aa8e8,
    0x7196c729e81852f0,
    0x7f5d3550b3194567,
    0xc58d862a525ca9bf,
    0x7f30396f966ae2bc,
    0xa5b58069490358a2,
    0x26a68b3ade6d7f4f,
    0xe8adb3f9a39e8643,
    0xb63a912f6ad49cda,
    0x1eb782a369c1f415,
    0x93df46a49079d40d,
    0x447860d924491630,
    0xdd56ffa70e048c7f,
    0xeb9fa73cee74deaa,
    0xd26f9f098a6dcfef,
    0xb23c84bbed3ef19b,
    0x4f6019d2a07b0118,
    0x76bd58899c979598,
    0x6fa41edbd5015533,
    0xb039d339e72f2023,
    0x56b69f22468cf6c7,
    0xfb7d0caab4b0730e,
    0xf37ea2f5edc48ad3,
    0x3e44d52e9634992b,
    0xe514a6f3acb4c552,
    0xc69902ecab4f779d,
    0x2d81704b179d1f47,
    0xe1dacf0c2947389a,
    0xf5b48f8fc212ff7e,
    0x842ffa2087ec19aa,
    0xd90157a3edab58fb,
    0xa62d34f28efad9cb,
    0x7d1555853b8af85f,
    0xd78ae3177d03c283,
    0x6a36d9dab9f306af,
    0x526d858523088fbb,
    0x4eb122339dbc9f01,
    0x9a047d28a3293a1a,
    0x56c4a4f3e274f99b,
    0xebba447f121df133,
    0xb1488abff0457029,
    0x18ee451716f87fdf,
    0x4e3fc0732807c385,
    0xfc8cb7bbb4577318,
    0xcd5072fae2f6646a,
    0xd60c3db19c3d53d0
};

inline uint64_t caloc_hash(uint64_t put, uint64_t hash) {
    return hash ^ hash_num[std::countl_zero(put)];
}
