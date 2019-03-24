#ifndef ARYIELE_NODESTATEMENTVARIABLEDECLARATION_H
#define ARYIELE_NODESTATEMENTVARIABLEDECLARATION_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeStatementVariableDeclaration : public Node
    {
    public:
        NodeStatementVariableDeclaration(const std::string &identifier,
                                         const std::string &type,
                                         std::shared_ptr<Node> expression = nullptr);

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;
        Nodes GetType() override;

        std::string Identifier;
        std::string Type;
        std::shared_ptr<Node> Expression;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODESTATEMENTVARIABLEDECLARATION_H. */
