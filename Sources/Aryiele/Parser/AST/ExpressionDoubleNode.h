#ifndef ARYIELE_EXPRESSIONDOUBLENODE_H
#define ARYIELE_EXPRESSIONDOUBLENODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/AST/ExpressionNode.h>

namespace Aryiele
{
    class ExpressionDoubleNode : public ExpressionNode
    {
    public:
        explicit ExpressionDoubleNode(double value);

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        double m_value;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_ExpressionDoubleNode_H. */
