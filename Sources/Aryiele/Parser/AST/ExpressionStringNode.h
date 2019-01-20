#ifndef ARYIELE_EXPRESSIONSTRINGNODE_H
#define ARYIELE_EXPRESSIONSTRINGNODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/AST/ExpressionNode.h>

namespace Aryiele
{
    class ExpressionStringNode : public ExpressionNode
    {
    public:
        explicit ExpressionStringNode(const std::string& value);

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;

    protected:
        std::string m_value;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_EXPRESSIONSTRINGNODE_H. */
