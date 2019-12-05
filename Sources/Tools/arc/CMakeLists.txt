#==================================================================================#
#                                                                                  #
#  Copyright (c) 2019 Hugo Kindel <kindelhugo.pro@gmail.com>                       #
#                                                                                  #
#  This file is part of the Aryiele project.                                       #
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
#==================================================================================#

cmake_minimum_required(VERSION 3.12)
project("arc")

set(LLVM_DIR ${PROJECT_SOURCE_DIR}/../../Dependencies/LLVM/cmake/modules)

find_package(LLVM REQUIRED CONFIG)

add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-unused-variable")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../Build/Lib/arc/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../Bin/Aryiele/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE}/bin)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND WIN32)
    set(ARC_LIBRARIES ${ARC_LIBRARIES}
            mingw32)
endif()
set(ARC_LIBRARIES ${ARC_LIBRARIES}
        Vanir
        Aryiele
        STD
        LLVMBitWriter
        LLVMAnalysis
        LLVMProfileData
        LLVMObject
        LLVMMCParser
        LLVMMC
        LLVMDebugInfoCodeView
        LLVMDebugInfoMSF
        LLVMBitReader
        LLVMCore
        LLVMBinaryFormat
        LLVMSupport
        LLVMDemangle
        ${X11_LIBRARIES})
set(ARC_INCLUDES
        ${PROJECT_SOURCE_DIR}/Include/
        ${PROJECT_SOURCE_DIR}/../../
        ${PROJECT_SOURCE_DIR}/../
        ${PROJECT_SOURCE_DIR}/../../Dependencies/
        ${PROJECT_SOURCE_DIR}/../../Dependencies/LLVM/include/
        ${PROJECT_SOURCE_DIR}/../../Dependencies/JSON/include/
        ${PROJECT_SOURCE_DIR}/../../../Build/Include/
        ${PROJECT_SOURCE_DIR}/../../../Build/.Temps/CMake/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE}/Sources/Dependencies/LLVM/include/)
if(UNIX AND NOT APPLE)
    set(ARC_LIBRARIES ${ARC_LIBRARIES}
            stdc++fs)
endif()

add_definitions(${LLVM_DEFINITIONS})

add_executable(arc
        Include/ARC/Core/ARC.h
        Include/ARC/Core/BuildType.h
        Include/ARC/Utility/Logger.h
        Sources/Core/ARC.cpp
        Sources/Main.cpp)
        

target_include_directories(arc PUBLIC ${ARC_INCLUDES})
target_link_libraries(arc ${ARC_LIBRARIES})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND WIN32)
    get_filename_component(COMPILER_PATH ${CMAKE_C_COMPILER} DIRECTORY)
    file(GLOB MINGW_GCC_DLL "${COMPILER_PATH}/libgcc*")
    file(GLOB MINGW_STD_DLL "${COMPILER_PATH}/libstd*")
    file(GLOB MINGW_WIN_DLL "${COMPILER_PATH}/libwin*")
    add_custom_command(TARGET arc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${MINGW_GCC_DLL}" "${MINGW_STD_DLL}" "${MINGW_WIN_DLL}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

if(WIN32)
    add_custom_command(TARGET arc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/../../../Build/Lib/STD/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE}/STD.lib" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../libs/STD.lib")
else()
    add_custom_command(TARGET arc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/../../../Build/Lib/STD/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE}/libSTD.a" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../libs/libSTD.a")
endif()