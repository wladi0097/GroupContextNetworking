cmake_minimum_required(VERSION 3.15)
project(Net)

set(CMAKE_CXX_STANDARD 17)

include_directories(deps/uWebSockets/uSockets/src)
include_directories(src/Models)
include_directories(src/Utils)
include_directories(src/Networking)
add_subdirectory(test)

file(GLOB SOURCES "src/**/*.cpp")
file(GLOB SOURCES_MAIN "src/*.cpp")

find_package(ZLIB)

add_custom_target(
        uSockets
        COMMAND make
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/deps/uWebSockets/uSockets/
)

add_executable(Net main.cpp ${SOURCES} ${SOURCES_MAIN})

target_link_libraries(Net ZLIB::ZLIB)
target_link_libraries(Net ${PROJECT_SOURCE_DIR}/deps/uWebSockets/uSockets/uSockets.a)
add_dependencies(Net uSockets)
