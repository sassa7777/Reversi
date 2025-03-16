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

ankerl::unordered_dense::map<pair<uint64_t, uint64_t>, int> book;

void book_init() {
    book.clear();
    fstream ifs(FileSystem::RelativePath(Resource(U"book.txt")).narrow());
    if (ifs.fail()){
        cerr << "book file does not exist" << endl;
        exit(1);
    }
    string line;
    uint64_t p, o;
    int ans;
    while (getline(ifs, line)) {
        istringstream stream(line);
        string token;
        stream >> token;
        p = stoull(token);
        stream >> token;
        o = stoull(token);
        stream >> token;
        ans = stoi(token);
        book[{p, o}] = ans;
        book[{r180(p), r180(o)}] = ans;
        book[{flipDiagonalA1H8(p), flipDiagonalA1H8(o)}] = ans;
        book[{flipDiagonalA8H1(p), flipDiagonalA8H1(o)}] = ans;
//        book[{o, p}] = -ans;
//        book[{r180(o), r180(p)}] = -ans;
//        book[{flipDiagonalA1H8(o), flipDiagonalA1H8(p)}] = -ans;
//        book[{flipDiagonalA8H1(o), flipDiagonalA8H1(p)}] = -ans;
    }
    cout << "book initialized" << endl;
}

#endif /* book_h */
