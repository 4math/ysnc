// Authors
// Maksims Uhanovs, 201RDB140
// Aleksandrs Samsonovičs, 201RDB124
// Kirils Trofimovs, 201RDB175

// g++ -std=c++17 -o repl repl.cpp -lstdc++fs

#include <iostream>
#include <experimental/filesystem>
#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <iomanip>
#include <set>
#include <algorithm>
#include <fstream>

// In the better times would be better to implement builder pattern
class Config {
public:
    Config() = default;

    ~Config() = default;

    Config(const Config &other) = default;

    enum Detector {
        JaccardIndex,
        Moss
    };

    [[nodiscard]] const std::experimental::filesystem::path &getResultPath() const {
        return resultPath;
    }

    void setResultPath(std::experimental::filesystem::path path) {
        resultPath = std::move(path);
        pagesPath = resultPath / std::experimental::filesystem::path("pages");
    }

    [[nodiscard]] const std::experimental::filesystem::path &getPagesPath() const {
        return pagesPath;
    }

    [[nodiscard]] const std::string &getRed() const {
        return red;
    }

    void setRed(const std::string &red) {
        validateColorHexString(red);
        Config::red = red;
    }

    [[nodiscard]] const std::string &getYellow() const {
        return yellow;
    }

    void setYellow(const std::string &yellow) {
        validateColorHexString(yellow);
        Config::yellow = yellow;
    }

    [[nodiscard]] const std::string &getGreen() const {
        return green;
    }

    void setGreen(const std::string &green) {
        validateColorHexString(green);
        Config::green = green;
    }

    [[nodiscard]] const std::string &getGray() const {
        return gray;
    }

    void setGray(const std::string &gray) {
        validateColorHexString(gray);
        Config::gray = gray;
    }

    const std::string &getCodeColorMark() const {
        return codeColorMark;
    }

    void setCodeColorMark(const std::string &codeMark) {
        validateColorHexString(codeMark);
        Config::codeColorMark = codeMark;
    }

    int getThresholdGreen() const {
        return thresholdGreen;
    }

    void setThresholdGreen(int thresholdGreen) {
        if (thresholdGreen > 100 || thresholdGreen < 0) {
            throw std::runtime_error("thresholdGreen should be in the range 0 < thresholdGreen < 100!");
        }
        Config::thresholdGreen = thresholdGreen;
    }

    int getThresholdYellow() const {
        return thresholdYellow;
    }

    void setThresholdYellow(int thresholdYellow) {
        if (thresholdYellow > 100 || thresholdYellow < thresholdGreen) {
            throw std::runtime_error("thresholdYellow should be in the range thresholdGreen < thresholdYellow < 100!");
        }
        Config::thresholdYellow = thresholdYellow;
    }

    int getAbbrInRow() const {
        return abbrInRow;
    }

    void setAbbrInRow(int abbrInRow) {
        if (abbrInRow < 1) {
            throw std::runtime_error("abbrInRow cannot be less than 1!");
        }
        Config::abbrInRow = abbrInRow;
    }

    int getHighlightingThreshold() const {
        return highlightingThreshold;
    }

    void setHighlightingThreshold(int highlightingThreshold) {
        if (highlightingThreshold < 1) {
            throw std::runtime_error("highlightingThreshold cannot be less than 1!");
        }
        Config::highlightingThreshold = highlightingThreshold;
    }

    [[nodiscard]] Detector getDetectorType() const {
        return detectorType;
    }

    void setDetectorType(Detector detectorType) {
        Config::detectorType = detectorType;
    }

    int getJaccardWindow() const {
        return jaccardWindow;
    }

    void setJaccardWindow(int jaccardWindow) {
        if (jaccardWindow < 2) {
            throw std::runtime_error("Window width cannot be less than 2!");
        }
        Config::jaccardWindow = jaccardWindow;
    }

    const std::pair<int, int> &getMossWindow() const {
        return mossWindow;
    }

    void setMossWindow(const std::pair<int, int> &mossWindow) {
        if (mossWindow.first < 2) {
            throw std::runtime_error("Window width cannot be less than 2!");
        }
        if (mossWindow.second < 2) {
            throw std::runtime_error("Hash width cannot be less than 2!");
        }
        Config::mossWindow = mossWindow;
    }

private:
    std::experimental::filesystem::path resultPath = std::experimental::filesystem::path("results");
    std::experimental::filesystem::path pagesPath = resultPath / std::experimental::filesystem::path("pages");
    std::string red = "#FF241C";
    std::string yellow = "#F5EF35";
    std::string green = "#4EF52C";
    std::string gray = "#B1A9A3";
    std::string codeColorMark = "#FF6F9D";
    int thresholdGreen = 40; // threshold by which files are decided to be fine.
    int thresholdYellow = 60; // threshold value which is thresholdGreen < value < thresholdYellow
    int abbrInRow = 6; // number of files in the legend table
    int highlightingThreshold = 3; // how many tokens should be equal to start highlighting the line
    Detector detectorType = Detector::JaccardIndex; // Which detector to use. Differs approach of showing the data
    int jaccardWindow = 5; // default window for Jaccard detector
    std::pair<int, int> mossWindow = {4, 3}; // 1. window width 2. hash width

