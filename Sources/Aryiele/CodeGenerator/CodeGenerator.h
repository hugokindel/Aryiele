#ifndef ARYIELE_CODEGENERATOR_H
#define ARYIELE_CODEGENERATOR_H

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

namespace Aryiele
{
    class Node;

    class CodeGenerator : public Vanir::Module<CodeGenerator>
    {
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

#endif /* ARYIELE_CODEGENERATOR_H. */
