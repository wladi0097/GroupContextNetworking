cmake_minimum_required(VERSION 3.15)
project(Net)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/Models)
include_directories(src/Utils)
add_subdirectory(test)

file(GLOB SOURCES "src/**/*.cpp")
file(GLOB SOURCES_MAIN "src/*.cpp")

add_executable(Net main.cpp ${SOURCES} ${SOURCES_MAIN})