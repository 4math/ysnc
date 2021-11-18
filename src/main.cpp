#include <iostream>
#include <filesystem>
#include "tokenizer.h"
#include "moss.h"
#include "detector.h"
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
        auto out = input.getData();

        Tokenizer tokenizer(out);

        auto tokens = tokenizer.result();

        std::vector<unsigned char> chars;
        for (const auto &t : tokens) {
            chars.push_back(t.getId());
        }

        //now we need to push std::vector<int> instead of std::vector<char>
        //m.nextFile(chars);

    }

    auto vec = m.run();

    outputHtml(vec);


    return 0;
}
