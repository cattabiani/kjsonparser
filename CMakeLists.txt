cmake_minimum_required(VERSION 3.10)
project(cpp_template)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_DIR src)
file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(cpp_template ${SOURCES})
set(CMAKE_GENERATOR "Ninja")
