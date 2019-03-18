#include "ExpressionIfNode.h"

namespace Aryiele
{
    ExpressionIfNode::ExpressionIfNode(std::shared_ptr<ExpressionNode> condition,
                                       std::vector<std::shared_ptr<ExpressionNode>> if_body,
                                       std::vector<std::shared_ptr<ExpressionNode>> else_body) :
        m_condition(condition), m_if_body(if_body), m_else_body(else_body)
    {

    }

    llvm::Value *ExpressionIfNode::GenerateCode()
    {

    }

    void ExpressionIfNode::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "If/Else");

        // -------------------------------------------------------------------------------------------------------------
        // IF
        // -------------------------------------------------------------------------------------------------------------
        auto ifNode = std::make_shared<ParserInformation>(node, "If");
        auto ifCondition = std::make_shared<ParserInformation>(ifNode, "Condition:");
        auto ifBody = std::make_shared<ParserInformation>(ifNode, "Body:");

        m_condition->DumpInformations(ifCondition);

        for (auto& i : m_if_body)
            i->DumpInformations(ifBody);

        ifNode->Children.emplace_back(ifCondition);
        ifNode->Children.emplace_back(ifBody);
        node->Children.emplace_back(ifNode);

        // -------------------------------------------------------------------------------------------------------------
        // ELSE
        // -------------------------------------------------------------------------------------------------------------
        if (!m_else_body.empty())
        {
            auto elseNode = std::make_shared<ParserInformation>(node, "Else");
            auto elseBody = std::make_shared<ParserInformation>(elseNode, "Body:");

            for (auto& i : m_else_body)
                i->DumpInformations(elseBody);

            elseNode->Children.emplace_back(elseBody);
            node->Children.emplace_back(elseNode);
        }

        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */