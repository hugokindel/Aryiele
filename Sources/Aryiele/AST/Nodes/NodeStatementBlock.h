#ifndef ARYIELE_NODESTATEMENTBLOCK_H
#define ARYIELE_NODESTATEMENTBLOCK_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeStatementBlock : public Node
    {
    public:
        explicit NodeStatementBlock(std::vector<std::shared_ptr<Node>> body = std::vector<std::shared_ptr<Node>>());

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;
        Nodes GetType() override;

        std::vector<std::shared_ptr<Node>> Body;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODESTATEMENTBLOCK_H. */
