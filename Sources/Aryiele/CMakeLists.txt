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
        ${PROJECT_SOURCE_DIR}/Include/
        ${PROJECT_SOURCE_DIR}/../
        ${PROJECT_SOURCE_DIR}/../Vanir/Include/
        ${PROJECT_SOURCE_DIR}/../Dependencies/
        ${PROJECT_SOURCE_DIR}/../Dependencies/LLVM/Include/
        ${PROJECT_SOURCE_DIR}/../../Build/Include/
        ${CMAKE_CURRENT_BINARY_DIR}/../Dependencies/LLVM/Include/)

if(UNIX AND NOT APPLE)
    set(ARYIELE_LIBRARIES ${ARYIELE_LIBRARIES}
            stdc++fs)
endif()

add_definitions(${LLVM_DEFINITIONS})

add_library(Aryiele
        Include/Aryiele/Common.h
        Include/Aryiele/Lexer/Lexer.h
        Include/Aryiele/Lexer/LexerToken.h
        Include/Aryiele/Lexer/LexerTable.h
        Include/Aryiele/Parser/Parser.h
        Include/Aryiele/Parser/ParserToken.h
        Include/Aryiele/Parser/ParserInformation.h
        Include/Aryiele/AST/Argument.h
        Include/Aryiele/AST/Variable.h
        Include/Aryiele/AST/Nodes/Node.h
        Include/Aryiele/AST/Nodes/NodeLiteralArray.h
        Include/Aryiele/AST/Nodes/NodeLiteralNumberFloating.h
        Include/Aryiele/AST/Nodes/NodeLiteralNumberInteger.h
        Include/Aryiele/AST/Nodes/NodeLiteralString.h
        Include/Aryiele/AST/Nodes/NodeLiteralCharacter.h
        Include/Aryiele/AST/Nodes/NodeLiteralBoolean.h
        Include/Aryiele/AST/Nodes/NodeOperationBinary.h
        Include/Aryiele/AST/Nodes/NodeOperationTernary.h
        Include/Aryiele/AST/Nodes/NodeOperationUnary.h
        Include/Aryiele/AST/Nodes/NodeRoot.h
        Include/Aryiele/AST/Nodes/NodeStatementArrayCall.h
        Include/Aryiele/AST/Nodes/NodeStatementBlock.h
        Include/Aryiele/AST/Nodes/NodeStatementBreak.h
        Include/Aryiele/AST/Nodes/NodeStatementCase.h
        Include/Aryiele/AST/Nodes/NodeStatementContinue.h
        Include/Aryiele/AST/Nodes/NodeStatementFor.h
        Include/Aryiele/AST/Nodes/NodeStatementFunctionCall.h
        Include/Aryiele/AST/Nodes/NodeStatementIf.h
        Include/Aryiele/AST/Nodes/NodeStatementReturn.h
        Include/Aryiele/AST/Nodes/NodeStatementSwitch.h
        Include/Aryiele/AST/Nodes/NodeStatementVariableDeclaration.h
        Include/Aryiele/AST/Nodes/NodeStatementWhile.h
        Include/Aryiele/AST/Nodes/NodeStatementVariable.h
        Include/Aryiele/AST/Nodes/NodeTopFile.h
        Include/Aryiele/AST/Nodes/NodeTopFunction.h
        Include/Aryiele/AST/Nodes/NodeTopNamespace.h
        Include/Aryiele/CodeGenerator/CodeGenerator.h
        Include/Aryiele/CodeGenerator/Block.h
        Include/Aryiele/CodeGenerator/BlockStack.h
        Include/Aryiele/CodeGenerator/BlockVariable.h
        Include/Aryiele/CodeGenerator/Function.h
        Include/Aryiele/CodeGenerator/FunctionArgument.h
        Include/Aryiele/CodeGenerator/GenerationError.h
        Include/Aryiele/Utility/Logger.h
        Sources/Lexer/Lexer.cpp
        Sources/Lexer/LexerToken.cpp
        Sources/Parser/Parser.cpp
        Sources/Parser/ParserToken.cpp
        Sources/Parser/ParserInformation.cpp
        Sources/AST/Argument.cpp
        Sources/AST/Variable.cpp
        Sources/AST/Nodes/Node.cpp
        Sources/AST/Nodes/NodeLiteralArray.cpp
        Sources/AST/Nodes/NodeLiteralNumberFloating.cpp
        Sources/AST/Nodes/NodeLiteralNumberInteger.cpp
        Sources/AST/Nodes/NodeLiteralString.cpp
        Sources/AST/Nodes/NodeLiteralCharacter.cpp
        Sources/AST/Nodes/NodeLiteralBoolean.cpp
        Sources/AST/Nodes/NodeOperationBinary.cpp
        Sources/AST/Nodes/NodeOperationTernary.cpp
        Sources/AST/Nodes/NodeOperationUnary.cpp
        Sources/AST/Nodes/NodeRoot.cpp
        Sources/AST/Nodes/NodeStatementArrayCall.cpp
        Sources/AST/Nodes/NodeStatementBlock.cpp
        Sources/AST/Nodes/NodeStatementBreak.cpp
        Sources/AST/Nodes/NodeStatementCase.cpp
        Sources/AST/Nodes/NodeStatementContinue.cpp
        Sources/AST/Nodes/NodeStatementFor.cpp
        Sources/AST/Nodes/NodeStatementFunctionCall.cpp
        Sources/AST/Nodes/NodeStatementIf.cpp
        Sources/AST/Nodes/NodeStatementReturn.cpp
        Sources/AST/Nodes/NodeStatementSwitch.cpp
        Sources/AST/Nodes/NodeStatementVariableDeclaration.cpp
        Sources/AST/Nodes/NodeStatementWhile.cpp
        Sources/AST/Nodes/NodeStatementVariable.cpp
        Sources/AST/Nodes/NodeTopFile.cpp
        Sources/AST/Nodes/NodeTopFunction.cpp
        Sources/AST/Nodes/NodeTopNamespace.cpp
        Sources/CodeGenerator/CodeGenerator.cpp
        Sources/CodeGenerator/Block.cpp
        Sources/CodeGenerator/BlockStack.cpp
        Sources/CodeGenerator/BlockVariable.cpp
        Sources/CodeGenerator/Function.cpp
        Sources/CodeGenerator/FunctionArgument.cpp
        Sources/CodeGenerator/GenerationError.cpp)

target_include_directories(Aryiele PUBLIC ${ARYIELE_INCLUDES})
target_link_libraries(Aryiele ${ARYIELE_LIBRARIES})