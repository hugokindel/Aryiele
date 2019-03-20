#ifndef ARYIELE_EXPRESSIONFUNCTIONCALLNODE_H
#define ARYIELE_EXPRESSIONFUNCTIONCALLNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <memory>
#include <vector>

namespace Aryiele
{
    class NodeStatementFunctionCall : public Node
    {
    public:
        NodeStatementFunctionCall(const std::string& name, std::vector<std::shared_ptr<Node>> arguments);

        llvm::Value* GenerateCode() override;
        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        std::string m_name;
        std::vector<std::shared_ptr<Node>> m_arguments;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONFUNCTIONCALLNODE_H. */
