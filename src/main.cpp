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
    std::string path = "E:/Programming/C++/ysnc/scrapper/datasets/2_files";
    for (const auto &entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;

        File input(entry.path());
        auto out = input.getData();

        Tokenizer tokenizer(out);
        const auto& tokenizedSource = tokenizer.result();
        for (auto& token : tokenizedSource)
            std::cout << token.getPosition() << ":" << token.getValue() << ":" << token.getId() << " ";

        std::cout << "\n";
        for (int i = 0; i < tokenizer.tokenToLine.size(); ++i) {
            std::cout << i << " : " << tokenizer.tokenToLine[i] << "\n";
        }

        auto tokens = tokenizer.result();

        std::vector<unsigned char> chars;
        for (const auto &t : tokens) {
            chars.push_back(t.getId());
        }

        m.nextFile(chars);

    }

    auto vec = m.run();

    outputHtml(vec);


    return 0;
}
