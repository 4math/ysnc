cmake_minimum_required(VERSION 3.20)
project(ysnc)

set(CMAKE_CXX_STANDARD 17)

add_executable(ysnc "src/main.cpp" "src/instance.h" "src/file.h" "src/analyzer.h" "src/tokenizer.h")