    // for the color code check
    std::map<char, char> allowedHexSymbols = {
            {'0', 0},
            {'1', 1},
            {'2', 2},
            {'3', 3},
            {'4', 4},
            {'5', 6},
            {'6', 6},
            {'7', 7},
            {'8', 8},
            {'9', 9},
            {'A', 10},
            {'B', 11},
            {'C', 12},
            {'D', 13},
            {'E', 14},
            {'F', 15}
    };

    void validateColorHexString(const std::string &color) {
        if (color.size() != 7) {
            throw std::runtime_error("Length of the color should be 6!");
        }
        if (color[0] != '#') {
            throw std::runtime_error("Color hex should start with #");
        }

        for (int i = 1; i < color.size(); ++i) {
            if (allowedHexSymbols.find(color[i]) == allowedHexSymbols.end()) {
                throw std::runtime_error("Incorrect hex symol was used!");
            }
        }
    }
};

class File {
public:
    explicit File(const std::experimental::filesystem::path &path) {
        read(path);
    }

    [[nodiscard]] std::vector<std::string> &getData() {
        return data;
    }

private:
    void read(const std::experimental::filesystem::path &path) {
        std::ifstream input(path);
        std::stringstream buffer;
        buffer << input.rdbuf();
        std::string rawString = buffer.str();

        auto result = clearComments(rawString);

        splitToDataVector(result);
    }

    void splitToDataVector(std::string &str) {
        std::string delimiter = "\n";

        size_t pos;
        std::string token;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            token = str.substr(0, pos);
            data.push_back(token);
            str.erase(0, pos + delimiter.length());
        }
        // push last line
        data.push_back(str);
    }

    static std::string clearComments(std::string &rawString) {
        std::stringstream result;

        bool skipOneLine = false;
        bool skipMultiLine = false;
        bool pushToBuf = true;
        for (int i = 0; i < rawString.size() - 1; ++i) {
            if (rawString[i] == '/' && rawString[i + 1] == '/' && !skipMultiLine) {
                skipOneLine = true;
                pushToBuf = false;
            }

            if (skipOneLine && rawString[i] == '\n') {
                skipOneLine = false;
                pushToBuf = true;
            }

            if (rawString[i] == '/' && rawString[i + 1] == '*') {
                pushToBuf = false;
                skipMultiLine = true;
            }

            if (rawString[i] == '*' && rawString[i + 1] == '/') {
                pushToBuf = true;
                skipMultiLine = false;
                // skip end comment symbols
                i += 2;
                // check for file end
                if (i >= rawString.size()) break;
            }

            if (pushToBuf) {
                result << rawString[i];
            }
        }

        return result.str();
    }


    std::vector<std::string> data;
};

struct page {
    std::map<unsigned int, unsigned int> first;
    std::map<unsigned int, unsigned int> second;
};

class HtmlOutput {
public:
    explicit HtmlOutput(const Config &config) : config(config) {
        std::experimental::filesystem::create_directory(config.getResultPath());
        std::experimental::filesystem::create_directory(config.getPagesPath());
    }

    void outputHtml(const std::vector<std::vector<double>> &table,
                    const std::vector<std::experimental::filesystem::path> &files,
                    const std::vector<page> &highlightedLines) {
        for (int i = 0; i < files.size(); ++i) {
            for (int j = 0; j < files.size(); ++j) {
                if (i == j) continue;
                unsigned int index = i * files.size() + j;
                outputComparisonPage(files[i], files[j], highlightedLines[index]);
            }
        }

        std::string html = createResultTable(table, files);

        std::ofstream output(config.getResultPath() / "result.html");
        output << html;
        output.close();
    }

    void outputComparisonPage(const std::experimental::filesystem::path &firstFile, const std::experimental::filesystem::path &secondFile,
                              const page &comparisonPage) {
        std::string html = createComparisonPage(firstFile, secondFile, comparisonPage);

        std::ofstream output(config.getPagesPath() / pageName(firstFile, secondFile));
        output << html;
        output.close();
    }


private:
    Config config;

    [[nodiscard]] static std::string pageName(const std::experimental::filesystem::path &firstFile, const std::experimental::filesystem::path &secondFile) {
        std::stringstream filename;
        filename << firstFile.stem().string() << "-" << secondFile.stem().string() << ".html";
        return filename.str();
    }

