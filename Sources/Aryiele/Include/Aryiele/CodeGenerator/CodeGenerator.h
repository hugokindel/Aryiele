//==================================================================================//
//                                                                                  //
//  Copyright (c) 2019 Hugo Kindel <kindelhugo.pro@gmail.com>                       //
//                                                                                  //
//  This file is part of the Aryiele project.                                       //
//  Licensed under MIT License:                                                     //
//                                                                                  //
//  Permission is hereby granted, free of charge, to any person obtaining           //
//  a copy of this software and associated documentation files (the "Software"),    //
//  to deal in the Software without restriction, including without limitation       //
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,        //
//  and/or sell copies of the Software, and to permit persons to whom the           //
//  Software is furnished to do so, subject to the following conditions:            //
//                                                                                  //
//  The above copyright notice and this permission notice shall be included in      //
//  all copies or substantial portions of the Software.                             //
//                                                                                  //
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      //
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        //
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     //
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          //
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   //
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   //
//  SOFTWARE.                                                                       //
//                                                                                  //
//==================================================================================//

#ifndef ARYIELE_CODEGENERATOR_CODEGENERATOR_H
#define ARYIELE_CODEGENERATOR_CODEGENERATOR_H

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/IR/LegacyPassManager.h>
#include <Vanir/Module/Module.h>
#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/NodeConstantDouble.h>
#include <Aryiele/AST/Nodes/NodeConstantInteger.h>
#include <Aryiele/AST/Nodes/NodeFunction.h>
#include <Aryiele/AST/Nodes/NodeOperationBinary.h>
#include <Aryiele/AST/Nodes/NodeStatementBlock.h>
#include <Aryiele/AST/Nodes/NodeStatementFunctionCall.h>
#include <Aryiele/AST/Nodes/NodeStatementIf.h>
#include <Aryiele/AST/Nodes/NodeStatementReturn.h>
#include <Aryiele/AST/Nodes/NodeStatementVariableDeclaration.h>
#include <Aryiele/AST/Nodes/NodeVariable.h>
#include <Aryiele/CodeGenerator/BlockStack.h>
#include <Aryiele/CodeGenerator/GenerationError.h>

namespace Aryiele {
    class Node;

    class CodeGenerator : public Vanir::Module<CodeGenerator> {
    public:
        CodeGenerator();
        void GenerateCode(std::vector<std::shared_ptr<Node>> nodes);
        std::shared_ptr<llvm::Module> GetModule();

    private:
        llvm::Value* CastType(llvm::Value *value, llvm::Type *returnType);
        llvm::AllocaInst *CreateEntryBlockAllocation(
            llvm::Function *function, const std::string &identifier, llvm::Type *type = nullptr);
        GenerationError GenerateCode(std::shared_ptr<Node> node);
        GenerationError GenerateCode(NodeFunction* node);
        GenerationError GenerateCode(NodeOperationBinary* node);
        GenerationError GenerateCode(NodeConstantDouble* node);
        GenerationError GenerateCode(NodeConstantInteger* node);
        GenerationError GenerateCode(NodeVariable* node);
        GenerationError GenerateCode(NodeStatementFunctionCall* node);
        GenerationError GenerateCode(NodeStatementIf* node);
        GenerationError GenerateCode(NodeStatementReturn* node);
        GenerationError GenerateCode(NodeStatementBlock* node);
        GenerationError GenerateCode(NodeStatementVariableDeclaration* node);

        llvm::LLVMContext m_context;
        llvm::IRBuilder<> m_builder = llvm::IRBuilder<>(m_context);
        std::shared_ptr<llvm::DataLayout> m_dataLayout;
        std::shared_ptr<llvm::Module> m_module;
        std::shared_ptr<BlockStack> m_blockStack;
    };
    
    CodeGenerator &GetCodeGenerator();

} /* Namespace Aryiele. */

#endif /* ARYIELE_CODEGENERATOR_CODEGENERATOR_H. */
