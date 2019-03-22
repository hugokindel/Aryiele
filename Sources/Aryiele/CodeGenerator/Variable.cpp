#include <Aryiele/CodeGenerator/Variable.h>

namespace Aryiele
{
    Variable::Variable(const std::string &identifier, llvm::Type *type, llvm::AllocaInst *value) :
        Identifier(identifier), Type(type), Value(value)
    {

    }

} /* Namespace Aryiele. */