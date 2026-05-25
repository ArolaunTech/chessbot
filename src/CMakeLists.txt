cmake_minimum_required(VERSION 3.25)

project(chessbot LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

file(GLOB_RECURSE chess_src
	"*.h"
	"*.cpp"
)

add_subdirectory("libs/glfw-3.4")

add_library (glad STATIC "libs/glad/src/glad.c")
target_include_directories(glad PUBLIC "libs/glad/include")

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
target_link_libraries(run glfw glad)
target_include_directories(run PUBLIC "libs/glad/include")