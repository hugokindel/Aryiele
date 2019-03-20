#include <Aryiele/AST/Nodes/NodeVariable.h>
#include "NodeVariable.h"


namespace Aryiele
{
    NodeVariable::NodeVariable(const std::string& identifier) :
            Identifier(identifier)
    {

    }

    void NodeVariable::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Variable");
        auto body = std::make_shared<ParserInformation>(node, "Identifier: " + Identifier);

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

    Nodes NodeVariable::GetType()
    {
        return Nodes_Variable;
    }

} /* Namespace Aryiele. */