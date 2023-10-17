# 指定 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(ExampleProject)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 其他库


# 添加可执行文件，将 main.cpp 添加到可执行文件
add_executable(foo1_main foo1_main.cpp)

# 添加库，将 foo1.cpp 添加到库中
add_library(foo1 foo1.cpp)

# 链接库到可执行文件
target_link_libraries(foo1_main foo1)

# 运行
# mkdir build
# cd build
# cmake ..
# make
