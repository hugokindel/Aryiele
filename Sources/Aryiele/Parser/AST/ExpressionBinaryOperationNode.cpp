#include <Aryiele/Parser/AST/ExpressionBinaryOperationNode.h>
#include <Aryiele/Parser/Parser.h>

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

    void ExpressionBinaryOperationNode::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Binary Operation");
        auto operationType = std::make_shared<ParserInformation>(node, "Operation Type: " + Parser::GetTokenName(m_operationCode));
        auto leftExpression = std::make_shared<ParserInformation>(node, "Left Expression:");
        auto rightExpression = std::make_shared<ParserInformation>(node, "Right Expression:");

        m_leftExpression->DumpInformations(leftExpression);
        m_rightExpression->DumpInformations(rightExpression);

        node->Children.emplace_back(operationType);
        node->Children.emplace_back(leftExpression);
        node->Children.emplace_back(rightExpression);

        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */