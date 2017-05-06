set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -l pthread")
INCLUDE_DIRECTORIES("../include/net" "${PROJECT_SOURCE_DIR}/common/include")
add_executable(buffer_test buffer_test.cc)
add_executable(cpu_test cpu_test.cc)
