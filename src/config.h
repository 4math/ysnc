//
// Created by admin on 19/11/2021.
//

#ifndef YSNC_CONFIG_H
#define YSNC_CONFIG_H

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// In the better times would be better to implement builder pattern
class Config {
public:
    Config() = default;
    ~Config() = default;
    Config(const Config& other) = default;

    enum Detector {
        JaccardIndex,
        Moss
    };

    [[nodiscard]] const fs::path &getResultPath() const {
        return resultPath;
    }

    void setResultPath(fs::path path) {
        resultPath = std::move(path);
        pagesPath = resultPath / fs::path("pages");
    }

    [[nodiscard]] const fs::path &getPagesPath() const {
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
    fs::path resultPath = fs::path("results");
    fs::path pagesPath = resultPath / fs::path("pages");
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
            {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
            {'4', 4}, {'5', 6}, {'6', 6}, {'7', 7},
            {'8', 8}, {'9', 9}, {'A', 10}, {'B', 11},
            {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}
    };

    void validateColorHexString(const std::string& color) {
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

#endif //YSNC_CONFIG_H
