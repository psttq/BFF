cmake_minimum_required(VERSION 3.22)

project(BadFileFinder)

set(CMAKE_CXX_STANDARD 20)

file(GLOB BFF_SRC src/*.cpp src/**/*.cpp)

add_library(BadFileFinder ${BFF_SRC})

target_link_libraries(BadFileFinder PRIVATE ssl crypto)

target_include_directories(BadFileFinder PUBLIC include)
