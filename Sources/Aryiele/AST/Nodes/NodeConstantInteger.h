#ifndef ARYIELE_EXPRESSIONINTEGERNODE_H
#define ARYIELE_EXPRESSIONINTEGERNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeVariableInteger : public Node
    {
    public:
        explicit NodeVariableInteger(int value);

        llvm::Value* GenerateCode() override;
        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        int m_value;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONINTEGERNODE_H. */
