cmake_minimum_required(VERSION 3.10)
project(temp_monitor_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic -O2)

file(GLOB SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_executable(temp_monitor_cpp_demo src/main.cpp ${SOURCES})
target_include_directories(temp_monitor_cpp_demo PRIVATE include)

enable_testing()
add_executable(temp_monitor_cpp_tests tests/test_suite.cpp ${SOURCES})
target_include_directories(temp_monitor_cpp_tests PRIVATE include tests)
add_test(NAME UnitTests COMMAND temp_monitor_cpp_tests)
