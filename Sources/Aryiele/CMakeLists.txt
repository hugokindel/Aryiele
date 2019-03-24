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
        LLVMPasses
        ${X11_LIBRARIES})
set(ARYIELE_INCLUDES
        ${PROJECT_SOURCE_DIR}/../
        ${PROJECT_SOURCE_DIR}/../Dependencies/
        ${PROJECT_SOURCE_DIR}/../Dependencies/LLVM/include/
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
        ${PROJECT_SOURCE_DIR}/AST/Argument.cpp
        ${PROJECT_SOURCE_DIR}/AST/Argument.h
        ${PROJECT_SOURCE_DIR}/AST/Nodes.h
        ${PROJECT_SOURCE_DIR}/AST/Nodes/Node.h
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeConstantDouble.cpp
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeConstantDouble.h
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeConstantInteger.cpp
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeConstantInteger.h
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeFunction.cpp
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeFunction.h
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeOperationBinary.cpp
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeOperationBinary.h
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeStatementBlock.cpp
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeStatementBlock.h
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeStatementFunctionCall.cpp
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeStatementFunctionCall.h
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeStatementIf.cpp
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeStatementIf.h
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeStatementReturn.cpp
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeStatementReturn.h
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeVariable.cpp
        ${PROJECT_SOURCE_DIR}/AST/Nodes/NodeVariable.h
        ${PROJECT_SOURCE_DIR}/CodeGenerator/CodeGenerator.h
        ${PROJECT_SOURCE_DIR}/CodeGenerator/CodeGenerator.cpp
        ${PROJECT_SOURCE_DIR}/CodeGenerator/Block.cpp
        ${PROJECT_SOURCE_DIR}/CodeGenerator/Block.h
        ${PROJECT_SOURCE_DIR}/CodeGenerator/BlockStack.cpp
        ${PROJECT_SOURCE_DIR}/CodeGenerator/BlockStack.h
        ${PROJECT_SOURCE_DIR}/CodeGenerator/Function.cpp
        ${PROJECT_SOURCE_DIR}/CodeGenerator/Function.h
        ${PROJECT_SOURCE_DIR}/CodeGenerator/FunctionArgument.cpp
        ${PROJECT_SOURCE_DIR}/CodeGenerator/FunctionArgument.h AST/Nodes/NodeStatementVariableDeclaration.cpp AST/Nodes/NodeStatementVariableDeclaration.h)

target_include_directories(Aryiele PUBLIC ${ARYIELE_INCLUDES})
target_link_libraries(Aryiele ${ARYIELE_LIBRARIES})