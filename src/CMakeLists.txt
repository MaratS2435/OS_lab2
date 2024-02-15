cmake_minimum_required(VERSION 3.10)
project(lab2_os)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mainAsync mainAsync.cpp)
add_executable(mainSync mainSync.cpp)

target_link_libraries(mainAsync m)
target_link_libraries(mainSync m)