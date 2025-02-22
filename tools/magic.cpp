#include <bits/stdc++.h>

using namespace std;

uint32_t rondom_num = 1804289383;

uint32_t get_romdom_32() {
    uint32_t number = rondom_num;
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    
    rondom_num = number;
    
    return number;
}

uint64_t get_random_64() {
    uint64_t n1, n2, n3, n4;
    n1 = (uint64_t)(get_romdom_32()) & 0xFFFF;
    n2 = (uint64_t)(get_romdom_32()) & 0xFFFF;
    n3 = (uint64_t)(get_romdom_32()) & 0xFFFF;
    n4 = (uint64_t)(get_romdom_32()) & 0xFFFF;
    
    return (n1 << 48) | (n2 << 32) | (n3 << 16) | n4;
}

uint64_t get_magic_num() {
    return get_random_64() & get_random_64() & get_random_64();
}

bool is_magic_number(uint64_t number, vector<uint64_t> &masks, size_t shift, size_t pop) {
    vector<bool> A(1 << pop, false);
    uint64_t idx;
    for (size_t i = 0, j = masks.size(); i < j; ++i) {
        idx = ((masks[i] * number) >> (shift));
        if (A.at(idx)) {
            return false;
        } else {
            A.at(idx) = true;
        }
    }
    return true;
}

vector<uint64_t> list_masks(uint64_t mask) {
    vector<uint64_t> masks;
    std::vector<int> positions;
    for (int i = 0; i < 64; ++i) {
        if (mask & (1ULL << i)) {
            positions.push_back(i);
        }
    }
    
    size_t pops = positions.size();
    
    for (size_t comb = 0; comb < (1ULL << pops); ++comb) {
        uint64_t newpattern = mask;
        for (size_t i = 0; i < pops; ++i) {
            if (comb & (1ULL << i)) {
                newpattern ^= (1ULL << positions[i]);
            }
        }
        masks.emplace_back(newpattern);
    }
    printf("%d\n", masks.size());
    return masks;
}

void solve(uint64_t mask, int comp) {
    size_t shift = 64 - popcount(mask) - comp;
    vector<uint64_t> masks = list_masks(mask);
    for (uint64_t i = 0; i < 0xFFFFFFFFFFFFFFFF; ++i) {
        uint64_t magic = get_magic_num();
        if (is_magic_number(magic, masks, shift, 64-shift)) {
            cout << "Magic number: 0x" << hex << magic << endl;
            return;
        }
    }
}

int main() {
//    cout << "diagonal8_idx" << endl;
//    solve(0x8040201008040201ULL, 0);
//    cout << endl;
//    solve(0x0102040810204080ULL, 0);
//    cout << endl;
//    cout << "diagonal7_idx" << endl;
//    solve(0x4020100804020100, 0);
//    cout << endl;
//    solve(0x0001020408102040, 0);
//    cout << endl;
//    solve(0x0080402010080402, 0);
//    cout << endl;
//    solve(0x0204081020408000, 0);
//    cout << endl;
//    cout << "diagonal6_idx" << endl;
//    solve(0x2010080402010000, 0);
//    cout << endl;
//    solve(0x0000010204081020, 0);
//    cout << endl;
//    solve(0x0000804020100804, 0);
//    cout << endl;
//    solve(0x0408102040800000, 0);
//    cout << endl;
//    cout << "diagonal5_idx" << endl;
//    solve(0x1008040201000000, 0);
//    cout << endl;
//    solve(0x0000000102040810, 0);
//    cout << endl;
//    solve(0x0000008040201008, 0);
//    cout << endl;
//    solve(0x0810204080000000, 0);
//    cout << endl;
//    cout << "edge_2x_idx" << endl;
//    solve(0xff42000000000000, 0);
//    cout << endl;
//    solve(0x0103010101010301, 0);
//    cout << endl;
//    solve(0x00000000000042ff, 0);
//    cout << endl;
//    solve(0x80c080808080c080, 0);
//    cout << endl;
//    cout << "h_v_2_idx" << endl;
//    solve(0x00ff000000000000, 0);
//    cout << endl;
//    solve(0x0202020202020202, 0);
//    cout << endl;
//    solve(0x000000000000ff00, 0);
//    cout << endl;
//    solve(0x4040404040404040, 0);
//    cout << endl;
//    cout << "h_v_3_idx" << endl;
//    solve(0x0000ff0000000000, 0);
//    cout << endl;
//    solve(0x0404040404040404, 0);
//    cout << endl;
//    solve(0x0000000000ff0000, 0);
//    cout << endl;
//    solve(0x2020202020202020, 0);
//    cout << endl;
//    cout << "h_v_4_idx" << endl;
//    solve(0x000000ff00000000, 0);
//    cout << endl;
//    solve(0x0808080808080808, 0);
//    cout << endl;
//    solve(0x00000000ff000000, 0);
//    cout << endl;
//    solve(0x1010101010101010, 0);
//    cout << endl;
//    cout << "corner_3x3_idx" << endl;
//    solve(0xe0e0e00000000000, 0);
//    cout << endl;
//    solve(0x0707070000000000, 0);
//    cout << endl;
//    solve(0x0000000000070707, 0);
//    cout << endl;
//    solve(0x0000000000e0e0e0, 0);
//    cout << endl;
//    cout << "edge_x_side_idx" << endl;
//    solve(0xf8c0808080000000, 0);
//    cout << endl;
//    solve(0x1f03010101000000, 0);
//    cout << endl;
//    solve(0x000000010101031f, 0);
//    cout << endl;
//    solve(551911735544, 0);
//    cout << endl;
    cout << "edge_block_idx" << endl;
    solve(0xbd3c000000000000, 1);
    cout << endl;
    solve(0x0100030303030001, 0);
    cout << endl;
    solve(0x0000000000003cbd, 0);
    cout << endl;
    solve(0x8000c0c0c0c00080, 0);
    cout << endl;
//    cout << "triangle_idx" << endl;
//    solve(0xf0e0c08000000000, 0);
//    cout << endl;
//    solve(0x0f07030100000000, 0);
//    cout << endl;
//    solve(16975631, 0);
//    cout << endl;
//    solve(0x0000000080c0e0f0, 0);
//    cout << endl;
//    solve(16975631, 0);
    return 0;
}
