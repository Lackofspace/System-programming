#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <string>
#include <cctype>


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

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <filename> <N> <letter>" << endl;
        return 1;
    }

    string filename = argv[1];
    string stringLine = read_string_from_file(filename);
    if (stringLine.size() < 2) {
        cerr << "Not enough numbers in file: " << filename << endl;
        return 1;
    }

    int N = atoi(argv[2]);
    string letter = argv[3];
    std::transform(letter.begin(), letter.end(), letter.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    vector<HANDLE> children(N);
    int chunk_size = (int) stringLine.size() / N;
    int rest = (int) stringLine.size() % N;

    for (int i = 0; i < N; ++i) {
        int start = i * chunk_size + min(i, rest);
        int end = start + chunk_size + (i < rest);
        STARTUPINFO si = {0};
        string str = stringLine.substr(start, end - start);
        string lpCommandLine = "a.exe " + str + " " + letter;
        PROCESS_INFORMATION pi = {nullptr};

        if (!CreateProcess(nullptr,
                           lpCommandLine.data(),
                                    nullptr,
                                    nullptr,
                                    TRUE,
                                    0,
                                    nullptr,
                                    nullptr,
                                    &si,
                                    &pi)) {

            cout << "Error: " << GetLastError() << endl;
            ExitProcess(0);
        }
        else{
            children[i] = pi.hProcess;
            CloseHandle(pi.hThread);
        }
    }

    size_t result = 0;
    for (int i = 0; i < N; ++i) {
        WaitForSingleObject(children[i], INFINITE);
        DWORD lpExitCode;
        GetExitCodeProcess(children[i], &lpExitCode);
        result += lpExitCode;
        CloseHandle( children[i] );
    }

    cout << "There are " << result << " letters, which are after letter '" << argv[3] << "' in alphabet" << endl;
    return 0;
}
