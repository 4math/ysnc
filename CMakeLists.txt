cmake_minimum_required(VERSION 3.2)
project(ysnc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(YSNC_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(YSNC_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(YSNC_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(YSNC_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${YSNC_INSTALL_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(tests)