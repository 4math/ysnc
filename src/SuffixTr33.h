#ifndef YSNC_SUFFIXTR33_H
#define YSNC_SUFFIXTR33_H

#include "Node.h"

class SuffixTr33 {
public:
    std::vector<std::vector<char>> files;
    Node root = Node(NULL, 0, 0);
    Node* active_node;
    char active_edge;
    int active_length;

    SuffixTr33() {

    }

    void next_file(std::vector<char> input) {
        files.push_back(std::vector<char>());
        for(int i = 0; i < input.size(); i++) {
            files[files.size() - 1].push_back(input[i]);
        }
        loop: for(int i = 0; i < input.size(); i++) {
            active_node = &root;
            active_length = 0;
            for(int j = i; j < input.size(); j++) {
                //iterirujemsa po inputu, zapolņaja derevo
            }
        }
    }
};

#endif //YSNC_SUFFIXTR33_H
