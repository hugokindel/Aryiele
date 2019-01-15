cmake_minimum_required(VERSION 3.12)
project("Aryiele")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../Build/Lib/Aryiele/${ARYIELE_CONFIGURATION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../Bin/Aryiele/${ARYIELE_CONFIGURATION})
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

add_executable(Aryiele
        ${PROJECT_SOURCE_DIR}/Main.cpp)

target_include_directories(Aryiele PUBLIC ${ARYIELE_INCLUDES})
target_link_libraries(Aryiele LINK_PUBLIC ${ARYIELE_LIBRARIES})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND WIN32)
    get_filename_component(COMPILER_PATH ${CMAKE_C_COMPILER} DIRECTORY)
    file(GLOB MINGW_GCC_DLL "${COMPILER_PATH}/libgcc*")
    file(GLOB MINGW_STD_DLL "${COMPILER_PATH}/libstd*")
    file(GLOB MINGW_WIN_DLL "${COMPILER_PATH}/libwin*")
    add_custom_command(TARGET Aryiele POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${MINGW_GCC_DLL}" "${MINGW_STD_DLL}" "${MINGW_WIN_DLL}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()