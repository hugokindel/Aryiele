#ifndef ARYIELE_EXPRESSIONFUNCTIONRETURNNODE_H
#define ARYIELE_EXPRESSIONFUNCTIONRETURNNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/ExpressionNode.h>

namespace Aryiele
{
    class ExpressionFunctionReturnNode : public ExpressionNode
    {
    public:
        ExpressionFunctionReturnNode(std::shared_ptr<ExpressionNode> expression);

        std::shared_ptr<ExpressionNode> GetExpression();

        llvm::Value* GenerateCode() override;
        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        std::shared_ptr<ExpressionNode> m_expression;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONFUNCTIONRETURNNODE_H. */
