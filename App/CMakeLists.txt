cmake_minimum_required(VERSION 4.0)

project(App)

add_executable(App src/main.cpp)

target_link_libraries(App PRIVATE BadFileFinder)
