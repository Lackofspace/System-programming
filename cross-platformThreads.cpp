#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <algorithm>
#include <string>
#include <atomic>


using namespace std;

atomic<int> globalVariable(0);

int countLettersFunction(const string& str, char ch){
    int count = 0;
    for (char c : str) {
        if (c > ch)
            count++;
    }
    return count;
}

void thread_func(const string& str, char ch) {
    globalVariable.fetch_add(countLettersFunction(str, ch));
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <filename> <num_threads> <char>" << endl;
        return 1;
    }

    string filename = argv[1];
    int num_threads = stoi(argv[2]);
    char ch = argv[3][0];

    ifstream infile(filename);
    if (!infile.good()) {
        cerr << "Error: file '" << filename << "' does not exist or cannot be read" << endl;
        return 1;
    }

    string file_contents((istreambuf_iterator<char>(infile)),
                              istreambuf_iterator<char>());

    if (file_contents.size() < 2) {
        cerr << "Error: file '" << filename << "' contains less than 2 characters" << endl;
        return 1;
    }

    thread threads[num_threads];
    vector<string> parts(num_threads);
    int part_size = file_contents.size() / num_threads;

    for (int i = 0; i < num_threads; i++) {
        int start = i * part_size;
        int end = (i == num_threads - 1) ? file_contents.size() : (i + 1) * part_size;
        parts[i] = file_contents.substr(start, end - start);
    }

    for (int i = 0; i < num_threads; i++) {
        try {
            threads[i] = thread(thread_func, parts[i], ch);
        } catch (const std::system_error& e) {
            cerr << "Error: failed to create thread " << i << ": " << e.what() << endl;
            for (int j = 0; j < i; j++) {
                threads[j].join();
            }
            return 1;
        }
    }

    for (thread& t : threads) {
        t.join();
    }

    cout << "There are " << globalVariable << " letters, which are after letter '"
         << argv[3] << "' in alphabet" << endl;

    return 0;
}
