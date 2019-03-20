#include <Aryiele/CodeGenerator/BlockStack.h>

namespace Aryiele
{
    std::shared_ptr<Block> BlockStack::CreateNew()
    {
        auto block = std::make_shared<Block>();

        if (Current)
        {
            block->Parent = Current;

            Current->Children.emplace_back(block);
        }

        Current = block;

        return Current;
    }

    std::shared_ptr<Block> BlockStack::EscapeCurrent()
    {
        if (Current->Parent)
        {
            Current = Current->Parent;

            return Current;
        }

        return nullptr;
    }

    std::shared_ptr<Variable> BlockStack::FindVariable(const std::string& identifier)
    {
        auto block = Current;
        auto variable = Current->Variables->Get(identifier);

        while (!variable)
        {
            block = Current->Parent;

            if (!block)
                break;

            variable = Current->Variables->Get(identifier);
        }

        return variable;
    }

} /* Namespace Aryiele. */