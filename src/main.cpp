#include <iostream>
#include <tokenizer.h>
#include <suffix_tree.h>
#include <moss.h>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include "Detector.h"

int main() {
    /*to detect some plagiarism, we at first create an object:
     * Detector d(int k, int w), where "k" is the number
     * of files to inspect; and "w" is the length of the sliding
     * window*/
    Detector d(5, 8);

    /*in this example, char arrays: c0, c1, c2, c3, c4
     * represent 5 different tokenized source code files*/
    char c0[15] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
    char c1[15] = {1,2,3,5,5,6,7,8,9,0,5,2,3,4,5};
    char c2[16] = {1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
    char c3[15] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
    char c4[15] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};

    /*we push these files into our Detector using method
     * next_file(int n, char *input, int l), where:
     * n - is the number of the file (files should always be
     * enumerated form zero, to k-1);
     * input - file data;
     * l - length of data*/
    d.next_file(0, c0, 15);
    d.next_file(1, c1, 15);
    d.next_file(2, c2, 16);
    d.next_file(3, c3, 15);
    d.next_file(4, c4, 15);

    /*after that, we inspect our files by calling
     * run() method*/
    d.run();

    /*we also may print results in a console*/
    d.print_results();
}
