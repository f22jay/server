set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
include_directories("../include/net" "${PROJECT_SOURCE_DIR}/common/include")
add_executable(date_server date.cc)
target_link_libraries(date_server server)
