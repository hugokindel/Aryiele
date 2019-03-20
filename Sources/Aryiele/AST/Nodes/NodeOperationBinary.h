#ifndef ARYIELE_NODEOPERATIONBINARY_H
#define ARYIELE_NODEOPERATIONBINARY_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/ParserTokens.h>
#include <Aryiele/AST/Nodes/Node.h>
#include <memory>

namespace Aryiele
{
    class NodeOperationBinary : public Node
    {
    public:
        NodeOperationBinary(ParserTokens operationType, std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs);

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;
        Nodes GetType() override;

        ParserTokens OperationType;
        std::shared_ptr<Node> LHS;
        std::shared_ptr<Node> RHS;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODEOPERATIONBINARY_H. */
