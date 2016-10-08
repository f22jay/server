cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

AUX_SOURCE_DIRECTORY(. DIR_SRC)
INCLUDE_DIRECTORIES(../include)
ADD_EXECUTABLE(log_test test.cc)
TARGET_LINK_LIBRARIES(log_test common)
