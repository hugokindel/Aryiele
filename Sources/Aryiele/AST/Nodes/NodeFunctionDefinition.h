#ifndef ARYIELE_EXPRESSIONFUNCTIONRETURNNODE_H
#define ARYIELE_EXPRESSIONFUNCTIONRETURNNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeFunctionDefinition : public Node
    {
    public:
        NodeFunctionDefinition(std::shared_ptr<Node> expression);

        std::shared_ptr<Node> GetExpression();

        llvm::Value* GenerateCode() override;
        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        std::shared_ptr<Node> m_expression; // TODO: All
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONFUNCTIONRETURNNODE_H. */
