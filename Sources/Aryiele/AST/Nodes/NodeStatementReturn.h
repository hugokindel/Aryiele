#ifndef ARYIELE_NODESTATEMENTRETURN_H
#define ARYIELE_NODESTATEMENTRETURN_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeStatementReturn : public Node
    {
    public:
        NodeStatementReturn(std::shared_ptr<Node> expression);

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;
        Nodes GetType() override;

        std::shared_ptr<Node> Expression; // TODO: All (block statement ?)
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODESTATEMENTRETURN_H. */
