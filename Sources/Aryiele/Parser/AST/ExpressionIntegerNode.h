#ifndef ARYIELE_EXPRESSIONINTEGERNODE_H
#define ARYIELE_EXPRESSIONINTEGERNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/AST/ExpressionNode.h>

namespace Aryiele
{
    class ExpressionIntegerNode : public ExpressionNode
    {
    public:
        explicit ExpressionIntegerNode(int value);

    protected:
        int m_value;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONINTEGERNODE_H. */
