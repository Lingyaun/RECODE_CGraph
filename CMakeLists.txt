cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(CGraph)

file(GLOB_RECURSE SRC_LIST "./src/*.cpp")

set(TUTORIAL_LIST
        T01-Simple

        )

# 编译libCGraph动态库
add_library(CGraph SHARED ${SRC_LIST})

foreach(tut ${TUTORIAL_LIST})
    add_executable(${tut} test/${tut}.cpp ${SRC_LIST})
endforeach()