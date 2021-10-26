cmake_minimum_required(VERSION 3.2)
project(ysnc)

set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_subdirectory(suffix-tree)
add_subdirectory(tokenizer)
add_subdirectory(moss)

add_executable(ysnc main.cpp Detector.h Tuple.h)

target_link_libraries(ysnc suffix-tree tokenizer moss)
install(TARGETS ysnc DESTINATION ${YSNC_INSTALL_BIN_DIR})