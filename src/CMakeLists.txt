cmake_minimum_required(VERSION 3.17)
project(ysnc)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(suffix-tree)
add_subdirectory(tokenizer)

add_executable(ysnc main.cpp)

target_link_libraries(ysnc suffix-tree tokenizer)
install(TARGETS ysnc DESTINATION ${YSNC_INSTALL_BIN_DIR})