cmake_minimum_required(VERSION 3.2)
project(tokenizer)

set(SOURCE_FILES
        tokenizer.cpp
        tokenizer.h)

add_library(tokenizer STATIC ${SOURCE_FILES})

install(TARGETS tokenizer DESTINATION ${YSNC_INSTALL_LIB_DIR})
install(FILES tokenizer.h DESTINATION ${YSNC_INSTALL_INCLUDE_DIR})