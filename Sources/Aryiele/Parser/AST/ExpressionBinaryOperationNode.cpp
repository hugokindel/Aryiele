#include <Aryiele/Parser/AST/ExpressionBinaryOperationNode.h>

namespace Aryiele
{

    ExpressionBinaryOperationNode::ExpressionBinaryOperationNode(ParserTokens operationCode,
                                                                 std::shared_ptr<ExpressionNode> leftExpression,
                                                                 std::shared_ptr<ExpressionNode> rightExpression) :
            m_operationCode(operationCode),
            m_leftExpression(leftExpression),
            m_rightExpression(rightExpression)
    {

    }

} /* Namespace Aryiele. */