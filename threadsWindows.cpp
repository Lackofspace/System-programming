#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>


using namespace std;

CRITICAL_SECTION cs;
int globalVar(0);

struct ThreadParams {
    string* subString;
    char letter;
}params;

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


DWORD WINAPI sumLettersAfterGivenOne(LPVOID lpParameter) {
    auto pParams = (ThreadParams*) lpParameter;
    auto* subString = pParams->subString;
    char letter = pParams->letter;
    int sum;

    sum = accumulate((*subString).begin(), (*subString).end(),
                           0, [letter](size_t a, char b) {
                return (b > letter) ? ++a : a;
            });

    EnterCriticalSection(&cs);
    globalVar += sum;
    LeaveCriticalSection(&cs);
//    Sleep(100000);
    ExitThread(0);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <filename> <N> <letter>" << endl;
        return 1;
    }

    string filename = argv[1];
    string stringLine = read_string_from_file(filename);
    if (stringLine.size() < 2) {
        cerr << "Not enough symbols in file: " << filename << endl;
        return 1;
    }

    int nThreads = stoi(argv[2]);
    params.letter = tolower(argv[3][0]);
    int chunkSize = stringLine.size() / nThreads;


    vector<HANDLE> hThread(nThreads);
    InitializeCriticalSection(& cs);

    for (int i = 0; i < nThreads; i++) {
        int start = i * chunkSize;
        int end = start + chunkSize;
        if (i == nThreads - 1){
            end = stringLine.size();
        }
        string subString = stringLine.substr(start, end - start);
        params.subString = &subString;
        
        hThread[i] = CreateThread(nullptr, 0, sumLettersAfterGivenOne, &params, 0, nullptr);
        if (hThread[i] == nullptr) {
            cout << "Error creating thread: " << GetLastError() << endl;
        }
    }



    for (int i = 0; i < nThreads; i++) {
        WaitForSingleObject(hThread[i], INFINITE);
        DWORD exitCode;
        if (!GetExitCodeThread(hThread[i], &exitCode)) {
            cout << "Error getting thread exit code: " << GetLastError() << endl;
        }
        CloseHandle(hThread[i]);
    }
    cout << "There are " << globalVar << " letters, which are after letter '"
    << argv[3] << "' in alphabet" << endl;

    DeleteCriticalSection(& cs);

    ExitProcess(0);
}
