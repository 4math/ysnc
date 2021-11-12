#ifndef YSNC_MOSS_H
#define YSNC_MOSS_H

#include <vector>

class Moss{
public:
    std::unordered_map<unsigned int, std::vector<int>> h;
    std::vector<std::vector<unsigned int>> vector_of_fingerprints;
    std::vector<std::vector<double>> r;
    std::vector<int> f;
    int k, w;

    Moss(int k, int w) {
        this -> k = k; this -> w = w;
    }

    void next_file(std::vector<char> input) {
        vector_of_fingerprints.push_back(std::vector<unsigned int>());
        std::vector<unsigned int> hashes;
        std::vector<unsigned int> fingerprint;
        for(int i = 0; i < input.size() - k + 1; i++) {
            std::string s(input.begin() + i, input.begin() + i + k);
            hashes.push_back(std::hash<std::string>()(s));
        }
        for(int i = 0; i < hashes.size() - w + 1; i++) {
            unsigned int min_value = hashes[i];
            for(int j = i; j < i + w; j++) {
                if(hashes[j] < min_value) {
                    min_value = hashes[j];
                }
            }
            vector_of_fingerprints[vector_of_fingerprints.size() - 1].push_back(min_value);
        }
        f.push_back(input.size());
    }

    void vector_of_fingerprints_to_hashmap() {
        for(int i = 0; i < vector_of_fingerprints.size(); i++) {
            for(int j = 0; j < vector_of_fingerprints.size(); j++) {
                if (h.find(vector_of_fingerprints[i][j]) == h.end()) {
                    h[vector_of_fingerprints[i][j]] = std::vector<int>();
                }
                h[vector_of_fingerprints[i][j]].push_back(i);
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

    std::vector<std::vector<double>> run() {
        for(int i = 0; i < f.size(); i++) {
            r.push_back(std::vector<double>());
            for(int j = 0; j < f.size(); j++) {
                r[i].push_back(0);
            }
        }
        vector_of_fingerprints_to_hashmap();
        hashmap_to_table();
        for(int i = 0; i < f.size(); i++) {
            for(int j = 0; j < f.size(); j++) {
                printf("%7.2f", r[i][j]);
            }
            std::cout << std::endl;
        }
        return r;
    }
};

#endif //YSNC_MOSS_H
