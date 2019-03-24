#include <Aryiele/AST/Nodes/NodeStatementVariableDeclaration.h>

namespace Aryiele
{
    NodeStatementVariableDeclaration::NodeStatementVariableDeclaration(const std::string &identifier,
                                                                       const std::string &type,
                                                                       std::shared_ptr<Node> expression) :
        Identifier(identifier), Type(type), Expression(expression)
    {

    }

    void NodeStatementVariableDeclaration::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Variable Declaration");
        auto valueNode = std::make_shared<ParserInformation>(node, "Expression:");

        Expression->DumpInformations(valueNode);

        node->Children.emplace_back(std::make_shared<ParserInformation>(node, "Identifier: " + Identifier));
        node->Children.emplace_back(std::make_shared<ParserInformation>(node, "Type: " + Type));
        node->Children.emplace_back(valueNode);

        parentNode->Children.emplace_back(node);
    }

    Nodes NodeStatementVariableDeclaration::GetType()
    {
        return Nodes_Statement_VariableDeclaration;
    }

} /* Namespace Aryiele. */