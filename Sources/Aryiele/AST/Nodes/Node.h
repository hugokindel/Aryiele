#ifndef ARYIELE_NODE_H
#define ARYIELE_NODE_H

#include <Aryiele/Core/Includes.h>
#include <Aryiele/Parser/ParserInformation.h>
#include <Aryiele/AST/Nodes.h>

namespace Aryiele
{
    class Node
    {
    public:
        virtual ~Node() = default;

        virtual void DumpInformations(std::shared_ptr<ParserInformation> parentNode) = 0;
        virtual Nodes GetType() = 0;
    };

} /* Namespace Aryiele. */

#endif /* ARYIELE_NODE_H. */
