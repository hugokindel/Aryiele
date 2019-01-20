#include <Aryiele/AST/ExpressionFunctionReturnNode.h>
#include "ExpressionFunctionReturnNode.h"


namespace Aryiele
{

    ExpressionFunctionReturnNode::ExpressionFunctionReturnNode(std::shared_ptr<ExpressionNode> expression) :
        m_expression(expression)
    {

    }

    std::shared_ptr<ExpressionNode> ExpressionFunctionReturnNode::GetExpression()
    {
        return m_expression;
    }

    llvm::Value *ExpressionFunctionReturnNode::GenerateCode()
    {
        return m_expression->GenerateCode();
    }

    void ExpressionFunctionReturnNode::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Return");
        auto body = std::make_shared<ParserInformation>(node, "Body:");

        m_expression->DumpInformations(body);

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */