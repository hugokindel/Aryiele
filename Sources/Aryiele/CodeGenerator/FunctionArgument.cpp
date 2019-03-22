#include <Aryiele/CodeGenerator/FunctionArgument.h>

namespace Aryiele
{
    FunctionArgument::FunctionArgument(const std::string &identifier, llvm::Type *type) :
        Identifier(identifier), Type(type)
    {

    }

} /* Namespace Aryiele. */