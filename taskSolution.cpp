#include <iostream>
#include <vector>
#include <numeric>
#include <cstdio>


using namespace std;

int main(int argc, char *argv[]) {

    string stringLine = argv[1];
    char letter = argv[2][0];

    int sums = accumulate(stringLine.begin(), stringLine.begin() + stringLine.size(),
                             0, [&letter](size_t a, char b) {
            return (b > letter) ? ++a : a;
        });

    return sums;
}
