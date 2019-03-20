#ifndef ARYIELE_NODESTATEMENTFUNCTIONCALL_H
#define ARYIELE_NODESTATEMENTFUNCTIONCALL_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <memory>
#include <vector>

namespace Aryiele
{
    class NodeStatementFunctionCall : public Node
    {
    public:
        NodeStatementFunctionCall(const std::string& identifier, std::vector<std::shared_ptr<Node>> arguments);

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;
        Nodes GetType() override;

        std::string Identifier;
        std::vector<std::shared_ptr<Node>> Arguments;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODESTATEMENTFUNCTIONCALL_H. */
