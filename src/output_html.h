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

namespace fs = std::filesystem;

std::string createHtml(const std::vector<std::vector<int>> &table) {
    std::string body =
            "<!DOCTYPE html>"
            "<html lang='en'>"
            "<head>"
            "<meta charset='UTF-8'>"
            "<title>Check results</title>"
            "<style>"
            "h1 {"
            "text-align: center;"
            "}"
            "table {"
            "margin: 0 auto;"
            "width: 40%;"
            "text-align: center;"
            "}"
            "table, th, td {"
            "border: 1px solid black;"
            "}"
            "</style>"
            "</head>"
            "<body>"
            "<h1>Plagiarism check results: </h1>"
            "<table>";

    for (int i = 0; i < table.size() + 1; ++i) {
        body += "<tr>";
        for (int j = 0; j < table[0].size() + 1; ++j) {
            // first row
            if (i == 0) {
                if (j == 0) {
                    body += "<th></th>";
                    continue;
                }
                std::stringstream ss;
                ss << "<th>" << j << "</th>";
                body += ss.str();
            }
                // next rows
            else {
                if (j == 0) {
                    std::stringstream ss;
                    ss << "<th>" << i << "</th>";
                    body += ss.str();
                    continue;
                }
                std::stringstream ss;
                ss << "<td>" << table[i - 1][j - 1] << "%</td>";
                body += ss.str();
            }
        }
        body += "</tr>";
    }
    body += "</table>";
    body += "</body>";
    body += "</html>";
    return body;
}


void outputHtml(const std::vector<std::vector<int>> &table) {
    std::string html = createHtml(table);
    fs::create_directory("results");

    std::ofstream output("results/result.html");
    output << html;
    output.close();
}

#endif //YSNC_OUTPUT_HTML_H
