#include <Aryiele/AST/Variable.h>

namespace Aryiele
{
    Variable::Variable(const std::string &identifier, const std::string &type, std::shared_ptr<Node> expression) :
        Identifier(identifier), Type(type), Expression(expression)
    {

    }

} /* Namespace Aryiele. */