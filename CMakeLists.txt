cmake_minimum_required(VERSION 3.10.0)
project(SearchingAlgorithmsV2 VERSION 0.1.0 LANGUAGES C CXX)
add_compile_options(-Wall -Wextra -O2 --std=gnu++23)

include_directories(${CMAKE_SOURCE_DIR}/testlib)

include(FetchContent)
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

add_executable(SearchingAlgorithmsV2 "exec/TSP.cpp")
add_executable(DataCreation statements/TSP/DataCreation.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set_property(TARGET SearchingAlgorithmsV2 PROPERTY CXX_STANDARD 23)
set_property(TARGET DataCreation PROPERTY CXX_STANDARD 23)

target_link_libraries(SearchingAlgorithmsV2 PRIVATE fmt::fmt-header-only)
target_link_libraries(DataCreation PRIVATE fmt::fmt-header-only)

include(CTest)
enable_testing()
add_executable(cpp_test testing/statements/TSP/testing.cpp)

add_test(NAME N_3 COMMAND cpp_test --size 3)
add_test(NAME N_4 COMMAND cpp_test --size 4)