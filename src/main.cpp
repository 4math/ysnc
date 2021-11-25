#include <iostream>
#include <filesystem>
#include <vector>
#include "tokenizer.h"
#include "moss.h"
#include "detector.h"
#include "instance.h"
#include "output_html.h"

namespace fs = std::filesystem;

int main() {
//    Moss moss(4, 4);
    Detector m(5);
    std::string path = "E:/Programming/C++/ysnc/scrapper/datasets/handmade2";
    std::vector<fs::path> filePaths;
    // TODO: check if the entry is file
    for (const auto &entry : fs::directory_iterator(path)) {
        if (is_regular_file(entry.path())) {
            std::cout << entry.path() << std::endl;
            File input(entry.path());
            const auto &out = input.getData();

            Tokenizer tokenizer(out);

            auto tokens = tokenizer.result();

            auto tokenToLine = tokenizer.getTokenToLine();

            std::cout << "\n";
            for (int i = 0; i < tokenToLine.size(); ++i) {
                std::cout << "num: " << i << " : " << tokens[i].getId() << " : " << tokens[i].getValue() << " : " << tokenToLine[i] << "\n";
            }

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

    Config config;
    HtmlOutput htmlOutput(config);
    htmlOutput.outputHtml(vec, filePaths, highlighting);


    return 0;
}
