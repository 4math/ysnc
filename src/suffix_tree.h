#ifndef YSNC_SUFFIXTREE_H
#define YSNC_SUFFIXTREE_H

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


class Node {
public:
    std::vector<Node*> children;
    int start;
    int end;
    std::pair<int, int> p;
    Node* parent;

    Node(Node* parent, int start, int end) {
        this -> parent = parent;
        this -> start = start;
        this -> end = end;
        for(int i = 0; i < 256; i++) {
            children.push_back(NULL);
        }
    }

    /*method to split the edge between upper and lower nodes*/
    //c - is upper node`s edge number
    //pos - position of split
    //char_on_pos - char on position of split
    Node* split_edge(char c, int pos, char char_on_pos) {
        //create splitting node
        //splitting node`s parent is upper node
        //splitting node`s start is where lower node used to start
        //splitting node`s end is "pos" - position of split
        Node n = Node(this, this->children[c] -> start, pos);
        //lower node`s start is now at pos
        this->children[c] -> start = pos;
        //lower node`s parent is now splitting node
        this->children[c]->parent = &n;
        //splitting node`s child is lower node
        n.children[char_on_pos] = this->children[c];
        //upper node`s child is splitting node
        this->children[c] = &n;
    }

    void add_node_to_node(char c, int start, int end) {
        *this -> children[c] = Node(this, start, end);
    }
};

#endif //YSNC_SUFFIXTREE_H
