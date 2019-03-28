#ifndef ARYIELE_VARIABLE_H
#define ARYIELE_VARIABLE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class Variable
    {
    public:
        Variable(const std::string &identifier, const std::string &type,
                 std::shared_ptr<Node> expression = nullptr);

        std::string Identifier;
        std::string Type;
        std::shared_ptr<Node> Expression;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_VARIABLE_H. */
