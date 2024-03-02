cmake_minimum_required(VERSION 3.10)
project(kp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(SOURCES 
    main.cpp
    parser.cpp
    jobs.cpp
    parser.h
    node.h
    jobs.h
)

add_executable(main ${SOURCES})
