cmake_minimum_required(VERSION 3.16)
project(first_app_1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-lglfw")

add_executable(first_app main.cpp)

find_package(glfw3 REQUIRED )
find_package(GLEW REQUIRED )
target_link_libraries(first_app GL GLEW glfw)