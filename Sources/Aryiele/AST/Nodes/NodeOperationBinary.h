#ifndef ARYIELE_EXPRESSIONBINARYOPERATIONNODE_H
#define ARYIELE_EXPRESSIONBINARYOPERATIONNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/ParserTokens.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <memory>

namespace Aryiele
{
    class NodeOperationBinary : public Node
    {
    public:
        NodeOperationBinary(ParserTokens operationType,
                                      std::shared_ptr<Node> leftExpression,
                                      std::shared_ptr<Node> rightExpression);

        llvm::Value* GenerateCode() override;
        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        ParserTokens m_operationType;
        std::shared_ptr<Node> m_leftExpression;
        std::shared_ptr<Node> m_rightExpression;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONBINARYOPERATIONNODE_H. */
