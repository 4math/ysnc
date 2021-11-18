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

namespace fs = std::filesystem;

class HtmlOutput {
public:
    HtmlOutput() {
        fs::create_directory("results");
        fs::create_directory("results/pages");
    }

    void outputHtml(const std::vector<std::vector<double>> &table) {
        std::string html = createResultTable(table);

        std::ofstream output("results/result.html");
        output << html;
        output.close();
    }

    static void outputComparisonPage(const fs::path &firstFile, const fs::path &secondFile) {
        std::string html = createComparisonPage(firstFile, secondFile);

        std::stringstream filename;
        filename << firstFile.stem().string() << "-" << secondFile.stem().string() << ".html";
        std::ofstream output("results/pages/" + filename.str());
        output << html;
        output.close();
    }

    std::string red = "#FF241C";
    std::string yellow = "#F5EF35";
    std::string green = "#4EF52C";
    std::string gray = "#B1A9A3";

private:
    [[nodiscard]] static std::vector<std::string> readSourceCode(const fs::path &path) {
        // It is easier to use vector since
        // code highlighting depends on the line number
        std::vector<std::string> codeLines;
        std::ifstream inputFile(path);
        std::stringstream codeLine;
        std::string line;
        while (std::getline(inputFile, line)) {
            auto pos = line.find('<');
            if (pos != std::string::npos) {
                line.replace(pos, 1, "&lt;");
            }

            pos = line.find('>');
            if (pos != std::string::npos) {
                line.replace(pos, 1, "&gt;");
            }

            codeLine << "<code>" << line << "</code>\n";
            codeLines.push_back(codeLine.str());
            codeLine.str("");
        }
        return codeLines;
    }

    [[nodiscard]] static std::string createComparisonPage(const fs::path &firstFile, const fs::path &secondFile) {
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
             "<p>Filename: " << firstFile.filename().string() <<
             "</p>"
             "<pre class='code'>";

        auto firstSourceCode = readSourceCode(firstFile);

        for (const auto &line: firstSourceCode) {
            body << line;
        }

        body << "</pre>";
        // closing source div
        body << "</div>";

        body <<
             "<div class='source'>"
             "<p>Filename: " << secondFile.filename().string() <<
             "</p>"
             "<pre class='code'>";

        auto secondSourceCode = readSourceCode(secondFile);

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

    [[nodiscard]] std::string createResultTable(const std::vector<std::vector<double>> &table) const {
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
             "background-color: " << green <<
             "}"
             ".red {"
             "background-color: " << red <<
             "}"
             ".yellow {"
             "background-color: " << yellow <<
             "}"
             ".gray {"
             "background-color: " << gray <<
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
                if (value < 40) {
                    classColor = "green";
                } else if (value < 60) {
                    classColor = "yellow";
                } else {
                    classColor = "red";
                }

                body << "<td class=" << classColor << ">" <<
                     std::fixed << std::setprecision(1) <<
                     value << "%</td>";
            }
            body << "</tr>";
        }
        body << "</table>";
        body << "</body>";
        body << "</html>";
        return body.str();
    }


};


#endif //YSNC_OUTPUT_HTML_H
