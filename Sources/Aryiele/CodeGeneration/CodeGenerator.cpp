#include <Aryiele/CodeGeneration/CodeGenerator.h>
#include <Aryiele/AST/Node.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Transforms/Scalar/Reassociate.h>

namespace Aryiele
{
    CodeGenerator::CodeGenerator() :
        Builder(llvm::IRBuilder<>(Context)),
        Module(std::make_shared<llvm::Module>("Aryiele", Context)),
        FunctionPassManager(std::make_shared<llvm::legacy::FunctionPassManager>(Module.get()))
    {
        FunctionPassManager->add(llvm::createInstructionCombiningPass());
        FunctionPassManager->add(llvm::createReassociatePass());
        FunctionPassManager->add(llvm::createGVNPass());
        FunctionPassManager->add(llvm::createCFGSimplificationPass());
    }

    void CodeGenerator::GenerateCode(std::vector<std::shared_ptr<Node>> nodes)
    {
        for (auto& node : nodes)
        {
            auto *functionIR = node->GenerateCode();
        }
    }

} /* Namespace Aryiele. */