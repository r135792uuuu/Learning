# 指定 CMake 的最低版本要求
cmake_minimum_required(VERSION 2.8)

# 项目名称
project(multiThread)

# 指定 C++ 标准
# 后面编译多线程库，不然报错
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g -O0")

# 其他库
# include_directories(./)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../flight-control)  # 一个/../是上级文件夹
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../core/inc)

# 要编译的头文件的定义cpp
FILE(GLOB SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../Learning/C++/Struct-Advanced/customStruct.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/foo1.cpp
        )

# 添加可执行文件，将 main.cpp 添加到可执行文件
add_executable(foo1_main ${SOURCE_FILES} foo1_main.cpp)


# 链接库到可执行文件
target_link_libraries(foo1_main crypto)

# 运行
# mkdir build
# cd build
# cmake ..
# make
