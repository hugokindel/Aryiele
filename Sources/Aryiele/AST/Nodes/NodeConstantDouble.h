#ifndef ARYIELE_NODECONSTANTDOUBLE_H
#define ARYIELE_NODECONSTANTDOUBLE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/AST/Nodes/Node.h>

namespace Aryiele
{
    class NodeConstantDouble : public Node
    {
    public:
        explicit NodeConstantDouble(double value);

        void DumpInformations(std::shared_ptr<ParserInformation> parentNode) override;
        Nodes GetType() override;

        double Value;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODECONSTANTDOUBLE_H. */
