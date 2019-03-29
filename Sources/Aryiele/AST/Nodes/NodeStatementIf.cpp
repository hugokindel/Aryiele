#include <Aryiele/AST/Nodes/NodeStatementIf.h>
#include "NodeStatementIf.h"


namespace Aryiele
{
    NodeStatementIf::NodeStatementIf(std::shared_ptr<Node> condition,
                                     std::vector<std::shared_ptr<Node>> ifBody,
                                     std::vector<std::shared_ptr<Node>> elseBody,
                                     std::vector<std::vector<std::shared_ptr<Node>>> elseIfBody) :
        Condition(condition), IfBody(ifBody), ElseBody(elseBody), ElseIfBody(elseIfBody)
    {

    }

    void NodeStatementIf::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "If/Else");

        auto ifNode = std::make_shared<ParserInformation>(node, "If");
        auto ifCondition = std::make_shared<ParserInformation>(ifNode, "Condition:");
        auto ifBody = std::make_shared<ParserInformation>(ifNode, "Body:");

        Condition->DumpInformations(ifCondition);

        for (auto& i : IfBody)
            i->DumpInformations(ifBody);

        ifNode->Children.emplace_back(ifCondition);
        ifNode->Children.emplace_back(ifBody);
        node->Children.emplace_back(ifNode);

        for (auto elseIfBody : ElseIfBody)
        {
            auto elseNode = std::make_shared<ParserInformation>(node, "Else If");
            auto elseBody = std::make_shared<ParserInformation>(elseNode, "Body:");

            for (auto& i : elseIfBody)
                i->DumpInformations(elseBody);

            elseNode->Children.emplace_back(elseBody);
            node->Children.emplace_back(elseNode);
        }

        if (!ElseBody.empty())
        {
            auto elseNode = std::make_shared<ParserInformation>(node, "Else");
            auto elseBody = std::make_shared<ParserInformation>(elseNode, "Body:");

            for (auto& i : ElseBody)
                i->DumpInformations(elseBody);

            elseNode->Children.emplace_back(elseBody);
            node->Children.emplace_back(elseNode);
        }

        parentNode->Children.emplace_back(node);
    }

    Nodes NodeStatementIf::GetType()
    {
        return Nodes_Statement_If;
    }

} /* Namespace Aryiele. */