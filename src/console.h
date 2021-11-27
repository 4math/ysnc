//
// Created by admin on 25/11/2021.
//

#ifndef YSNC_CONSOLE_H
#define YSNC_CONSOLE_H

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <iostream>
#include "config.h"
#include "jaccard_index.h"
#include "moss.h"
#include "tokenizer.h"
#include "output_html.h"
#include "file.h"

class ConsoleUI {
public:

    ConsoleUI() = default;

    void run() {
        std::cout << "Write help or ? to see commands\n";
        std::string input;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);
            if (input.empty()) std::getline(std::cin, input);
            std::vector<std::string> v = split(input, ' ');
            if (v.size() > 2 || v.empty()) {
                std::cout << "Incorrect number of arguments!\n";
                continue;
            }

            auto command = v[0];
            std::string arg;

            if (v.size() == 2) {
                arg = v[1];
            } else {
                arg = "";
            }

            if (command == "get") {
                if (arg.empty()) {
                    continue;
                } else if (arg == "resultPath") {
                    std::cout << config.getResultPath() << "\n";
                } else if (arg == "resultPath") {
                    std::cout << config.getResultPath() << "\n";
                } else if (arg == "pagesPath") {
                    std::cout << config.getPagesPath() << "\n";
                } else if (arg == "redColor") {
                    std::cout << config.getRed() << "\n";
                } else if (arg == "greenColor") {
                    std::cout << config.getGreen() << "\n";
                } else if (arg == "yellowColor") {
                    std::cout << config.getYellow() << "\n";
                } else if (arg == "grayColor") {
                    std::cout << config.getGray() << "\n";
                } else if (arg == "codeColorMark") {
                    std::cout << config.getCodeColorMark() << "\n";
                } else if (arg == "thresholdGreen") {
                    std::cout << config.getThresholdGreen() << "\n";
                } else if (arg == "thresholdYellow") {
                    std::cout << config.getThresholdYellow() << "\n";
                } else if (arg == "abbrInRow") {
                    std::cout << config.getAbbrInRow() << "\n";
                } else if (arg == "highlightingThreshold") {
                    std::cout << config.getHighlightingThreshold() << "\n";
                } else if (arg == "detectorType") {
                    std::string detector;
                    switch (config.getDetectorType()) {
                        case Config::JaccardIndex: {
                            detector = "Jaccard";
                            break;
                        }
                        case Config::Moss: {
                            detector = "Moss";
                            break;
                        }
                    }
                    std::cout << detector << "\n";
                } else if (arg == "jaccardWindow") {
                    std::cout << config.getJaccardWindow() << "\n";
                } else if (arg == "mossWindow") {
                    auto pair = config.getMossWindow();
                    std::cout << "window size: " << pair.first << " | hash window size: " << pair.second << "\n";
                }
            } else if (command == "set") {
                if (arg.empty()) {
                    continue;
                } else if(arg == "resultPath"){
                    auto inp = getUserInput("Result path: ");
                    try {
                        config.setResultPath(inp);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "redColor") {
                    auto i = getUserInput("Red color hex: ");
                    try {
                        config.setRed(i);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "greenColor") {
                    auto i = getUserInput("Green color hex: ");
                    try {
                        config.setGreen(i);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "yellowColor") {
                    auto i = getUserInput("Yellow color hex: ");
                    try {
                        config.setYellow(i);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "grayColor") {
                    auto i = getUserInput("Gray color hex: ");
                    try {
                        config.setGray(i);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "codeColorMark") {
                    auto i = getUserInput("Code highlighting color hex: ");
                    try {
                        config.setCodeColorMark(i);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "thresholdGreen") {
                    auto i = getIntUserInput("Threshold of the green color: ");
                    try {
                        config.setThresholdGreen(i);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "thresholdYellow") {
                    auto i = getIntUserInput("Threshold of the yellow color: ");
                    try {
                        config.setThresholdYellow(i);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "abbrInRow") {
                    auto i = getIntUserInput("Amount of file names in the table row: ");
                    try {
                        config.setAbbrInRow(i);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "highlightingThreshold") {
                    auto i = getIntUserInput("Threshold of the highlighting: ");
                    try {
                        config.setHighlightingThreshold(i);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "detectorType") {
                    auto i = getUserInput("Jaccard or Moss: ");
                    if (i == "Jaccard") {
                        config.setDetectorType(Config::Detector::JaccardIndex);
                    } else if (i == "Moss") {
                        config.setDetectorType(Config::Detector::Moss);
                    } else {
                        std::cout << "Incorrect detector type!\n";
                    }
                } else if(arg == "jaccardWindow") {
                    auto i = getIntUserInput("Jaccard window size: ");
                    try {
                        config.setJaccardWindow(i);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "mossWindow") {
                    auto i = getIntUserInput("Moss window size: ");
                    auto value = config.getMossWindow();
                    value.first = i;
                    try {
                        config.setMossWindow(value);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                } else if(arg == "mossHashWindow") {
                    auto i = getIntUserInput("Moss hash window size: ");
                    auto value = config.getMossWindow();
                    value.second = i;
                    try {
                        config.setMossWindow(value);
                    } catch (const std::exception& e) {
                        std::cout << e.what() << "\n";
                    }
                }
            } else if (command == "commands" || command == "help" || command == "?") {
                listCommands();
            } else if (command == "execute") {
                try {
                    execute();
                } catch (const std::exception &e) {
                    std::cout << e.what() << "\n";
                    continue;
                }
            } else if (command == "quit") {
                break;
            }
        }
    }



private:
    Config config;

    template <typename Out>
    void split(const std::string &s, char delim, Out result) {
        std::istringstream iss(s);
        std::string item;
        while (std::getline(iss, item, delim)) {
            *result++ = item;
        }
    }

    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    static std::string getUserInput(const std::string& message) {
        std::string input;
        std::cout << message;
        std::cin >> input;
        return input;
    }

    static int getIntUserInput(const std::string& message) {
        std::string input;
        std::cout << message;
        std::cin >> input;
        int i;
        try {
            i = std::stoi(input);
        } catch (const std::exception&) {
            std::cout << "Please, input an integer!\n";
        }
        return i;
    }

    void execute() {
        auto inp = getUserInput("Path to the folder with source codes: ");
        auto detector = config.getDetectorType();

        if (detector == Config::JaccardIndex) {
            JaccardIndex m(config.getJaccardWindow());
            auto path = fs::path(inp);
            std::vector<fs::path> filePaths;

            std::cout << "Checked files: \n";
            for (const auto &entry : fs::directory_iterator(path)) {
                if (is_regular_file(entry.path())) {
                    std::cout << entry.path() << std::endl;
                    File input(entry.path());
                    const auto &out = input.getData();

                    Tokenizer tokenizer(out);

                    auto tokens = tokenizer.result();

                    auto tokenToLine = tokenizer.getTokenToLine();

                    // std::cout << "\n";
                    // for (int i = 0; i < tokenToLine.size(); ++i) {
                    //     std::cout << "num: " << i << " : " << tokens[i].getId() << " : "
                    //     << tokens[i].getValue() << " : " << tokenToLine[i] << "\n";
                    // }

                    std::vector<unsigned int> chars;
                    chars.reserve(tokens.size());
                    for (const auto &t : tokens) {
                        chars.push_back(t.getId());
                    }

                    m.nextFile(chars, tokenToLine);
                    filePaths.push_back(entry.path());
                }
            }

            auto vec = m.run();
            auto highlighting = m.returnLines();

            HtmlOutput htmlOutput(config);
            htmlOutput.outputHtml(vec, filePaths, highlighting);

        } else {
            auto windows = config.getMossWindow();
            Moss m(windows.first, windows.second);

            auto path = fs::path(inp);
            std::vector<fs::path> filePaths;

            std::cout << "Checked files: \n";
            for (const auto &entry : fs::directory_iterator(path)) {
                if (is_regular_file(entry.path())) {
                    std::cout << entry.path() << std::endl;
                    File input(entry.path());
                    const auto &out = input.getData();

                    Tokenizer tokenizer(out);

                    auto tokens = tokenizer.result();

                    //  std::cout << "\n";
                    //  for (int i = 0; i < tokenToLine.size(); ++i) {
                    //      std::cout << "num: " << i << " : " << tokens[i].getId() << " : "
                    //      << tokens[i].getValue() << " : " << tokenToLine[i] << "\n";
                    //  }

                    std::vector<unsigned int> chars;
                    chars.reserve(tokens.size());
                    for (const auto &t : tokens) {
                        chars.push_back(t.getId());
                    }

                    m.nextFile(chars);
                    filePaths.push_back(entry.path());
                }
            }

            auto vec = m.run();
            auto n = filePaths.size();
            std::vector<page> highlighting(n * n, page());

            HtmlOutput htmlOutput(config);
            htmlOutput.outputHtml(vec, filePaths, highlighting);
        }
        std::cout << "Done.\n";
    }

    static void listCommands() {
        std::cout << "----GET-----\n"
                     "get resultPath - prints where the results folder is relative to the executable. Default is results. \n"
                     "get pagesPath - prints where the pages folder is relative to the executable. Default is results/pages. Cannot be changed. \n"
                     "get redColor - prints the red color code for critical similarities in the table. Default is #FF241C. \n"
                     "get greenColor - prints the green color code for the code with less amount of similarities. Default is #4EF52C. \n"
                     "get yellowColor - prints the yellow color code for possibly similar works which should be examined. Default is #F5EF35. \n"
                     "get grayColor - prints the gray color code for table diagonal. Default is #B1A9A3.\n"
                     "get codeColorMark - prints the color code for line highlighting in file comparison page. Default is #FF6F9D. \n"
                     "get thresholdGreen - prints the threshold percent for the green source codes. Default is 30.\n"
                     "get thresholdYellow - prints the threshold percent for the yellow work and start of the red source code. Default is 60.\n"
                     "get abbrInRow - prints the number of files in the row in the abbreviation page. Default is 6.\n"
                     "get highlightingThreshold - prints the threshold of tokens when to start highlighting. Default is 3.\n"
                     "get detectorType - prints the current detector type. Default is Jaccard. In Moss it is not possible to get comparison pages.\n"
                     "get jaccardWindow - prints the window size used for Jaccard detector. Default is 5. \n"
                     "get mossWindow - prints the search window size and hash window size for Moss detector. Default is 4 and 3.\n"
                     "-----SET-----\n"
                     "set resultPath - sets where the results folder is relative to the executable. Default is results. \n"
                     "set pagesPath - sets where the pages folder is relative to the executable. Default is results/pages. Cannot be changed. \n"
                     "set redColor - sets the red color code for critical similarities in the table. Default is #FF241C. \n"
                     "set greenColor - sets the green color code for the code with less amount of similarities. Default is #4EF52C. \n"
                     "set yellowColor - sets the yellow color code for possibly similar works which should be examined. Default is #F5EF35. \n"
                     "set grayColor - sets the gray color code for table diagonal. Default is #B1A9A3.\n"
                     "set codeColorMark - sets the color code for line highlighting in file comparison page. Default is #FF6F9D. \n"
                     "set thresholdGreen - sets the threshold percent for the green source codes. Default is 30.\n"
                     "set thresholdYellow - sets the threshold percent for the yellow work and start of the red source code. Default is 60.\n"
                     "set abbrInRow - sets the number of files in the row in the abbreviation page. Default is 6.\n"
                     "set highlightingThreshold - sets the threshold of tokens when to start highlighting. Default is 3.\n"
                     "set detectorType - sets the current detector type. Default is Jaccard. In Moss it is not possible to get comparison pages.\n"
                     "set jaccardWindow - sets the window size used for Jaccard detector. Default is 5. \n"
                     "set mossWindow - sets the search window size for Moss detector. Default is 4. \n"
                     "set mossHashWindow - sets the hash window size for Moss detector. Default is 3.\n"
                     "----ACTION----\n"
                     "execute - starts the detection. Creates HTML result page in resultPath.\n"
                     "quit - quits the program\n";
    }

};

#endif //YSNC_CONSOLE_H
