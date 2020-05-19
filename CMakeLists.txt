cmake_minimum_required(VERSION 3.0)               # cmake最低版本要求
project(sudoku VERSION 0.1.0)                     # 工程名 sudoku

message(STATUS "Identifying the OS...")
if(WIN32)
    message(STATUS "This is Windows.")
elseif(APPLE)
    message(STATUS "This is macOS.")
elseif(UNIX)
    message(STATUS "This is Linux.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")            # 添加c++11标准支持
set(CMAKE_BUILD_TYPE "Debug")                                   # Debug模式 
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb") # debug模式下 gdb相关选项
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")         # release模式下 gdb相关选项

# set(CMAKE_VERBOSE_MAKEFILE ON)                        # 开启调试 出现问题时开启
# enable_testing()                                      # 打开测试
add_definitions(-DDEBUG)                                # 相当于代码中 #define DEBUG

# add_subdirectory(test)                                # 添加test子目录
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin") # 可执行文件输出目录
include_directories("${PROJECT_SOURCE_DIR}/include")    # 头文件包含目录

aux_source_directory(src DIR_SRCS)                      # 添加源代码文件夹, 自动扫描所有文件
add_executable(sudoku ${DIR_SRCS})                      # 输出名为 sudoku 的可执行文件
