#ifndef ARYIELE_EXPRESSIONDOUBLENODE_H
#define ARYIELE_EXPRESSIONDOUBLENODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeVariableDouble : public Node
    {
    public:
        explicit NodeVariableDouble(double value);

        llvm::Value* GenerateCode() override;
        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        double m_value;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_ExpressionDoubleNode_H. */
