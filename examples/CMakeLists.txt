set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
INCLUDE_DIRECTORIES("../include/net" "${PROJECT_SOURCE_DIR}/common/include")
ADD_EXECUTABLE(date_server date.cc)
TARGET_LINK_libraries(date_server server)
