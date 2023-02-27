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
    char letter = tolower(argv[3][0]);
    vector<HANDLE> children(N);
    size_t chunk_size = stringLine.size() / N;
    size_t rest = stringLine.size() % N;
    const char* fileExe = "a.exe ";

    for (size_t i = 0; i < N; ++i) {
        size_t start = i * chunk_size + min(i, rest);
        size_t end = start + chunk_size + (i < rest);
        STARTUPINFO si = {0};
        string str = stringLine.substr(start, end - start);
        string lpCommandLine = fileExe + str + " " + letter;
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
