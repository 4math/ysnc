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
    Detector d(8);

    /*in this example, char arrays: c0, c1, c2, c3, c4
     * represent 5 different tokenized source code files*/
    std::vector<char> c0 = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
    std::vector<char> c1 = {1,2,3,5,5,6,7,8,9,0,5,2,3,4,5};
    std::vector<char> c2 = {1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
    std::vector<char> c3 = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};
    std::vector<char> c4 = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5};

    /*we push these files into our Detector using method
     * next_file(int n, char *input, int l), where:
     * n - is the number of the file (files should always be
     * enumerated form zero, to k-1);
     * input - file data;
     * l - length of data*/
    d.next_file(c0);
    d.next_file(c1);
    d.next_file(c2);
    d.next_file(c3);
    d.next_file(c4);

    /*after that, we inspect our files by calling
     * run() method*/
    std::vector<std::vector<double>> r = d.run();      //run() method returns 2D vector with results
    for(int i = 0; i < r.size(); i++) {                //we print 2D vector`s elements
        for(int j = 0; j < r[i].size(); j++) {
            printf("%7.2f", r[i][j]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /*or, we can use this method*/
    d.print_results();
}
