#include <Aryiele/AST/Nodes/NodeConstantInteger.h>
#include "NodeConstantInteger.h"


namespace Aryiele
{
    NodeConstantInteger::NodeConstantInteger(int value) :
            Value(value)
    {

    }

    void NodeConstantInteger::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Integer");
        auto body = std::make_shared<ParserInformation>(node, "Value: " + std::to_string(Value));

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

    Nodes NodeConstantInteger::GetType()
    {
        return Nodes_Constant_Integer;
    }

} /* Namespace Aryiele. */