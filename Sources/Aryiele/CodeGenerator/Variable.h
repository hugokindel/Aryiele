#ifndef ARYIELE_VARIABLE_H
#define ARYIELE_VARIABLE_H

#include <Aryiele/Core/Includes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace Aryiele
{
    struct Variable
    {
        Variable(const std::string& identifier, llvm::Type *type, llvm::Value *value = nullptr);

        std::string Identifier;
        llvm::Type *Type;
        llvm::Value *Value;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_VARIABLE_H. */
