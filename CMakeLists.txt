# 规定最小版本
cmake_minimum_required(VERSION 2.8)

#项目名称
project(miniHTTP)

#添加可执行文件
add_executable(miniHTTP.out main.cpp custom_handle.cpp http.cpp)

#链接线程支持库
target_link_libraries(miniHTTP.out pthread)