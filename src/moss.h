#ifndef YSNC_MOSS_H
#define YSNC_MOSS_H

#include <vector>
#include <unordered_map>

class Moss {
public:
    std::unordered_map<unsigned int, std::vector<int>> map;
    std::vector<std::vector<unsigned int>> fingers;
    std::vector<std::vector<double>> res;
    int k, w; // k-grams, winnowing

    Moss(int k, int w) : k(k), w(w) {}

    std::size_t hashed(std::vector<unsigned int> const& vec) const {
        std::size_t seed = vec.size();
        for(auto& i : vec) {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    void nextFile(const std::vector<unsigned int> &input) {
        fingers.emplace_back();
        std::vector<unsigned int> hashes;
        hashes.reserve(input.size() - k + 1);
        for (int i = 0; i < input.size() - k + 1; ++i) {
            std::vector<unsigned int> toBeHashed(input.begin() + i, input.begin() + i + k);
            auto hash = hashed(toBeHashed);
            hashes.push_back(hash);
        }

        for (int i = 0; i < hashes.size() - w + 1; ++i) {
            unsigned int minValue = hashes[i];
            int minIndex = i;
            for (int j = i; j < i + w; ++j) {
                if (hashes[j] < minValue) {
                    minValue = hashes[j];
                    minIndex = j;
                }
            }
            i = minIndex;
            fingers[fingers.size() - 1].push_back(minValue);
        }
    }

    void fingerprintsToHashmap() {
        for (int i = 0; i < fingers.size(); ++i) {
            for (int j = 0; j < fingers[i].size(); ++j) {
                if (map.find(fingers[i][j]) == map.end()) {
                    map[fingers[i][j]] = std::vector<int>();
                }
                map[fingers[i][j]].push_back(i);
            }
        }
    }

    void hashmapToTable() {
        for (const auto &p : map) {
            std::vector<int> c(fingers.size(), 0);

            for (int i = 0; i < p.second.size(); ++i) {
                c[p.second[i]] = 1;
            }

            std::vector<int> d;
            for (int i = 0; i < c.size(); ++i) {
                if (c[i] == 1) {
                    d.push_back(i);
                }
            }
            for (int i = 0; i < p.second.size(); ++i) {
                for (int j = 0; j < d.size(); ++j) {
                    res[p.second[i]][d[j]]++;
                }
            }
        }
    }

    void calculateResults() {
        for (int i = 0; i < res.size(); ++i) {
            for (int j = 0; j < res[i].size(); ++j) {
                res[i][j] = (double) (res[i][j] / fingers[i].size()) * 100;
            }
        }
    }

    std::vector<std::vector<double>> run() {
        for (int i = 0; i < fingers.size(); ++i) {
            res.emplace_back();
            for (int j = 0; j < fingers.size(); ++j) {
                res[i].push_back(0);
            }
        }
        fingerprintsToHashmap();
        hashmapToTable();
        calculateResults();
        return res;
    }

};

#endif //YSNC_MOSS_H
