#ifndef ARYIELE_EXPRESSIONIFNODE_H
#define ARYIELE_EXPRESSIONIFNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeStatementIf : public Node
    {
    public:
        NodeStatementIf(std::shared_ptr<Node> condition,
                         std::vector<std::shared_ptr<Node>> if_body,
                         std::vector<std::shared_ptr<Node>> else_body);

        llvm::Value* GenerateCode() override;
        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    private:
        std::shared_ptr<Node> m_condition;
        std::vector<std::shared_ptr<Node>> m_if_body;
        std::vector<std::shared_ptr<Node>> m_else_body;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONIFNODE_H. */
