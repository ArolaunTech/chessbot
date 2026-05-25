cmake_minimum_required(VERSION 3.25)

project(chessbot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

file(GLOB_RECURSE chess_src
	"*.h"
	"*.cpp"
)

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-Wall temp)

if(temp)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

check_cxx_compiler_flag(-O3 temp2)

if(temp2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_executable(run ${chess_src})