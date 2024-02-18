cmake_minimum_required(VERSION 3.17)
project(os_lab_4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(_first SHARED realization_1.cpp)
add_library(_second SHARED realization_2.cpp)

add_executable(static_main Static_Main.cpp)
add_executable(dynamic_main Dynamic_Main.cpp)

target_link_libraries(static_main PRIVATE _first)
