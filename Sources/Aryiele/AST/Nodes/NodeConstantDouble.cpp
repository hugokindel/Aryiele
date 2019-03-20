#include <Aryiele/AST/Nodes/NodeConstantDouble.h>
#include "NodeConstantDouble.h"


namespace Aryiele
{
    NodeConstantDouble::NodeConstantDouble(double value) :
        Value(value)
    {

    }

    void NodeConstantDouble::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Double");
        auto body = std::make_shared<ParserInformation>(node, "Value: " + std::to_string(Value));

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

    Nodes NodeConstantDouble::GetType()
    {
        return Nodes_Constant_Double;
    }

} /* Namespace Aryiele. */