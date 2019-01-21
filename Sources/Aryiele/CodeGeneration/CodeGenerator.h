#ifndef ARYIELE_CODEGENERATOR_H
#define ARYIELE_CODEGENERATOR_H

#include <Aryiele/Core/Includes.h>
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

    class CodeGenerator : public Vanir::Singleton<CodeGenerator>
    {
    public:
        CodeGenerator();

        void GenerateCode(std::vector<std::shared_ptr<Node>> nodes);

        llvm::LLVMContext Context;
        llvm::IRBuilder<> Builder;
        std::shared_ptr<llvm::Module> Module;
        std::map<std::string, llvm::Value*> NamedValues;
        std::shared_ptr<llvm::legacy::FunctionPassManager> FunctionPassManager;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_CODEGENERATOR_H. */
