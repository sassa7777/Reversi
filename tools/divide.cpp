#include <bits/stdc++.h>

using namespace std;

int main() {
    int num = 0;
    string kifu;
    for (int i = 0; i < 200; ++i) {
        ifstream ifs("self_play/" + format("{:0>7}", to_string(i)) + ".txt");
        while (getline(ifs, kifu)) {
            ofstream out("self_play2/" + format("{:0>7}", to_string(num)) + ".txt");
            if (kifu.length() < 120) continue;
            out << kifu << "\n";
            out.close();
            printf("out : %d\n", num);
            ++num;
        }
        ifs.close();
    }
    printf("out : %d\n", num);
}
