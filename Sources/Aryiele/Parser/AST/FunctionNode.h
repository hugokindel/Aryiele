#ifndef ARYIELE_FUNCTIONNODE_H
#define ARYIELE_FUNCTIONNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/AST/ExpressionNode.h>
#include <Aryiele/Parser/AST/Node.h>
#include <Aryiele/Parser/AST/Argument.h>

namespace Aryiele
{
    class FunctionNode : public Node
    {
    public:
        FunctionNode(const std::string& name,
                     const std::string& type,
                     std::vector<Argument> argumentsName,
                     std::vector<std::shared_ptr<ExpressionNode>> implementations);

        const std::string& GetName() const;
        const std::string& GetType() const;
        const std::vector<Argument>& GetArguments() const;

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        std::string m_name;
        std::string m_type;
        std::vector<Argument> m_arguments;
        std::vector<std::shared_ptr<ExpressionNode>> m_implementations;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_FUNCTIONNODE_H. */
