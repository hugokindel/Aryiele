#include <Aryiele/AST/Nodes/NodeStatementBlock.h>

namespace Aryiele
{
    NodeStatementBlock::NodeStatementBlock(std::vector<std::shared_ptr<Node>> body) :
        Body(body)
    {

    }

    void NodeStatementBlock::DumpInformations(std::shared_ptr<ParserInformation> parentNode)
    {
        auto node = std::make_shared<ParserInformation>(parentNode, "Block");
        auto body = std::make_shared<ParserInformation>(node, "Body:");

        for (auto& statement : Body)
            statement->DumpInformations(body);

        node->Children.emplace_back(body);
        parentNode->Children.emplace_back(node);
    }

    Nodes NodeStatementBlock::GetType()
    {
        return Nodes_Statement_Block;
    }

} /* Namespace Aryiele. */