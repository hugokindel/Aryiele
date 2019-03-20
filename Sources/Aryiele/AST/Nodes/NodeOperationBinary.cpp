#include <Aryiele/AST/Nodes/NodeOperationBinary.h>
#include <Aryiele/Parser/Parser.h>
#include "NodeOperationBinary.h"


namespace Aryiele
{

    NodeOperationBinary::NodeOperationBinary(ParserTokens operationType,
                                             std::shared_ptr<Node> lhs,
                                             std::shared_ptr<Node> rhs) :
            OperationType(operationType),
            LHS(lhs),
            RHS(rhs)
    {

    }

    void NodeOperationBinary::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Binary Operation");
        auto operationType = std::make_shared<ParserInformation>(
            node, "Operation Type: " + Parser::GetTokenName(OperationType));
        auto lhs = std::make_shared<ParserInformation>(node, "LHS:");
        auto rhs = std::make_shared<ParserInformation>(node, "RHS:");

        LHS->DumpInformations(lhs);
        RHS->DumpInformations(rhs);

        node->Children.emplace_back(operationType);
        node->Children.emplace_back(lhs);
        node->Children.emplace_back(rhs);

        parentNode->Children.emplace_back(node);
    }

    Nodes NodeOperationBinary::GetType()
    {
        return Nodes_Operation_Binary;
    }

} /* Namespace Aryiele. */