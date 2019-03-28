#include <Aryiele/AST/Nodes/NodeStatementVariableDeclaration.h>

namespace Aryiele
{
    NodeStatementVariableDeclaration::NodeStatementVariableDeclaration(
            std::vector<std::shared_ptr<Variable>> variables) :
        Variables(variables)
    {

    }

    void NodeStatementVariableDeclaration::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Variable Declaration");

        auto i = 0;

        for (auto &variable : Variables)
        {
            auto variableNode = std::make_shared<ParserInformation>(node, std::to_string(i));

            variableNode->Children.emplace_back(std::make_shared<ParserInformation>(
                    variableNode, "Identifier: " + variable->Identifier));
            variableNode->Children.emplace_back(std::make_shared<ParserInformation>(variableNode, "Type: " + variable->Type));

            if (variable->Expression)
            {
                auto valueNode = std::make_shared<ParserInformation>(variableNode, "Expression:");
                variable->Expression->DumpInformations(valueNode);
                variableNode->Children.emplace_back(valueNode);
            }

            node->Children.emplace_back(variableNode);

            i++;
        }

        parentNode->Children.emplace_back(node);
    }

    Nodes NodeStatementVariableDeclaration::GetType()
    {
        return Nodes_Statement_VariableDeclaration;
    }

} /* Namespace Aryiele. */