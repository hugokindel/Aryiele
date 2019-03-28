#include <Aryiele/CodeGenerator/GenerationError.h>

namespace Aryiele
{
    GenerationError::GenerationError(bool success, llvm::Value *value) :
        Success(success), Value(value)
    {

    }

} /* Namespace Aryiele. */