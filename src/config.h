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
        Config::red = red;
    }

    [[nodiscard]] const std::string &getYellow() const {
        return yellow;
    }

    void setYellow(const std::string &yellow) {
        Config::yellow = yellow;
    }

    [[nodiscard]] const std::string &getGreen() const {
        return green;
    }

    void setGreen(const std::string &green) {
        Config::green = green;
    }

    [[nodiscard]] const std::string &getGray() const {
        return gray;
    }

    void setGray(const std::string &gray) {
        Config::gray = gray;
    }

    const std::string &getCodeColorMark() const {
        return codeColorMark;
    }

    void setCodeColorMark(const std::string &codeMark) {
        Config::codeColorMark = codeMark;
    }

    int getThresholdGreen() const {
        return thresholdGreen;
    }

    void setThresholdGreen(int thresholdGreen) {
        Config::thresholdGreen = thresholdGreen;
    }

    int getThresholdYellow() const {
        return thresholdYellow;
    }

    void setThresholdYellow(int thresholdYellow) {
        Config::thresholdYellow = thresholdYellow;
    }

    int getAbbrInRow() const {
        return abbrInRow;
    }

    void setAbbrInRow(int abbrInRow) {
        Config::abbrInRow = abbrInRow;
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
};

#endif //YSNC_CONFIG_H
