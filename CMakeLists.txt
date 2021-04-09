cmake_minimum_required(VERSION 3.16)
project(artpad C)
include(GNUInstallDirs)

set(CMAKE_C_STANDARD 99)

## Load subdirs
add_subdirectory(src)   # src : source files
add_subdirectory(test)  # tests: test files

# Executables
add_executable(${PROJECT_NAME} ${SRC})

# Tests
enable_testing()
