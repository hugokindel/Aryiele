#ifndef ARYIELE_FUNCTION_H
#define ARYIELE_FUNCTION_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/CodeGenerator/FunctionArgument.h>
#include <Aryiele/CodeGenerator/BlockStack.h>
#include <llvm/IR/Type.h>

namespace Aryiele
{
    struct Function
    {
        Function(const std::string &identifier, llvm::Type *returnType,
                 std::vector<std::shared_ptr<FunctionArgument>> arguments,
                 std::shared_ptr<BlockStack> blocks);

        std::string Identifier;
        llvm::Type *ReturnType;
        std::vector<std::shared_ptr<FunctionArgument>> Arguments;
        std::shared_ptr<BlockStack> Blocks;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_FUNCTION_H. */
