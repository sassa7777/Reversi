//
//  book.hpp
//  Reversi
//
//  Created by sasa on 2024/11/23.
//

#ifndef book_h
#define book_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <ankerl/unordered_dense.h>

using namespace std;

multimap<pair<uint64_t, uint64_t>, uint64_t> book;
ankerl::unordered_dense::map<string, uint64_t> play_record_to_put;

void play_record_to_coordinate_init() {
    play_record_to_put.clear();
    string put = "aa";
    constexpr char x[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    constexpr char y[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            put[0] = x[i];
            put[1] = y[j];
            play_record_to_put[put] = cordinate_to_bit(i, j);
        }
    }
        
}

void put_book_init(uint64_t put, uint64_t &p, uint64_t &o) {
    uint64_t rev = Flip(put, p, o);
    p ^= (rev | put);
    o ^= rev;
    swap(p, o);
}

void book_init() {
    book.clear();
    play_record_to_coordinate_init();
    fstream ifs(FileSystem::RelativePath(Resource(U"book.txt")).narrow());
    if (ifs.fail()){
        cerr << "book file does not exist" << endl;
        exit(1);
    }
    string line;
    uint64_t p = 0x0000000810000000ULL;
    uint64_t o = 0x0000001008000000ULL;
    uint64_t put;
    int n = 0;
    while (true) {
        p = 0x0000000810000000ULL;
        o = 0x0000001008000000ULL;
        getline(ifs, line);
        if (ifs.eof()) break;
        while (line.size() > 2) {
            put_book_init(play_record_to_put.at(line.substr(0, 2)), p, o);
            line.erase(0, 2);
        }
        put = play_record_to_put.at(line.substr(0, 2));
        book.emplace(make_pair(p, o), put);
        book.emplace(make_pair(rotateClockwise90(p), rotateClockwise90(o)),
                     rotateClockwise90(put));
        book.emplace(make_pair(rotateClockwise90(rotateClockwise90(p)), rotateClockwise90(rotateClockwise90(o))), rotateClockwise90(rotateClockwise90(put)));
        book.emplace(make_pair(rotateCounterclockwise90(p), rotateCounterclockwise90(o)), rotateCounterclockwise90((put)));
        book.emplace(make_pair(flipDiagonalA1H8(p), flipDiagonalA1H8(o)), flipDiagonalA1H8(put));
        book.emplace(make_pair(flipDiagonalA1H8(rotateClockwise90(p)), flipDiagonalA1H8(rotateClockwise90(o))), flipDiagonalA1H8(rotateClockwise90(put)));
        book.emplace(make_pair(flipDiagonalA1H8(rotateClockwise90(rotateClockwise90(p))), flipDiagonalA1H8(rotateClockwise90(rotateClockwise90(o)))), flipDiagonalA1H8(rotateClockwise90(rotateClockwise90(put))));
        book.emplace(make_pair(flipDiagonalA1H8(rotateCounterclockwise90(p)), flipDiagonalA1H8(rotateCounterclockwise90(o))), flipDiagonalA1H8(rotateCounterclockwise90(put)));
        book.emplace(make_pair(flipDiagonalA8H1(p), flipDiagonalA8H1(o)), flipDiagonalA8H1(put));
        book.emplace(make_pair(flipDiagonalA8H1(rotateClockwise90(p)), flipDiagonalA8H1(rotateClockwise90(o))), flipDiagonalA8H1(rotateClockwise90(put)));
        book.emplace(make_pair(flipDiagonalA8H1(rotateClockwise90(rotateClockwise90(p))), flipDiagonalA8H1(rotateClockwise90(rotateClockwise90(o)))), flipDiagonalA8H1(rotateClockwise90(rotateClockwise90(put))));
        book.emplace(make_pair(flipDiagonalA8H1(rotateCounterclockwise90(p)), flipDiagonalA8H1(rotateCounterclockwise90(o))), flipDiagonalA8H1(rotateCounterclockwise90(put)));
        ++n;
    }
    cout << "book initialized " << n << endl;
}

#endif /* book_h */
