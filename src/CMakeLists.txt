cmake_minimum_required(VERSION 3.2)
project(ysnc)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ysnc main.cpp Detector.h Moss.h Node.h SuffixTree.h)

target_link_libraries(ysnc suffix-tree tokenizer moss)
install(TARGETS ysnc DESTINATION ${YSNC_INSTALL_BIN_DIR})