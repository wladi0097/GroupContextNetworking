file(GLOB TESTS "./tests/**/*.cpp")
file(GLOB SOURCES "../src/**/*.cpp")

add_executable(Catch_tests_run tests/MainInterfaceTest.cpp ${TESTS} ${SOURCES})
target_link_libraries(Catch_tests_run)