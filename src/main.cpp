#include <iostream>
#include "tokenizer.h"
#include "moss.h"
#include "instance.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main() {
    Moss m(4, 4);
    std::string path = "E:/Programming/C++/ysnc/scrapper/datasets/1602A";
    for (const auto &entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
        char ch;
        std::fstream fin(entry.path(), std::fstream::in);
        std::vector<char> v;
        while (fin >> std::noskipws >> ch) {

            v.push_back(ch);
        }
        m.nextFile(v);
    }
    auto vec = m.run();
    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < vec[i].size(); j++) {
            std::cout << vec[i][j];
        }
        std::cout << std::endl;
    }
    return 0;
}
