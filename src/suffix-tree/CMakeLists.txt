cmake_minimum_required(VERSION 3.2)
project(suffix-tree)

set(SOURCE_FILES
        suffix_tree.cpp
        suffix_tree.h)

add_library(suffix-tree STATIC ${SOURCE_FILES})
install(TARGETS suffix-tree DESTINATION ${YSNC_INSTALL_LIB_DIR})
install(FILES suffix_tree.h DESTINATION ${YSNC_INSTALL_INCLUDE_DIR})
