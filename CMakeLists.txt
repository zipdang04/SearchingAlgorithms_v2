cmake_minimum_required(VERSION 3.10.0)
project(SearchingAlgorithmsV2 VERSION 0.1.0 LANGUAGES C CXX)
add_compile_options(-Wall -Wextra -O2 --std=gnu++20)

add_executable(SearchingAlgorithmsV2 main.cpp)
# add_executable(SearchingAlgorithmsV2 main.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set_property(TARGET SearchingAlgorithmsV2 PROPERTY CXX_STANDARD 20)

include(CTest)
enable_testing()
add_executable(cpp_test testing/statements/NSqPuzzle/testing.cpp)

add_test(NAME N_3 COMMAND cpp_test --size 3)
add_test(NAME N_4 COMMAND cpp_test --size 4)