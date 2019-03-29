#ifndef ARYIELE_NODESTATEMENTIF_H
#define ARYIELE_NODESTATEMENTIF_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeStatementIf : public Node
    {
    public:
        NodeStatementIf(std::shared_ptr<Node> condition,
                         std::vector<std::shared_ptr<Node>> ifBody,
                         std::vector<std::shared_ptr<Node>> elseBody = std::vector<std::shared_ptr<Node>>(),
                         std::vector<std::vector<std::shared_ptr<Node>>> elseIfBody =
                                 std::vector<std::vector<std::shared_ptr<Node>>>());

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;
        Nodes GetType() override;

        std::shared_ptr<Node> Condition;
        std::vector<std::shared_ptr<Node>> IfBody;
        std::vector<std::shared_ptr<Node>> ElseBody;
        std::vector<std::vector<std::shared_ptr<Node>>> ElseIfBody;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODESTATEMENTIF_H. */
