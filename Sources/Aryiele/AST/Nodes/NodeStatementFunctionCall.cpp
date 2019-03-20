#include <Aryiele/AST/Nodes/NodeStatementFunctionCall.h>
#include "NodeStatementFunctionCall.h"


namespace Aryiele
{

    NodeStatementFunctionCall::NodeStatementFunctionCall(const std::string &identifier,
                                                           std::vector<std::shared_ptr<Node>> arguments) :
        Identifier(identifier), Arguments(arguments)
    {

    }

    void NodeStatementFunctionCall::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Function Call");
        auto identifier = std::make_shared<ParserInformation>(node, "Identifier: " + Identifier);
        auto argumentsNode = std::make_shared<ParserInformation>(node, "Arguments:");

        auto i = 0;

        for(auto& argument : Arguments)
        {
            auto argumentNode = std::make_shared<ParserInformation>(argumentsNode, std::to_string(i));
            auto body = std::make_shared<ParserInformation>(argumentNode, "Body:");

            argument->DumpInformations(body);

            argumentNode->Children.emplace_back(body);
            argumentsNode->Children.emplace_back(argumentNode);

            i++;
        }

        node->Children.emplace_back(identifier);
        node->Children.emplace_back(argumentsNode);
        parentNode->Children.emplace_back(node);
    }

    Nodes NodeStatementFunctionCall::GetType()
    {
        return Nodes_Statement_FunctionCall;
    }

} /* Namespace Aryiele. */