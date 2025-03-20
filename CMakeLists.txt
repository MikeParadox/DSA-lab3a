cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")                
# set(CMAKE_CXX_COMPILER "/usr/bin/g++-14")

project(lab3a
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)."
 TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()


enable_testing()

add_executable(main Source.cpp)

target_compile_options(main PUBLIC -fopenmp -g -ggdb
-Werror -Wall -Wpedantic -Wno-parentheses)
 # -ftime-report) # to get detailed compilation timer
 # -finput-charset=UTF-8 # enable UTF-8 support for GCC


