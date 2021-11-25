#ifndef YSNC_JACCARD_INDEX_H
#define YSNC_JACCARD_INDEX_H

#include <vector>
#include <unordered_map>
#include <map>

struct page {
    std::map<unsigned int, unsigned int> first;
    std::map<unsigned int, unsigned int> second;
};

class JaccardIndex {
public:
    std::unordered_map<unsigned int, std::vector<std::pair<int, int>>> map;
    std::vector<std::vector<std::vector<int>>> t; // temporary table with results and file indices
    std::vector<std::vector<double>> res;
    std::vector<int> fsize; // files sizes
    std::vector<std::vector<unsigned int>> tokenLines;
    std::vector<page> highlightedLines;
    int width; // window width

    explicit JaccardIndex(int width): width(width) {}

    [[nodiscard]] unsigned int hashed(std::vector<unsigned int> const& vec) const {
        std::size_t seed = vec.size();
        for(auto& i : vec) {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    void nextFile(const std::vector<unsigned int> &input, const std::vector<unsigned int> &lines) {
        tokenLines.emplace_back();
        for (int i = 0; i < input.size() - width + 1; ++i) {
            tokenLines[tokenLines.size() - 1].push_back(lines[i]);
            std::vector<unsigned int> toBeHashed(input.begin() + i, input.begin() + i + width);
            auto hash = hashed(toBeHashed);
            if (map.find(hash) == map.end()) {
                map[hash] = std::vector<std::pair<int, int>>();
            }
            map[hash].push_back(std::pair(fsize.size(), i));
        }
        fsize.push_back(input.size());
    }

    std::vector<std::vector<double>> run() {
        for(int i = 0; i < fsize.size(); ++i) {
            t.emplace_back();
            res.emplace_back();
            for(int j = 0; j < fsize.size(); ++j) {
                t[i].emplace_back();
                res[i].push_back(0);
            }
        }
        filesIntoTable();
        sortResults();
        calculateResults();
        return res;
    }

    void printResults() {
        for(int i = 0; i < fsize.size(); ++i) {
            std::cout << "     " << i << " ";
        }
        std::cout << std::endl;
        for(int i = 0; i < fsize.size(); ++i) {
            std::cout << i;
            for(int j = 0; j < fsize.size(); ++j) {
                if(i == j) {
                    std::cout << "       ";
                }
                else {
                    printf("%7.2f", res[i][j]);
                }
            }
            std::cout << std::endl;
        }
    }

    std::vector<page> returnLines() {
        for(int i = 0; i < fsize.size(); i++) {
            for(int j = 0; j < fsize.size(); j++) {
                highlightedLines.emplace_back();
                if(i == j) {
                    continue;
                }
                for(int l = 0; l < t[i][j].size(); l++) {
                    for(int q = 0; q < width; q++) {
                        auto &lineOccur = highlightedLines[highlightedLines.size() - 1].first;
                        if(lineOccur.find(tokenLines[i][t[i][j][l]] + q) == lineOccur.end()) {
                            lineOccur[tokenLines[i][t[i][j][l]] + q] = 0;
                        }
                        lineOccur[tokenLines[i][t[i][j][l]] + q]++;
                    }
                }
                for(int l = 0; l < t[j][i].size(); l++) {
                    for(int q = 0; q < width; q++) {
                        auto &lineOccur = highlightedLines[highlightedLines.size() - 1].second;
                        if(lineOccur.find(tokenLines[j][t[j][i][l]] + q) == lineOccur.end()) {
                            lineOccur[tokenLines[j][t[j][i][l]] + q] = 0;
                        }
                        lineOccur[tokenLines[j][t[j][i][l]] + q]++;
                    }
                }
            }
        }
        return highlightedLines;
    }

private:
    void filesIntoTable() {
        for (const auto& p : map) {
            std::vector<int> c(fsize.size(), 0);

            for(int i = 0; i < p.second.size(); ++i) {
                c[p.second[i].first] = 1;
            }

            std::vector<int> d;
            for(int i = 0; i < fsize.size(); i++) {
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

    void countingSort(std::vector<int> &v) {
        int m = -1;
        for(const auto &elem : v) {
            if(elem > m) {
                m = elem;
            }
        }

        std::vector<int> e(m + 1, 0);

        for(const auto &elem : v) {
            e[elem]++;
        }

        int c = 0;
        for(int i = 0; i < m + 1; i++) {
            for(int j = 0; j < e[i]; j++) {
                v[c] = i;
                c++;
            }
        }
    }

    void sortResults() {
        for(int i = 0; i < fsize.size(); i++) {
            for(int j = 0; j < fsize.size(); j++) {
                countingSort(t[i][j]);
            }
        }
    }

    void calculateResults() {
        for(int i = 0; i < fsize.size(); ++i) {
            for(int j = 0; j < fsize.size(); ++j) {
                if(t[i][j].size() > 0) {
                    res[i][j] += width;
                }
                for(int l = 1; l < t[i][j].size(); ++l) {
                    if(t[i][j][l] - t[i][j][l - 1] < width) {
                        res[i][j] += (t[i][j][l] - t[i][j][l - 1]);
                    }
                    else {
                        res[i][j] += width;
                    }
                }
                res[i][j] = res[i][j] / (fsize[i] + fsize[j] - res[i][j]) * 100;
            }
        }
    }
};

#endif //YSNC_JACCARD_INDEX_H