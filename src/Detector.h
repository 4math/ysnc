#ifndef YSNC_DETECTOR_H
#define YSNC_DETECTOR_H

#include <vector>
#include <unordered_map>

class Detector {
public:
    std::unordered_map<std::string, std::vector<std::pair<int, int>>> h;
    std::vector<std::vector<std::vector<int>>> t;
    std::vector<std::vector<double>> r;
    std::vector<int> f;
    int w;

    Detector(int w) {
        this -> w = w;
    }

    void next_file(std::vector<char> input) {
        for (int i = 0; i < input.size() - w + 1; i++) {
            std::string v(input.begin() + i, input.begin() + i + w);
            if (h.find(v) == h.end()) {
                h[v] = std::vector<std::pair<int, int>>();
            }
            h[v].push_back(std::pair(f.size(), i));
        }
        f.push_back(input.size());
    }

    std::vector<std::vector<double>> run() {
        for(int i = 0; i < f.size(); i++) {
            t.push_back(std::vector<std::vector<int>>());
            r.push_back(std::vector<double>());
            for(int j = 0; j < f.size(); j++) {
                t[i].push_back(std::vector<int>());
                r[i].push_back(0);
            }
        }
        files_into_table();
        sort_results();
        calculate_results();
        return r;
    }

    void print_results() {
        for(int i = 0; i < f.size(); i++) {
            std::cout << "     " << i << " ";
        }
        std::cout << std::endl;
        for(int i = 0; i < f.size(); i++) {
            std::cout << i;
            for(int j = 0; j < f.size(); j++) {
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
        for (const std::pair<std::string, std::vector<std::pair<int, int>>>& p : h) {
            std::vector<int> c;
            for(int i = 0; i < f.size(); i++) {
                c.push_back(0);
            }
            for(int i = 0; i < p.second.size(); i++) {
                c[p.second[i].first] = 1;
            }
            std::vector<int> d;
            for(int i = 0; i < f.size(); i++) {
                if(c[i] == 1) {
                    d.push_back(i);
                }
            }
            for(int i = 0; i < p.second.size(); i++) {
                for(int j = 0; j < d.size(); j++) {
                    t[p.second[i].first][d[j]].push_back(p.second[i].second);
                }
            }
        }
    }

    void counting_sort(std::vector<int> &v) {
        int m = -1;
        for(int i = 0; i < v.size(); i++) {
            if(v[i] > m) {
                m = v[i];
            }
        }
        std::vector<int> e;
        for(int i = 0; i < m + 1; i++) {
            e.push_back(0);
        }
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
    }

    void sort_results() {
        for(int i = 0; i < f.size(); i++) {
            for(int j = 0; j < f.size(); j++) {
                counting_sort(t[i][j]);
            }
        }
    }

    void calculate_results() {
        for(int i = 0; i < f.size(); i++) {
            for(int j = 0; j < f.size(); j++) {
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
                r[i][j] = r[i][j] / f[i] * 100;
            }
        }
    }
};

#endif //YSNC_DETECTOR_H