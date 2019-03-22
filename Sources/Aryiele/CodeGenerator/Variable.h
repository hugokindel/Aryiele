#ifndef ARYIELE_VARIABLE_H
#define ARYIELE_VARIABLE_H

#include <Aryiele/Core/Includes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Instructions.h>

namespace Aryiele
{
    struct Variable
    {
        Variable(const std::string& identifier, llvm::Type *type, llvm::AllocaInst *value = nullptr);

        std::string Identifier;
        llvm::Type *Type;
        llvm::AllocaInst *Value;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_VARIABLE_H. */
