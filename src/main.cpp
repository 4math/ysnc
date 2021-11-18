#include <iostream>
#include <filesystem>
#include "tokenizer.h"
#include "moss.h"
#include "instance.h"
#include "output_html.h"

namespace fs = std::filesystem;

int main() {
    Moss m(4, 4);
//    Detector m(4);
    std::string path = "E:/Programming/C++/ysnc/scrapper/datasets/1602A";
    for (const auto &entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;

        File input(entry.path());
        const auto& out = input.getData();

        Tokenizer tokenizer(out);

        auto tokens = tokenizer.result();

        std::vector<unsigned char> chars;
        for (const auto &t : tokens) {
            chars.push_back(t.getId());
        }

        m.nextFile(chars);

    }

    auto vec = m.run();

    HtmlOutput htmlOutput;
    htmlOutput.outputHtml(vec);


    return 0;
}
