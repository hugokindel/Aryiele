# ----------------------------------------------------------------
# CMake build file for Aryiele.
# ----------------------------------------------------------------
cmake_minimum_required(VERSION 3.12)
project("Aryiele")

set(LLVM_DIR ${PROJECT_SOURCE_DIR}/../Dependencies/LLVM/cmake/modules)

find_package(LLVM REQUIRED CONFIG)

add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-unused-variable")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../Build/Lib/Aryiele/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE})
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND WIN32)
    set(ARYIELE_LIBRARIES ${ARYIELE_LIBRARIES}
            mingw32)
endif()
set(ARYIELE_LIBRARIES ${ARYIELE_LIBRARIES}
        Vanir
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
set(ARYIELE_INCLUDES
        ${PROJECT_SOURCE_DIR}/../
        ${PROJECT_SOURCE_DIR}/../Dependencies/
        ${PROJECT_SOURCE_DIR}/../Dependencies/LLVM/include/
        ${PROJECT_SOURCE_DIR}/../Dependencies/JSON/include/
        ${PROJECT_SOURCE_DIR}/../../Build/Include/
        ${PROJECT_SOURCE_DIR}/../../Build/.Temps/CMake/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SIZEOF_VOID_P}_${CMAKE_BUILD_TYPE}/Sources/Dependencies/LLVM/include/)
if(UNIX AND NOT APPLE)
    set(ARYIELE_LIBRARIES ${ARYIELE_LIBRARIES}
            stdc++fs)
endif()

add_definitions(${LLVM_DEFINITIONS})

add_library(Aryiele
        ${PROJECT_SOURCE_DIR}/Core/Includes.h
        ${PROJECT_SOURCE_DIR}/Core/Logger.h
        ${PROJECT_SOURCE_DIR}/Lexer/Lexer.cpp
        ${PROJECT_SOURCE_DIR}/Lexer/Lexer.h
        ${PROJECT_SOURCE_DIR}/Lexer/LexerToken.cpp
        ${PROJECT_SOURCE_DIR}/Lexer/LexerToken.h
        ${PROJECT_SOURCE_DIR}/Lexer/LexerTokens.h
        ${PROJECT_SOURCE_DIR}/Lexer/LexerTable.h
        ${PROJECT_SOURCE_DIR}/Parser/Parser.cpp
        ${PROJECT_SOURCE_DIR}/Parser/Parser.h
        ${PROJECT_SOURCE_DIR}/Parser/ParserToken.cpp
        ${PROJECT_SOURCE_DIR}/Parser/ParserToken.h
        ${PROJECT_SOURCE_DIR}/Parser/ParserTokens.h
        ${PROJECT_SOURCE_DIR}/Parser/ParserPrecedence.cpp
        ${PROJECT_SOURCE_DIR}/Parser/ParserPrecedence.h
        ${PROJECT_SOURCE_DIR}/Parser/ParserInformation.cpp
        ${PROJECT_SOURCE_DIR}/Parser/ParserInformation.h
        ${PROJECT_SOURCE_DIR}/AST/ExpressionNode.cpp
        ${PROJECT_SOURCE_DIR}/AST/ExpressionNode.h
        ${PROJECT_SOURCE_DIR}/AST/ExpressionDoubleNode.cpp
        ${PROJECT_SOURCE_DIR}/AST/ExpressionDoubleNode.h
        ${PROJECT_SOURCE_DIR}/AST/ExpressionIntegerNode.cpp
        ${PROJECT_SOURCE_DIR}/AST/ExpressionIntegerNode.h
        ${PROJECT_SOURCE_DIR}/AST/ExpressionStringNode.cpp
        ${PROJECT_SOURCE_DIR}/AST/ExpressionStringNode.h
        ${PROJECT_SOURCE_DIR}/AST/ExpressionBooleanNode.cpp
        ${PROJECT_SOURCE_DIR}/AST/ExpressionBooleanNode.h
        ${PROJECT_SOURCE_DIR}/AST/ExpressionVariableNode.cpp
        ${PROJECT_SOURCE_DIR}/AST/ExpressionVariableNode.h
        ${PROJECT_SOURCE_DIR}/AST/ExpressionBinaryOperationNode.cpp
        ${PROJECT_SOURCE_DIR}/AST/ExpressionBinaryOperationNode.h
        ${PROJECT_SOURCE_DIR}/AST/ExpressionFunctionReturnNode.cpp
        ${PROJECT_SOURCE_DIR}/AST/ExpressionFunctionReturnNode.h
        ${PROJECT_SOURCE_DIR}/AST/ExpressionFunctionCallNode.cpp
        ${PROJECT_SOURCE_DIR}/AST/ExpressionFunctionCallNode.h
        ${PROJECT_SOURCE_DIR}/AST/FunctionNode.cpp
        ${PROJECT_SOURCE_DIR}/AST/FunctionNode.h
        ${PROJECT_SOURCE_DIR}/AST/Node.cpp
        ${PROJECT_SOURCE_DIR}/AST/Node.h
        ${PROJECT_SOURCE_DIR}/AST/Argument.h
        ${PROJECT_SOURCE_DIR}/AST/Argument.cpp
        ${PROJECT_SOURCE_DIR}/CodeGeneration/CodeGenerator.h
        ${PROJECT_SOURCE_DIR}/CodeGeneration/CodeGenerator.cpp)

target_include_directories(Aryiele PUBLIC ${ARYIELE_INCLUDES})
target_link_libraries(Aryiele ${ARYIELE_LIBRARIES})