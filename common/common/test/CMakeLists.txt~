cmake_minimum_required(VERSION 2.8)
project(common)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

ADD_SUBDIRECTORY(test)
AUX_SOURCE_DIRECTORY(src DIR_SRC)
INCLUDE_DIRECTORIES(./include)

LINK_LIBRARIES("pthread")
ADD_LIBRARY(common ${DIR_SRC})
