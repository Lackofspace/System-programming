#include <iostream>
#include <fstream>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <string>
//#include <unistd.h>

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int globalVar = 0;

void* thread_func(void* arg) {
    auto* data = static_cast<pair<string, char>*>(arg);
    string& str = data->first;
    char ch = data->second;

    int local_globalVar = 0;
    for (char c : str) {
        if (c > ch)
            local_globalVar++;
    }

    pthread_mutex_lock(&mutex);
    globalVar += local_globalVar;
    pthread_mutex_unlock(&mutex);

    delete data;
    // sleep(10000);
    pthread_exit(nullptr);
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

    if (file_contents.size() < num_threads) {
        cerr << "Error: file '" << filename << "' the data in file is less than threads" << endl;
        num_threads /= 2;
        cerr << "The number of threads will be " << num_threads << endl;
    }

    int part_size = file_contents.size() / num_threads;
    vector<string> parts(num_threads);
    vector<pthread_t> threads(num_threads);

    for (int i = 0; i < num_threads; i++) {
        int start = i * part_size;
        int end = (i == num_threads - 1) ? file_contents.size() : (i + 1) * part_size;
        parts[i] = file_contents.substr(start, end - start);

        auto *data = new pair<string, char>{parts[i], ch};
        pthread_create(&threads[i], nullptr, thread_func, static_cast<void *>(data));
    }

    for (pthread_t &thread: threads) {
        int rc = pthread_join(thread, nullptr);
        if (rc != 0) {
            switch (rc) {
                case EINVAL:
                    cerr << "Error: invalid thread ID" << endl;
                    break;
                case ESRCH:
                    cerr << "Error: thread not found" << endl;
                    break;
                case EDEADLK:
                    cerr << "Error: deadlock detected" << endl;
                    break;
                default:
                    cerr << "Error: pthread_join failed with error code " << rc << endl;
            }
            exit(1);
        }
    }

    cout << "Total globalVar: " << globalVar << endl;

    return 0;
}
