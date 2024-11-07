cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(incomplete)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++20")
set(CMAKE_CXX_FLAGS_DEBUG "-g -pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_RL_FLAGS}")

add_executable(incomplete
                 src/incomplete.cpp
                 include/EliminationTree.hpp
                 include/DenseMatrix.hpp
                 include/Symbolic.hpp
                 include/Name.hpp
                 include/CSCMatrix.hpp
                 include/TripletMatrix.hpp
                 include/Incomplete.hpp)

target_include_directories(incomplete PRIVATE include)

set_property(TARGET incomplete PROPERTY CXX_STANDARD 17)
