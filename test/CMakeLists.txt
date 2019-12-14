file(GLOB TESTS "**/*.cpp")
file(GLOB SOURCES "../src/**/*.cpp")

add_executable(Catch_tests_run ${TESTS} ${SOURCES})
target_link_libraries(Catch_tests_run)