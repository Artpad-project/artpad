cmake_minimum_required(VERSION 3.16)
project(artpad LANGUAGES C VERSION 1.0.1)

include(GNUInstallDirs)
include_directories(./)

### CFLAGS #####

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -lm -pthread")

### GTK ########

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})
# Link targets to GTK3
link_libraries(${GTK3_LIBRARIES})

## ARTPAD_LIB ##

add_subdirectory(src)   # src : source files

# create library
SET(PROJECT_LIB ${PROJECT_NAME}_LIB)
add_library(${PROJECT_LIB} SHARED ${SRC})

set_target_properties(${PROJECT_LIB} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_LIB} PROPERTIES PUBLIC_HEADER include/${PROJECT_NAME}.h)
target_include_directories(${PROJECT_LIB} PRIVATE .)

# install library
install(TARGETS ${PROJECT_LIB}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

### TESTS #####

enable_testing()
add_subdirectory(test)  # tests: test files

################

add_executable(${PROJECT_NAME} ${SRC} src/main.c)