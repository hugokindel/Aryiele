# ----------------------------------------------------------------
# CMake build file for the Aryiele Compiler.
# ----------------------------------------------------------------
cmake_minimum_required(VERSION 3.12)
project("arc")

set(LLVM_DIR ${PROJECT_SOURCE_DIR}/../../Dependencies/LLVM/cmake/modules)

find_package(LLVM REQUIRED CONFIG)

add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-unused-variable")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../Build/Lib/Aryiele/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../Bin/Aryiele/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE})
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND WIN32)
    set(ARC_LIBRARIES ${ARC_LIBRARIES}
            mingw32)
endif()
set(ARC_LIBRARIES ${ARC_LIBRARIES}
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
        ${X11_LIBRARIES})
set(ARC_INCLUDES
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
        ${PROJECT_SOURCE_DIR}/Main.cpp ARC.cpp ARC.h BuildType.h)
        

target_include_directories(arc PUBLIC ${ARC_INCLUDES})
target_link_libraries(arc ${ARC_LIBRARIES})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND WIN32)
    get_filename_component(COMPILER_PATH ${CMAKE_C_COMPILER} DIRECTORY)
    file(GLOB MINGW_GCC_DLL "${COMPILER_PATH}/libgcc*")
    file(GLOB MINGW_STD_DLL "${COMPILER_PATH}/libstd*")
    file(GLOB MINGW_WIN_DLL "${COMPILER_PATH}/libwin*")
    add_custom_command(TARGET arc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${MINGW_GCC_DLL}" "${MINGW_STD_DLL}" "${MINGW_WIN_DLL}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()
