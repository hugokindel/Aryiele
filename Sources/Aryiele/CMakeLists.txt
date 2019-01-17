# ----------------------------------------------------------------
# CMake build file for Aryiele.
# ----------------------------------------------------------------
cmake_minimum_required(VERSION 3.12)
project("Aryiele")

set(LLVM_DIR ../Dependencies/LLVM/cmake/modules)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../Build/Lib/Aryiele/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../Bin/Aryiele/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE})
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND WIN32)
    set(ARYIELE_LIBRARIES ${ARYIELE_LIBRARIES}
            mingw32)
endif()
set(ARYIELE_LIBRARIES ${ARYIELE_LIBRARIES}
        Vanir
        ${X11_LIBRARIES})
set(ARYIELE_INCLUDES
        ${PROJECT_SOURCE_DIR}/../
        ${PROJECT_SOURCE_DIR}/../Dependencies/
        ${PROJECT_SOURCE_DIR}/../../Build/Include/)
if(UNIX AND NOT APPLE)
    set(ARYIELE_LIBRARIES ${ARYIELE_LIBRARIES}
            stdc++fs)
endif()

add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(all)

add_executable(Aryiele
        ${PROJECT_SOURCE_DIR}/Main.cpp
        ${PROJECT_SOURCE_DIR}/Tokenizer/Token.cpp
        ${PROJECT_SOURCE_DIR}/Tokenizer/Token.h
        ${PROJECT_SOURCE_DIR}/Tokenizer/Tokenizer.cpp
        ${PROJECT_SOURCE_DIR}/Tokenizer/Tokenizer.h
        ${PROJECT_SOURCE_DIR}/Tokenizer/TokenPrecedence.cpp
        ${PROJECT_SOURCE_DIR}/Tokenizer/TokenPrecedence.h
        ${PROJECT_SOURCE_DIR}/Tokenizer/TokenTable.h
        ${PROJECT_SOURCE_DIR}/Tokenizer/TokenTypes.h
        ${PROJECT_SOURCE_DIR}/Core/Includes.h
        ${PROJECT_SOURCE_DIR}/Core/Logger.h
        ${PROJECT_SOURCE_DIR}/Parser/ParserTokens.h)

target_include_directories(Aryiele PUBLIC ${ARYIELE_INCLUDES} ${LLVM_INCLUDE_DIRS})
target_link_libraries(Aryiele ${ARYIELE_LIBRARIES} ${llvm_libs})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND WIN32)
    get_filename_component(COMPILER_PATH ${CMAKE_C_COMPILER} DIRECTORY)
    file(GLOB MINGW_GCC_DLL "${COMPILER_PATH}/libgcc*")
    file(GLOB MINGW_STD_DLL "${COMPILER_PATH}/libstd*")
    file(GLOB MINGW_WIN_DLL "${COMPILER_PATH}/libwin*")
    add_custom_command(TARGET Aryiele POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${MINGW_GCC_DLL}" "${MINGW_STD_DLL}" "${MINGW_WIN_DLL}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()