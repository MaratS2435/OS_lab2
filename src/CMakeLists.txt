cmake_minimum_required(VERSION 3.10)
project(lab3_os)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(parent parent.cpp)
add_executable(child1 child1.cpp)
add_executable(child2 child2.cpp)

target_link_libraries(parent m)
target_link_libraries(child1 m)
target_link_libraries(child2 m)