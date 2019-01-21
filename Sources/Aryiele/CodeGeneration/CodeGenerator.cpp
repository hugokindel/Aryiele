#include <Aryiele/CodeGeneration/CodeGenerator.h>
#include <Aryiele/AST/Node.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Transforms/Scalar/Reassociate.h>

namespace Aryiele
{
    CodeGenerator::CodeGenerator() :
        Builder(llvm::IRBuilder<>(Context)),
        Module(std::make_shared<llvm::Module>("Aryiele", Context)),
        FunctionPassManager(std::make_shared<llvm::FunctionPassManager>(Module.get()))
    {
        FunctionPassManager->addPass(llvm::InstCombinePass());
        FunctionPassManager->addPass(llvm::ReassociatePass());
        FunctionPassManager->addPass(llvm::GVN());
        FunctionPassManager->addPass(llvm::SimplifyCFGPass());
    }

    void CodeGenerator::GenerateCode(std::vector<std::shared_ptr<Node>> nodes)
    {
        for (auto& node : nodes)
        {
            auto *functionIR = node->GenerateCode();
        }
    }

} /* Namespace Aryiele. */