#include <iostream>
#include <filesystem>
#include <vector>
#include "tokenizer.h"
#include "moss.h"
#include "instance.h"
#include "output_html.h"

namespace fs = std::filesystem;

int main() {
    Moss m(4, 4);
//    Detector m(4);
    std::string path = "E:/Programming/C++/ysnc/scrapper/datasets/1607D";
    std::vector<fs::path> filePaths;
    // TODO: check if the entry is file
    for (const auto &entry : fs::directory_iterator(path)) {
        if (is_regular_file(entry.path())) {
            std::cout << entry.path() << std::endl;
            File input(entry.path());
            const auto &out = input.getData();

            Tokenizer tokenizer(out);

            auto tokens = tokenizer.result();

            std::vector<unsigned char> chars;
            chars.reserve(tokens.size());
            for (const auto &t : tokens) {
                chars.push_back(t.getId());
            }

            m.nextFile(chars);
            filePaths.push_back(entry.path());
        }
    }

    auto vec = m.run();

    HtmlOutput htmlOutput;
    htmlOutput.outputHtml(vec, filePaths);


    return 0;
}
