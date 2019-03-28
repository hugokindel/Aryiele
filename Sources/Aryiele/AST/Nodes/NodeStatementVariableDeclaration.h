#ifndef ARYIELE_NODESTATEMENTVARIABLEDECLARATION_H
#define ARYIELE_NODESTATEMENTVARIABLEDECLARATION_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Variable.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeStatementVariableDeclaration : public Node
    {
    public:
        NodeStatementVariableDeclaration(
                std::vector<std::shared_ptr<Variable>> variables = std::vector<std::shared_ptr<Variable>>());

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;
        Nodes GetType() override;

        std::vector<std::shared_ptr<Variable>> Variables;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODESTATEMENTVARIABLEDECLARATION_H. */
