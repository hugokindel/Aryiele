#ifndef ARYIELE_GENERATIONERROR_H
#define ARYIELE_GENERATIONERROR_H

#include <Aryiele/Core/Includes.h>
#include <llvm/IR/Value.h>

namespace Aryiele
{
    struct GenerationError
    {
        GenerationError(bool success = false, llvm::Value *value = nullptr);

        bool Success;
        llvm::Value *Value;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_GENERATIONERROR_H. */
