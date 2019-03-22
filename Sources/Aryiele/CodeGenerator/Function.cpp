#include "Function.h"

namespace Aryiele
{
    Function::Function(const std::string &identifier, llvm::Type *returnType,
                       std::vector<std::shared_ptr<FunctionArgument>> arguments,
                       std::shared_ptr<BlockStack> blocks) :
        Identifier(identifier), ReturnType(returnType), Arguments(arguments), Blocks(blocks)
    {

    }

} /* Namespace Aryiele. */