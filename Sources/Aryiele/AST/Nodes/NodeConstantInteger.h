#ifndef ARYIELE_NODECONSTANTINTEGER_H
#define ARYIELE_NODECONSTANTINTEGER_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeConstantInteger : public Node
    {
    public:
        explicit NodeConstantInteger(int value);

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;
        Nodes GetType() override;

        int Value;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODECONSTANTINTEGER_H. */