    [[nodiscard]] std::vector<std::string> readSourceCode(const std::experimental::filesystem::path &path, const page &comparisonPage,
                                                          unsigned int whichFileToCompare) {
        std::map<unsigned int, unsigned int> highlightedLines;
        if (whichFileToCompare == 1) {
            highlightedLines = comparisonPage.first;
        } else if (whichFileToCompare == 2) {
            highlightedLines = comparisonPage.second;
        }

        // It is easier to use vector since
        // code highlighting depends on the line number
        std::vector<std::string> codeLines;
        File input(path);
        auto &inputVec = input.getData();
        std::stringstream codeLine;
        unsigned int lineNumber = 1;
        for (auto &line : inputVec) {
            auto pos = line.find('<');
            if (pos != std::string::npos) {
                line.replace(pos, 1, "&lt;");
            }

            pos = line.find('>');
            if (pos != std::string::npos) {
                line.replace(pos, 1, "&gt;");
            }

            if (config.getDetectorType() != Config::Detector::Moss &&
                highlightedLines.find(lineNumber) != highlightedLines.end() &&
                highlightedLines[lineNumber] > config.getHighlightingThreshold()) {
                codeLine << "<code style='background-color:" << config.getCodeColorMark()
                         << "'>" << line << "</code>\n";
            } else {
                codeLine << "<code>" << line << "</code>\n";
            }
            codeLines.push_back(codeLine.str());
            codeLine.str("");
            lineNumber++;
        }

        return codeLines;
    }

    [[nodiscard]]  std::string createComparisonPage(const std::experimental::filesystem::path &firstFile, const std::experimental::filesystem::path &secondFile,
                                                    const page &comparisonPage) {
        std::stringstream body;
        body <<
             "<!DOCTYPE html>"
             "<html lang='en'>"
             "<head>"
             "<meta charset='UTF-8'>"
             "<title>" << firstFile.filename().string() << " vs " << secondFile.filename().string() <<
             "</title>"
             "<style>"
             "code {"
             "font-family: 'Courier New', Courier, monospace;"
             "}"
             ".container {"
             "display: grid;"
             "grid-template-columns: 1fr 1fr;"
             "grid-gap: 20px;"
             "}"
             "pre.code {"
             "white-space: pre-wrap;"
             "}"
             "pre.code::before {"
             "counter-reset: listing;"
             "}"
             "pre.code code {"
             "counter-increment: listing;"
             "}"
             "pre.code code::before {"
             "content: counter(listing) '. ';"
             "display: inline-block;"
             "width: 8em;"
             "padding-left: auto;"
             "margin-left: auto;"
             "text-align: right;"
             "}"
             "</style>"
             "</head>"
             "<body>"
             "<div class='container'>"
             "<div class='source'>"
             "<h3>Filename: " << firstFile.filename().string() <<
             "</h3>"
             "<pre class='code'>";

        auto firstSourceCode = readSourceCode(firstFile, comparisonPage, 1);

        for (const auto &line: firstSourceCode) {
            body << line;
        }

        body << "</pre>";
        // closing source div
        body << "</div>";

        body <<
             "<div class='source'>"
             "<h3>Filename: " << secondFile.filename().string() <<
             "</h3>"
             "<pre class='code'>";

        auto secondSourceCode = readSourceCode(secondFile, comparisonPage, 2);

        for (const auto &line: secondSourceCode) {
            body << line;
        }

        body << "</pre>";
        // closing source div
        body << "</div>";
        // closing container div
        body << "</div>";
        body << "</body>";
        body << "</html>";

        return body.str();
    }

