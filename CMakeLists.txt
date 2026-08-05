cmake_minimum_required(VERSION 3.14)
project(hot_jupiter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")

include_directories(cpp/include)

# Shared Library
add_library(hot_jupiter_cpp SHARED
    cpp/src/interior.cpp
)

# Benchmark Executables
add_executable(cpp_jupiter_cooling cpp/examples/jupiter_cooling.cpp cpp/src/interior.cpp)
add_executable(cpp_coupled_orbital_spin cpp/examples/coupled_orbital_spin.cpp cpp/src/interior.cpp)
add_executable(cpp_eccentric_rlof cpp/examples/eccentric_rlof_scenario.cpp cpp/src/interior.cpp)

# Unit Test Executable
add_executable(cpp_unit_tests cpp/tests/unit_tests.cpp cpp/src/interior.cpp)
