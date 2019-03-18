#ifndef ARYIELE_EXPRESSIONIFNODE_H
#define ARYIELE_EXPRESSIONIFNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/ExpressionNode.h>

namespace Aryiele
{
    class ExpressionIfNode : public ExpressionNode
    {
    public:
        ExpressionIfNode(std::shared_ptr<ExpressionNode> condition,
                         std::vector<std::shared_ptr<ExpressionNode>> if_body,
                         std::vector<std::shared_ptr<ExpressionNode>> else_body);

        llvm::Value* GenerateCode() override;
        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    private:
        std::shared_ptr<ExpressionNode> m_condition;
        std::vector<std::shared_ptr<ExpressionNode>> m_if_body;
        std::vector<std::shared_ptr<ExpressionNode>> m_else_body;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONIFNODE_H. */
