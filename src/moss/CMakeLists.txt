cmake_minimum_required(VERSION 3.2)
project(moss)


set(SOURCE_FILES
        moss.h
        moss.cpp)

add_library(moss STATIC ${SOURCE_FILES})
install(TARGETS moss DESTINATION ${YSNC_INSTALL_LIB_DIR})
install(FILES moss DESTINATION ${YSNC_INSTALL_INCLUDE_DIR})