    [[nodiscard]] std::string createResultTable(const std::vector<std::vector<double>> &table,
                                                const std::vector<std::experimental::filesystem::path> &files) const {
        std::stringstream body;
        body <<
             "<!DOCTYPE html>"
             "<html lang='en'>"
             "<head>"
             "<meta charset='UTF-8'>"
             "<title>Check results</title>"
             "<style>"
             "h1 {"
             "text-align: center;"
             "}"
             ".green {"
             "background-color: " << config.getGreen() <<
             "}"
             ".red {"
             "background-color: " << config.getRed() <<
             "}"
             ".yellow {"
             "background-color: " << config.getYellow() <<
             "}"
             ".gray {"
             "background-color: " << config.getGray() <<
             "}"
             "table {"
             "margin: 0 auto;"
             "width: 40%;"
             "text-align: center;"
             "}"
             "table, th, td {"
             "border: 1px solid black;"
             "padding: 4px;"
             "}"
             "a {"
             "color: black;"
             "}"
             "</style>"
             "</head>"
             "<body>"
             "<h1>Plagiarism check results: </h1>"
             "<table>";


        for (int i = 0; i < table.size() + 1; ++i) {
            body << "<tr>";
            for (int j = 0; j < table[0].size() + 1; ++j) {
                // first row
                if (i == 0) {
                    if (j == 0) {
                        body << "<th></th>";
                        continue;
                    }
                    body << "<th>" << j << "</th>";
                    continue;
                }
                // next rows
                // ------------

                // first column
                if (j == 0) {
                    body << "<th>" << i << "</th>";
                    continue;
                }

                // gray out diagonal
                if (j == i) {
                    body << "<td class=gray></td>";
                    continue;
                }

                auto value = table[i - 1][j - 1];
                if (value > 100) value = 100;
                std::string classColor;
                if (value < config.getThresholdGreen()) {
                    classColor = "green";
                } else if (value < config.getThresholdYellow()) {
                    classColor = "yellow";
                } else {
                    classColor = "red";
                }

                if (config.getDetectorType() == Config::Detector::Moss) {
                    body << "<td class=" << classColor << ">" <<
                         std::fixed << std::setprecision(1) <<
                         value << "</td>";
                } else {
                    body << "<td class=" << classColor << ">" <<
                         "<a href=pages/" <<
                         pageName(files[i - 1], files[j - 1]) <<
                         ">" << std::fixed << std::setprecision(1) <<
                         value << "%</a>" << "</td>";
                }

            }
            body << "</tr>";
        }
        // Result table is created
        body << "</table>";

        // Starting the creation of abbreviation table
        body << "<h1>Abbreviation</h1>";
        body << "<table>";
        int fileCountInRow = config.getAbbrInRow();
        for (int i = 0; i < files.size(); ++i) {
            if (i % fileCountInRow == 0) {
                body << "<tr>";
            }

            body << "<th>" << i + 1 << ": " << "</th>";
            std::string filename = files[i].filename().string();
            body << "<td>" << "<a href=" << files[i].string() << ">" <<
                 filename << "</a></td>";

            if (i % fileCountInRow == fileCountInRow - 1) {
                body << "</tr>";
            }
        }
        body << "</table>";

        body << "</body>";
        body << "</html>";
        return body.str();
    }


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

    explicit JaccardIndex(int width) : width(width) {}

