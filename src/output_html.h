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
    void outputHtml(const std::vector<std::vector<double>> &table) {
        std::string html = createHtml(table);
        fs::create_directory("results");

        std::ofstream output("results/result.html");
        output << html;
        output.close();
    }

    std::string red = "#FF241C";
    std::string yellow = "#F5EF35";
    std::string green = "#4EF52C";
    std::string gray = "#B1A9A3";

private:
    [[nodiscard]] std::string createHtml(const std::vector<std::vector<double>> &table) const {
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
