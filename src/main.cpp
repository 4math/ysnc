#include <iostream>
#include <tokenizer.h>
#include <suffix_tree.h>
#include <moss.h>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include "Detector.h"

//std::vector<std::string> readDirectory(std::string directory) {
//    std::vector<std::string> v;
//    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
//        v.push_back(entry.path().string());
//    }
//    return v;
//}
//
//void write_directory() {
//    std::string path = "";
//    std::vector<std::string> v = readDirectory(path);
//    for (int i = 0; i < v.size(); i++) {
//        std::cout << v[i] << std::endl;
//    }
//}

int main() {
    Detector d(5, 8);
    char c0[15] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
    char c1[15] = {1,2,3,5,5,6,7,8,9,0,5,2,3,4,5};
    char c2[16] = {1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
    char c3[15] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
    char c4[15] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
    d.next_file(0, c0, 15);
    d.next_file(1, c1, 15);
    d.next_file(2, c2, 16);
    d.next_file(3, c3, 15);
    d.next_file(4, c4, 15);
    d.run();
    d.print_results();
}