    [[nodiscard]] unsigned int hashed(std::vector<unsigned int> const &vec) const {
        std::size_t seed = vec.size();
        for (auto &i : vec) {
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
        for (int i = 0; i < fsize.size(); ++i) {
            t.emplace_back();
            res.emplace_back();
            for (int j = 0; j < fsize.size(); ++j) {
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
        for (int i = 0; i < fsize.size(); ++i) {
            std::cout << "     " << i << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < fsize.size(); ++i) {
            std::cout << i;
            for (int j = 0; j < fsize.size(); ++j) {
                if (i == j) {
                    std::cout << "       ";
                } else {
                    printf("%7.2f", res[i][j]);
                }
            }
            std::cout << std::endl;
        }
    }

    std::vector<page> returnLines() {
        for (int i = 0; i < fsize.size(); i++) {
            for (int j = 0; j < fsize.size(); j++) {
                highlightedLines.emplace_back();
                if (i == j) {
                    continue;
                }
                for (int l = 0; l < t[i][j].size(); l++) {
                    for (int q = 0; q < width; q++) {
                        auto &lineOccur = highlightedLines[highlightedLines.size() - 1].first;
                        if (lineOccur.find(tokenLines[i][t[i][j][l]] + q) == lineOccur.end()) {
                            lineOccur[tokenLines[i][t[i][j][l]] + q] = 0;
                        }
                        lineOccur[tokenLines[i][t[i][j][l]] + q]++;
                    }
                }
                for (int l = 0; l < t[j][i].size(); l++) {
                    for (int q = 0; q < width; q++) {
                        auto &lineOccur = highlightedLines[highlightedLines.size() - 1].second;
                        if (lineOccur.find(tokenLines[j][t[j][i][l]] + q) == lineOccur.end()) {
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
        for (const auto &p : map) {
            std::vector<int> c(fsize.size(), 0);

            for (int i = 0; i < p.second.size(); ++i) {
                c[p.second[i].first] = 1;
            }

            std::vector<int> d;
            for (int i = 0; i < fsize.size(); i++) {
                if (c[i] == 1) {
                    d.push_back(i);
                }
            }

            for (int i = 0; i < p.second.size(); i++) {
                for (int j = 0; j < d.size(); j++) {
                    t[p.second[i].first][d[j]].push_back(p.second[i].second);
                }
            }
        }
    }

    void countingSort(std::vector<int> &v) {
        int m = -1;
        for (const auto &elem : v) {
            if (elem > m) {
                m = elem;
            }
        }

        std::vector<int> e(m + 1, 0);

        for (const auto &elem : v) {
            e[elem]++;
        }

        int c = 0;
        for (int i = 0; i < m + 1; i++) {
            for (int j = 0; j < e[i]; j++) {
                v[c] = i;
                c++;
            }
        }
    }

    void sortResults() {
        for (int i = 0; i < fsize.size(); i++) {
            for (int j = 0; j < fsize.size(); j++) {
                countingSort(t[i][j]);
            }
        }
    }

    void calculateResults() {
        for (int i = 0; i < fsize.size(); ++i) {
            for (int j = 0; j < fsize.size(); ++j) {
                if (t[i][j].size() > 0) {
                    res[i][j] += width;
                }
                for (int l = 1; l < t[i][j].size(); ++l) {
                    if (t[i][j][l] - t[i][j][l - 1] < width) {
                        res[i][j] += (t[i][j][l] - t[i][j][l - 1]);
                    } else {
                        res[i][j] += width;
                    }
                }
                res[i][j] = res[i][j] / (fsize[i] + fsize[j] - res[i][j]) * 100;
            }
        }
    }
};

class Moss {
public:
    std::unordered_map<unsigned int, std::vector<int>> map;
    std::vector<std::vector<unsigned int>> fingers;
    std::vector<std::vector<double>> res;
    int k, w; // k-grams, winnowing

    Moss(int k, int w) : k(k), w(w) {}

    std::size_t hashed(std::vector<unsigned int> const &vec) const {
        std::size_t seed = vec.size();
        for (auto &i : vec) {
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

static int freeTokenId = 100;

class Tokenizer {
public:
    class Token {
    public:
        Token(int position_, int tokenId_) : position(position_), id(tokenId_) {
            if (reverseTokenMap.count(tokenId_) > 0)
                value = reverseTokenMap[tokenId_];
        }

        Token(int position_, const std::string &value_) : position(position_), value(value_) {
            if (tokenMap.count(value_) > 0)
                id = tokenMap[value];
        }

        int getPosition() const {
            return position;
        }

        int getId() const {
            return id;
        }

        const std::string &getValue() const {
            return value;
        }

    private:
        int position = -1;
        int id = -1;
        std::string value = "invalid";
    };

    explicit Tokenizer(const std::vector<std::string> &inputStrings_) : inputStrings(inputStrings_) {}

    static std::vector<std::string> splitData(const std::string &data) {
        static const std::set<char> splitters = {
                '{', '}', '(', ')', '\"', '+', '-', '/', '*', '%', '=', '!',
                '<', '>', '?', ':', '&', '|', '^', '~', '[', ']', ',', '.', ';'
        };

        std::vector<std::string> result;
        std::string token;
        for (const auto &c : data) {
            if (splitters.count(c) > 0) {
                if (!token.empty())
                    result.push_back(token);
                result.push_back({c});
                token = "";
            } else {
                token += c;
            }
        }

        if (!token.empty())
            result.push_back(token);

        return result;
    }

    const std::vector<Token> &result() {
        if (!tokenVector.empty())
            return tokenVector;

        long long currentPos = 0;
        unsigned int lineNumber = 0;

        for (const auto &str : inputStrings) {
            ++lineNumber;
            std::string data;
            std::stringstream sstream(str);

            while (sstream >> data) {
                if (tokenMap.count(data) > 0) { // Processing the keywords
                    Token token(currentPos, data);
                    tokenVector.push_back(token);
                    tokenToLine.push_back(lineNumber);
                    currentPos = sstream.tellg();

                    continue;
                }

                auto dataVector = splitData(data);

                for (auto &data_ : dataVector) {
                    if (tokenMap.count(data_) > 0) {
                        Token token(currentPos, data_);
                        tokenVector.push_back(token);
                        tokenToLine.push_back(lineNumber);
                        currentPos = sstream.tellg();
                    } else { // The token is an indentifier
                        if (identifierMap.count(data_) > 0) { // Was found previously
                            Token token(currentPos, identifierMap[data_]);
                            tokenVector.push_back(token);
                            tokenToLine.push_back(lineNumber);
                        } else {
                            Token token(currentPos, freeTokenId);
                            identifierMap[data_] = freeTokenId++;
                            tokenVector.push_back(token);
                            tokenToLine.push_back(lineNumber);
                        }

                        currentPos = sstream.tellg();
                    }
                }

                currentPos = sstream.tellg();
            }
        }

        return tokenVector;
    }

    typedef std::map<std::string, int> tMap;
    typedef std::map<int, std::string> rtMap;

    static tMap identifierMap;
    static tMap tokenMap;
    static rtMap reverseTokenMap;

    static rtMap initReverseTokenMap() {
        rtMap map;

        for (auto &it : tokenMap)
            map[it.second] = it.first;

        return map;
    }

    const std::vector<unsigned int> &getTokenToLine() const {
        return tokenToLine;
    }

private:
    const std::vector<std::string> &inputStrings;
    std::vector<Token> tokenVector;
    std::vector<unsigned int> tokenToLine;
};

Tokenizer::tMap Tokenizer::identifierMap = {};

Tokenizer::tMap Tokenizer::tokenMap = {
        {"abstract",     1},
        {"boolean",      2},
        {"byte",         3},
        {"break",        4},
        {"class",        5},
        {"case",         6},
        {"catch",        7},
        {"char",         8},
        {"continue",     9},
        {"default",      10},
        {"do",           11},
        {"double",       12},
        {"else",         13},
        {"extends",      14},
        {"final",        15},
        {"finally",      16},
        {"float",        17},
        {"for",          18},
        {"if",           19},
        {"implements",   20},
        {"import",       21},
        {"instanceof",   22},
        {"int",          23},
        {"interface",    24},
        {"long",         25},
        {"native",       26},
        {"new",          27},
        {"package",      28},
        {"private",      29},
        {"protected",    30},
        {"public",       31},
        {"return",       32},
        {"short",        33},
        {"static",       34},
        {"super",        35},
        {"switch",       36},
        {"synchronized", 37},
        {"this",         38},
        {"throw",        39},
        {"throws",       40},
        {"transient",    41},
        {"try",          42},
        {"void",         43},
        {"volatile",     44},
        {"while",        45},
        {"assert",       46},
        {"const",        47},
        {"enum",         48},
        {"goto",         49},
        {"strictfp",     50},
        {"{",            51},
        {"}",            52},
        {"(",            53},
        {")",            54},
        {"\"",           55},
        {"+",            56},
        {"-",            57},
        {"/",            58},
        {"*",            59},
        {"%",            60},
        {"=",            61},
        {"!",            62},
        {"<",            63},
        {">",            64},
        {"?",            65},
        {":",            66},
        {"&",            67},
        {"|",            68},
        {"^",            69},
        {"~",            70},
        {"[",            71},
        {"]",            72},
        {",",            73},
        {".",            74},
        {";",            75},
        {"identifier",   90},
        {"literal",      91}
};

Tokenizer::rtMap Tokenizer::reverseTokenMap = Tokenizer::initReverseTokenMap();

class ConsoleUI {
public:

    ConsoleUI() = default;

    void run() {
        std::cout << "Write help or ? to see commands\n";
        std::string input;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);
            if (input.empty()) std::getline(std::cin, input);
            std::vector<std::string> v = split(input, ' ');
            if (v.size() > 2 || v.empty()) {
                std::cout << "Incorrect number of arguments!\n";
                continue;
            }

            auto command = v[0];
            std::string arg;

            if (v.size() == 2) {
                arg = v[1];
            } else {
                arg = "";
            }

            if (command == "get") {
                if (arg.empty()) {
                    continue;
                } else if (arg == "resultPath") {
                    std::cout << config.getResultPath() << "\n";
                } else if (arg == "resultPath") {
                    std::cout << config.getResultPath() << "\n";
                } else if (arg == "pagesPath") {
                    std::cout << config.getPagesPath() << "\n";
                } else if (arg == "redColor") {
                    std::cout << config.getRed() << "\n";
                } else if (arg == "greenColor") {
                    std::cout << config.getGreen() << "\n";
                } else if (arg == "yellowColor") {
                    std::cout << config.getYellow() << "\n";
                } else if (arg == "grayColor") {
                    std::cout << config.getGray() << "\n";
                } else if (arg == "codeColorMark") {
                    std::cout << config.getCodeColorMark() << "\n";
                } else if (arg == "thresholdGreen") {
                    std::cout << config.getThresholdGreen() << "\n";
                } else if (arg == "thresholdYellow") {
                    std::cout << config.getThresholdYellow() << "\n";
                } else if (arg == "abbrInRow") {
                    std::cout << config.getAbbrInRow() << "\n";
                } else if (arg == "highlightingThreshold") {
                    std::cout << config.getHighlightingThreshold() << "\n";
                } else if (arg == "detectorType") {
                    std::string detector;
                    switch (config.getDetectorType()) {
                        case Config::JaccardIndex: {
                            detector = "Jaccard";
                            break;
                        }
                        case Config::Moss: {
                            detector = "Moss";
                            break;
                        }
                    }
                    std::cout << detector << "\n";
                } else if (arg == "jaccardWindow") {
                    std::cout << config.getJaccardWindow() << "\n";
                } else if (arg == "mossWindow") {
                    auto pair = config.getMossWindow();
                    std::cout << "window size: " << pair.first << " | hash window size: " << pair.second << "\n";
                }
            } else if (command == "set") {
                if (arg.empty()) {
                    continue;
                } else if (arg == "resultPath") {
                    auto inp = getUserInput("Result path: ");
                    try {
                        config.setResultPath(inp);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "redColor") {
                    auto i = getUserInput("Red color hex: ");
                    try {
                        config.setRed(i);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "greenColor") {
                    auto i = getUserInput("Green color hex: ");
                    try {
                        config.setGreen(i);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "yellowColor") {
                    auto i = getUserInput("Yellow color hex: ");
                    try {
                        config.setYellow(i);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "grayColor") {
                    auto i = getUserInput("Gray color hex: ");
                    try {
                        config.setGray(i);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "codeColorMark") {
                    auto i = getUserInput("Code highlighting color hex: ");
                    try {
                        config.setCodeColorMark(i);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "thresholdGreen") {
                    auto i = getIntUserInput("Threshold of the green color: ");
                    try {
                        config.setThresholdGreen(i);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "thresholdYellow") {
                    auto i = getIntUserInput("Threshold of the yellow color: ");
                    try {
                        config.setThresholdYellow(i);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "abbrInRow") {
                    auto i = getIntUserInput("Amount of file names in the table row: ");
                    try {
                        config.setAbbrInRow(i);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "highlightingThreshold") {
                    auto i = getIntUserInput("Threshold of the highlighting: ");
                    try {
                        config.setHighlightingThreshold(i);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "detectorType") {
                    auto i = getUserInput("Jaccard or Moss: ");
                    if (i == "Jaccard") {
                        config.setDetectorType(Config::Detector::JaccardIndex);
                    } else if (i == "Moss") {
                        config.setDetectorType(Config::Detector::Moss);
                    } else {
                        std::cout << "Incorrect detector type!\n";
                    }
                } else if (arg == "jaccardWindow") {
                    auto i = getIntUserInput("Jaccard window size: ");
                    try {
                        config.setJaccardWindow(i);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "mossWindow") {
                    auto i = getIntUserInput("Moss window size: ");
                    auto value = config.getMossWindow();
                    value.first = i;
                    try {
                        config.setMossWindow(value);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                } else if (arg == "mossHashWindow") {
                    auto i = getIntUserInput("Moss hash window size: ");
                    auto value = config.getMossWindow();
                    value.second = i;
                    try {
                        config.setMossWindow(value);
                    } catch (const std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                }
            } else if (command == "commands" || command == "help" || command == "?") {
                listCommands();
            } else if (command == "execute") {
                try {
                    execute();
                } catch (const std::exception &e) {
                    std::cout << e.what() << "\n";
                    continue;
                }
            } else if (command == "quit") {
                break;
            }
        }
    }


private:
    Config config;

    template<typename Out>
    void split(const std::string &s, char delim, Out result) {
        std::istringstream iss(s);
        std::string item;
        while (std::getline(iss, item, delim)) {
            *result++ = item;
        }
    }

    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    static std::string getUserInput(const std::string &message) {
        std::string input;
        std::cout << message;
        std::cin >> input;
        return input;
    }

    static int getIntUserInput(const std::string &message) {
        std::string input;
        std::cout << message;
        std::cin >> input;
        int i;
        try {
            i = std::stoi(input);
        } catch (const std::exception &) {
            std::cout << "Please, input an integer!\n";
        }
        return i;
    }

    void execute() {
        auto inp = getUserInput("Path to the folder with source codes: ");
        auto detector = config.getDetectorType();

        if (detector == Config::JaccardIndex) {
            JaccardIndex m(config.getJaccardWindow());
            auto path = std::experimental::filesystem::path(inp);
            std::vector<std::experimental::filesystem::path> filePaths;

            std::cout << "Checked files: \n";
            for (const auto &entry : std::experimental::filesystem::directory_iterator(path)) {
                if (is_regular_file(entry.path())) {
                    std::cout << entry.path() << std::endl;
                    File input(entry.path());
                    const auto &out = input.getData();

                    Tokenizer tokenizer(out);

                    auto tokens = tokenizer.result();

                    auto tokenToLine = tokenizer.getTokenToLine();

                    // std::cout << "\n";
                    // for (int i = 0; i < tokenToLine.size(); ++i) {
                    //     std::cout << "num: " << i << " : " << tokens[i].getId() << " : "
                    //     << tokens[i].getValue() << " : " << tokenToLine[i] << "\n";
                    // }

                    std::vector<unsigned int> chars;
                    chars.reserve(tokens.size());
                    for (const auto &t : tokens) {
                        chars.push_back(t.getId());
                    }

                    m.nextFile(chars, tokenToLine);
                    filePaths.push_back(entry.path());
                }
            }

            auto vec = m.run();
            auto highlighting = m.returnLines();

            HtmlOutput htmlOutput(config);
            htmlOutput.outputHtml(vec, filePaths, highlighting);

        } else {
            auto windows = config.getMossWindow();
            Moss m(windows.first, windows.second);

            auto path = std::experimental::filesystem::path(inp);
            std::vector<std::experimental::filesystem::path> filePaths;

            std::cout << "Checked files: \n";
            for (const auto &entry : std::experimental::filesystem::directory_iterator(path)) {
                if (is_regular_file(entry.path())) {
                    std::cout << entry.path() << std::endl;
                    File input(entry.path());
                    const auto &out = input.getData();

                    Tokenizer tokenizer(out);

                    auto tokens = tokenizer.result();

                    //  std::cout << "\n";
                    //  for (int i = 0; i < tokenToLine.size(); ++i) {
                    //      std::cout << "num: " << i << " : " << tokens[i].getId() << " : "
                    //      << tokens[i].getValue() << " : " << tokenToLine[i] << "\n";
                    //  }

                    std::vector<unsigned int> chars;
                    chars.reserve(tokens.size());
                    for (const auto &t : tokens) {
                        chars.push_back(t.getId());
                    }

                    m.nextFile(chars);
                    filePaths.push_back(entry.path());
                }
            }

            auto vec = m.run();
            auto n = filePaths.size();
            std::vector<page> highlighting(n * n, page());

            HtmlOutput htmlOutput(config);
            htmlOutput.outputHtml(vec, filePaths, highlighting);
        }
        std::cout << "Done.\n";
    }

    static void listCommands() {
        std::cout << "----GET-----\n"
                     "get resultPath - prints where the results folder is relative to the executable. Default is results. \n"
                     "get pagesPath - prints where the pages folder is relative to the executable. Default is results/pages. Cannot be changed. \n"
                     "get redColor - prints the red color code for critical similarities in the table. Default is #FF241C. \n"
                     "get greenColor - prints the green color code for the code with less amount of similarities. Default is #4EF52C. \n"
                     "get yellowColor - prints the yellow color code for possibly similar works which should be examined. Default is #F5EF35. \n"
                     "get grayColor - prints the gray color code for table diagonal. Default is #B1A9A3.\n"
                     "get codeColorMark - prints the color code for line highlighting in file comparison page. Default is #FF6F9D. \n"
                     "get thresholdGreen - prints the threshold percent for the green source codes. Default is 30.\n"
                     "get thresholdYellow - prints the threshold percent for the yellow work and start of the red source code. Default is 60.\n"
                     "get abbrInRow - prints the number of files in the row in the abbreviation page. Default is 6.\n"
                     "get highlightingThreshold - prints the threshold of tokens when to start highlighting. Default is 3.\n"
                     "get detectorType - prints the current detector type. Default is Jaccard. In Moss it is not possible to get comparison pages.\n"
                     "get jaccardWindow - prints the window size used for Jaccard detector. Default is 5. \n"
                     "get mossWindow - prints the search window size and hash window size for Moss detector. Default is 4 and 3.\n"
                     "-----SET-----\n"
                     "set resultPath - sets where the results folder is relative to the executable. Default is results. \n"
                     "set pagesPath - sets where the pages folder is relative to the executable. Default is results/pages. Cannot be changed. \n"
                     "set redColor - sets the red color code for critical similarities in the table. Default is #FF241C. \n"
                     "set greenColor - sets the green color code for the code with less amount of similarities. Default is #4EF52C. \n"
                     "set yellowColor - sets the yellow color code for possibly similar works which should be examined. Default is #F5EF35. \n"
                     "set grayColor - sets the gray color code for table diagonal. Default is #B1A9A3.\n"
                     "set codeColorMark - sets the color code for line highlighting in file comparison page. Default is #FF6F9D. \n"
                     "set thresholdGreen - sets the threshold percent for the green source codes. Default is 30.\n"
                     "set thresholdYellow - sets the threshold percent for the yellow work and start of the red source code. Default is 60.\n"
                     "set abbrInRow - sets the number of files in the row in the abbreviation page. Default is 6.\n"
                     "set highlightingThreshold - sets the threshold of tokens when to start highlighting. Default is 3.\n"
                     "set detectorType - sets the current detector type. Default is Jaccard. In Moss it is not possible to get comparison pages.\n"
                     "set jaccardWindow - sets the window size used for Jaccard detector. Default is 5. \n"
                     "set mossWindow - sets the search window size for Moss detector. Default is 4. \n"
                     "set mossHashWindow - sets the hash window size for Moss detector. Default is 3.\n"
                     "----ACTION----\n"
                     "execute - starts the detection. Creates HTML result page in resultPath.\n"
                     "quit - quits the program\n";
    }

};


int main() {
    ConsoleUI console;
    console.run();

    return 0;
}
