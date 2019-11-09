#===----- Aryiele Build System -------------------------------------------===#
#
# Root CMake build file for the Aryiele project.
#
#===----------------------------------------------------------------------===#
cmake_minimum_required(VERSION 3.13)

project("Aryiele Project")

if(WIN32)
    add_compile_definitions(_WIN32)
    add_compile_definitions(WIN32)
    add_compile_definitions(PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(_UNIX)
    add_compile_definitions(UNIX)
    add_compile_definitions(PLATFORM_LINUX)
elseif(APPLE)
    add_compile_definitions(_APPLE)
    add_compile_definitions(APPLE)
    add_compile_definitions(PLATFORM_MACOSX)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_compile_definitions(_DEBUG)
    add_compile_definitions(DEBUG)
    add_compile_definitions(CONFIGURATION_DEBUG)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_compile_definitions(_RELEASE)
    add_compile_definitions(RELEASE)
    add_compile_definitions(CONFIGURATION_RELEASE)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_definitions(COMPILER_GNU)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_definitions(COMPILER_MSVC)
endif()

include_directories(.)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/Dependencies/LLVM/include/")

set(VANIR_BUILD_JSON OFF CACHE STRING "Enable this to build the JSON utilities" FORCE)
set(VANIR_BUILD_PROFILER OFF CACHE STRING "Enable this to build the profiling macros" FORCE)

add_subdirectory("Sources/Dependencies/LLVM")
add_subdirectory("Sources/Vanir")
add_subdirectory("Sources/Aryiele")
add_subdirectory("Sources/STD")
add_subdirectory("Sources/Tools/arc")