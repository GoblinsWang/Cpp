# cmake最低版本限定
cmake_minimum_required(VERSION 3.0)

# 项目名称
project(SOLIDER_GUN)

# 设置编译的一些参数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# 编译类型选择，debug or release
set(CMAKE_BUILD_TYPE Debug)

# 包含头文件所在的目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 定义SRC变量，其值为当前目录下所有的源代码文件
aux_source_directory(./src SRC)

# 生成可执行文件
add_executable(my_cmake_exe main.cpp  ${SRC})



