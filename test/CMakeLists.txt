set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
INCLUDE_DIRECTORIES("../include/net" "${PROJECT_SOURCE_DIR}/common/include")
add_executable(buffer_test buffer_test.cc)
