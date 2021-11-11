cmake_minimum_required(VERSION 3.2)
project(ysnc)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES main.cpp)

set(HEADER_FILES
        tokenizer.h
        suffix_tree.h
        moss.h
        instance.h
        output_html.h)

add_executable(ysnc ${SOURCE_FILES} ${HEADER_FILES})

install(TARGETS ysnc DESTINATION ${YSNC_INSTALL_BIN_DIR})
install(FILES ${HEADER_FILES} DESTINATION ${YSNC_INSTALL_INCLUDE_DIR})