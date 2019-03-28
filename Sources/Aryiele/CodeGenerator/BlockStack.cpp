#include <Aryiele/CodeGenerator/BlockStack.h>

namespace Aryiele
{
    std::shared_ptr<Block> BlockStack::Create(bool setAsCurrent)
    {
        auto block = std::make_shared<Block>();

        if (Current)
        {
            block->Parent = Current;
            Current->Children.emplace_back(block);
        }

        if (setAsCurrent)
            Current = block;

        return Current;
    }

    std::shared_ptr<Block> BlockStack::EscapeCurrent()
    {
        if (Current->Parent)
            Current = Current->Parent;
        else
            Current = nullptr;

        return Current;
    }

    llvm::AllocaInst* BlockStack::FindVariable(const std::string& identifier)
    {
        auto block = Current;
        llvm::AllocaInst* variable = nullptr;

        if (block->Variables.find(identifier) != block->Variables.end())
            variable = Current->Variables[identifier];

        while (!variable)
        {
            block = block->Parent;

            if (!block)
                break;

            if (block->Variables.find(identifier) != block->Variables.end())
                variable = block->Variables[identifier];
        }

        return variable;
    }

} /* Namespace Aryiele. */