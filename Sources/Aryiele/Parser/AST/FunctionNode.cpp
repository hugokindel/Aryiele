#include <Aryiele/Parser/AST/FunctionNode.h>
#include <Vanir/StringUtils.h>

namespace Aryiele
{
    FunctionNode::FunctionNode(const std::string& name,
                               const std::string& type,
                               std::vector<Argument> arguments,
                               std::vector<std::shared_ptr<ExpressionNode>> implementations) :
        m_name(name), m_type(type), m_arguments(arguments), m_implementations(implementations)
    {

    }

    const std::string& FunctionNode::GetName() const
    {
        return m_name;
    }

    const std::string& FunctionNode::GetType() const
    {
        return m_type;
    }

    const std::vector<Argument>& FunctionNode::GetArguments() const
    {
        return m_arguments;
    }

    void FunctionNode::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Function");
        auto argumentsNode = std::make_shared<ParserInformation>(node, "Arguments:");
        auto valueNode = std::make_shared<ParserInformation>(node, "Body:");

        for (auto& childNode : m_implementations)
            childNode->DumpInformations(valueNode);

        int i = 0;

        for(auto& argument : m_arguments)
        {
            auto argumentNode = std::make_shared<ParserInformation>(argumentsNode, std::to_string(i));

            argumentNode->Children.emplace_back(std::make_shared<ParserInformation>(argumentNode, "Name: " + argument.Name));
            argumentNode->Children.emplace_back(std::make_shared<ParserInformation>(argumentNode, "Type: " + argument.Type));

            argumentsNode->Children.emplace_back(argumentNode);

            i++;
        }

        node->Children.emplace_back(std::make_shared<ParserInformation>(node, "Name: " + m_name));
        node->Children.emplace_back(std::make_shared<ParserInformation>(node, "Type: " + m_type));
        node->Children.emplace_back(argumentsNode);
        node->Children.emplace_back(valueNode);

        parentNode->Children.emplace_back(node);
    }

} /* Namespace Aryiele. */