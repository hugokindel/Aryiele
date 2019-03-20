#ifndef ARYIELE_BLOCK_H
#define ARYIELE_BLOCK_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/CodeGenerator/VariableStack.h>

namespace Aryiele
{
    struct Block
    {
        std::shared_ptr<VariableStack> Variables;
        std::shared_ptr<Block> Parent = nullptr;
        std::vector<std::shared_ptr<Block>> Children;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_BLOCK_H. */
