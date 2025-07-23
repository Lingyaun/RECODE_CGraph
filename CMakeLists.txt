cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(CGraph)

# 如果开启此宏定义，则CGraph执行过程中，不会在控制台打印任何信息
# add_definitions(-D_CGRAPH_SILENCE_)

file(GLOB_RECURSE SRC_LIST "./src/*.cpp")

set(TUTORIAL_LIST
        T01-Simple
        T02-Cluster
        T03-Region
        T04-Complex
        T05-Param
        T06-Condition
        T07-MultiPipeline
        T08-UThreadPool
        )

# 编译libCGraph动态库
add_library(CGraph SHARED ${SRC_LIST})

# 查找并链接线程库
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

foreach(tes ${TUTORIAL_LIST})
    add_executable(${tes} test/${tes}.cpp ${SRC_LIST})
    target_link_libraries(${tes} Threads::Threads)
endforeach()

target_link_libraries(CGraph Threads::Threads)