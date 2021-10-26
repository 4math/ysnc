#ifndef YSNC_DETECTOR_H
#define YSNC_DETECTOR_H

#include "Tuple.h"

class Detector {
public:
    std::unordered_map<std::string, std::vector<Tuple>> h;
    int k, w;
    std::vector<int>** t;
    double** r;
    int* f;

    Detector(int k, int w) {
        this -> k = k;
        this -> w = w;
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
        for (int i = 0; i < l - w + 1; i++) {
            std::string v(&input[i], &input[i] + w);
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
        for (const std::pair<std::string, std::vector<Tuple>>& p : h) {
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
                    r[i][j] += w;
                }
                for(int l = 1; l < t[i][j].size(); l++) {
                    if(t[i][j][l] - t[i][j][l - 1] < w) {
                        r[i][j] += (t[i][j][l] - t[i][j][l - 1]);
                    }
                    else {
                        r[i][j] += w;
                    }
                }
                r[i][j] /= f[i];
            }
        }
    }
};

#endif //YSNC_DETECTOR_H