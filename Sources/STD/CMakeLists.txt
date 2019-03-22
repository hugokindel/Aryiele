# ----------------------------------------------------------------
# CMake build file for STD.
# ----------------------------------------------------------------
cmake_minimum_required(VERSION 3.12)
project("STD")

set(LLVM_DIR ${PROJECT_SOURCE_DIR}/../Dependencies/LLVM/cmake/modules)

find_package(LLVM REQUIRED CONFIG)

add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-unused-variable")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../Build/Lib/STD/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE})
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND WIN32)
    set(STD_LIBRARIES ${STD_LIBRARIES}
            mingw32)
endif()
set(STD_LIBRARIES ${STD_LIBRARIES}
        Vanir
        Aryiele
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
        LLVMPasses
        ${X11_LIBRARIES})
set(STD_INCLUDES
        ${PROJECT_SOURCE_DIR}/../
        ${PROJECT_SOURCE_DIR}/../Dependencies/
        ${PROJECT_SOURCE_DIR}/../Dependencies/LLVM/include/
        ${PROJECT_SOURCE_DIR}/../../Build/Include/
        ${PROJECT_SOURCE_DIR}/../../Build/.Temps/CMake/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE}/Sources/Dependencies/LLVM/include/)
if(UNIX AND NOT APPLE)
    set(STD_LIBRARIES ${STD_LIBRARIES}
            stdc++fs)
endif()

add_definitions(${LLVM_DEFINITIONS})

add_library(STD
        ${PROJECT_SOURCE_DIR}/IO/Print.h
        ${PROJECT_SOURCE_DIR}/IO/Print.cpp)

target_include_directories(STD PUBLIC ${STD_INCLUDES})
target_link_libraries(STD ${STD_LIBRARIES})