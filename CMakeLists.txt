cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -O0 -g3 -std=c++17")

set(SRC
  src/eval-runtime.cc
  src/main.cc
)
add_executable(calc ${SRC})
