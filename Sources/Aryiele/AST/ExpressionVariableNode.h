#ifndef ARYIELE_EXPRESSIONVARIABLENODE_H
#define ARYIELE_EXPRESSIONVARIABLENODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/ExpressionNode.h>

namespace Aryiele
{
    class ExpressionVariableNode : public ExpressionNode
    {
    public:
        explicit ExpressionVariableNode(const std::string& name);

        llvm::Value* GenerateCode() override;
        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        std::string m_name;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONVARIABLENODE_H. */
