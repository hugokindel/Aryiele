#ifndef ARYIELE_BLOCKSTACK_H
#define ARYIELE_BLOCKSTACK_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/CodeGenerator/Block.h>

namespace Aryiele
{
    class BlockStack
    {
    public:
        BlockStack() = default;

        std::shared_ptr<Block> Create(bool setAsCurrent = true);
        std::shared_ptr<Block> EscapeCurrent();
        std::shared_ptr<Variable> FindVariable(const std::string& identifier);

        std::shared_ptr<Block> Current = nullptr;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_BLOCKSTACK_H. */
