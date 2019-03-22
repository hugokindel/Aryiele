#include "Function.h"

namespace Aryiele
{
    Function::Function(const std::string &identifier, llvm::Type *returnType,
                       std::vector<std::shared_ptr<FunctionArgument>> arguments) :
        Identifier(identifier), ReturnType(returnType), Arguments(arguments)
    {

    }

} /* Namespace Aryiele. */