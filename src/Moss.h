#ifndef YSNC_MOSS_H
#define YSNC_MOSS_H

#include <vector>
#include <unordered_map>

class Moss{
public:
    std::unordered_map<unsigned int, std::vector<int>> h;
    std::vector<std::vector<unsigned int>> f;
    std::vector<std::vector<double>> r;
    int k, w;

    Moss(int k, int w) {
        this -> k = k; this -> w = w;
    }

    void next_file(std::vector<char> input) {
        f.push_back(std::vector<unsigned int>());
        std::vector<unsigned int> hashes;
        for(int i = 0; i < input.size() - k + 1; i++) {
            std::string s(input.begin() + i, input.begin() + i + k);
            hashes.push_back(std::hash<std::string>()(s));
        }
        for(int i = 0; i < hashes.size() - w + 1; i++) {
            unsigned int min_value = hashes[i];
            int min_index = i;
            for(int j = i; j < i + w; j++) {
                if(hashes[j] < min_value) {
                    min_value = hashes[j];
                    min_index = j;
                }
            }
            i = min_index;
            f[f.size() - 1].push_back(min_value);
        }
    }

    void fingerprints_to_hashmap() {
        for(int i = 0; i < f.size(); i++) {
            for(int j = 0; j < f[i].size(); j++) {
                if (h.find(f[i][j]) == h.end()) {
                    h[f[i][j]] = std::vector<int>();
                }
                h[f[i][j]].push_back(i);
            }
        }
    }

    void hashmap_to_table() {
        for (const std::pair<unsigned int, std::vector<int>>& p : h) {
            std::vector<int> c;
            for(int i = 0; i < f.size(); i++) {
                c.push_back(0);
            }
            for(int i = 0; i < p.second.size(); i++) {
                c[p.second[i]] = 1;
            }
            std::vector<int> d;
            for(int i = 0; i < c.size(); i++) {
                if(c[i] == 1) {
                    d.push_back(i);
                }
            }
            for(int i = 0; i < p.second.size(); i++) {
                for(int j = 0; j < d.size(); j++) {
                    r[p.second[i]][d[j]]++;
                }
            }
        }
    }

    void calculate_results() {
        for(int i = 0; i < r.size(); i++) {
            for(int j = 0; j < r[i].size(); j++) {
                r[i][j] = r[i][j] / f[i].size() * 100;
            }
        }
    }

    std::vector<std::vector<double>> run() {
        for(int i = 0; i < f.size(); i++) {
            r.push_back(std::vector<double>());
            for(int j = 0; j < f.size(); j++) {
                r[i].push_back(0);
            }
        }
        fingerprints_to_hashmap();
        hashmap_to_table();
        calculate_results();
        return r;
    }

};

#endif //YSNC_MOSS_H
