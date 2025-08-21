cmake_minimum_required(VERSION 3.16)
project(cplus LANGUAGES C VERSION 0.0.1)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(external/OOP-in-C EXCLUDE_FROM_ALL)

file(GLOB_RECURSE SRC_CPLUS "src/*.c")

add_executable(cplus ${SRC_CPLUS})
target_link_libraries(cplus PRIVATE cplus_oop)
target_include_directories(cplus PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/OOP-in-C/include
)
target_compile_options(cplus PRIVATE
    -Wall -Wextra -Werror -pedantic
    -Wconversion -Wsign-conversion
    -Wshadow -Wnull-dereference
    -Wundef -Wuninitialized
    -Winit-self -Wredundant-decls
    -Wcast-align -Wcast-qual
    -Wmissing-declarations -Wswitch-default
    -Wdouble-promotion -Wformat=2 -Wwrite-strings
)

option(ENABLE_DEBUG "Enable debug macros and flags" OFF)
if(ENABLE_DEBUG)
    target_compile_definitions(cplus PRIVATE DEBUG=1)
endif()

