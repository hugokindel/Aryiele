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
#include <Aryiele/Common.h>
#include <Aryiele/AST/Nodes/NodeLiteralNumberFloating.h>
#include <Aryiele/AST/Nodes/NodeLiteralNumberInteger.h>
#include <Aryiele/AST/Nodes/NodeTopFunction.h>
#include <Aryiele/AST/Nodes/NodeOperationBinary.h>
#include <Aryiele/AST/Nodes/NodeStatementBlock.h>
#include <Aryiele/AST/Nodes/NodeStatementFunctionCall.h>
#include <Aryiele/AST/Nodes/NodeStatementIf.h>
#include <Aryiele/AST/Nodes/NodeStatementReturn.h>
#include <Aryiele/AST/Nodes/NodeStatementVariableDeclaration.h>
#include <Aryiele/AST/Nodes/NodeStatementVariable.h>
#include <Aryiele/CodeGenerator/BlockStack.h>
#include <Aryiele/CodeGenerator/GenerationError.h>

namespace Aryiele {
    class Node;

    class CodeGenerator : public Vanir::Module<CodeGenerator> {
    public:
        CodeGenerator();
        
        void generateCode(std::vector<std::shared_ptr<Node>> nodes);
        std::shared_ptr<llvm::Module> getModule();

    private:
        llvm::Type* getType(const std::string& type);
        llvm::Value* getTypeDefaultValue(const std::string& type);
        llvm::Value* castType(llvm::Value *value, llvm::Type *returnType, bool isSigned = true);
        llvm::AllocaInst *createEntryBlockAllocation(
            llvm::Function *function, const std::string &identifier, llvm::Type *type = nullptr);
        GenerationError generateCode(std::shared_ptr<Node> node);
        GenerationError generateCode(NodeTopFunction* node);
        GenerationError generateCode(NodeOperationBinary* node);
        GenerationError generateCode(NodeLiteralNumberFloating* node);
        GenerationError generateCode(NodeLiteralNumberInteger* node);
        GenerationError generateCode(NodeStatementVariable* node);
        GenerationError generateCode(NodeStatementFunctionCall* node);
        GenerationError generateCode(NodeStatementIf* node);
        GenerationError generateCode(NodeStatementReturn* node);
        GenerationError generateCode(NodeStatementBlock* node);
        GenerationError generateCode(NodeStatementVariableDeclaration* node);

        llvm::LLVMContext m_context;
        llvm::IRBuilder<> m_builder = llvm::IRBuilder<>(m_context);
        std::shared_ptr<llvm::DataLayout> m_dataLayout;
        std::shared_ptr<llvm::Module> m_module;
        std::shared_ptr<BlockStack> m_blockStack;
    };
    
    CodeGenerator &getCodeGenerator();

} /* Namespace Aryiele. */

#endif /* ARYIELE_CODEGENERATOR_CODEGENERATOR_H. */
