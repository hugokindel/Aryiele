#ifndef ARYIELE_EXPRESSIONBINARYOPERATIONNODE_H
#define ARYIELE_EXPRESSIONBINARYOPERATIONNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/ParserTokens.h>
#include <Aryiele/AST/ExpressionNode.h>
#include <memory>

namespace Aryiele
{
    class ExpressionBinaryOperationNode : public ExpressionNode
    {
    public:
        ExpressionBinaryOperationNode(ParserTokens operationType,
                                      std::shared_ptr<ExpressionNode> leftExpression,
                                      std::shared_ptr<ExpressionNode> rightExpression);

        llvm::Value* GenerateCode() override;
        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        ParserTokens m_operationType;
        std::shared_ptr<ExpressionNode> m_leftExpression;
        std::shared_ptr<ExpressionNode> m_rightExpression;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONBINARYOPERATIONNODE_H. */
