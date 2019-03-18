#ifndef ARYIELE_EXPRESSIONIFNODE_H
#define ARYIELE_EXPRESSIONIFNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/ExpressionNode.h>

namespace Aryiele
{
    class ExpressionIfNode : public ExpressionNode
    {
    public:
        ExpressionIfNode(std::shared_ptr<ExpressionNode> condition, std::shared_ptr<ExpressionNode> if_body,
                         std::shared_ptr<ExpressionNode> else_body);

        llvm::Value* GenerateCode() override;

    private:
        std::shared_ptr<ExpressionNode> m_condition;
        std::shared_ptr<ExpressionNode> m_if_body;
        std::shared_ptr<ExpressionNode> m_else_body;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONIFNODE_H. */
