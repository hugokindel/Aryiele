#ifndef ARYIELE_FUNCTIONARGUMENT_H
#define ARYIELE_FUNCTIONARGUMENT_H

#include <Aryiele/Core/Includes.h>
#include <llvm/IR/Type.h>

namespace Aryiele
{
    struct FunctionArgument
    {
        FunctionArgument(const std::string& identifier, llvm::Type *type);

        std::string Identifier;
        llvm::Type *Type;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_FUNCTIONARGUMENT_H. */
