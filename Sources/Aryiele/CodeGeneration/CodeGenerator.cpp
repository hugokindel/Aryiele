#include <Aryiele/CodeGeneration/CodeGenerator.h>
#include <Aryiele/AST/Node.h>

namespace Aryiele
{
    CodeGenerator::CodeGenerator() :
        Builder(llvm::IRBuilder<>(Context)), Module(std::make_shared<llvm::Module>("Aryiele", Context))
    {

    }

    void CodeGenerator::GenerateCode(std::vector<std::shared_ptr<Node>> nodes)
    {
        for (auto& node : nodes)
        {
            auto *functionIR = node->GenerateCode();
        }
    }

} /* Namespace Aryiele. */