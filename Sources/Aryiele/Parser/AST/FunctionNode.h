#ifndef ARYIELE_FUNCTIONNODE_H
#define ARYIELE_FUNCTIONNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/AST/ExpressionNode.h>
#include <Aryiele/Parser/AST/Node.h>
#include <Aryiele/Parser/AST/FunctionPrototypeNode.h>
#include <memory>

namespace Aryiele
{
    class FunctionNode : public Node
    {
    public:
        FunctionNode(std::shared_ptr<FunctionPrototypeNode> prototype, std::vector<std::shared_ptr<ExpressionNode>> implementations);

        void DumpInformations(std::shared_ptr<ParserDumpTreeNode> parentNode) override;

    protected:
        std::shared_ptr<FunctionPrototypeNode> m_prototype;
        std::vector<std::shared_ptr<ExpressionNode>> m_implementations;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_FUNCTIONNODE_H. */
