#include <Aryiele/AST/Nodes/NodeStatementReturn.h>
#include "NodeStatementReturn.h"


namespace Aryiele
{

    NodeStatementReturn::NodeStatementReturn(std::shared_ptr<Node> expression) :
        Expression(expression)
    {

    }

    void NodeStatementReturn::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Return");
        auto body = std::make_shared<ParserInformation>(node, "Body:");

        Expression->DumpInformations(body);

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

    Nodes NodeStatementReturn::GetType()
    {
        return Nodes_Statement_Return;
    }

} /* Namespace Aryiele. */