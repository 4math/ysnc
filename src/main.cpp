#include <iostream>
#include <tokenizer.h>
#include <suffix_tree.h>
#include <moss.h>

#include <filesystem>
#include <vector>
#include <unordered_map>

//std::vector<std::string> readDirectory(std::string directory) {
//    std::vector<std::string> v;
//    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
//        v.push_back(entry.path().string());
//    }
//    return v;
//}
//
//void write_directory() {
//    std::string path = "";
//    std::vector<std::string> v = readDirectory(path);
//    for (int i = 0; i < v.size(); i++) {
//        std::cout << v[i] << std::endl;
//    }
//}

class Tuple {
public:
    int n, l;
    Tuple(int n, int l) {
        this->n = n; this->l = l;
    }
};

class Detector {
public:
    std::unordered_map<long long, std::vector<Tuple>> h;
    int k;
    std::vector<int>** t;
    double** r;
    int* f;

    Detector(int k) {
        this->k = k;
        t = new std::vector<int>* [k];
        r = new double* [k];
        f = new int[k];
        for (int i = 0; i < k; i++) {
            t[i] = new std::vector<int>[k];
            r[i] = new double[k];
            for(int j = 0; j < k; j++) {
                t[i][j] = std::vector<int>();
                r[i][j] = 0;
            }
        }
    }

    void next_file(int n, char *input, int l) {
        long long v;
        for (int i = 0; i < l - 7; i++) {
            memcpy(&v, input + i, 8);
            if (h.find(v) == h.end()) {
                h[v] = std::vector<Tuple>();
            }
            h[v].push_back(Tuple(n, i));
        }
        f[n] = l;
    }

    void run() {
        files_into_table();
        sort_results();
        calculate_results();
    }

    void print_results() {
        for(int i = 0; i < k; i++) {
            std::cout << "     " << i << " ";
        }
        std::cout << std::endl;
        for(int i = 0; i < k; i++) {
            std::cout << i;
            for(int j = 0; j < k; j++) {
                if(i == j) {
                    std::cout << "       ";
                }
                else {
                    printf("%7.2f", r[i][j]);
                }
            }
            std::cout << std::endl;
        }
    }

private:
    void files_into_table() {
        for (const std::pair<long long, std::vector<Tuple>>& p : h) {
            int *c;
            c = new int[k];
            memset(c,0,k * sizeof(int));
            for(int i = 0; i < p.second.size(); i++) {
                c[p.second[i].n] = 1;
            }
            std::vector<int> d;
            for(int i = 0; i < k; i++) {
                if(c[i] == 1) {
                    d.push_back(i);
                }
            }
            for(int i = 0; i < p.second.size(); i++) {
                for(int j = 0; j < d.size(); j++) {
                    t[p.second[i].n][d[j]].push_back(p.second[i].l);
                }
            }
            delete[] c;
        }
    }

    void counting_sort(std::vector<int> &v) {
        int m = -1;
        for(int i = 0; i < v.size(); i++) {
            if(v[i] > m) {
                m = v[i];
            }
        }
        int *e;
        e = new int[m + 1];
        memset(e,0,(m+1) * sizeof(int));
        for(int i = 0; i < v.size(); i++) {
            e[v[i]]++;
        }
        int c = 0;
        for(int i = 0; i < m + 1; i++) {
            for(int j = 0; j < e[i]; j++) {
                v[c] = i;
                c++;
            }
        }
        delete[] e;
    }

    void sort_results() {
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < k; j++) {
                counting_sort(t[i][j]);
            }
        }
    }

    void calculate_results() {
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < k; j++) {
                if(t[i][j].size() > 0) {
                    r[i][j] = r[i][j] + 8;
                }
                for(int l = 1; l < t[i][j].size(); l++) {
                    if(t[i][j][l] - t[i][j][l - 1] < 8) {
                        r[i][j] += (t[i][j][l] - t[i][j][l - 1]);
                    }
                    else {
                        r[i][j] += 8;
                    }
                }
                r[i][j] /= f[i];
            }
        }
    }
};

int main() {
    Detector d(5);
    char c0[15] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
    char c1[15] = {1,2,3,5,5,6,7,8,9,0,5,2,3,4,5};
    char c2[16] = {1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
    char c3[15] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
    char c4[15] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
    d.next_file(0, c0, 15);
    d.next_file(1, c1, 15);
    d.next_file(2, c2, 16);
    d.next_file(3, c3, 15);
    d.next_file(4, c4, 15);
    d.run();
    d.print_results();
}
