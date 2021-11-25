//
// Created by admin on 11/11/2021.
//

#ifndef YSNC_OUTPUT_HTML_H
#define YSNC_OUTPUT_HTML_H

#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "config.h"

namespace fs = std::filesystem;

class HtmlOutput {
public:
    explicit HtmlOutput(const Config &config) : config(config) {
        fs::create_directory(config.getResultPath());
        fs::create_directory(config.getPagesPath());
    }

    void outputHtml(const std::vector<std::vector<double>> &table,
                    const std::vector<fs::path> &files,
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

    void outputComparisonPage(const fs::path &firstFile, const fs::path &secondFile,
                              const page &comparisonPage) {
        std::string html = createComparisonPage(firstFile, secondFile, comparisonPage);

        std::ofstream output(config.getPagesPath() / pageName(firstFile, secondFile));
        output << html;
        output.close();
    }


private:
    Config config;

    [[nodiscard]] static std::string pageName(const fs::path &firstFile, const fs::path &secondFile) {
        std::stringstream filename;
        filename << firstFile.stem().string() << "-" << secondFile.stem().string() << ".html";
        return filename.str();
    }

    [[nodiscard]] std::vector<std::string> readSourceCode(const fs::path &path, const page &comparisonPage,
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
        auto& inputVec = input.getData();
        std::stringstream codeLine;
        unsigned int lineNumber = 1;
        for (auto& line : inputVec) {
            auto pos = line.find('<');
            if (pos != std::string::npos) {
                line.replace(pos, 1, "&lt;");
            }

            pos = line.find('>');
            if (pos != std::string::npos) {
                line.replace(pos, 1, "&gt;");
            }

            if (highlightedLines.find(lineNumber) != highlightedLines.end() &&
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

    [[nodiscard]]  std::string createComparisonPage(const fs::path &firstFile, const fs::path &secondFile,
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
                                                const std::vector<fs::path> &files) const {
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
                std::string classColor;
                if (value < config.getThresholdGreen()) {
                    classColor = "green";
                } else if (value < config.getThresholdYellow()) {
                    classColor = "yellow";
                } else {
                    classColor = "red";
                }

                body << "<td class=" << classColor << ">" <<
                     "<a href=pages/" <<
                     pageName(files[i - 1], files[j - 1]) <<
                     ">" << std::fixed << std::setprecision(1) <<
                     value << "%</a>" << "</td>";
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


#endif //YSNC_OUTPUT_HTML_H
