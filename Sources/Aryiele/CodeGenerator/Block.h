#ifndef ARYIELE_BLOCK_H
#define ARYIELE_BLOCK_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/CodeGenerator/Variable.h>

namespace Aryiele
{
    struct Block
    {
        Block(std::shared_ptr<Block> parent = nullptr,
              std::map<std::string, std::shared_ptr<Variable>> variables = std::map<std::string, std::shared_ptr<Variable>> (),
              std::vector<std::shared_ptr<Block>> children = std::vector<std::shared_ptr<Block>> ());

        std::shared_ptr<Block> Parent = nullptr;
        std::map<std::string, std::shared_ptr<Variable>> Variables;
        std::vector<std::shared_ptr<Block>> Children;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_BLOCK_H. */
