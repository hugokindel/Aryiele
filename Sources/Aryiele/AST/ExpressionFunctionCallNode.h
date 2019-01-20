#ifndef ARYIELE_EXPRESSIONFUNCTIONCALLNODE_H
#define ARYIELE_EXPRESSIONFUNCTIONCALLNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/ExpressionNode.h>
#include <memory>
#include <vector>

namespace Aryiele
{
    class ExpressionFunctionCallNode : public ExpressionNode
    {
    public:
        ExpressionFunctionCallNode(const std::string& name, std::vector<std::shared_ptr<ExpressionNode>> arguments);

        llvm::Value* GenerateCode() override;

    protected:
        std::string m_name;
        std::vector<std::shared_ptr<ExpressionNode>> m_arguments;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONFUNCTIONCALLNODE_H. */
