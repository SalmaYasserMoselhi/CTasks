cmake_minimum_required(VERSION 3.16)

project(Task1
  VERSION 0.1.0
  LANGUAGES C CXX
)

# Compilers and standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources (list them explicitly or glob from src/)
add_executable(
  main.cpp
  terminal_utils.cpp
  # add more .cpp files here
)

# If you have headers in include/
target_include_directories(task1 PRIVATE ${PROJECT_SOURCE_DIR}/include)