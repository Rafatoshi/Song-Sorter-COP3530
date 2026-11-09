cmake_minimum_required(VERSION 3.14)
project(SongSorters LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build in Release by default so the timings mean something
# (Debug builds can be 10x slower and distort the comparison).
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Catch2 v3 (both test files include <catch2/catch_test_macros.hpp>).
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# Shared code used by both the program and the tests.
add_library(song_sorters_lib
    src/SongData.cpp
    src/MergeSort.cpp
    src/QuickSort.cpp
    src/Benchmark.cpp
)
target_include_directories(song_sorters_lib PUBLIC include)

# Menu-driven console program.
add_executable(SongSorters src/main.cpp)
target_link_libraries(SongSorters PRIVATE song_sorters_lib)

# Catch2 tests.
add_executable(tests
    tests/test_merge_sort.cpp
    tests/test_quick_sort.cpp
)
target_link_libraries(tests PRIVATE song_sorters_lib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
