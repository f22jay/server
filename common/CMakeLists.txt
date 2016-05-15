#cmake_minimum_required(VERSION 3.2)
project(common)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
LINK_LIBRARIES("pthread")
#include_directories("./include")
set(include_directories $PROJECT_BINARY_DIR/include)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(test)
