#==================================================================================#
#                                                                                  #
#  Copyright (c) 2019 Hugo Kindel <kindelhugo.pro@gmail.com>                       #
#                                                                                  #
#  This file is part of the Aesir project.                                         #
#  Licensed under MIT License:                                                     #
#                                                                                  #
#  Permission is hereby granted, free of charge, to any person obtaining           #
#  a copy of this software and associated documentation files (the "Software"),    #
#  to deal in the Software without restriction, including without limitation       #
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,        #
#  and/or sell copies of the Software, and to permit persons to whom the           #
#  Software is furnished to do so, subject to the following conditions:            #
#                                                                                  #
#  The above copyright notice and this permission notice shall be included in      #
#  all copies or substantial portions of the Software.                             #
#                                                                                  #
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      #
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        #
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     #
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          #
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   #
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   #
#  SOFTWARE.                                                                       #
#                                                                                  #
#===----- Aryiele Build System -------------------------------------------------===#
#                                                                                  #
# Root CMake build file for the Aryiele project.                                   #
#                                                                                  #
#==================================================================================#

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