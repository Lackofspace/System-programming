#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cstring>
#include <unistd.h>
#include <wait.h>

using namespace std;

string read_string_from_file(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Failed to open file: " << filename << endl;
        exit(1);
    }
    string line;
    getline(file, line);
    string letters;

    for (char c : line) {
        if (isalpha(c)) {
            letters.push_back(tolower(c));
        }
    }
    return letters;
}

int func(string stringLine, size_t start, size_t end, char letter){
    int sum = accumulate(stringLine.begin() + start, stringLine.begin() + end,
                            0, [&letter](size_t a, char b) {
                return (b > letter) ? ++a : a;
            });
    return(sum);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <filename> <N> <letter>" << endl;
        return 1;
    }
    string filename = argv[1];
    string stringLine = read_string_from_file(filename);
    if (stringLine.size() < 2) {
        cerr << "Not enough letters in file: " << filename << endl;
        return 1;
    }

    int N = atoi(argv[2]);
    char letter = tolower(argv[3][0]);
    vector<pid_t> children(N);
    vector<int> sums(N);
    size_t chunk_size = stringLine.size() / N;
    size_t rest = stringLine.size() % N;

    for (size_t i = 0; i < N; ++i) {
        size_t start = i * chunk_size + min(i, rest);
        size_t end = start + chunk_size + (i < rest);
        pid_t pid = fork();
        switch(pid){
            case 0:
                sums[i] = func(stringLine, start, end, letter);
                exit(sums[i]);
            case -1:
                printf("Error %d\n", errno);
                exit(1);
            default:
                children[i] = pid;
        }
    }

    int result = 0;
    for (int i = 0; i < N; ++i) {
        waitpid(children[i], &sums[i], 0);
        result += WEXITSTATUS(sums[i]);
    }

    cout << "There are " << result << " letters, which are after letter '" << argv[3] << "' in alphabet" << endl;
    return 0;
}
