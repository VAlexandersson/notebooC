cmake_minimum_required(VERSION 3.10)

project(MyCProject
    DESCRIPTION "A simple C project"
    LANGUAGES C
)

add_executable(notebooC
    src/notebooc.c
)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -pedantic")

include_directories(include)

