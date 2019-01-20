#ifndef ARYIELE_EXPRESSIONBOOLEANNODE_H
#define ARYIELE_EXPRESSIONBOOLEANNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/AST/ExpressionNode.h>

namespace Aryiele
{
    class ExpressionBooleanNode : public ExpressionNode
    {
    public:
        explicit ExpressionBooleanNode(bool value);

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        bool m_value;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONBOOLEANNODE_H. */
