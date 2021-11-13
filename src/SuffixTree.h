#ifndef YSNC_SUFFIXTREE_H
#define YSNC_SUFFIXTREE_H

#include "Node.h"

class SuffixTree {
public:
    std::vector<std::vector<char>> files;
    Node root = Node(nullptr, 0, 0);
    Node* active_node;
    char active_edge;
    int active_length;

    SuffixTree() {

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

#endif //YSNC_SUFFIXTREE_H
