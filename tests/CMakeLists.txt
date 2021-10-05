cmake_minimum_required(VERSION 3.2)
project(ysnc_tests)

add_subdirectory(lib/googletest)

include_directories(${YSNC_HEADERS_DIR})
include_directories(lib/googletest/googletest/include)

set(SOURCE_FILES
        main.cpp
        suffix_tree_test.cpp
        tokenizer_test.cpp
        moss_test.cpp)

add_executable(ysnc_tests ${SOURCE_FILES})
target_link_libraries(ysnc_tests
        gtest
        gtest_main)
install(TARGETS ysnc_tests DESTINATION bin)