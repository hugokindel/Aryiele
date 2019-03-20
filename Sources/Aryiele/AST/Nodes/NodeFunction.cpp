#include <Aryiele/AST/Nodes/NodeFunction.h>
#include <Vanir/StringUtils.h>
#include "NodeFunction.h"


namespace Aryiele
{
    NodeFunction::NodeFunction(const std::string& identifier,
                               const std::string& type,
                               std::vector<Argument> arguments,
                               std::vector<std::shared_ptr<Node>> body) :
        Identifier(identifier), Type(type), Arguments(arguments), Body(body)
    {

    }

    void NodeFunction::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Function");
        auto argumentsNode = std::make_shared<ParserInformation>(node, "Arguments:");
        auto valueNode = std::make_shared<ParserInformation>(node, "Body:");

        for (auto& childNode : Body)
            childNode->DumpInformations(valueNode);

        int i = 0;

        for(auto& argument : Arguments)
        {
            auto argumentNode = std::make_shared<ParserInformation>(argumentsNode, std::to_string(i));

            argumentNode->Children.emplace_back(std::make_shared<ParserInformation>(
                argumentNode, "Identifier: " + argument.Identifier));
            argumentNode->Children.emplace_back(std::make_shared<ParserInformation>(
                argumentNode, "Type: " + argument.Type));

            argumentsNode->Children.emplace_back(argumentNode);

            i++;
        }

        node->Children.emplace_back(std::make_shared<ParserInformation>(node, "Identifier: " + Identifier));
        node->Children.emplace_back(std::make_shared<ParserInformation>(node, "Type: " + Type));
        node->Children.emplace_back(argumentsNode);
        node->Children.emplace_back(valueNode);

        parentNode->Children.emplace_back(node);
    }

    Nodes NodeFunction::GetType()
    {
        return Nodes_Function_Prototype;
    }

} /* Namespace Aryiele. */